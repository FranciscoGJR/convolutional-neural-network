#ifndef OPENMP_H
#define OPENMP_H

#include "imagem.h"
#include "convolucao.h"

#define OMP_STATIC 0
#define OMP_DYNAMIC 1
#define OMP_COLLAPSE 2

Imagem* aplicarConvolucaoOpenMP(Imagem* entrada, Kernel* kernel, int estrategia);

#endif
