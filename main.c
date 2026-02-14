
/**
 * @file ee14lib_hw3.c
 * @brief This file uses ee14lib_hw3.h to implement the functions that configure and control the GPIO pins. Scanning a 4x3 keypad and using D13 to measure the state of the scan.
 * @author Ariadna Colina-Valeri
 * @date 2/10/2026
 */

#include "stm32l432xx.h"
#include "ee14lib_hw3.h"

/**
 * @brief Scan a 4x3 keypad by activating each row high and pulling up the columns. When a button is pressed, the row and column will both be low. The state of the scan is indicated by D13. 
 */
void scan_keypad() {

    // Row pins
    EE14Lib_Pin row_pins[4] = {D2, D3, D4, D5}; 

    // Column pins
    EE14Lib_Pin col_pins[3] = {D6, D9, D10}; 

    // Set up to read keypad
    for (int i=0; i<4; i++) {
        gpio_config_direction(row_pins[i], OUTPUT);
        gpio_write(row_pins[i], 1); 
    }

    for (int i=0; i<3; i++) {
        gpio_config_direction(col_pins[i], INPUT);
        gpio_config_pullup(col_pins[i], PULL_UP);
    }

    // Set up for measurement
    gpio_config_direction(D13, OUTPUT);


    static bool d13_state = false;

    // Scan the keypad and mark low when needed
    while (1) {

        // Makes LED turn on AND makes scan approriately slow
        d13_state = !d13_state;
        
        gpio_write(D13, d13_state);

        for (int row=0; row<4; row++) {

            gpio_write(row_pins[row], 0);

            for(volatile int delay=0; delay<500; delay++);

            for (int col=0; col<3; col++) {

                gpio_read(col_pins[col]);
            }

            // Set back to high when reset
            gpio_write(row_pins[row], 1);
        }
    }
}

int main() {

    scan_keypad();
    
    return 0;
}
