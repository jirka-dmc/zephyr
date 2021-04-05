/* Zephyr-specific include */ 
#include <device.h>

/* XMC (Infineon) specific includes */ 
#include <drivers/interrupt_controller/exti_xmc4xxx.h>
#include <xmc_eru.h>


/***************************************************************************************
 * ERU pin/port mapping
 * *************************************************************************************/

typedef struct XMC_ERU_INIT 
{
    /* associative information about device */
    XMC_GPIO_PORT_t* port;
    gpio_pin_t pin;  

    /* information needed to initialize/identify ERU */
    XMC_ERU_t* eru;
    uint8_t channel;
    uint32_t source;
    uint8_t input; 
} XMC_ERU_INIT_t;

const XMC_ERU_INIT_t GPIO_ERUS[] = 
{
    { .port=XMC_GPIO_PORT0, .pin=0,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL0_INPUTB_P0_0  }, /* P0.0 */
    { .port=XMC_GPIO_PORT0, .pin=1,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL0_INPUTA_P0_1  }, /* P0.1 */
    { .port=XMC_GPIO_PORT0, .pin=2,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL3_INPUTB_P0_2  }, /* P0.2 */
    { .port=XMC_GPIO_PORT0, .pin=3,  .eru=XMC_ERU1, .channel=3, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU1_ETL3_INPUTB_P0_3  }, /* P0.3 */
    { .port=XMC_GPIO_PORT0, .pin=4,  .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL2_INPUTB_P0_4  }, /* P0.4 */
    { .port=XMC_GPIO_PORT0, .pin=5,  .eru=XMC_ERU1, .channel=3, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU1_ETL3_INPUTA_P0_5  }, /* P0.5 */
    { .port=XMC_GPIO_PORT0, .pin=6,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL3_INPUTB_P0_6  }, /* P0.6 */
    { .port=XMC_GPIO_PORT0, .pin=7,  .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL2_INPUTB_P0_7  }, /* P0.7 */
    { .port=XMC_GPIO_PORT0, .pin=8,  .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL2_INPUTA_P0_8  }, /* P0.8 */
    { .port=XMC_GPIO_PORT0, .pin=9,  .eru=XMC_ERU0, .channel=1, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL1_INPUTB_P0_9  }, /* P0.9 */
    { .port=XMC_GPIO_PORT0, .pin=10, .eru=XMC_ERU0, .channel=1, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL1_INPUTA_P0_10 }, /* P0.10 */
    { .port=XMC_GPIO_PORT0, .pin=11, .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL3_INPUTA_P0_11 }, /* P0.11 */
    { .port=XMC_GPIO_PORT0, .pin=12, .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL2_INPUTB_P0_12 }, /* P0.12 */
    { .port=XMC_GPIO_PORT0, .pin=13, .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL2_INPUTA_P0_13 }, /* P0.13 */
    { .port=XMC_GPIO_PORT1, .pin=0,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL3_INPUTB_P1_0  }, /* P1.0 */
    { .port=XMC_GPIO_PORT1, .pin=1,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL3_INPUTA_P1_1  }, /* P1.1 */
    { .port=XMC_GPIO_PORT1, .pin=2,  .eru=XMC_ERU1, .channel=2, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU1_ETL2_INPUTB_P1_2  }, /* P1.2 */
    { .port=XMC_GPIO_PORT1, .pin=3,  .eru=XMC_ERU1, .channel=2, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU1_ETL2_INPUTA_P1_3  }, /* P1.3 */
    { .port=XMC_GPIO_PORT1, .pin=4,  .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL2_INPUTB_P1_4  }, /* P1.4 */
    { .port=XMC_GPIO_PORT1, .pin=5,  .eru=XMC_ERU0, .channel=2, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL2_INPUTA_P1_5  }, /* P1.5 */
    { .port=XMC_GPIO_PORT1, .pin=5,  .eru=XMC_ERU1, .channel=0, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU1_ETL0_INPUTA_P1_5  }, /* P1.5 */
    { .port=XMC_GPIO_PORT1, .pin=15, .eru=XMC_ERU1, .channel=1, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU1_ETL1_INPUTA_P1_15 }, /* P1.15 */
    { .port=XMC_GPIO_PORT2, .pin=0,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL0_INPUTB_P2_0  }, /* P2.0 */
    { .port=XMC_GPIO_PORT2, .pin=1,  .eru=XMC_ERU1, .channel=0, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU1_ETL0_INPUTB_P2_1  }, /* P2.1 */
    { .port=XMC_GPIO_PORT2, .pin=2,  .eru=XMC_ERU0, .channel=1, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL1_INPUTB_P2_2  }, /* P2.2 */
    { .port=XMC_GPIO_PORT2, .pin=3,  .eru=XMC_ERU0, .channel=1, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL1_INPUTA_P2_3  }, /* P2.3 */
    { .port=XMC_GPIO_PORT2, .pin=4,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL0_INPUTB_P2_4  }, /* P2.4 */
    { .port=XMC_GPIO_PORT2, .pin=5,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL0_INPUTA_P2_5  }, /* P2.5 */
    { .port=XMC_GPIO_PORT2, .pin=6,  .eru=XMC_ERU0, .channel=1, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL1_INPUTB_P2_6  }, /* P2.6 */
    { .port=XMC_GPIO_PORT2, .pin=7,  .eru=XMC_ERU1, .channel=1, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU1_ETL1_INPUTB_P2_7  }, /* P2.7 */
    { .port=XMC_GPIO_PORT3, .pin=1,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL0_INPUTB_P3_1  }, /* P3.1 */
    { .port=XMC_GPIO_PORT3, .pin=2,  .eru=XMC_ERU0, .channel=0, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL0_INPUTA_P3_2  }, /* P3.2 */
    { .port=XMC_GPIO_PORT3, .pin=5,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_B, .input=ERU0_ETL3_INPUTB_P3_5  }, /* P3.5 */
    { .port=XMC_GPIO_PORT3, .pin=6,  .eru=XMC_ERU0, .channel=3, .source=XMC_ERU_ETL_SOURCE_A, .input=ERU0_ETL3_INPUTA_P3_6  }  /* P3.6 */
};

