#ifndef CAPTURA_H
#define CAPTURA_H

#include "app.h"

int  captura_iniciar(AppState *app);
void captura_parar(AppState *app);
int  captura_pop(AppState *app, EventoMouse *out);

#endif