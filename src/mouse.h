#ifndef MOUSE_H
#define MOUSE_H

typedef struct {
    int x, y;
    int botao_esq;
    int botao_dir;
} MouseState;

int mouse_obter(MouseState *out);

#endif