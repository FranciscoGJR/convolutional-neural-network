#include "../../include/threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void* processarRegiao(void* arg) {
   DadosThread* dados = (DadosThread*)arg;
   Imagem* entrada = dados->entrada;
   Imagem* saida = dados->saida;
   Kernel* kernel = dados->kernel;
   int inicioLinha = dados->inicioLinha;
   int fimLinha = dados->fimLinha;
   
   int largura = entrada->largura;
   int altura = entrada->altura;
   int raioKernel = kernel->tamanho / 2;
   
   for (int y = inicioLinha; y < fimLinha; y++) {
       for (int x = 0; x < largura; x++) {
           float soma = 0.0f;
           
           for (int ky = -raioKernel; ky <= raioKernel; ky++) {
               for (int kx = -raioKernel; kx <= raioKernel; kx++) {
                   int imgY = y + ky;
                   int imgX = x + kx;
                   
                   // Tratar bordas (espelhamento)
                   if (imgY < 0) imgY = -imgY;
                   if (imgX < 0) imgX = -imgX;
                   if (imgY >= altura) imgY = 2 * altura - imgY - 1;
                   if (imgX >= largura) imgX = 2 * largura - imgX - 1;
                   
                   // Aplicar valor do kernel
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
   
   return NULL;
}

Imagem* aplicarConvolucaoThreads(Imagem* entrada, Kernel* kernel, int numThreads) {
   if (!entrada || !kernel) return NULL;
   
   int largura = entrada->largura;
   int altura = entrada->altura;
   
   Imagem* saida = criarImagem(largura, altura);
   if (!saida) return NULL;
   
   pthread_t* threads = (pthread_t*)malloc(numThreads * sizeof(pthread_t));
   DadosThread* dadosThreads = (DadosThread*)malloc(numThreads * sizeof(DadosThread));
   
   if (!threads || !dadosThreads) {
       fprintf(stderr, "Erro: Falha ao alocar memória para threads\n");
       liberarImagem(saida);
       free(threads);
       free(dadosThreads);
       return NULL;
   }
   
   int linhasPorThread = altura / numThreads;
   int linhasRestantes = altura % numThreads;
   
   int inicioLinha = 0;
   for (int i = 0; i < numThreads; i++) {
       // Calcular intervalo de linhas para esta thread
       int fimLinha = inicioLinha + linhasPorThread;
       if (i < linhasRestantes) fimLinha++; // Distribuir linhas restantes
       
       dadosThreads[i].entrada = entrada;
       dadosThreads[i].saida = saida;
       dadosThreads[i].kernel = kernel;
       dadosThreads[i].inicioLinha = inicioLinha;
       dadosThreads[i].fimLinha = fimLinha;
       
       // Criar thread
       if (pthread_create(&threads[i], NULL, processarRegiao, &dadosThreads[i]) != 0) {
           fprintf(stderr, "Erro: Falha ao criar thread %d\n", i);

           for (int j = 0; j < i; j++) {
               pthread_cancel(threads[j]);
               pthread_join(threads[j], NULL);
           }

           liberarImagem(saida);
           free(threads);
           free(dadosThreads);
           return NULL;
       }
       
       inicioLinha = fimLinha;
   }
   
   // Aguardar as threads terminarem
   for (int i = 0; i < numThreads; i++) {
       pthread_join(threads[i], NULL);
   }
   
   free(threads);
   free(dadosThreads);
   
   return saida;
}
