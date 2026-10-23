#ifndef CONFIG_H
#define CONFIG_H

#include "app.h"

int  config_carregar(AppState *app, const char *caminho);
int  config_salvar(const AppState *app, const char *caminho);

#endif