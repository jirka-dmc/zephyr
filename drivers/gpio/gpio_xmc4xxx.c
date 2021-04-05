
#define DT_DRV_COMPAT infineon_xmc4xxx_gpio

/* Zephyr includes */
#include <drivers/gpio.h>
#include "gpio_utils.h"
// #include <drivers/pinmux.h> <---TODO will probably need to uncomment to use pinmux?

/* XMCLib include */
#include <drivers/interrupt_controller/exti_xmc4xxx.h>


/* 
 * structs that will define the device that we are passed for the following API calls: 
 */
struct gpio_xmc4xxx_config {
	/* gpio_driver_config needs to be first, required by Zephyr */
	struct gpio_driver_config common;
	/* port base address */
	uint32_t *base;
};

struct gpio_xmc4xxx_data {
	/* gpio_driver_data needs to be first, required by Zephyr */
	struct gpio_driver_data common;
	/* user ISR cb */
	sys_slist_t callback_list; 
};


/* 
 * Macros for unpacking the above structs from the device pointer 
 */
#define DEV_CFG(dev) \
	((const struct gpio_xmc4xxx_config * const)(dev)->config)
#define DEV_BASE(dev) \
    (XMC_GPIO_PORT_t *)(DEV_CFG(dev)->base)
#define DEV_DATA(dev) \
	((struct gpio_xmc4xxx_data * const)(dev)->data)


/* *
 * Macros for simplifying CMSIS calls
 */
#define SET_BIT_SHIFT 0
#define UNSET_BIT_SHIFT 16

/*
    For initialization purposes, we need a way to convert the Zephyr IO flags 
    into the form that XMCLib is expecting
*/
static XMC_GPIO_CONFIG_t convert_zephyr_to_xmc_flags(gpio_flags_t flags, XMC_GPIO_PORT_t *port, int *error)
{
    XMC_GPIO_CONFIG_t config;

    if (flags & GPIO_DISCONNECTED)
    {
        *error = -EIO; 
    }

    XMC_GPIO_MODE_t mode = 0; 
    XMC_GPIO_OUTPUT_LEVEL_t output_level = 0;
    if (flags & GPIO_INPUT)
    {
        if (flags & GPIO_PULL_DOWN)
        {
            mode = XMC_GPIO_MODE_INPUT_PULL_DOWN; 
        }
        else if (flags & GPIO_PULL_UP)
        {
            mode = XMC_GPIO_MODE_INPUT_PULL_UP; 
        }
        else
        {
            mode = XMC_GPIO_MODE_INPUT_TRISTATE; 
        }

        /* NOTE: other possible XMCLib input flags, unsure how these would translate in Zephyr:
            XMC_GPIO_MODE_INPUT_SAMPLING 
            XMC_GPIO_MODE_INPUT_INVERTED_TRISTATE   
            XMC_GPIO_MODE_INPUT_INVERTED_PULL_DOWN  
            XMC_GPIO_MODE_INPUT_INVERTED_PULL_UP   
            XMC_GPIO_MODE_INPUT_INVERTED_SAMPLING 
        */
    }
    else if (flags & GPIO_OUTPUT)
    {
        /* ports 14 and 15 are not available for general GPIO output */
        if (port==XMC_GPIO_PORT14 || port==XMC_GPIO_PORT15)
        {
            *error = -EINVAL; 
        }

        // handle output init
        if (flags & GPIO_OPEN_DRAIN)
        {
            mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN; 
        }
        else if (flags & (GPIO_OPEN_DRAIN | GPIO_PUSH_PULL))
        {
            *error = -EINVAL; 
        }
        else
        { 
            /* default to PUSH_PULL */
            mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
            if (flags & GPIO_OUTPUT_INIT_HIGH)
            {
                output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
            }
            else
            {
                /* default to active low */
                output_level = XMC_GPIO_OUTPUT_LEVEL_LOW; 
            }
        }
    }
    else 
    {
        /* If input or output not defined, return an error */
        *error = -EINVAL; 
    }

    /*
    * NOTE: the XMC supports multiple different drive strengths, but the Zephyr API assumes that it can be configured per HIGH/Low
    * which, the XMC does not support
    * To compromise, we will force the HIGH/LOW values to be the same, or throw EINVAL
    */
    XMC_GPIO_OUTPUT_STRENGTH_t output_strength = 0;
    switch (flags & (GPIO_DS_ALT_LOW | GPIO_DS_ALT_HIGH))
    {
        case GPIO_DS_ALT_LOW | GPIO_DS_ALT_HIGH:
            output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE; 
            break;
        case 0x0: /* no strength is specified, so we will use the default */
            output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM; 
            break; 
        default:
            *error = -EINVAL; 
    }

    config.mode = mode;
    config.output_level = output_level;
    config.output_strength = output_strength;
    return config; 
}

