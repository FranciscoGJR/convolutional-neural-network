#ifndef CONVOLUCAO_H
#define CONVOLUCAO_H

#include "imagem.h"

typedef struct {
   float** dados;
   int tamanho;
} Kernel;

Kernel* criarKernelBlur(int tamanho);

void liberarKernel(Kernel* kernel);

Imagem* aplicarConvolucao(Imagem* entrada, Kernel* kernel);

#endif
