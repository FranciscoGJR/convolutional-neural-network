#include "../../include/convolucao.h"
#include <stdlib.h>
#include <stdio.h>

Kernel* criarKernelBlur(int tamanho) {
   Kernel* kernel = (Kernel*)malloc(sizeof(Kernel));
   if (!kernel) {
       fprintf(stderr, "Erro: Falha ao alocar memória para kernel\n");
       return NULL;
   }
   
   kernel->tamanho = tamanho;
   
   kernel->dados = (float**)malloc(tamanho * sizeof(float*));
   if (!kernel->dados) {
       fprintf(stderr, "Erro: Falha ao alocar memória para linhas do kernel\n");
       free(kernel);
       return NULL;
   }
   
   for (int i = 0; i < tamanho; i++) {
       kernel->dados[i] = (float*)malloc(tamanho * sizeof(float));
       if (!kernel->dados[i]) {
           fprintf(stderr, "Erro: Falha ao alocar memória para elementos do kernel\n");
           for (int j = 0; j < i; j++) {
               free(kernel->dados[j]);
           }
           free(kernel->dados);
           free(kernel);
           return NULL;
       }
   }
   
   float valor = 1.0f / (tamanho * tamanho);
   for (int i = 0; i < tamanho; i++) {
       for (int j = 0; j < tamanho; j++) {
           kernel->dados[i][j] = valor;
       }
   }
   
   return kernel;
}

void liberarKernel(Kernel* kernel) {
   if (kernel) {
       if (kernel->dados) {
           for (int i = 0; i < kernel->tamanho; i++) {
               free(kernel->dados[i]);
           }
           free(kernel->dados);
       }
       free(kernel);
   }
}

Imagem* aplicarConvolucao(Imagem* entrada, Kernel* kernel) {
   if (!entrada || !kernel) return NULL;
   
   int largura = entrada->largura;
   int altura = entrada->altura;
   
   Imagem* saida = criarImagem(largura, altura);
   if (!saida) return NULL;
   
   int raio_kernel = kernel->tamanho / 2;
   
   // Aplicar convolução usando loops aninhados (abordagem sequencial)
   for (int y = 0; y < altura; y++) {
       for (int x = 0; x < largura; x++) {
           float soma = 0.0f;
           
           // Aplicar kernel na vizinhança
           for (int ky = -raio_kernel; ky <= raio_kernel; ky++) {
               for (int kx = -raio_kernel; kx <= raio_kernel; kx++) {
                   int img_y = y + ky;
                   int img_x = x + kx;
                   
                   // Tratar bordas (espelhamento)
                   if (img_y < 0) img_y = -img_y;
                   if (img_x < 0) img_x = -img_x;
                   if (img_y >= altura) img_y = 2 * altura - img_y - 1;
                   if (img_x >= largura) img_x = 2 * largura - img_x - 1;
                   
                   soma += entrada->dados[img_y][img_x] * 
                           kernel->dados[ky + raio_kernel][kx + raio_kernel];
               }
           }
           
           int valor_final = (int)soma;
           if (valor_final < 0) valor_final = 0;
           if (valor_final > 255) valor_final = 255;
           saida->dados[y][x] = (unsigned char)valor_final;
       }
   }
   
   return saida;
}
