#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void registrarLog(const char* arquivo_log, const char* formato, ...);

void iniciarCronometro();

double pararCronometro();

#endif
