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