/* List of available ERU interrupts, organized by ERU number */
typedef struct ERU_IRQ_INFORMATION {
    const IRQn_Type irq;
    xmc_callback_t cb; 
    XMC_GPIO_PORT_t *port;
    gpio_pin_t pin;  
    const struct device *dev; // TODO: think of a better way to do this
} ERU_IRQ_INFOMRATION_t; 

static ERU_IRQ_INFOMRATION_t ERU_IRQs[2][4] = {
    {
        { .irq=ERU0_0_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU0_1_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU0_2_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU0_3_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL }
    },
    {
        { .irq=ERU1_0_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU1_1_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU1_2_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL },
        { .irq=ERU1_3_IRQn, .cb=NULL, .port=NULL, .pin=-1, .dev=NULL }
    }
};

/* 
 * ERU channel meta information
 * Note that, while the array size is the same, this is different information becuase this is per ERU channel
 * vs. IRQ 
 */
static bool ERU_CHANNELS_TAKEN[2][4] = {
    { false, false, false, false },
    { false, false, false, false }
};

/************************************************************
 * Declaration of helper functions (defined at EOF)
 ************************************************************/
static XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t get_ogu_channel(IRQn_Type irq);
static XMC_ERU_ETL_EDGE_DETECTION_t get_trigger_flag(enum gpio_int_trig trig);

/*************************************************************
 * Main functions 
 ************************************************************/
