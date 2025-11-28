#include "../include/imagem.h"
#include "../include/convolucao.h"
#include "../include/util.h"
#ifdef USE_THREADS
#include "../include/threads.h"
#endif
#ifdef USE_OPENMP
#include "../include/openmp.h"
#endif
#ifdef USE_GPU
#include "../include/gpu.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_LOG "logs/execucao.log"

// Função para mostrar o menu e obter a escolha do usuário
int mostrarMenu() {
   int opcao = 0;
   
   printf("\n==== CONVOLUÇÃO 2D ====\n");
   printf("1. Executar com imagem 512x512\n");
   printf("2. Executar com imagem 1024x1024\n");
   printf("3. Executar com imagem 4096x4096\n");
   printf("4. Usar imagem personalizada\n");
   printf("0. Sair\n");
   printf("Escolha uma opção: ");
   
   scanf("%d", &opcao);
   return opcao;
}

// Função para executar a convolução com uma determinada resolução
void executarConvolucao(int resolucao, const char* arquivoEntrada) {
   char arquivoSaida[256];
   char infoLog[512];
   
   #ifdef USE_THREADS

   int numThreads;
   printf("\nDigite o número de threads a utilizar: ");
   scanf("%d", &numThreads);
   if (numThreads <= 0) {
       printf("Número inválido. Usando 4 threads.\n");
       numThreads = 4;
   }
   #endif
   
   #ifdef USE_OPENMP

   int estrategiaOMP;
   printf("\nEscolha a estratégia OpenMP:\n");
   printf("0. schedule(static)\n");
   printf("1. schedule(dynamic)\n");
   printf("2. collapse(2)\n");
   printf("Opção: ");
   scanf("%d", &estrategiaOMP);
   if (estrategiaOMP < 0 || estrategiaOMP > 2) {
       printf("Estratégia inválida. Usando static.\n");
       estrategiaOMP = 0;
   }
   #endif
   
   if (arquivoEntrada && strcmp(arquivoEntrada, "teste") != 0) {
       sprintf(arquivoSaida, "imagens/saida_%s", arquivoEntrada);
   } else {
       sprintf(arquivoSaida, "imagens/saida_%dx%d.pgm", resolucao, resolucao);
   }
   
   printf("\nIniciando processamento...\n");
   printf("- Carregando imagem");
   
   Imagem* imagemOriginal = NULL;
   if (arquivoEntrada) {
       printf(" do arquivo %s\n", arquivoEntrada);
       imagemOriginal = carregarImagem(arquivoEntrada);
   } else {
       printf(" de teste\n");
       imagemOriginal = carregarImagem("teste");
   }
   
   if (!imagemOriginal) {
       printf("Erro: Falha ao carregar a imagem\n");
       return;
   }
   
   printf("- Redimensionando para %dx%d\n", resolucao, resolucao);
   Imagem* imagem = redimensionarImagem(imagemOriginal, resolucao, resolucao);
   liberarImagem(imagemOriginal);
   
   if (!imagem) {
       printf("Erro: Falha ao redimensionar a imagem\n");
       return;
   }
   
   printf("- Criando kernel de blur 3x3\n");
   Kernel* kernel = criarKernelBlur(3);
   if (!kernel) {
       printf("Erro: Falha ao criar kernel\n");
       liberarImagem(imagem);
       return;
   }
   
   printf("- Aplicando convolução");
   
   #ifdef USE_THREADS
   printf(" com %d threads", numThreads);
   registrarLog(ARQUIVO_LOG, "Iniciando convolução com threads (%d) em imagem %dx%d", 
                numThreads, resolucao, resolucao);
   #elif defined(USE_OPENMP)
   const char* estrategiaNome[] = {"static", "dynamic", "collapse(2)"};
   printf(" com OpenMP (%s)", estrategiaNome[estrategiaOMP]);
   registrarLog(ARQUIVO_LOG, "Iniciando convolução com OpenMP (%s) em imagem %dx%d", 
                estrategiaNome[estrategiaOMP], resolucao, resolucao);
   #elif defined(USE_GPU)
   printf(" na GPU");
   registrarLog(ARQUIVO_LOG, "Iniciando convolução na GPU em imagem %dx%d", 
                resolucao, resolucao);
   #else
   printf(" sequencial");
   registrarLog(ARQUIVO_LOG, "Iniciando convolução sequencial em imagem %dx%d", 
                resolucao, resolucao);
   #endif
   
   printf("...\n");
   
   iniciarCronometro();
   Imagem* resultado = NULL;
   
   #ifdef USE_THREADS
   resultado = aplicarConvolucaoThreads(imagem, kernel, numThreads);
   #elif defined(USE_OPENMP)
   resultado = aplicarConvolucaoOpenMP(imagem, kernel, estrategiaOMP);
   #elif defined(USE_GPU)
   resultado = aplicarConvolucaoGPU(imagem, kernel);
   #else
   resultado = aplicarConvolucao(imagem, kernel);
   #endif
   
   double tempoExecucao = pararCronometro();
   
   if (!resultado) {
       printf("Erro: Falha ao aplicar convolução\n");
       liberarKernel(kernel);
       liberarImagem(imagem);
       return;
   }
   
   printf("\nResultados:\n");
   printf("- Tempo de execução: %.2f ms\n", tempoExecucao);
   
   #ifdef USE_THREADS
   sprintf(infoLog, "Convolução com threads (%d) em imagem %dx%d concluída em %.2f ms",
           numThreads, resolucao, resolucao, tempoExecucao);
   #elif defined(USE_OPENMP)
   sprintf(infoLog, "Convolução com OpenMP (%s) em imagem %dx%d concluída em %.2f ms",
           estrategiaNome[estrategiaOMP], resolucao, resolucao, tempoExecucao);
   #elif defined(USE_GPU)
   sprintf(infoLog, "Convolução na GPU em imagem %dx%d concluída em %.2f ms",
           resolucao, resolucao, tempoExecucao);
   #else
   sprintf(infoLog, "Convolução sequencial em imagem %dx%d concluída em %.2f ms",
           resolucao, resolucao, tempoExecucao);
   #endif
   
   registrarLog(ARQUIVO_LOG, "%s", infoLog);
   
   printf("- Salvando imagem resultante em %s\n", arquivoSaida);
   salvarImagem(resultado, arquivoSaida);
   
   liberarImagem(imagem);
   liberarImagem(resultado);
   liberarKernel(kernel);
   
   printf("\nProcessamento concluído com sucesso!\n");
}

