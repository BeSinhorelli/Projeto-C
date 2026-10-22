#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static void log_impl(const char *nivel, const char *fmt, va_list ap) {
    fprintf(stderr, "%s", nivel);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

void log_info(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); log_impl("[INFO] ", fmt, ap); va_end(ap);
}

void log_erro(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); log_impl("[ERRO] ", fmt, ap); va_end(ap);
}