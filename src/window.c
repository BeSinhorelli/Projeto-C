#include "window.h"
#include "render.h"
#include "captura.h"
#include "config.h"
#include "log.h"

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            g_app.hwnd = (void*)hwnd;
            captura_iniciar(&g_app);
            SetTimer(hwnd, ID_TIMER, 16, NULL); 
            log_info("Janela criada. Thread de captura ativa.");
            return 0;
        }

        case WM_TIMER: {
            int atualizou = 0;
            EventoMouse ev;

            while (captura_pop(&g_app, &ev)) {
                g_app.mouse_anterior = g_app.mouse_atual;
                g_app.mouse_atual = ev.posicao;

                g_app.botoes_anterior = g_app.botoes_atual;
                g_app.botoes_atual = ev.botoes;

                if (clique_ocorreu(g_app.botoes_anterior.esquerdo, g_app.botoes_atual.esquerdo))
                    g_app.contador_esq++;
                if (clique_ocorreu(g_app.botoes_anterior.direito, g_app.botoes_atual.direito))
                    g_app.contador_dir++;

                if (ev.posicao.x != g_app.mouse_anterior.x || ev.posicao.y != g_app.mouse_anterior.y) {
                    rastro_adicionar(&g_app.rastro, ev.posicao.x, ev.posicao.y);
                }
                atualizou = 1;
            }

            if (atualizou) {
                render_invalidar(hwnd);
            }
            return 0;
        }

        case WM_PAINT: {
            render_desenhar(hwnd, &g_app);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1; 

        case WM_KEYDOWN:
            if (wParam == 'C') {
                g_app.contador_esq = 0;
                g_app.contador_dir = 0;
                g_app.rastro.count = 0;
                g_app.rastro.head = 0;
                render_invalidar(hwnd);
            }
            return 0;

        case WM_CLOSE:
            /* NOVO: Salva as configurações ao fechar */
            config_salvar(&g_app, "mouse_monitor.ini");
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            KillTimer(hwnd, ID_TIMER);
            captura_parar(&g_app); 
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HWND window_criar(HINSTANCE hInst, int nShow) {
    const char CLASS_NAME[] = "MouseGUIClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) return NULL;

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Monitor de Mouse - Threaded",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        g_app.config.largura_janela, g_app.config.altura_janela,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return NULL;

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);
    return hwnd;
}