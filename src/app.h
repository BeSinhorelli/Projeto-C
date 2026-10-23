#ifndef APP_H
#define APP_H

/* ============================================================
 *  Tipos básicos
 * ============================================================ */

typedef struct {
    int x, y;
} Ponto;

typedef struct {
    int esquerdo;
    int direito;
} MouseBotoes;

/* ============================================================
 *  Fila de Eventos (Thread -> UI)
 * ============================================================ */

#define FILA_MAX 1024

typedef struct {
    Ponto posicao;
    MouseBotoes botoes;
} EventoMouse;

typedef struct {
    EventoMouse itens[FILA_MAX];
    int head;
    int tail;
    int count;
} FilaEventos;

/* ============================================================
 *  Rastro do mouse
 * ============================================================ */

#define RASTRO_MAX 512

typedef struct {
    Ponto pontos[RASTRO_MAX];
    int   count;
    int   head;
} Rastro;

/* ============================================================
 *  Configuração
 * ============================================================ */

typedef struct {
    int tamanho_circulo;
    int modo_escuro;
    int largura_janela;
    int altura_janela;
} Config;

/* ============================================================
 *  Estado global da aplicação
 * ============================================================ */

typedef struct {
    void *hwnd;

    /* Mouse — estado atual e anterior */
    Ponto       mouse_atual;
    Ponto       mouse_anterior;
    MouseBotoes botoes_atual;
    MouseBotoes botoes_anterior;

    /* Contadores */
    int contador_esq;
    int contador_dir;

    /* Rastro e Config */
    Rastro rastro;
    Config config;

    /* Concorrência (Thread) */
    FilaEventos fila;
} AppState;

extern AppState g_app;

/* ============================================================
 *  Helpers inline
 * ============================================================ */

static inline int clique_ocorreu(int anterior, int atual) {
    return (!anterior && atual) ? 1 : 0;
}

static inline void rastro_adicionar(Rastro *r, int x, int y) {
    r->pontos[r->head].x = x;
    r->pontos[r->head].y = y;
    r->head = (r->head + 1) % RASTRO_MAX;
    if (r->count < RASTRO_MAX) r->count++;
}

static inline Ponto rastro_obter(const Rastro *r, int i) {
    int idx = (r->head - r->count + i + RASTRO_MAX) % RASTRO_MAX;
    return r->pontos[idx];
}

/* Helpers da Fila */
static inline int fila_vazia(const FilaEventos *f) {
    return f->count == 0;
}

static inline void fila_push(FilaEventos *f, EventoMouse ev) {
    if (f->count < FILA_MAX) {
        f->itens[f->tail] = ev;
        f->tail = (f->tail + 1) % FILA_MAX;
        f->count++;
    }
}

static inline EventoMouse fila_pop(FilaEventos *f) {
    EventoMouse ev = f->itens[f->head];
    f->head = (f->head + 1) % FILA_MAX;
    f->count--;
    return ev;
}

#endif