int main() {
   int opcao;
   char arquivoPersonalizado[256];
   
   system("mkdir -p logs");
   system("mkdir -p imagens");
   
   #ifdef USE_THREADS
   registrarLog(ARQUIVO_LOG, "Iniciando programa de convolução 2D (versão threads)");
   #elif defined(USE_OPENMP)
   registrarLog(ARQUIVO_LOG, "Iniciando programa de convolução 2D (versão OpenMP)");
   #elif defined(USE_GPU)
   registrarLog(ARQUIVO_LOG, "Iniciando programa de convolução 2D (versão GPU)");
   #else
   registrarLog(ARQUIVO_LOG, "Iniciando programa de convolução 2D (versão sequencial)");
   #endif
   
   do {
       opcao = mostrarMenu();
       
       switch (opcao) {
           case 1:
               executarConvolucao(512, NULL);
               break;
           case 2:
               executarConvolucao(1024, NULL);
               break;
           case 3:
               executarConvolucao(4096, NULL);
               break;
           case 4:
               printf("\nDigite o caminho da imagem: ");
               scanf("%255s", arquivoPersonalizado);
               printf("Digite a resolução desejada: ");
               int resolucao;
               scanf("%d", &resolucao);
               executarConvolucao(resolucao, arquivoPersonalizado);
               break;
           case 0:
               printf("\nEncerrando programa. Obrigado!\n");
               break;
           default:
               printf("\nOpção inválida. Por favor, tente novamente.\n");
       }
   } while (opcao != 0);
   
   registrarLog(ARQUIVO_LOG, "Programa encerrado");
   return 0;
}
