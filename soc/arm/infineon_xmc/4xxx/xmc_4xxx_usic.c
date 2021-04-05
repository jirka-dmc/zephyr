#include <logging/log.h>
LOG_MODULE_REGISTER(usic_xmc4xxx);

#include "xmc_4xxx_usic.h"

struct xmc4xxx_usic_input {
    XMC_GPIO_PORT_t *port;
    uint8_t pin;
    XMC_USIC_CH_t *channel;
    XMC_USIC_CH_INPUT_t input_stage;
    uint8_t input_source;
};

struct xmc4xxx_usic_output {
    XMC_GPIO_PORT_t *port;
    uint8_t pin;
    XMC_USIC_CH_t *channel;
    xmc4xxx_usic_output_mode_t output_mode;
    XMC_GPIO_MODE_t alt_func;
};


static const struct xmc4xxx_usic_input xmc4xxx_usic_inputs[] = {
    /* Use #ifdef to exclude pins that are not exposed on all packages */
    
    {P0_0, XMC_USIC1_CH1, XMC_USIC_CH_INPUT_DX0, USIC1_C1_DX0_P0_0},

    {P0_9, XMC_USIC1_CH1, XMC_USIC_CH_INPUT_DX2, USIC1_C1_DX2_P0_9},
    {P0_10, XMC_USIC1_CH1, XMC_USIC_CH_INPUT_DX1, USIC1_C1_DX1_P0_10},



    {P1_4, XMC_USIC0_CH0, XMC_USIC_CH_INPUT_DX0, USIC0_C0_DX0_P1_4},


    #ifdef P0_13 // to P0_15
    #endif

    {P2_2, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX0, USIC0_C1_DX0_P2_2},
    {P2_3, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX2, USIC0_C1_DX2_P2_3},
    {P2_4, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX1, USIC0_C1_DX1_P2_4},
    {P2_5, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX0, USIC0_C1_DX0_P2_5},

    #ifdef P2_11 // to P2_13
    #endif

    {P3_0, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX1, USIC0_C1_DX1_P3_0},
    {P3_1, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX2, USIC0_C1_DX2_P3_1},

    #ifdef P3_7 // to P3_15

    {P3_7, XMC_USIC2_CH0, XMC_USIC_CH_INPUT_DX0, USIC2_C0_DX0_P3_7},

    {P3_13, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX0, USIC0_C1_DX0_P3_13},
    {P3_14, XMC_USIC1_CH1, XMC_USIC_CH_INPUT_DX0, USIC1_C1_DX0_P3_14},

    #endif

    {P4_0, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX0, USIC0_C1_DX0_P4_0},

    #ifdef P4_2 // to P4_7

    {P4_6, XMC_USIC2_CH1, XMC_USIC_CH_INPUT_DX0, USIC2_C1_DX0_P4_6},

    #endif

    #ifdef P5_3 // to P5_6
    #endif

    #ifdef P5_8 // to P6_6

    {P6_1, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX2, USIC0_C1_DX2_P6_1},
    {P6_2, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX1, USIC0_C1_DX1_P6_2},
    {P6_3, XMC_USIC0_CH1, XMC_USIC_CH_INPUT_DX0, USIC0_C1_DX0_P6_3},

    #endif

    #ifdef P7_0 // to P9_11

    {P8_1, XMC_USIC0_CH0, XMC_USIC_CH_INPUT_DX2, USIC0_C0_DX2_P8_1},
    {P8_3, XMC_USIC0_CH0, XMC_USIC_CH_INPUT_DX1, USIC0_C0_DX2_P8_3},
    {P8_8, XMC_USIC0_CH0, XMC_USIC_CH_INPUT_DX0, USIC0_C0_DX0_P8_8},
    
    #endif

    #ifdef P15_4 // to P15_7
    #endif

    #ifdef P15_12 // to P15_15
    #endif
};


