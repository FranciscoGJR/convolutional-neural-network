#ifndef GPU_H
#define GPU_H

#include "imagem.h"
#include "convolucao.h"

Imagem* aplicarConvolucaoGPU(Imagem* entrada, Kernel* kernel);

#endif
