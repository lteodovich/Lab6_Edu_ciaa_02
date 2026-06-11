/************************************************************************************************
Copyright (c) 2022-2023, Electrónica IV
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://facetvirtual.facet.unt.edu.ar/course/view.php?id=165

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file main.c
 ** @brief EDU-CIAA-NXP board
 */

/* === Headers files inclusions =============================================================== */

#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif

#include "bsp.h" /**< Declaraciones publicas del tipo placa */
#include "GPIO.h" /**< Declaraciones publicas del tipo GPIO */
#include <stdio.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/**
 * @brief Enumeration with color sequence of RGB led
 */
typedef enum rgb_color_e {
    LED_RED_ON = 0,
    LED_RED_OFF,
    LED_GREEN_ON,
    LED_GREEN_OFF,
    LED_BLUE_ON,
    LED_BLUE_OFF,
} rgb_color_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */


/**
 * @brief Function to flash RGB led in sequence
 */
static void FlashLed(placa_t self);

/**
 * @brief Function to switch on and off a led with two keys
 */
static void SwitchLed(placa_t self);

/**
 * @brief Function to switch on and off a led with a single key
 */
static void ToggleLed(placa_t self);

/**
 * @brief Function to turn on a led while a key is pressed
 */
static void TestLed(placa_t self);

/**
 * @brief Function para generar delay por software
 *  clock a 204Mhz, tarda aprox. 50 ms
*/
static void Delay(void);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static void FlashLed(placa_t self) {
    static int divisor = 0;
    static rgb_color_t state = LED_BLUE_OFF;

    divisor++;
    if (divisor == 5) {
        divisor = 0;
        state = (state + 1) % (LED_BLUE_OFF + 1);

        switch (state) {
        case LED_RED_ON:
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, true);
            GPIO_dig_out_activate(self->Led_R);
            break;
        case LED_GREEN_ON:
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);
            GPIO_dig_out_activate(self->Led_G);
            break;
        case LED_BLUE_ON:
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, true);
            GPIO_dig_out_activate(self->Led_B);
            break;
        default:
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, false);
            GPIO_dig_out_deactivate(self->Led_R);
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, false);
            GPIO_dig_out_deactivate(self->Led_G);
            // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, false);
            GPIO_dig_out_deactivate(self->Led_B);
            break;
        }
    }
}

static void SwitchLed(placa_t self) {
    // if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_1_GPIO, TEC_1_BIT) == 0) {
    if (GPIO_dig_in_GetState(self->Tec_1)) {
        // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, true);
        GPIO_dig_out_activate(self->Led_1);
    }
    // if (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_2_GPIO, TEC_2_BIT) == 0) {
    if (GPIO_dig_in_GetState(self->Tec_2)) {
        // Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, false);
        GPIO_dig_out_deactivate(self->Led_1);
    }
}

static void ToggleLed(placa_t self) {
    // static bool last_state = false;
    // bool current_state;

    // current_state = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_3_GPIO, TEC_3_BIT) == 0);
    // if ((current_state) && (!last_state)) {
    if (GPIO_dig_in_HasActivated(self->Tec_3)) {
        // Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT);
        GPIO_dig_out_toggle(self->Led_2);
    }
    // last_state = current_state;
}

static void TestLed(placa_t self) {
    //if (GPIO_dig_in_GetState(self->Tec_4)) {        
    if (GPIO_dig_in_GetState(self->poncho->f_4)) {        
        GPIO_dig_out_activate(self->Led_3);
    } else {        
        GPIO_dig_out_deactivate(self->Led_3);
    }
}

static void Delay(void) {
    for (int index = 0; index < 100; index++) {
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
    }
}

/* === Public function implementation ========================================================== */

int main(void) {
    //uint8_t buffer[4] = {4,2,3,1};
    //uint16_t frec = 0;

    placa_t placa = Placa_create();

    // BoardSetup();
    // ConfigureLeds();
    // ConfigureKeys();

    while (true) {
        FlashLed(placa);
        SwitchLed(placa);
        ToggleLed(placa);
        TestLed(placa);

        Delay();
    }

    return 0;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
