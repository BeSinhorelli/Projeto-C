#ifndef CAPTURA_H
#define CAPTURA_H

#include "app.h"

int  captura_iniciar(AppState *app);
void captura_parar(AppState *app);

/* Consome um evento da fila. Retorna 1 se conseguiu, 0 se vazia. */
int  captura_pop(AppState *app, EventoMouse *out);

#endif