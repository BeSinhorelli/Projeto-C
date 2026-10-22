#include "mouse.h"
#include <windows.h>

int mouse_obter(MouseState *out) {
    POINT p;
    if (!GetCursorPos(&p)) return -1;
    out->x = (int)p.x;
    out->y = (int)p.y;
    out->botao_esq = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;
    out->botao_dir = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;
    return 0;
}