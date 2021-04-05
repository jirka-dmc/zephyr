#ifndef XMC_4XXX_USIC_H
#define XMC_4XXX_USIC_H


#include <device.h>


#include <xmc_usic.h>
#include <xmc_gpio.h>

typedef struct xmc4xxx_pin {
	XMC_GPIO_PORT_t *port;
	uint8_t pin;
} xmc4xxx_pin_t;

typedef enum xmc4xxx_usic_output_mode {
    none = -1,
    xmc4xxx_usic_output_mode_DOUT0,
    xmc4xxx_usic_output_mode_DOUT1,
    xmc4xxx_usic_output_mode_DOUT2,
    xmc4xxx_usic_output_mode_DOUT3,
    xmc4xxx_usic_output_mode_SCLKOUT,
    xmc4xxx_usic_output_mode_MCLKOUT,
    xmc4xxx_usic_output_mode_SELO0,
    xmc4xxx_usic_output_mode_SELO1,
    xmc4xxx_usic_output_mode_SELO2,
    xmc4xxx_usic_output_mode_SELO3,
    xmc4xxx_usic_output_mode_SELO4,
    xmc4xxx_usic_output_mode_SELO5,
    xmc4xxx_usic_output_mode_SELO6,
    xmc4xxx_usic_output_mode_SELO7
} xmc4xxx_usic_output_mode_t;

typedef enum xmc4xxx_usic_output_type {
    xmc4xxx_usic_output_type_DOUT,
    xmc4xxx_usic_output_type_CLKOUT,
    xmc4xxx_usic_output_type_SELO
} xmc4xxx_usic_output_type_t;

typedef struct xmc4xxx_usic_output_config {
    XMC_GPIO_MODE_t alt_func;
    xmc4xxx_usic_output_mode_t output_mode;
} xmc4xxx_usic_output_config_t;

int xmc4xxx_usic_get_input_src(XMC_USIC_CH_t *channel,
	xmc4xxx_pin_t pin);

xmc4xxx_usic_output_config_t xmc4xxx_usic_get_output_cfg(XMC_USIC_CH_t *channel,
    xmc4xxx_pin_t pin, xmc4xxx_usic_output_type_t output_type);




// TODO: we'll probably need some sort of config??
typedef void (*xmc_usic_callback_t)(const struct device *dev); 

typedef struct irq_info {
    int idx; 
    IRQn_Type irq; 
} irq_info_t; 
typedef int (*xmc_init_callback_t)(const struct device *dev); 


typedef struct xmc4xxx_usic_callback_st {
    const struct device *dev; 
    XMC_USIC_CH_t *channel;
    xmc_usic_callback_t cb; 
    int flags; 
} xmc4xxx_usic_callback_st_t;






typedef struct xmc4xxx_usic_channel_config {
    const struct device *dev; 

    /* required for configuration of channel */ 
    XMC_USIC_CH_t *channel; 
    uint32_t tx_fifo_limit; /* -1 when Tx FIFO is not required */ 
    uint32_t rx_fifo_limit; /* -1 when Rx FIFO is not required */ 

    /* for post-initialization */ 
    xmc_init_callback_t init_cb; 
} xmc4xxx_usic_channel_config_t;



/* main interface for requesting access to USIC */ 
int xmc4xxx_usic_request_channel(xmc4xxx_usic_channel_config_t config);

/* request an IRQ */ 
int xmc4xxx_usic_request_irq(xmc4xxx_usic_callback_st_t cb_config, irq_info_t *irq_info); 



#endif