
/**
 * @file ee14lib_hw3.c
 * @brief These functions are used to configure and control the GPIO pins. The direction of a pin, configuring pull-up/pull-down resistors, writing to a pin, and reading from a pin.
 * @author Ariadna Colina-Valeri
 * @date 2/10/2026
 */

#include "ee14lib_hw3.h"

// Pins are in our Nucleo list
#define NUM_PINS 22 // A0-A7, D0-D13

/**
 * @brief This function maps the Nucleo pin number to GPIO port
 */
static GPIO_TypeDef * g_GPIO_port[D13+1] = {
  GPIOA,GPIOA,GPIOA,GPIOA,  // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  GPIOA,GPIOA,GPIOA,GPIOA,  // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  GPIOA,GPIOA,GPIOA,GPIOB,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  GPIOB,GPIOB,GPIOB,GPIOC,  // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  GPIOC,GPIOA,GPIOA,GPIOB,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  GPIOB,GPIOB               // D12=PB4,D13=PB3.
};

/**
 * @brief This function maps the Nucleo pin number to GPIO pin, using this function and g_GPIO_port[] from above, we can translate a Nucleo pin name into the chip's actual GPIO port and pin number.
 */
static uint8_t g_GPIO_pin[D13+1] = {
  0,1,3,4,    // A0=PA0,A1=PA1,A2=PA3,A3=PA4
  5,6,7,2,    // A4=PA5,A5=PA6,A6=PA7,A7=PA2
  10,9,12,0,  // D0=PA10,D1=PA9,D2=PA12,D3=PB0
  7,6,1,14,   // D4=PB7,D5=PB6,D6=PB1,D7=PC14
  15,8,11,5,  // D8=PC15,D9=PA8,D10=PA11,D11=PB5
  4,3         // D12=PB4,D13=PB3.
};

/**
 * @brief A pointer to the GPIO port to enable (ex. GPIOA, GPIOB, etc.)
 */
static void gpio_enable_port(GPIO_TypeDef *gpio) {
    uint32_t field;
    if (gpio==GPIOA)      field=RCC_AHB2ENR_GPIOAEN;
    else if (gpio==GPIOB) field=RCC_AHB2ENR_GPIOBEN;
    else if (gpio==GPIOC) field=RCC_AHB2ENR_GPIOCEN;
    else           field=RCC_AHB2ENR_GPIOHEN;
    RCC->AHB2ENR |= field; // Turn on the GPIO clock
    (void)RCC->AHB2ENR; // Dummy read to ensure clock is stable
}

/**
 * @brief This function configures the direction of the GPIO pin. This way the Nucleo pin can be used as an input or output. The direction parameter should be either INPUT or OUTPUT.
 */
EE14Lib_Err gpio_config_direction(EE14Lib_Pin pin, uint32_t direction)
{
    // Check for valid
    if (direction != INPUT && direction != OUTPUT) {
        return EE14Lib_ERR_INVALID_CONFIG;
    }

    // Get the GPIO port and pin 
    GPIO_TypeDef* port = g_GPIO_port[pin];
    
    uint8_t pin_offset = g_GPIO_pin[pin];

    // Enable 
    gpio_enable_port(port);

    // Clear two bits 
    port->MODER &= ~(0b11 << pin_offset*2); 

    // Set the bits
    port->MODER |=  (direction << pin_offset*2);

    return EE14Lib_Err_OK;
}


/**
 * @brief The function configures the pull-up/pull-down resistors for a GPIO pin. This is so that when the pin is an input, it can be pulled up to read a high value at all times. The mode parameter should be either PULL_OFF, PULL_UP, or PULL_DOWN.
 */
EE14Lib_Err gpio_config_pullup(EE14Lib_Pin pin, uint32_t mode)
{
    // Check for valid
    if (mode != PULL_OFF && mode != PULL_UP && mode != PULL_DOWN) {
        return EE14Lib_ERR_INVALID_CONFIG;
    }
    // Get the GPIO port and pin 
    GPIO_TypeDef* port = g_GPIO_port[pin];
    uint8_t pin_offset = g_GPIO_pin[pin];

    // Enable the GPIO port 
    gpio_enable_port(port);

    // Clear two bits
    port->PUPDR &= ~(0b11 << pin_offset*2);

    // Set the bits
    port->PUPDR |=  (mode << pin_offset*2);

    return EE14Lib_Err_OK;
}

/**
 * @brief Only if the pin is an output, write a value to the pin that will set the state of the signal. The value parameter should be either 0 or 1.
 */
void gpio_write(EE14Lib_Pin pin, bool value)
{
    // Check for valid 
    if (value != 0 && value != 1) {
        return;
    }
    // Get the GPIO port and pin 
    GPIO_TypeDef* port = g_GPIO_port[pin];
    uint8_t pin_offset = g_GPIO_pin[pin];

    if (value) {
        // Set the pin
        port->BSRR = 1 << pin_offset;
    } else {
        // Reset the pin
        port->BRR = 1 << pin_offset;
    }
    
}

/**
 * @brief Read the value of a single GPIO pin so that the written state can be read back.
 */
bool gpio_read(EE14Lib_Pin pin)
{
    // Check for valid 
    if (pin < 0 || pin >= NUM_PINS) {
        return false; 
    }
    // Get the GPIO port and pin
    GPIO_TypeDef* port = g_GPIO_port[pin];
    uint8_t pin_offset = g_GPIO_pin[pin];

    // location of the pin, push forward
    return (port->IDR >> pin_offset) & 1;
}
