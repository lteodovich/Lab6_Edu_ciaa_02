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

/* === Macros definitions ====================================================================== */

#ifndef DISP_MAX_DIGIT
#define DISP_MAX_DIGIT 8
#endif

/* === Private data type declarations ========================================================== */

struct display_s {
	uint8_t digitos;
	uint8_t digitos_activos;
	uint8_t flash_desde;
	uint8_t flash_a;
	uint16_t flash_frec;
	uint16_t flash_cuenta;
	uint8_t display_mem[DISP_MAX_DIGIT];
	struct display_driver_s driver[1];
};

/* === Private function declarations =========================================================== */

static display_t DisplayAllocate(void);

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

display_t DisplayCreate(uint8_t digits, display_driver_t driver){
	display_t display = DisplayAllocate();
	if (display){
		display->digitos = digits;
		display->digitos_activos = digits-1;
		display->flash_desde = 0;
		display->flash_a = 0;
		display->flash_frec = 0;
		display->flash_cuenta = 0;
		memcpy(display->driver, driver, sizeof(display->driver));
		memeset(display->display_mem, 0, sizeof(display->display_mem));
		display->driver->UpdateSegments(0x00);
	}
}

/* === End of documentation ==================================================================== */
