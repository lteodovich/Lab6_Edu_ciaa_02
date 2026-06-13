/*********************************************************************************************************************
Copyright 2025-2026, Electrónica IV
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

/** @file screen.c
 ** @brief
 **/

/* === Headers files inclusions ==================================================================================== */
#include "screen.h" /**< Librería de pantalla */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

/* === Macros definitions ====================================================================== */

// definición de la maxima cantidad de dígitos del display
#ifndef DISP_MAX_DIGIT
#define DISP_MAX_DIGIT 8
#endif

/* === Private data type declarations ========================================================== */

/** @brief Estructura para representar una instancia de display */
struct display_s {
    uint8_t digitos;                     /**< Número de dígitos del display */
    uint8_t digito_activo;               /**< Dígito activo */
    uint8_t flash_desde;                 /**< Dígito desde el cual se va a hacer el flash */
    uint8_t flash_a;                     /**< Dígito hasta el cual se va a hacer el flash */
    uint16_t flash_frec;                 /**< Frecuencia de parpadeo en ciclos de refresco del display */
    uint16_t flash_cuenta;               /**< Contador para el parpadeo */
    uint8_t display_mem[DISP_MAX_DIGIT]; /**< Memoria para almacenar los datos del display */
    struct display_driver_s driver[1];   /**< punteros a las instancias de los controladores del display */
};

/* === Private function declarations =========================================================== */

/**
 * @brief Asigna memoria para una instancia de display
 * @return Puntero a la instancia de display
 */
static display_t DisplayAllocate(void);

/* === Private variable definitions ============================================================ */

/**
 * @brief Imágenes para los dígitos del display, esta en flash
 */
static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             /**< 0 */
    SEGMENT_B | SEGMENT_C,                                                             /**< 1 */
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         /**< 2 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         /**< 3 */
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     /**< 4 */
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         /**< 5 */
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             /**< 6 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 /**< 7 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, /**< 8 */
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G                          /**< 9 */
};

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

display_t DisplayCreate(uint8_t digits, display_driver_t driver) {
    display_t display = DisplayAllocate();
    if (display) {
        display->digitos = digits;
        // el dígito activo se inicializa en el último dígito para que al hacer el primer refresco se active el primer
        // dígito
        display->digito_activo = digits - 1;
        display->flash_desde = 0;
        display->flash_a = 0;
        display->flash_frec = 0;
        display->flash_cuenta = 0;
        // copia los punteros a las funciones del driver a la estructura del display
        memcpy(display->driver, driver, sizeof(display->driver));
        // inicializa la memoria del display en 0, lo que apaga todos los segmentos
        memset(display->display_mem, 0, sizeof(display->display_mem));
        display->driver->UpdateSegments(0x00);
    }
    return display;
}

display_t DisplayAllocate(void) {
    static struct display_s displays[1] = {0};

    return &displays[0];
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size) {
    // limpia la memoria del display, lo que apaga todos los segmentos
    memset(display->display_mem, 0, sizeof(display->display_mem));
    // recorre el número a escribir
    for (int index = 0; index < size; index++) {
        if (index >= display->digitos) {
            // si el número tiene más dígitos que el display, se ignoran los dígitos sobrantes
            break;
        }
        // asigna la imagen correspondiente al dígito a la memoria del display
        display->display_mem[index] = IMAGES[number[index]];
    }
}

void DisplayRefresh(display_t display) {
    uint8_t segments;

    // apaga el dígito activo actual para evitar que se muestren segmentos
    //  encendidos mientras se cambia el dígito activo
    display->driver->UpdateDigits(0x00);
    // incrementa el dígito activo, si se supera el número de dígitos del
    //  display, vuelve al primer dígito
    display->digito_activo = (display->digito_activo + 1) % display->digitos;

    // si el dígito activo es el primer dígito, se incrementa el contador de parpadeo
    //  para hacer que los dígitos parpadeen a la frecuencia deseada
    if (display->digito_activo == 0) {
        if (display->flash_frec) {
            display->flash_cuenta++;
            if (display->flash_cuenta >= display->flash_frec) {
                display->flash_cuenta = 0;
            }
        }
    }

    // obtiene los segmentos que deben estar encendidos para el dígito activo
    segments = display->display_mem[display->digito_activo];

    // si el dígito activo está dentro del rango de dígitos a parpadear y el
    //  contador de parpadeo está en la mitad de la frecuencia,
    //  se apagan los segmentos para hacer que el dígito parpadee
    if (display->flash_frec > 0) {
        if (display->flash_cuenta >= (display->flash_frec / 2)) {
            if (display->digito_activo >= display->flash_desde && display->digito_activo <= display->flash_a) {
                segments = 0;
            }
        }
    }

    // actualiza el dígito activo y los segmentos encendidos a través del driver
    display->driver->UpdateDigits(display->digito_activo);
    // el driver se encarga de encender los segmentos correspondientes al dígito activo
    display->driver->UpdateSegments(segments);
}

void DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frecuency) {
    if (from < display->digitos && to < display->digitos && from <= to) {
        display->flash_desde = from;
        display->flash_a = to;
        display->flash_frec = frecuency;
        display->flash_cuenta = 0;
    }
}

void DisplayToggleDots(display_t display, uint8_t from, uint8_t to) {
    if (from < display->digitos && to < display->digitos && from <= to) {
        for (int index = from; index <= to; index++) {
            display->display_mem[index] ^= SEGMENT_P;
        }
    }
}

/* === End of documentation ==================================================================== */
