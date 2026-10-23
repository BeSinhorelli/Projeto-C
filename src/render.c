#include "render.h"
#include "window.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

void render_invalidar(void *hwnd) {
    InvalidateRect((HWND)hwnd, NULL, FALSE);
}

void render_desenhar(void *hwnd_void, AppState *app) {
    HWND hwnd = (HWND)hwnd_void;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rect;
    GetClientRect(hwnd, &rect);
    int largura = rect.right - rect.left;
    int altura  = rect.bottom - rect.top;

    HDC hdc_mem = CreateCompatibleDC(hdc);
    HBITMAP bmp_mem = CreateCompatibleBitmap(hdc, largura, altura);
    HBITMAP bmp_antigo = (HBITMAP)SelectObject(hdc_mem, bmp_mem);

    COLORREF cor_fundo = app->config.modo_escuro ? RGB(30, 30, 30) : RGB(240, 240, 240);
    COLORREF cor_texto = app->config.modo_escuro ? RGB(220, 220, 220) : RGB(20, 20, 20);
    COLORREF cor_rastro = RGB(0, 150, 255);
    COLORREF cor_circulo = RGB(255, 60, 60);

    HBRUSH brush_fundo = CreateSolidBrush(cor_fundo);
    FillRect(hdc_mem, &rect, brush_fundo);
    DeleteObject(brush_fundo);

    if (app->rastro.count > 1) {
        HPEN pen_rastro = CreatePen(PS_SOLID, 2, cor_rastro);
        HPEN pen_antiga = (HPEN)SelectObject(hdc_mem, pen_rastro);

        for (int i = 0; i < app->rastro.count - 1; i++) {
            Ponto p1 = rastro_obter(&app->rastro, i);
            Ponto p2 = rastro_obter(&app->rastro, i + 1);
            
            POINT pt1 = {p1.x, p1.y};
            POINT pt2 = {p2.x, p2.y};
            ScreenToClient(hwnd, &pt1);
            ScreenToClient(hwnd, &pt2);
            
            MoveToEx(hdc_mem, pt1.x, pt1.y, NULL);
            LineTo(hdc_mem, pt2.x, pt2.y);
        }
        SelectObject(hdc_mem, pen_antiga);
        DeleteObject(pen_rastro);
    }

    POINT pt_mouse = {app->mouse_atual.x, app->mouse_atual.y};
    ScreenToClient(hwnd, &pt_mouse);
    int raio = app->config.tamanho_circulo;

    HBRUSH brush_circulo = CreateSolidBrush(cor_circulo);
    HBRUSH brush_antiga = (HBRUSH)SelectObject(hdc_mem, brush_circulo);
    Ellipse(hdc_mem, pt_mouse.x - raio, pt_mouse.y - raio, 
                     pt_mouse.x + raio, pt_mouse.y + raio);
    SelectObject(hdc_mem, brush_antiga);
    DeleteObject(brush_circulo);

    SetBkMode(hdc_mem, TRANSPARENT);
    SetTextColor(hdc_mem, cor_texto);
    
    char buf[256];
    sprintf(buf, "Pos: X=%d Y=%d   |   Esq: %d   Dir: %d", 
            app->mouse_atual.x, app->mouse_atual.y, 
            app->contador_esq, app->contador_dir);
    TextOut(hdc_mem, 15, 15, buf, (int)strlen(buf));

    BitBlt(hdc, 0, 0, largura, altura, hdc_mem, 0, 0, SRCCOPY);

    SelectObject(hdc_mem, bmp_antigo);
    DeleteObject(bmp_mem);
    DeleteDC(hdc_mem);

    EndPaint(hwnd, &ps);
}