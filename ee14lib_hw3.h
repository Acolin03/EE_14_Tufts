
/* Header file for EE14lib functions [Homework 3, GPIO only]. */

#ifndef EE14LIB_H
#define EE14LIB_H

#include "stm32l432xx.h"
#include <stdint.h>
#include <stdbool.h>

// Pin names on the Nucleo silkscreen, copied from the Arduino Nano form factor
typedef enum {A0,A1,A2,A3,A4,A5,A6,A7,
	  D0,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13 } EE14Lib_Pin ;

// Some common error types
typedef int32_t EE14Lib_Err;

#define EE14Lib_Err_OK 0
#define EE14Lib_Err_INEXPLICABLE_FAILURE -1
#define EE14Lib_Err_NOT_IMPLEMENTED -2
#define EE14Lib_ERR_INVALID_CONFIG -3

// GPIO modes
#define INPUT 0b00
#define OUTPUT 0b01

// GPIO pullup modes
#define PULL_OFF 0b00
#define PULL_UP 0b01
#define PULL_DOWN 0b10
// Both on is an error

EE14Lib_Err gpio_config_direction(EE14Lib_Pin pin, uint32_t direction);
EE14Lib_Err gpio_config_pullup(EE14Lib_Pin pin, uint32_t mode);
void gpio_write(EE14Lib_Pin pin, bool value);
bool gpio_read(EE14Lib_Pin pin);

#endif
