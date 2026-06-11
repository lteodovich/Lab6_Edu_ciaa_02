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

#ifndef GPIO_H_
#define GPIO_H_

/** @file GPIO.h
 ** @brief Declaraciones publicas del tipo GPIO
 **/

/* === Headers files inclusions ==================================================================================== */
#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura anonima que representa a un GPIO digital como salida.
 */
typedef struct GPIO_dig_out_s * GPIO_dig_out_t;

/**
 * @brief Estructura anonima que representa a un GPIO digital como entrada.
 */
typedef struct GPIO_dig_in_s * GPIO_dig_in_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Función para crear una nueva instancia de GPIO digital.
 * @param puerto_p Número del puerto GPIO.
 * @param terminal_p Número del terminal GPIO.
 * @return Puntero a la nueva instancia de GPIO o NULL en caso de error.
 */
GPIO_dig_out_t GPIO_dig_out_create(const uint32_t puerto_p, const uint8_t terminal_p);

/**
 * @brief Función para prender una instancia de GPIO digital.
 * @param self Puntero a la instancia de GPIO.
 */
void GPIO_dig_out_activate(const GPIO_dig_out_t self);

/**
 * @brief Función para apagar una instancia de GPIO digital.
 * @param self Puntero a la instancia de GPIO.
 */
void GPIO_dig_out_deactivate(const GPIO_dig_out_t self);

/**
 * @brief Función para alternar el estado de una instancia de GPIO digital.
 * @param self Puntero a la instancia de GPIO.
 */
void GPIO_dig_out_toggle(const GPIO_dig_out_t self);

/**
 * @brief Función para obtener el estado de una instancia de GPIO digital.
 * @param self Puntero a la instancia de GPIO.
 */
bool GPIO_dig_out_get_state(const GPIO_dig_out_t self);

/**
 * @brief Función para crear una nueva instancia de GPIO digital como entrada.
 * @param puerto_p Número del puerto GPIO.
 * @param terminal_p Número del terminal GPIO.
 * @param inverted_p Indica si la señal está invertida.
 * @return Puntero a la nueva instancia de GPIO o NULL en caso de error.
 */
GPIO_dig_in_t GPIO_dig_in_create(const uint32_t puerto_p, const uint8_t terminal_p, const bool inverted_p);

/**
 * @brief Función para obtener el estado de una instancia de GPIO digital como entrada.
 * @param self Puntero a la instancia de GPIO.
 * @return true si la señal está activa, false en caso contrario.
 */
bool GPIO_dig_in_GetState(const GPIO_dig_in_t self);

/**
 * @brief Función para verificar si una instancia de GPIO digital como entrada ha sido activada.
 * @param self Puntero a la instancia de GPIO.
 * @return true si la señal ha sido activada, false en caso contrario.
 */
bool GPIO_dig_in_HasActivated(const GPIO_dig_in_t self);

/**
 * @brief Función para verificar si una instancia de GPIO digital como entrada ha sido desactivada.
 * @param self Puntero a la instancia de GPIO.
 * @return true si la señal ha sido desactivada, false en caso contrario.
 */
bool GPIO_dig_in_HasDeactivated(const GPIO_dig_in_t self);

/**
 * @brief Función para verificar si una instancia de GPIO digital como entrada ha cambiado de estado.
 * @param self Puntero a la instancia de GPIO.
 * @return true si la señal ha cambiado de estado, false en caso contrario.
 */
bool GPIO_dig_in_HasChanged(const GPIO_dig_in_t self);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ */

/* === End of documentation ==================================================================== */