int xmc4xxx_enable_irq(XMC_GPIO_PORT_t* port, uint32_t pin, enum gpio_int_trig trig, xmc_callback_t cb, const struct device *dev)
{
    int error = -ENOTSUP; 
    const XMC_ERU_INIT_t* eru_init_params = NULL; 
    for (int i=0; i<ARRAY_SIZE(GPIO_ERUS); i++)
    {
        if (GPIO_ERUS[i].port==port && GPIO_ERUS[i].pin==pin)
        {
            /* even if there is a valid configuration for this port+pin, the ERU channel might already be in use */
            int eru_num = (GPIO_ERUS[i].eru==XMC_ERU0)? 0: 1; 
            if (!ERU_CHANNELS_TAKEN[eru_num][GPIO_ERUS[i].channel])
            {
                ERU_CHANNELS_TAKEN[eru_num][GPIO_ERUS[i].channel] = true; 
                eru_init_params = &GPIO_ERUS[i]; 
                error = 0; 
                break; 
            }
            else 
            {
                /* 
                 we can't immediately return here because some pins have multiple ERU configuration options,
                 so we need to iterate to the end of the array to check if there is a valid alternative option
                 */ 
                error = -EBUSY; 
            }
        }
    }

    if (error < 0)
    {
        return error; 
    }

    int irq = -1;
    int eru_num = (eru_init_params->eru==XMC_ERU0)? 0: 1; 
    for (int i=0; i<ARRAY_SIZE(ERU_IRQs); i++)
    {
        if (ERU_IRQs[eru_num][i].cb==NULL)
        {
            irq = ERU_IRQs[eru_num][i].irq;
            ERU_IRQs[eru_num][i].cb = cb; 
            ERU_IRQs[eru_num][i].port = port; 
            ERU_IRQs[eru_num][i].pin = pin; 
            ERU_IRQs[eru_num][i].dev = dev; 
            break; 
        }
    }

    /* check that we found a valid interrupt number */
    if (irq<0)
    {
        return -ENOTSUP; 
    }

    /* 
     * Note: there is a difference between the ERU channel and the IRQ "channel", 
     * even though the init function below calls them the same 
     */
    XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t ogu_channel = get_ogu_channel(irq); 
    XMC_ERU_ETL_CONFIG_t generator_config = 
    {
        .input_a = eru_init_params->input,
        .input_b = eru_init_params->input,
        .source = eru_init_params->source, 
        .edge_detection = get_trigger_flag(trig),
        .status_flag_mode = XMC_ERU_ETL_STATUS_FLAG_MODE_HWCTRL, /* allow hardware to clear status flag so we don't have to */
        .enable_output_trigger = true, /* allow ERU to trigger output */
        .output_trigger_channel = ogu_channel /* configure correct output based on selected IRQn */
    };
    XMC_ERU_ETL_Init(eru_init_params->eru, eru_init_params->channel, &generator_config);

    /* now enable the output of the ERU to trigger an ISR */ 
    XMC_ERU_OGU_CONFIG_t output_isr_config =
    {
        .service_request = XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER
    };
    XMC_ERU_OGU_Init(eru_init_params->eru, ogu_channel, &output_isr_config);

    /* let Zephyr handle enabling the IRQ */
    irq_enable(irq); 
    return 0; 
}

int xmc4xxx_disable_irq(XMC_GPIO_PORT_t* port, uint32_t pin)
{
    int irq = -1; 

    /* make sure to clear the callback in the array so that it can be reused */
    for (int i=0; i<ARRAY_SIZE(ERU_IRQs); i++)
    {
        for (int j=0; j<ARRAY_SIZE(ERU_IRQs[i]); j++)
        {
            if (ERU_IRQs[i][j].port==port && ERU_IRQs[i][j].pin==pin) 
            {
                ERU_IRQs[i][j].port = NULL; 
                ERU_IRQs[i][j].pin = -1; 
                ERU_IRQs[i][j].cb = NULL;
                irq = ERU_IRQs[i][j].irq; 
                break; 
            }
        }
    }

    /* let Zephyr handle disabling the IRQ */ 
    if (irq < 0)
    {
        return -1; 
    }
    irq_disable(irq); 
    return 0; 
}



