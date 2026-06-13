/*********************************************************************************************************************
Copyright 2016-2026, Electrónica IV
Facultad de Ciencias Exactas y Tecnologia
Universidad Nacional de Tucuman
https://facetvirtual.facet.unt.edu.ar/course/view.php?id=165

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

#ifndef BSP_H_
#define BSP_H_

/** @file bsp.h
 ** @brief Declaraciones publicas del tipo placa
 **/

/* === Headers files inclusions ==================================================================================== */
#include "GPIO.h"
#include "poncho.h"
#include "screen.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura que representa al poncho.
 */
typedef struct poncho_s {
    GPIO_dig_out_t buzzer;  /**< Puntero a la instancia de GPIO digital out para el buzzer. */
    GPIO_dig_in_t f_1;      /**< Puntero a la instancia de GPIO digital in para la tecla 1. */
    GPIO_dig_in_t f_2;      /**< Puntero a la instancia de GPIO digital in para la tecla 2. */
    GPIO_dig_in_t f_3;      /**< Puntero a la instancia de GPIO digital in para la tecla 3. */
    GPIO_dig_in_t f_4;      /**< Puntero a la instancia de GPIO digital in para la tecla 4. */
    GPIO_dig_in_t aceptar;  /**< Puntero a la instancia de GPIO digital in para la tecla aceptar. */
    GPIO_dig_in_t cancelar; /**< Puntero a la instancia de GPIO digital in para la tecla cancelar. */
    display_t display;      /**< Puntero a la instancia de display multiplexado. */
} const * poncho_t;

/**
 * @brief Estructura que representa a la placa.
 */
typedef struct placa_s {
    GPIO_dig_out_t Led_R; /**< Puntero a la instancia de GPIO digital out para el LED rojo. */
    GPIO_dig_out_t Led_G; /**< Puntero a la instancia de GPIO digital out para el LED verde. */
    GPIO_dig_out_t Led_B; /**< Puntero a la instancia de GPIO digital out para el LED azul. */
    GPIO_dig_out_t Led_1; /**< Puntero a la instancia de GPIO digital out para el LED amarillo. */
    GPIO_dig_out_t Led_2; /**< Puntero a la instancia de GPIO digital out para el LED rojo. */
    GPIO_dig_out_t Led_3; /**< Puntero a la instancia de GPIO digital out para el LED verde. */
    GPIO_dig_in_t Tec_1;  /**< Puntero a la instancia de GPIO digital in para la tecla 1. */
    GPIO_dig_in_t Tec_2;  /**< Puntero a la instancia de GPIO digital in para la tecla 2. */
    GPIO_dig_in_t Tec_3;  /**< Puntero a la instancia de GPIO digital in para la tecla 3. */
    GPIO_dig_in_t Tec_4;  /**< Puntero a la instancia de GPIO digital in para la tecla 4. */
    poncho_t poncho;      /**< Puntero a la instancia de poncho. */
} const * const placa_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Crea una instancia de la placa
 * @return Puntero a la instancia de la placa
 */
placa_t Placa_create(void);

/**
 * @brief Crea una instancia del poncho
 * @return Puntero a la instancia del poncho
 */
poncho_t Poncho_create(void);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */

/* === End of documentation ==================================================================== */
