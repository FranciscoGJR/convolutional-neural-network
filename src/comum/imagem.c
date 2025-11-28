#include "../../include/imagem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Imagem* criarImagem(int largura, int altura) {
   Imagem* img = (Imagem*)malloc(sizeof(Imagem));
   if (!img) {
       fprintf(stderr, "Erro: Falha ao alocar memória para imagem\n");
       return NULL;
   }

   img->largura = largura;
   img->altura = altura;
   img->dados = (unsigned char**)malloc(altura * sizeof(unsigned char*));

   if (!img->dados) {
       fprintf(stderr, "Erro: Falha ao alocar memória para linhas da imagem\n");
       free(img);
       return NULL;
   }

   for (int i = 0; i < altura; i++) {
       img->dados[i] = (unsigned char*)calloc(largura, sizeof(unsigned char));
       if (!img->dados[i]) {
           fprintf(stderr, "Erro: Falha ao alocar memória para pixels na linha %d\n", i);
           for (int j = 0; j < i; j++) {
               free(img->dados[j]);
           }
           free(img->dados);
           free(img);
           return NULL;
       }
   }

   return img;
}

void liberarImagem(Imagem* img) {
   if (img) {
       if (img->dados) {
           for (int i = 0; i < img->altura; i++) {
               free(img->dados[i]);
           }
           free(img->dados);
       }
       free(img);
   }
}

Imagem* redimensionarImagem(Imagem* img, int nova_largura, int nova_altura) {
   if (!img) return NULL;
   
   Imagem* nova_img = criarImagem(nova_largura, nova_altura);
   if (!nova_img) return NULL;
   
   float escala_x = (float)img->largura / nova_largura;
   float escala_y = (float)img->altura / nova_altura;
   
   for (int y = 0; y < nova_altura; y++) {
       for (int x = 0; x < nova_largura; x++) {
           int src_x = (int)(x * escala_x);
           int src_y = (int)(y * escala_y);
           
           if (src_x >= img->largura) src_x = img->largura - 1;
           if (src_y >= img->altura) src_y = img->altura - 1;
           
           nova_img->dados[y][x] = img->dados[src_y][src_x];
       }
   }
   
   return nova_img;
}

Imagem* criarImagemTeste() {
   int largura = 20;
   int altura = 20;
   Imagem* img = criarImagem(largura, altura);
   
   for (int y = 0; y < altura; y++) {
       for (int x = 0; x < largura; x++) {
           img->dados[y][x] = (unsigned char)((x + y) * 255 / (largura + altura));
       }
   }
   
   return img;
}

Imagem* carregarImagem(const char* arquivo) {
   if (strcmp(arquivo, "teste") == 0) {
       return criarImagemTeste();
   }
   
   FILE* fp = fopen(arquivo, "r");
   if (!fp) {
       fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", arquivo);
       return NULL;
   }
   
   char linha[100];
   int largura, altura, max_val;
   
   if (!fgets(linha, sizeof(linha), fp)) goto erro;
   if (linha[0] != 'P' || linha[1] != '2') goto erro;
   
   do {
       if (!fgets(linha, sizeof(linha), fp)) goto erro;
   } while (linha[0] == '#');
   
   if (sscanf(linha, "%d %d", &largura, &altura) != 2) goto erro;
   
   if (!fgets(linha, sizeof(linha), fp)) goto erro;

   if (sscanf(linha, "%d", &max_val) != 1) goto erro;
   
   Imagem* img = criarImagem(largura, altura);
   if (!img) goto erro;
   
   for (int y = 0; y < altura; y++) {
       for (int x = 0; x < largura; x++) {
           int pixel;
           if (fscanf(fp, "%d", &pixel) != 1) {
               liberarImagem(img);
               goto erro;
           }
           img->dados[y][x] = (unsigned char)pixel;
       }
   }
   
   fclose(fp);
   return img;
   
erro:
   fprintf(stderr, "Erro: Formato de arquivo inválido ou corrompido\n");
   if (fp) fclose(fp);
   return NULL;
}

void salvarImagem(Imagem* img, const char* arquivo) {
   if (!img) return;
   
   FILE* fp = fopen(arquivo, "w");
   if (!fp) {
       fprintf(stderr, "Erro: Não foi possível criar o arquivo %s\n", arquivo);
       return;
   }
   
   // Cabeçalho PGM
   fprintf(fp, "P2\n");
   fprintf(fp, "# Gerado pelo programa de convolucao\n");
   fprintf(fp, "%d %d\n", img->largura, img->altura);
   fprintf(fp, "255\n");
   
   for (int y = 0; y < img->altura; y++) {
       for (int x = 0; x < img->largura; x++) {
           fprintf(fp, "%d ", img->dados[y][x]);
       }
       fprintf(fp, "\n");
   }
   
   fclose(fp);
}

void imprimirInfoImagem(Imagem* img) {
   if (!img) {
       printf("Imagem não existe\n");
       return;
   }
   
   printf("Informações da Imagem:\n");
   printf("  Dimensões: %d x %d pixels\n", img->largura, img->altura);
   printf("  Memória ocupada: %.2f KB\n", 
          (float)(img->largura * img->altura) / 1024.0);
}
