#ifndef RENDER_H
#define RENDER_H

#include "app.h"

/* Pede ao Windows para redesenhar a janela inteira */
void render_invalidar(void *hwnd);

/* Chamado no WM_PAINT para desenhar */
void render_desenhar(void *hwnd, AppState *app);

#endif