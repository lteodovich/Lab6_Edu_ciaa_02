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

/** @file GPIO.c
 ** @brief Declaraciones publicas del tipo GPIO
 **/

/* === Headers files inclusions ==================================================================================== */
#include "GPIO.h" /**< Librería de GPIO, de termianles de entrada y salida */
#include "chip.h" /**< definiciones de la librería del fabriante */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/** @brief Estructura privada que representa a un GPIO como salida. */
struct GPIO_dig_out_s {
    uint32_t puerto;  /**< Número del puerto GPIO. */
    uint8_t terminal;    /**< Número del PIN. */
    bool estado;      /**< Bandera que indica el estado del GPIO. */
};

/** @brief Estructura privada que representa a un GPIO como salida. */
struct GPIO_dig_in_s {
    uint32_t puerto;  /**< Número del puerto GPIO. */
    uint8_t terminal;    /**< Número del PIN. */
    bool inverted;    /**< Bandera que indica si la lectura del GPIO está invertida. */
    bool last_estado;      /**< Bandera que indica el último estado leído del GPIO. */
};

/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

GPIO_dig_out_t GPIO_dig_out_create(const uint32_t puerto_p, const uint8_t terminal_p) {
    
    GPIO_dig_out_t self = malloc(sizeof(struct GPIO_dig_out_s));

    if (self == NULL) {
        return NULL; // Error al asignar memoria
    }

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, puerto_p, terminal_p, false);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puerto_p, terminal_p, true);

    self->puerto = puerto_p;
    self->terminal = terminal_p;
    self->estado = false; // Inicialmente apagado

    return self;
}

void GPIO_dig_out_activate(const GPIO_dig_out_t self){
    if (self == NULL) {
        return; // Error: puntero nulo
    }

    self->estado = true; // Actualizar el estado a encendido
    
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, true);
    
    return;
}

void GPIO_dig_out_deactivate(const GPIO_dig_out_t self){
    if (self == NULL) {
        return; // Error: puntero nulo
    }

    self->estado = false; // Actualizar el estado a apagado

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, false);
    
    return;
}

void GPIO_dig_out_toggle(const GPIO_dig_out_t self){
    if (self == NULL) {
        return; // Error: puntero nulo
    }

    self->estado = !self->estado; // Alternar el estado

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, self->estado);
    return;
}

bool GPIO_dig_out_get_state(const GPIO_dig_out_t self){
    if (self == NULL) {
        return false; // Error: puntero nulo
    }

    return self->estado; // Devuelve el estado actual
}

GPIO_dig_in_t GPIO_dig_in_create(const uint32_t puerto_p, const uint8_t terminal_p, const bool inverted_p) {
    GPIO_dig_in_t self = malloc(sizeof(struct GPIO_dig_in_s));

    if (self == NULL) {
        return NULL; // Error al asignar memoria
    }

    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, puerto_p, terminal_p, false);
    self->last_estado = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, puerto_p, terminal_p) == 0);

    self->puerto = puerto_p;
    self->terminal = terminal_p;
    self->inverted = inverted_p;
    
    // Inicialmente lee el estado
    if (self->inverted) {
        self->last_estado = !self->last_estado; // Invertir el estado si está configurado como invertido
    }

    return self;
}

bool GPIO_dig_in_GetState(const GPIO_dig_in_t self) {
    if (self == NULL) {
        return false; // Error: puntero nulo
    }
    bool state = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal) == 0);
    if (self->inverted) {
        state = !state; // Invertir el estado si está configurado como invertido
    }
    self->last_estado = state; // Actualizar el último estado leído

    return state;
}

bool GPIO_dig_in_HasActivated(const GPIO_dig_in_t self) {
    if (self == NULL) {
        return false; // Error: puntero nulo
    }
    bool state = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal) == 0);
    if (self->inverted) {
        state = !state; // Invertir el estado si está configurado como invertido
    }
    if (state && !self->last_estado) {
        self->last_estado = state; // Actualizar el último estado leído
        return true; // El GPIO se ha activado
    }else{
        self->last_estado = state; // Actualizar el último estado leído
        return false; // El GPIO no se ha activado
    }
}

bool GPIO_dig_in_HasDeactivated(const GPIO_dig_in_t self) {
    if (self == NULL) {
        return false; // Error: puntero nulo
    }
    bool state = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal) == 0);
    if (self->inverted) {
        state = !state; // Invertir el estado si está configurado como invertido
    }
    if (!state && self->last_estado) {
        self->last_estado = state; // Actualizar el último estado leído
        return true; // El GPIO se ha desactivado
    }else{
        self->last_estado = state; // Actualizar el último estado leído
        return false; // El GPIO no se ha activado
    }
}

bool GPIO_dig_in_HasChanged(const GPIO_dig_in_t self) {
    if (self == NULL) {
        return false; // Error: puntero nulo
    }
    bool state = (Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal) == 0);
    if (self->inverted) {
        state = !state; // Invertir el estado si está configurado como invertido
    }
    
    if (state != self->last_estado) {
        self->last_estado = state; // Actualizar el último estado leído
        return true; // El GPIO ha cambiado de estado
    }else{
        self->last_estado = state; // Actualizar el último estado leído
        return false; // El GPIO no ha cambiado de estado
    }
}

/* === End of documentation ==================================================================== */
