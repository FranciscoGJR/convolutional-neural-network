#ifndef THREADS_H
#define THREADS_H

#include "imagem.h"
#include "convolucao.h"

typedef struct {
   Imagem* entrada;
   Imagem* saida;
   Kernel* kernel;
   int inicioLinha;
   int fimLinha;
} DadosThread;

// Função executada por cada thread
void* processarRegiao(void* arg);

// Função principal para aplicar convolução com threads
Imagem* aplicarConvolucaoThreads(Imagem* entrada, Kernel* kernel, int numThreads);

#endif