static const struct xmc4xxx_usic_output xmc4xxx_usic_outputs[] = {
    /* Use #ifdef to exclude pins that are not exposed on all packages */

    {P0_1, XMC_USIC1_CH1, xmc4xxx_usic_output_mode_DOUT0, P0_1_AF_U1C1_DOUT0},


    {P1_5, XMC_USIC0_CH0, xmc4xxx_usic_output_mode_DOUT0, P1_5_AF_U0C0_DOUT0},




    #ifdef P0_13 // to P0_15
    #endif

    {P1_13, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO3, P1_13_AF_U0C1_SELO3},
    {P1_14, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO3, P1_14_AF_U0C1_SELO2},

    {P2_3, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO0, P2_3_AF_U0C1_SELO0},
    {P2_4, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SCLKOUT, P2_4_AF_U0C1_SCLKOUT},
    {P2_5, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_DOUT0, P2_5_AF_U0C1_DOUT0},

    #ifdef P2_11 // to P2_13
    #endif

    {P3_0, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SCLKOUT, P3_0_AF_U0C1_SCLKOUT},
    {P3_0, XMC_USIC2_CH1, xmc4xxx_usic_output_mode_SELO0, P3_0_AF_U2C1_SELO0},
    {P3_1, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO0, P3_1_AF_U0C1_SELO0},

    {P3_5, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_DOUT0, P3_5_AF_U0C1_DOUT0},
    {P3_6, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SCLKOUT, P3_6_AF_U0C1_SCLKOUT},

    #ifdef P3_7 // to P3_15

    {P3_8, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO3, P3_8_AF_U0C1_SELO3},
    {P3_8, XMC_USIC2_CH0, xmc4xxx_usic_output_mode_DOUT0, P3_8_AF_U2C0_DOUT0},
    {P3_9, XMC_USIC2_CH0, xmc4xxx_usic_output_mode_SCLKOUT, P3_9_AF_U2C0_SCLKOUT},
    {P3_10, XMC_USIC2_CH0, xmc4xxx_usic_output_mode_SELO0, P3_10_AF_U2C0_SELO0},
    {P3_11, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO2, P3_11_AF_U0C1_SELO2},
    {P3_12, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO1, P3_12_AF_U0C1_SELO1},
    {P3_13, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_DOUT0, P3_13_AF_U0C1_DOUT0},
    #endif

    {P4_1, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO0, P4_1_AF_U0C1_SELO0},
    {P4_1, XMC_USIC2_CH1, xmc4xxx_usic_output_mode_SELO0, P4_1_AF_U2C1_SELO0},

    #ifdef P4_2 // to P4_7

    {P4_2, XMC_USIC2_CH1, xmc4xxx_usic_output_mode_SCLKOUT, P4_2_AF_U2C1_SCLKOUT},
    {P4_3, XMC_USIC2_CH1, xmc4xxx_usic_output_mode_SELO2,   P4_3_AF_U2C1_SELO2},
    {P4_7, XMC_USIC2_CH1, xmc4xxx_usic_output_mode_DOUT0,   P4_7_AF_U2C1_DOUT0},

    #endif

    #ifdef P5_3 // to P5_6
    #endif

    #ifdef P5_8 // to P6_6

    {P6_0, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO1, P6_0_AF_U0C1_SELO1},
    {P6_1, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SELO0, P6_1_AF_U0C1_SELO0},
    {P6_2, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_SCLKOUT, P6_2_AF_U0C1_SCLKOUT},
    {P6_4, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_DOUT0, P6_4_AF_U0C1_DOUT0},
    {P6_5, XMC_USIC0_CH1, xmc4xxx_usic_output_mode_MCLKOUT, P6_5_AF_U0C1_MCLKOUT},

    #endif

    #ifdef P7_0 // to P9_11
    #endif

    #ifdef P15_4 // to P15_7
    #endif

    #ifdef P15_12 // to P15_15
    #endif
};


int xmc4xxx_usic_get_input_src(XMC_USIC_CH_t *channel, xmc4xxx_pin_t pin)
{
    for (int i=0; i<sizeof(xmc4xxx_usic_inputs)/sizeof(xmc4xxx_usic_inputs[0]); i++) {

        if (pin.port == xmc4xxx_usic_inputs[i].port
            && pin.pin == xmc4xxx_usic_inputs[i].pin
            && channel == xmc4xxx_usic_inputs[i].channel)
        {
            /* Return the input source (A-G as 0-6) */
            return xmc4xxx_usic_inputs[i].input_source;
        }
    }
    printk("USIC input source not found for port:%p, pin:%d, ch:%p\n",
            pin.port, pin.pin, channel);
    return -1;
}