/*
    Hook for all ISRs, handles callbacks based on constructed list and passed in pin mask 
*/
static void gpio_xmc4xxx_isr(uint32_t pin_mask, const struct device *dev)
{
    struct gpio_xmc4xxx_data *data = DEV_DATA(dev); 
	gpio_fire_callbacks(&data->callback_list, dev, pin_mask);
}


/*
    Initialization for the driver, called automatically by Zephyr before allowing any other API calls

    From Zephyr's perspective, the driver is the PORT, so we need to do port initalization as well as meta (struct) things here
*/
static int gpio_xmc4xxx_init(const struct device *dev)
{
    return 0; 
}

/*
    Direct API call: gpio_pin_configure

    * dev = pointer to main device struct for this driver instance
    * pin = pin number 
    * flags = init flags, defined at top of gpio.h
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EINVAL if invalid argument
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_pin_configure(const struct device *dev, gpio_pin_t pin,
			     gpio_flags_t flags)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);
    int error = 0;
    const XMC_GPIO_CONFIG_t config = convert_zephyr_to_xmc_flags(flags, port, &error);
    if (error < 0)
    {
        return error; 
    }
    XMC_GPIO_Init(port, pin, &config);
    return 0;
}

/*
    Direct API call: gpio_port_get_raw

    * dev = pointer to main device struct for this driver instance
    * value = pointer where results will be stored
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
    
    Notes:
        * Results (value) is expected as bit string encoding the current voltage level of all configured pins
          where the LSB is nominally pin 0, e.g. a result of 0x5 means that pins 0 and 2 are high and all others are low 
        * The way the Zephyr API handles GPIO read/writes is on a port-level basis, but the XMClib
          calls are on a pin-level basis, so for the most part, the XMCLib calls in this file have been unwrapped
          to expose the underlying CMSIS calls
*/
static int gpio_xmc4xxx_port_get_raw(const struct device *dev,
			    gpio_port_value_t *value)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);
    const struct gpio_xmc4xxx_config *config = DEV_CFG(dev); 

    /* adapted from: XMC_GPIO_GetInput */
    *value = (port->IN) & config->common.port_pin_mask; 
    return 0; 
}

/*
    Direct API call: gpio_port_set_masked_raw

    * dev = pointer to main device struct for this driver instance
    * mask = bitstring encoding pins to be set (LSB = pin 0)
    * value = bitmask of values to be set (e.g. 0=write low, 1=write high)
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_port_set_masked_raw(const struct device *dev,
				   gpio_port_pins_t mask,
				   gpio_port_value_t value)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);

    /* adapted from: XMC_GPIO_SetOutputHigh and XMC_GPIO_SetOutputLow */
    port->OMR = (value & mask) << SET_BIT_SHIFT | (~value & mask) << UNSET_BIT_SHIFT; 
    return 0;
}

/*
    Direct API call: gpio_port_set_raw

    * dev = pointer to main device struct for this driver instance
    * pins = bitstring encoding pins that should be set (LSB = pin 0)
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_port_set_bits_raw(const struct device *dev,
				 gpio_port_pins_t pins)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);

    /* adapted from: XMC_GPIO_SetOutputHigh */
    port->OMR = pins << SET_BIT_SHIFT;
    return 0; 
}

/*
    Direct API call: gpio_port_clear_raw

    * dev = pointer to main device struct for this driver instance
    * pins = bitstring encoding pins that should be cleared (LSB = pin 0)
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_port_clear_bits_raw(const struct device *dev,
				 gpio_port_pins_t pins)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);

    /* adapted from: XMC_GPIO_SetOutputLow */
    port->OMR = pins << UNSET_BIT_SHIFT; 
    return 0; 
}