/*************************************************************
 * Define all of the actual ISR placeholders
**************************************************************/
static inline void general_isr(int eru, int eru_idx)
{
    if (ERU_IRQs[eru][eru_idx].cb)
    {
        /* TODO: verify if "pin" is supposed to refer to the absolute pin or the pin mask */ 
        ERU_IRQs[eru][eru_idx].cb(BIT(ERU_IRQs[eru][eru_idx].pin), ERU_IRQs[eru][eru_idx].dev); 
    }
}

static void eru0_0_isr(const void *arg)
{
    general_isr(0, 0);
}

static void eru0_1_isr(const void *arg)
{
    general_isr(0, 1); 
}

static void eru0_2_isr(const void *arg)
{
    general_isr(0, 2);
}

static void eru0_3_isr(const void *arg)
{
    general_isr(0, 3);
}

static void eru1_0_isr(const void *arg)
{
    general_isr(1, 0);
}

static void eru1_1_isr(const void *arg)
{
    general_isr(1, 1);
}

static void eru1_2_isr(const void *arg)
{
    general_isr(1, 2);
}

static void eru1_3_isr(const void *arg)
{
    general_isr(1, 3);
}

static void connect_all_irqs(const struct device* dev)
{
    IRQ_CONNECT(ERU0_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru0_0_isr, 0, 0); 
    IRQ_CONNECT(ERU0_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru0_1_isr, 0, 0); 
    IRQ_CONNECT(ERU0_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru0_2_isr, 0, 0); 
    IRQ_CONNECT(ERU0_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru0_3_isr, 0, 0); 
    IRQ_CONNECT(ERU1_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru1_0_isr, 0, 0); 
    IRQ_CONNECT(ERU1_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru1_1_isr, 0, 0); 
    IRQ_CONNECT(ERU1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru1_2_isr, 0, 0); 
    IRQ_CONNECT(ERU1_3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63, 0), eru1_3_isr, 0, 0); 
}

/********************************************************************************
 * Set up device
 *******************************************************************************/
/**
 * Initialize interrupt controller as a driver 
 */
static int xmc4xxx_exti_init(const struct device *dev)
{
	connect_all_irqs(dev);
	return 0;
}

DEVICE_INIT(exti_stm32, "XMC4XX Interrupt Controller", xmc4xxx_exti_init,
	    NULL, NULL,
	    PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

/***************************************************************************
 * Private helper functions
 * *************************************************************************/
static XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL_t get_ogu_channel(IRQn_Type irq)
{
    switch (irq)
    {
        case ERU0_0_IRQn:
        case ERU1_0_IRQn: /* intentional fall-through */
            return XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0; 
            break;
        case ERU0_1_IRQn:
        case ERU1_1_IRQn: /* intentional fall-through */
            return XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1; 
            break;
        case ERU0_2_IRQn:
        case ERU1_2_IRQn: /* intentional fall-through */
            return XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL2; 
            break;
        case ERU0_3_IRQn:
        case ERU1_3_IRQn: /* intentional fall-through */
            return XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL3; 
            break;
        default:
            return -1; 
            break; 
    } 
}

static XMC_ERU_ETL_EDGE_DETECTION_t get_trigger_flag(enum gpio_int_trig trig)
{
    switch (trig)
    {
        case GPIO_INT_TRIG_LOW: /* falling edge/active low */
            return XMC_ERU_ETL_EDGE_DETECTION_FALLING; 
            break;
        case GPIO_INT_TRIG_HIGH: /* rising edge/active high */
            return XMC_ERU_ETL_EDGE_DETECTION_RISING;
            break;
        case GPIO_INT_TRIG_BOTH: /* rising and falling edge */
            return XMC_ERU_ETL_EDGE_DETECTION_BOTH; 
            break;
        default:
            return XMC_ERU_ETL_EDGE_DETECTION_DISABLED;
    }
}