xmc4xxx_usic_output_config_t xmc4xxx_usic_get_output_cfg(XMC_USIC_CH_t *channel,
    xmc4xxx_pin_t pin, xmc4xxx_usic_output_type_t output_type)
{
    for (int i=0; i<sizeof(xmc4xxx_usic_outputs)/sizeof(xmc4xxx_usic_outputs[0]); i++) {

        /* Look for a matching entry in the lookup array */
        if (pin.port == xmc4xxx_usic_outputs[i].port
            && pin.pin == xmc4xxx_usic_outputs[i].pin
            && channel == xmc4xxx_usic_outputs[i].channel
            && (   (output_type == xmc4xxx_usic_output_type_DOUT
                    && xmc4xxx_usic_outputs[i].output_mode >= xmc4xxx_usic_output_mode_DOUT0
                    && xmc4xxx_usic_outputs[i].output_mode <= xmc4xxx_usic_output_mode_DOUT3)
                || (output_type == xmc4xxx_usic_output_type_CLKOUT
                    && xmc4xxx_usic_outputs[i].output_mode >= xmc4xxx_usic_output_mode_SCLKOUT
                    && xmc4xxx_usic_outputs[i].output_mode <= xmc4xxx_usic_output_mode_MCLKOUT)
                || (output_type == xmc4xxx_usic_output_type_SELO
                    && xmc4xxx_usic_outputs[i].output_mode >= xmc4xxx_usic_output_mode_SELO0
                    && xmc4xxx_usic_outputs[i].output_mode <= xmc4xxx_usic_output_mode_SELO7)
                )
            ) {
            /* Return the alternate function and exact output mode found in the lookup array */
            return (xmc4xxx_usic_output_config_t) {xmc4xxx_usic_outputs[i].alt_func, xmc4xxx_usic_outputs[i].output_mode};
        }
    }
    printk("USIC output alt function not found for port:%p, pin:%d, ch:%p, type:%d\n",
            pin.port, pin.pin, channel, output_type);
    return (xmc4xxx_usic_output_config_t) {-1, -1};
}









/**************************************************************************************
 * Set up interrupt handling 
 **************************************************************************************/
typedef struct callback_data {
    xmc_usic_callback_t cb; 
    const struct device *dev; 
    const IRQn_Type irq; 
} callback_data_t;

static callback_data_t isr_data[3][6] = {
    {
        { .cb=NULL, .dev=NULL, .irq=USIC0_0_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC0_1_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC0_2_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC0_3_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC0_4_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC0_5_IRQn }
    },
    {
        { .cb=NULL, .dev=NULL, .irq=USIC1_0_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC1_1_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC1_2_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC1_3_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC1_4_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC1_5_IRQn }
    },
    {
        { .cb=NULL, .dev=NULL, .irq=USIC2_0_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC2_1_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC2_2_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC2_3_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC2_4_IRQn },
        { .cb=NULL, .dev=NULL, .irq=USIC2_5_IRQn }
    }
};

/* ISRs */ 
static void general_isr(int usic, int idx)
{
    if (isr_data[usic][idx].cb)
    {
        isr_data[usic][idx].cb(isr_data[usic][idx].dev);
    }
}

static void usic0_0_isr(const void *arg)
{
    general_isr(0, 0);
}

static void usic0_1_isr(const void *arg)
{
    general_isr(0, 1);
}

static void usic0_2_isr(const void *arg)
{
    general_isr(0, 2);
}

static void usic0_3_isr(const void *arg)
{
    general_isr(0, 3);
}

static void usic0_4_isr(const void *arg)
{
    general_isr(0, 4);
}

static void usic0_5_isr(const void *arg)
{
    general_isr(0, 5);
}

static void usic1_0_isr(const void *arg)
{
    general_isr(1, 0);
}

static void usic1_1_isr(const void *arg)
{
    general_isr(1, 1);
}

static void usic1_2_isr(const void *arg)
{
    general_isr(1, 2);
}

static void usic1_3_isr(const void *arg)
{
    general_isr(1, 3);
}

static void usic1_4_isr(const void *arg)
{
    general_isr(1, 4);
}

static void usic1_5_isr(const void *arg)
{
    general_isr(1, 5);
}

static void usic2_0_isr(const void *arg)
{
    general_isr(2, 0);
}

static void usic2_1_isr(const void *arg)
{
    general_isr(2, 1);
}

static void usic2_2_isr(const void *arg)
{
    general_isr(2, 2);
}

static void usic2_3_isr(const void *arg)
{
    general_isr(2, 3);
}

static void usic2_4_isr(const void *arg)
{
    general_isr(2, 4);
}

static void usic2_5_isr(const void *arg)
{
    general_isr(2, 5);
}

