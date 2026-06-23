// Al iniciar el reloj empieza en 00:00 y con hora inválida

#include "unity.h"
#include "reloj.h"

static const uint8_t hora_inicial[6] = {0, 0, 0, 0, 0, 0};

void test_reloj_inicia_invalido(void) {
    reloj_t reloj;
    hora_t hora_actual;
    bool es_hora_valida;

    reloj = RelojCreate(1, NULL);

    es_hora_valida = RelojGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_FALSE(es_hora_valida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_actual, hora_inicial, 6);
}