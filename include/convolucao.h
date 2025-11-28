#ifndef CONVOLUCAO_H
#define CONVOLUCAO_H

#include "imagem.h"

typedef struct {
	float** dados;
	int tamanho;
} Kernel;

// Funções para criação e liberação de kernels
Kernel* criarKernelBlur(int tamanho);

void liberarKernel(Kernel* kernel);

// Função para aplicar convolução
Imagem* aplicarConvolucao(Imagem* entrada, Kernel* kernel);

#endif
