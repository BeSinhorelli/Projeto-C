#include <windows.h>
#include "app.h"
#include "window.h"
#include "config.h"
#include "log.h"

AppState g_app = {0};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    (void)hPrev; (void)lpCmd;

    /* 1. Define os padrões (caso o arquivo .ini não exista) */
    g_app.config.tamanho_circulo = 15;
    g_app.config.modo_escuro     = 1;
    g_app.config.largura_janela  = 800;
    g_app.config.altura_janela   = 600;

    /* 2. Tenta carregar o que foi salvo da última vez */
    config_carregar(&g_app, "mouse_monitor.ini");
    log_info("Iniciando Mouse Monitor");

    /* 3. Cria a janela com os dados carregados */
    if (!window_criar(hInst, nShow)) {
        MessageBox(NULL, "Falha ao criar a janela.", "Erro", MB_ICONERROR | MB_OK);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    log_info("Encerrando");
    return (int)msg.wParam;
}