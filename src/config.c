#include "config.h"
#include <windows.h>
#include <stdio.h>

int config_carregar(AppState *app, const char *caminho) {
    /* Carrega as configurações visuais (se não achar, usa os padrões) */
    app->config.tamanho_circulo = GetPrivateProfileInt("Visual", "tamanho_circulo", app->config.tamanho_circulo, caminho);
    app->config.modo_escuro     = GetPrivateProfileInt("Visual", "modo_escuro", app->config.modo_escuro, caminho);
    app->config.largura_janela  = GetPrivateProfileInt("Janela", "largura", app->config.largura_janela, caminho);
    app->config.altura_janela   = GetPrivateProfileInt("Janela", "altura", app->config.altura_janela, caminho);

    /* Carrega os contadores (para não perder o progresso ao fechar) */
    app->contador_esq = GetPrivateProfileInt("Estatisticas", "contador_esq", 0, caminho);
    app->contador_dir = GetPrivateProfileInt("Estatisticas", "contador_dir", 0, caminho);

    return 0;
}

int config_salvar(const AppState *app, const char *caminho) {
    char buf[32];

    /* Salva configurações visuais */
    sprintf(buf, "%d", app->config.tamanho_circulo);
    WritePrivateProfileString("Visual", "tamanho_circulo", buf, caminho);

    sprintf(buf, "%d", app->config.modo_escuro);
    WritePrivateProfileString("Visual", "modo_escuro", buf, caminho);

    sprintf(buf, "%d", app->config.largura_janela);
    WritePrivateProfileString("Janela", "largura", buf, caminho);

    sprintf(buf, "%d", app->config.altura_janela);
    WritePrivateProfileString("Janela", "altura", buf, caminho);

    /* Salva os contadores */
    sprintf(buf, "%d", app->contador_esq);
    WritePrivateProfileString("Estatisticas", "contador_esq", buf, caminho);

    sprintf(buf, "%d", app->contador_dir);
    WritePrivateProfileString("Estatisticas", "contador_dir", buf, caminho);

    return 0;
}