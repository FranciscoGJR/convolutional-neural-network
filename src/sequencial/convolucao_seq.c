#include "../../include/convolucao.h"
#include <stdlib.h>
#include <stdio.h>

Imagem* aplicarConvolucao(Imagem* entrada, Kernel* kernel) {
	if (!entrada || !kernel) return NULL;

	int largura = entrada->largura;
	int altura = entrada->altura;

	Imagem* saida = criarImagem(largura, altura);
	if (!saida) return NULL;

	int raioKernel = kernel->tamanho / 2;

	// Aplicar convolução usando loops aninhados
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

	return saida;
}