static void connect_all_isrs()
{
    int priority = 0;
    IRQ_CONNECT(USIC0_0_IRQn, priority, usic0_0_isr, 0, 0);
    IRQ_CONNECT(USIC0_1_IRQn, priority, usic0_1_isr, 0, 0);
    IRQ_CONNECT(USIC0_2_IRQn, priority, usic0_2_isr, 0, 0);
    IRQ_CONNECT(USIC0_3_IRQn, priority, usic0_3_isr, 0, 0);
    IRQ_CONNECT(USIC0_4_IRQn, priority, usic0_4_isr, 0, 0);
    IRQ_CONNECT(USIC0_5_IRQn, priority, usic0_5_isr, 0, 0);
    IRQ_CONNECT(USIC1_0_IRQn, priority, usic1_0_isr, 0, 0);
    IRQ_CONNECT(USIC1_1_IRQn, priority, usic1_1_isr, 0, 0);
    IRQ_CONNECT(USIC1_2_IRQn, priority, usic1_2_isr, 0, 0);
    IRQ_CONNECT(USIC1_3_IRQn, priority, usic1_3_isr, 0, 0);
    IRQ_CONNECT(USIC1_4_IRQn, priority, usic1_4_isr, 0, 0);
    IRQ_CONNECT(USIC1_5_IRQn, priority, usic1_5_isr, 0, 0);
    IRQ_CONNECT(USIC2_0_IRQn, priority, usic2_0_isr, 0, 0);
    IRQ_CONNECT(USIC2_1_IRQn, priority, usic2_1_isr, 0, 0);
    IRQ_CONNECT(USIC2_2_IRQn, priority, usic2_2_isr, 0, 0);
    IRQ_CONNECT(USIC2_3_IRQn, priority, usic2_3_isr, 0, 0);
    IRQ_CONNECT(USIC2_4_IRQn, priority, usic2_4_isr, 0, 0);
    IRQ_CONNECT(USIC2_5_IRQn, priority, usic2_5_isr, 0, 0);
}










static int get_usic_idx(uint32_t channel)
{
    switch(channel)
    {
        case USIC0_CH0_BASE:
        case USIC0_CH1_BASE: /* intentional fall through */ 
            return 0; 
        case USIC1_CH0_BASE:
        case USIC1_CH1_BASE: /* intentional fall through */ 
            return 1; 
        case USIC2_CH0_BASE:
        case USIC2_CH1_BASE: /* intentional fall through */ 
            return 2; 
        default:
            return -1; 
    }
}

static int get_usic_channel(uint32_t channel)
{
    switch(channel)
    {
        case USIC0_CH0_BASE:
        case USIC1_CH0_BASE: /* intentional fall through */
        case USIC2_CH0_BASE: /* intentional fall through */
            return 0; 
        case USIC0_CH1_BASE:
        case USIC1_CH1_BASE: /* intentional fall through */
        case USIC2_CH1_BASE: /* intentional fall through */
            return 1; 
        default:
            return -1; 
    }
}


int configure_irq(const struct device *dev, XMC_USIC_CH_t *channel, xmc_usic_callback_t cb, int flags, irq_info_t *info)
{
    int usic_idx = get_usic_idx((uint32_t)channel); 
    if (usic_idx < 0)
    {
        return usic_idx; 
    }

    /* find the first available ISR */ 
    int idx = -1; 
    for (int i=0; i<ARRAY_SIZE(isr_data[usic_idx]); i++)
    {
        if (!isr_data[usic_idx][i].cb)
        {
            idx = i; 
            isr_data[usic_idx][i].cb = cb; 
            isr_data[usic_idx][i].dev = dev; 
            break; 
        }
    }

    if (idx < 0)
    {
        return -1; 
    } 

    /* let caller know what USIC IRQ and index they were assigned in case they need to do additional set up */ 
    info->idx = idx; 
    info->irq = isr_data[usic_idx][idx].irq;
    return idx; 
}

int disable_irq(XMC_USIC_CH_t *channel, int idx)
{
    int usic_idx = get_usic_idx((uint32_t)channel);
    if (usic_idx < 0)
    {
        return usic_idx;
    }

    irq_disable(isr_data[usic_idx][idx].irq);
    isr_data[usic_idx][idx].cb = NULL; 
    isr_data[usic_idx][idx].dev = NULL; 

    return 0;
}

/******************************************************************************************************
 * Set up driver as Zephyr "device"
 *****************************************************************************************************/
struct xmc4xxx_usic_config {

};

struct xmc4xxx_usic_data {
    int error; 
    xmc4xxx_usic_channel_config_t configs[3][2];
};


static const struct xmc4xxx_usic_config usic_config;
static struct xmc4xxx_usic_data usic_data = {
    .error = 0,
    .configs = {
        { /* USIC0 */
            { .dev=NULL }, /* CH0 */
            { .dev=NULL }  /* CH1 */
        },
        { /* USIC1 */
            { .dev=NULL }, /* CH0 */
            { .dev=NULL }  /* CH1 */
        },
        { /* USIC2 */ 
            { .dev=NULL }, /* CH0 */
            { .dev=NULL }  /* CH1 */ 
        }
    }
};

