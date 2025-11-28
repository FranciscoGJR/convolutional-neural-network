#include "../../include/openmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

Imagem* aplicarConvolucaoOpenMP(Imagem* entrada, Kernel* kernel, int estrategia) {
   if (!entrada || !kernel) return NULL;
   
   int largura = entrada->largura;
   int altura = entrada->altura;
   
   // Criar imagem de saída com as mesmas dimensões
   Imagem* saida = criarImagem(largura, altura);
   if (!saida) return NULL;
   
   int raioKernel = kernel->tamanho / 2;
   
   // Aplicar convolução usando OpenMP com diferentes estratégias
   switch (estrategia) {
       case OMP_STATIC: // schedule(static)
           #pragma omp parallel for schedule(static)
           for (int y = 0; y < altura; y++) {
               for (int x = 0; x < largura; x++) {
                   float soma = 0.0f;
                   
                   // Aplicar kernel na vizinhança
                   for (int ky = -raioKernel; ky <= raioKernel; ky++) {
                       for (int kx = -raioKernel; kx <= raioKernel; kx++) {
                           int imgY = y + ky;
                           int imgX = x + kx;
                           
                           // Tratar bordas (espelhamento)
                           if (imgY < 0) imgY = -imgY;
                           if (imgX < 0) imgX = -imgX;
                           if (imgY >= altura) imgY = 2 * altura - imgY - 1;
                           if (imgX >= largura) imgX = 2 * largura - imgX - 1;
                           
                           soma += entrada->dados[imgY][imgX] * 
                                   kernel->dados[ky + raioKernel][kx + raioKernel];
                       }
                   }
                   
                   // Armazenar resultado na imagem de saída
                   int valorFinal = (int)soma;
                   if (valorFinal < 0) valorFinal = 0;
                   if (valorFinal > 255) valorFinal = 255;
                   saida->dados[y][x] = (unsigned char)valorFinal;
               }
           }
           break;
           
       case OMP_DYNAMIC: // schedule(dynamic)
           #pragma omp parallel for schedule(dynamic)
           for (int y = 0; y < altura; y++) {
               for (int x = 0; x < largura; x++) {
                   float soma = 0.0f;
                   
                   for (int ky = -raioKernel; ky <= raioKernel; ky++) {
                       for (int kx = -raioKernel; kx <= raioKernel; kx++) {
                           int imgY = y + ky;
                           int imgX = x + kx;
                           
                           if (imgY < 0) imgY = -imgY;
                           if (imgX < 0) imgX = -imgX;
                           if (imgY >= altura) imgY = 2 * altura - imgY - 1;
                           if (imgX >= largura) imgX = 2 * largura - imgX - 1;
                           
                           soma += entrada->dados[imgY][imgX] * 
                                   kernel->dados[ky + raioKernel][kx + raioKernel];
                       }
                   }
                   
                   int valorFinal = (int)soma;
                   if (valorFinal < 0) valorFinal = 0;
                   if (valorFinal > 255) valorFinal = 255;
                   saida->dados[y][x] = (unsigned char)valorFinal;
               }
           }
           break;
           
       case OMP_COLLAPSE: // collapse(2)
           #pragma omp parallel for collapse(2)
           for (int y = 0; y < altura; y++) {
               for (int x = 0; x < largura; x++) {
                   float soma = 0.0f;
                   
                   for (int ky = -raioKernel; ky <= raioKernel; ky++) {
                       for (int kx = -raioKernel; kx <= raioKernel; kx++) {
                           int imgY = y + ky;
                           int imgX = x + kx;
                           
                           if (imgY < 0) imgY = -imgY;
                           if (imgX < 0) imgX = -imgX;
                           if (imgY >= altura) imgY = 2 * altura - imgY - 1;
                           if (imgX >= largura) imgX = 2 * largura - imgX - 1;
                           
                           soma += entrada->dados[imgY][imgX] * 
                                   kernel->dados[ky + raioKernel][kx + raioKernel];
                       }
                   }
                   
                   int valorFinal = (int)soma;
                   if (valorFinal < 0) valorFinal = 0;
                   if (valorFinal > 255) valorFinal = 255;
                   saida->dados[y][x] = (unsigned char)valorFinal;
               }
           }
           break;
           
       default:
           fprintf(stderr, "Estratégia OpenMP desconhecida: %d\n", estrategia);
           liberarImagem(saida);
           return NULL;
   }
   
   return saida;
}
