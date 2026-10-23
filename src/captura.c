#include "captura.h"
#include "mouse.h"
#include "log.h"
#include <windows.h>

static CRITICAL_SECTION g_cs;
static HANDLE g_thread = NULL;
static volatile int g_rodando = 0;

static DWORD WINAPI thread_captura(LPVOID lpParam) {
    AppState *app = (AppState*)lpParam;
    MouseState m;

    while (g_rodando) {
        if (mouse_obter(&m) == 0) {
            EventoMouse ev;
            ev.posicao.x = m.x;
            ev.posicao.y = m.y;
            ev.botoes.esquerdo = m.botao_esq;
            ev.botoes.direito  = m.botao_dir;

            EnterCriticalSection(&g_cs);
            fila_push(&app->fila, ev);
            LeaveCriticalSection(&g_cs);
        }
        Sleep(1); 
    }
    return 0;
}

int captura_iniciar(AppState *app) {
    if (g_rodando) return 0;
    InitializeCriticalSection(&g_cs);
    g_rodando = 1;

    g_thread = CreateThread(NULL, 0, thread_captura, app, 0, NULL);
    if (!g_thread) {
        log_erro("Falha ao criar thread.");
        g_rodando = 0;
        return -1;
    }
    return 0;
}

void captura_parar(AppState *app) {
    (void)app;
    if (!g_rodando) return;
    g_rodando = 0;
    if (g_thread) {
        WaitForSingleObject(g_thread, INFINITE);
        CloseHandle(g_thread);
        g_thread = NULL;
    }
    DeleteCriticalSection(&g_cs);
}

int captura_pop(AppState *app, EventoMouse *out) {
    int sucesso = 0;
    EnterCriticalSection(&g_cs);
    if (!fila_vazia(&app->fila)) {
        *out = fila_pop(&app->fila);
        sucesso = 1;
    }
    LeaveCriticalSection(&g_cs);
    return sucesso;
}