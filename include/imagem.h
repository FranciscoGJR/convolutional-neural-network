#ifndef IMAGEM_H
#define IMAGEM_H

typedef struct {
   unsigned char** dados;
   int largura;
   int altura;
} Imagem;

Imagem* criarImagem(int largura, int altura);

void liberarImagem(Imagem* img);

Imagem* redimensionarImagem(Imagem* img, int nova_largura, int nova_altura);

Imagem* carregarImagem(const char* arquivo);

void salvarImagem(Imagem* img, const char* arquivo);

void imprimirInfoImagem(Imagem* img);

#endif
