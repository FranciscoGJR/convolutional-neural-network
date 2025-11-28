#include "../include/imagem.h"
#include "../include/convolucao.h"
#include "../include/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_LOG "logs/execucao.log"

int mostrarMenu() {
   int opcao = 0;
   
   printf("\n==== CONVOLUÇÃO 2D - VERSÃO SEQUENCIAL ====\n");
   printf("1. Executar com imagem 512x512\n");
   printf("2. Executar com imagem 1024x1024\n");
   printf("3. Executar com imagem 4096x4096\n");
   printf("4. Usar imagem personalizada\n");
   printf("0. Sair\n");
   printf("Escolha uma opção: ");
   
   scanf("%d", &opcao);
   return opcao;
}

void executarConvolucao(int resolucao, const char* arquivo_entrada) {
   char arquivo_saida[256];
   char log_info[512];
   
   if (arquivo_entrada && strcmp(arquivo_entrada, "teste") != 0) {
       sprintf(arquivo_saida, "imagens/saida_%s", arquivo_entrada);
   } else {
       sprintf(arquivo_saida, "imagens/saida_%dx%d.pgm", resolucao, resolucao);
   }
   
   printf("\nIniciando processamento...\n");
   printf("- Carregando imagem");
   
   Imagem* imagem_original = NULL;
   if (arquivo_entrada) {
       printf(" do arquivo %s\n", arquivo_entrada);
       imagem_original = carregarImagem(arquivo_entrada);
   } else {
       printf(" de teste\n");
       imagem_original = carregarImagem("teste");
   }
   
   if (!imagem_original) {
       printf("Erro: Falha ao carregar a imagem\n");
       return;
   }
   
   printf("- Redimensionando para %dx%d\n", resolucao, resolucao);
   Imagem* imagem = redimensionarImagem(imagem_original, resolucao, resolucao);
   liberarImagem(imagem_original);
   
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
   
   printf("- Aplicando convolução...\n");
   registrarLog(ARQUIVO_LOG, "Iniciando convolução sequencial em imagem %dx%d", 
                resolucao, resolucao);
   
   iniciarCronometro();
   Imagem* resultado = aplicarConvolucao(imagem, kernel);
   double tempo_execucao = pararCronometro();
   
   if (!resultado) {
       printf("Erro: Falha ao aplicar convolução\n");
       liberarKernel(kernel);
       liberarImagem(imagem);
       return;
   }
   
   printf("\nResultados:\n");
   printf("- Tempo de execução: %.2f ms\n", tempo_execucao);
   
   sprintf(log_info, "Convolução sequencial em imagem %dx%d concluída em %.2f ms",
           resolucao, resolucao, tempo_execucao);
   registrarLog(ARQUIVO_LOG, "%s", log_info);
   
   printf("- Salvando imagem resultante em %s\n", arquivo_saida);
   salvarImagem(resultado, arquivo_saida);
   
   liberarImagem(imagem);
   liberarImagem(resultado);
   liberarKernel(kernel);
   
   printf("\nProcessamento concluído com sucesso!\n");
}

int main() {
   int opcao;
   char arquivo_personalizado[256];
   
   registrarLog(ARQUIVO_LOG, "Iniciando programa de convolução 2D sequencial");
   
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
               scanf("%255s", arquivo_personalizado);
               printf("Digite a resolução desejada: ");
               int resolucao;
               scanf("%d", &resolucao);
               executarConvolucao(resolucao, arquivo_personalizado);
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
