// Al iniciar el reloj empieza en 00:00 y con hora inválida

#include "unity.h"

static const uint8_t hora_inicial[6] = {0, 0, 0, 0, 0, 0};

void test_reloj_inicia_invalido(void) {
    clock_t reloj;
    hora_t hora_actual;
    bool es_hora_valida;

    reloj = RelojCreate(1, NULL);

    es_hora_valida = GetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_FALSE(es_hora_valida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_actual, hora_inicial, 6);
}