/*
    Direct API call: gpio_port_toggle_bits

    * dev = pointer to main device struct for this driver instance
    * pins = bitstring encoding pins that should be cleared (LSB = pin 0)
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_toggle_bits(const struct device *dev,
				 gpio_port_pins_t pins)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev);

    /* adapted from: XMC_GPIO_ToggleOutput */
    port->OMR = pins << SET_BIT_SHIFT | pins << UNSET_BIT_SHIFT; 
    return 0; 
}

/*
    Intermediary API call, used to define BOTH: gpio_add_callback AND gpio_remove_callback

    * dev = pointer to main device struct for this driver instance
    * cb = pointer to callback 
    * set = true if adding callback, false if removing
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_manage_callback(const struct device *port,
			       struct gpio_callback *cb,
			       bool set)
{
    struct gpio_xmc4xxx_data* data = DEV_DATA(port);
    return gpio_manage_callback(&data->callback_list, cb, set); 
}

/*
    Direct API call: gpio_pin_interrupt_configure
        NOTE: this function can also be used to configure interrupts on pins not controlled directly by GPIO module,
          e.g. I2C, SPI, UART

    * dev = pointer to main device struct for this driver instance
    * pin = pin number 
    * mode = flag for mode: enabled to level triggered, disabled, or edge triggered
    * trig = flag for whether it interrupt should be triggered when transition to high, low, or both
    * 
    * return = 0 if successful
    *         -ENOTSUP if not supported
    *         -EBUSY if interrupt line required to configure interrupt is already in use
    *         -EINVAL if invalid argument
    *         -EIO if IO error when accessing chip
    *         -EWOULDBLOCK if operation would block
*/
static int gpio_xmc4xxx_pin_interrupt_configure(const struct device *dev,
                                        gpio_pin_t pin,
                                        enum gpio_int_mode mode,
                                        enum gpio_int_trig trig)
{
    XMC_GPIO_PORT_t *port = DEV_BASE(dev); 
    if (mode & GPIO_INT_ENABLE)
    {
        return xmc4xxx_enable_irq(port, pin, trig, gpio_xmc4xxx_isr, dev); 
    }
    else
    {
        return xmc4xxx_disable_irq(port, pin); 
    }
    
}

/* Define the API call mapping for Zephyr */
static const struct gpio_driver_api gpio_xmc4xxx_driver_api = {
	.pin_configure = gpio_xmc4xxx_pin_configure,
	.port_get_raw = gpio_xmc4xxx_port_get_raw,
	.port_set_masked_raw = gpio_xmc4xxx_port_set_masked_raw,
	.port_set_bits_raw = gpio_xmc4xxx_port_set_bits_raw,
	.port_clear_bits_raw = gpio_xmc4xxx_port_clear_bits_raw,
	.port_toggle_bits = gpio_xmc4xxx_toggle_bits,
    .manage_callback = gpio_xmc4xxx_manage_callback,
    // .get_pending_int = gpio_xmc4xxx_get_pending_int,
    .pin_interrupt_configure = gpio_xmc4xxx_pin_interrupt_configure
};

#define GPIO_XMC4XXX_INIT(index)						\
static struct gpio_xmc4xxx_data xmc4xxx_data_##index = {		\
};                                  \
									\
static const struct gpio_xmc4xxx_config xmc4xxx_config_##index = {	\
	.base = (void *)DT_INST_REG_ADDR(index),			\
    .common = {                                         \
        .port_pin_mask = DT_INST_PROP(index, pin_mask),  \
    }                                                   \
};									\
									\
DEVICE_AND_API_INIT(gpio_xmc4xxx_##index, DT_INST_LABEL(index),	\
            &gpio_xmc4xxx_init, &xmc4xxx_data_##index,	\
            &xmc4xxx_config_##index, PRE_KERNEL_1,	\
            CONFIG_KERNEL_INIT_PRIORITY_DEVICE,		\
            &gpio_xmc4xxx_driver_api);

DT_INST_FOREACH_STATUS_OKAY(GPIO_XMC4XXX_INIT)
