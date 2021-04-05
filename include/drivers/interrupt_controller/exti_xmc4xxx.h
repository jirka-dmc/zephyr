#ifndef ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_EXTI_XMC4XXX_H_
#define ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_EXTI_XMC4XXX_H_

/* Zephyr includes */ 
#include <drivers/gpio.h>

/* XMC includes */ 
#include <xmc_gpio.h>


/* Define the main interface for interrupt handling for the XMC4 series
 * This file will be primarily used by the GPIO driver
 */
typedef void (*xmc_callback_t)(uint32_t pin_mask, const struct device *dev); 
int xmc4xxx_enable_irq(XMC_GPIO_PORT_t* port, uint32_t pin, enum gpio_int_trig trig, xmc_callback_t cb, const struct device *dev);
int xmc4xxx_disable_irq(XMC_GPIO_PORT_t* port, uint32_t pin);

#endif /* ZEPHYR_DRIVERS_INTERRUPT_CONTROLLER_INTC_EXTI_XMC4XXX_H_ */ 