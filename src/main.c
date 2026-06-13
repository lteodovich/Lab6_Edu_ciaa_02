/************************************************************************************************
Copyright (c) 2025-2026, Electrónica IV
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

#include "bsp.h"  /**< Declaraciones publicas del tipo placa */
#include "poncho.h" /**< Declaraciones publicas del tipo poncho */
#include "screen.h" /**< Declaraciones publicas del tipo display */
#include "GPIO.h" /**< Declaraciones publicas del tipo GPIO */


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
            GPIO_dig_out_activate(self->Led_R);
            break;
        case LED_GREEN_ON:
            GPIO_dig_out_activate(self->Led_G);
            break;
        case LED_BLUE_ON:
            GPIO_dig_out_activate(self->Led_B);
            break;
        default:
            GPIO_dig_out_deactivate(self->Led_R);
            GPIO_dig_out_deactivate(self->Led_G);
            GPIO_dig_out_deactivate(self->Led_B);
            break;
        }
    }
}

static void SwitchLed(placa_t self) {
    // if (GPIO_dig_in_GetState(self->Tec_1)) {
    if (GPIO_dig_in_GetState(self->poncho->f_4)) {
        GPIO_dig_out_activate(self->Led_1);
    }    
    // if (GPIO_dig_in_GetState(self->Tec_2)) {
    if (GPIO_dig_in_GetState(self->poncho->f_3)) {
        GPIO_dig_out_deactivate(self->Led_1);
    }
}

static void ToggleLed(placa_t self) {
    // if (GPIO_dig_in_HasActivated(self->Tec_3)) {
    if (GPIO_dig_in_HasActivated(self->poncho->f_2)) {
        GPIO_dig_out_toggle(self->Led_2);
    }
}

static void TestLed(placa_t self) {
    // if (GPIO_dig_in_GetState(self->Tec_4)) {
    if (GPIO_dig_in_GetState(self->poncho->f_1)) {
        GPIO_dig_out_activate(self->Led_3);
    } else {
        GPIO_dig_out_deactivate(self->Led_3);
    }
}

/* === Public function implementation ========================================================== */

int main(void) {
    uint8_t buffer[4] = {4, 2, 3, 1};
    uint16_t frec = 0;

    // Inicialización de la placa (esa misma rut. inicia el poncho y la pantalla)
    placa_t placa = Placa_create();

    // coloca 4231 en la panatlla
    DisplayWriteBCD(placa->poncho->display, buffer, sizeof(buffer));

    // enciende los puntos entre el segundo y tercer dígito
    DisplayToggleDots(placa->poncho->display, 1, 2);

    // hace que los dígitos 1 y 2 parpadeen a una frecuencia de 250 ciclos de refresco del display
    frec = 250;
    DisplayFlashDigits(placa->poncho->display, 0, 1, frec);

    while (true) {
        FlashLed(placa);
        SwitchLed(placa);
        ToggleLed(placa);
        TestLed(placa);

        // delay para que el multiplecado sea visible
        for (int index = 0; index < 50; index++) {
            for (int delay = 0; delay < 10000; delay++) {
                __asm("NOP");
            }
            // refresco del display, hace que se actualice el dígito activo y los segmentos encendidos
            DisplayRefresh(placa->poncho->display);
        }
    }

    return 0;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
