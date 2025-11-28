#include "../../include/gpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

Imagem* aplicarConvolucaoGPU(Imagem* entrada, Kernel* kernel) {
   if (!entrada || !kernel) return NULL;
   
   int largura = entrada->largura;
   int altura = entrada->altura;
   int tamKernel = kernel->tamanho;
   int raioKernel = tamKernel / 2;
   
   Imagem* saida = criarImagem(largura, altura);
   if (!saida) return NULL;
   
   unsigned char* dadosEntrada = (unsigned char*)malloc(largura * altura * sizeof(unsigned char));
   unsigned char* dadosSaida = (unsigned char*)malloc(largura * altura * sizeof(unsigned char));
   float* dadosKernel = (float*)malloc(tamKernel * tamKernel * sizeof(float));
   
   if (!dadosEntrada || !dadosSaida || !dadosKernel) {
       fprintf(stderr, "Erro: Falha na alocação de memória para transferência GPU\n");
       liberarImagem(saida);
       if (dadosEntrada) free(dadosEntrada);
       if (dadosSaida) free(dadosSaida);
       if (dadosKernel) free(dadosKernel);
       return NULL;
   }
   
   for (int y = 0; y < altura; y++) {
       for (int x = 0; x < largura; x++) {
           dadosEntrada[y * largura + x] = entrada->dados[y][x];
       }
   }
   
   for (int y = 0; y < tamKernel; y++) {
       for (int x = 0; x < tamKernel; x++) {
           dadosKernel[y * tamKernel + x] = kernel->dados[y][x];
       }
   }
   
   printf("- Transferindo dados para a GPU...\n");
   
   // Mapeamento de dados para a GPU e execução do kernel
   #pragma omp target data map(to: dadosEntrada[0:largura*altura], dadosKernel[0:tamKernel*tamKernel]) \
                         map(tofrom: dadosSaida[0:largura*altura])
   {
       printf("- Executando convolução na GPU...\n");
       
       #pragma omp target teams distribute parallel for collapse(2)
       for (int y = 0; y < altura; y++) {
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
                       
                       soma += dadosEntrada[imgY * largura + imgX] * 
                               dadosKernel[(ky + raioKernel) * tamKernel + (kx + raioKernel)];
                   }
               }
               
               int valorFinal = (int)soma;
               if (valorFinal < 0) valorFinal = 0;
               if (valorFinal > 255) valorFinal = 255;
               dadosSaida[y * largura + x] = (unsigned char)valorFinal;
           }
       }
   }
   
   printf("- Transferindo resultados de volta da GPU...\n");
   
   for (int y = 0; y < altura; y++) {
       for (int x = 0; x < largura; x++) {
           saida->dados[y][x] = dadosSaida[y * largura + x];
       }
   }
   
   free(dadosEntrada);
   free(dadosSaida);
   free(dadosKernel);
   
   return saida;
}