static int xmc4xxx_usic_init(const struct device *dev);
DEVICE_INIT(usic_xmc4xxx, "USIC", 
    &xmc4xxx_usic_init, &usic_data, &usic_config, 
    PRE_KERNEL_2, /* needs to be initialized last to give other devices opportunity to request a channel */ 
    CONFIG_KERNEL_INIT_PRIORITY_DEVICE
);

/*************************************************************************************************
 * Functions
 *************************************************************************************************/
int xmc4xxx_usic_request_irq(xmc4xxx_usic_callback_st_t cb_config, irq_info_t *irq_info)
{
    int usic_idx = get_usic_idx((uint32_t)cb_config.channel);
    if (usic_idx < 0)
    {
        return usic_idx; 
    }
    if (configure_irq(cb_config.dev, cb_config.channel, cb_config.cb, cb_config.flags, irq_info) < 0)
    {
        return -1; 
    }

    return 0; 
}

int xmc4xxx_usic_request_channel(xmc4xxx_usic_channel_config_t config)
{
    int usic_idx = get_usic_idx((uint32_t)config.channel);
    int usic_channel = get_usic_channel((uint32_t)config.channel);
    if (usic_idx < 0 || usic_channel < 0)
    {
        /* we were not passed a valid USIC channel */ 
        usic_data.error = -1; 
        return -1; 
    }

    /* if either the USIC channel is already taken, throw an error */ 
    if (usic_data.configs[usic_idx][usic_channel].dev!=NULL)
    {
        usic_data.error = -1; 
        return -1; 
    }

    usic_data.configs[usic_idx][usic_channel] = config; 
    return 0; 
}


static void configure_fifo(XMC_USIC_CH_t *channel, int size, int start, int tx_limit, int rx_limit)
{
    /* neither the transmit or receive buffer are required */ 
    if (tx_limit < 0 && rx_limit < 0)
    {
        return; 
    }

    /* 
     * The enum for buffer sizes are organized as powers of 2, 
     * so subtracting 1 is equivalent to dividing the buffer in half
     */
    int actual_size = (tx_limit >= 0 && rx_limit >= 0) ? size - 1 : size; 
    if (tx_limit >= 0)
    {
        XMC_USIC_CH_TXFIFO_Configure(channel, start+size, actual_size, tx_limit); 
    }

    if (rx_limit >= 0)
    {
        XMC_USIC_CH_RXFIFO_Configure(channel, start, actual_size, rx_limit); 
    }
}

static int configure_usic(xmc4xxx_usic_channel_config_t config[2])
{
    /* helper for below function */
    int error = 0; 

    if (config[0].dev!=NULL)
    {
        XMC_USIC_CH_Enable(config[0].channel);  
    }

    if (config[1].dev!=NULL)
    {
        XMC_USIC_CH_Enable(config[1].channel); 
    } 

    /* configure a FIFO if it was requested for at least one of the two channels */
    bool first_channel_needs_fifo = (config[0].dev!=NULL) && (config[0].tx_fifo_limit >= 0 || config[0].rx_fifo_limit >= 0);
    bool second_channel_needs_fifo = (config[1].dev!=NULL) && (config[1].tx_fifo_limit >= 0 || config[1].rx_fifo_limit >= 0); 

    int fifo_size = (first_channel_needs_fifo && second_channel_needs_fifo)? XMC_USIC_CH_FIFO_SIZE_32WORDS : XMC_USIC_CH_FIFO_SIZE_64WORDS; 
    int start = 0; 
    if (first_channel_needs_fifo)
    {
        configure_fifo(config[0].channel, fifo_size, start, config[0].tx_fifo_limit, config[0].rx_fifo_limit);
        start += fifo_size; 
    }
    if (second_channel_needs_fifo)
    {
        configure_fifo(config[1].channel, fifo_size, start, config[1].tx_fifo_limit, config[1].rx_fifo_limit);
    }

    /* configure all of the interrupts that were specified */ 
    for (int i=0; i<2; i++)
    {
        if (config[i].init_cb)
        {
            error += config[i].init_cb(config[i].dev); 
        }
    }

    /* call device's specified init function after configuring interrupts, if applicable */ 
    return error; 
}


static int xmc4xxx_usic_init(const struct device *dev)
{
    /* check that we did not encounter an error during pre-processing */
    if (usic_data.error < 0)
    {
        return usic_data.error; 
    }

    /* set up all interrupts */ 
    connect_all_isrs(); 

    /* call all passed in init callbacks */ 
    int error = 0; 
    for (int i=0; i<3; i++)
    {
        error += configure_usic(usic_data.configs[i]);
    }

    return error;
}
