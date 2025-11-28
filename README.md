# Projeto de Convolução 2D: Análise de Estratégias de Paralelização

Este projeto implementa um algoritmo de convolução 2D para processamento de imagens com múltiplas estratégias de paralelização, permitindo a análise comparativa de desempenho entre diferentes abordagens.

## 📋 Sumário

- [Visão Geral](#visão-geral)
- [Requisitos do Sistema](#requisitos-do-sistema)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Compilação](#compilação)
- [Utilização](#utilização)
- [Formatos de Entrada](#formatos-de-entrada)

## 🔍 Visão Geral

O objetivo deste projeto é implementar diferentes estratégias de paralelização aplicadas ao algoritmo de convolução 2D, frequentemente utilizado em processamento de imagens. O foco está na análise do desempenho de:

- **Execução sequencial**
- **Paralelismo explícito**
- **Paralelismo implícito**
- **Paralelismo com GPU**

O projeto processa imagens em escala de cinza com um kernel de blur 3×3 e compara o tempo de execução em diferentes resoluções.

## 💻 Requisitos do Sistema

- **Sistema Operacional**: Linux (testado em Debian 13)
- **Compilador**: GCC 9.0+ com suporte a OpenMP
- **Bibliotecas**: pthread, OpenMP
- **Ferramentas de Build**: Make

## 📁 Estrutura do Projeto

```txt
convolution_projeto/
├── bin/               # Binários compilados
├── include/           # Arquivos de cabeçalho (.h)
├── src/
│   ├── comum/
│   │   ├── imagem.c   # Operações de imagem
│   │   ├── kernel.c   # Implementação do kernel
│   │   └── util.c     # Funções utilitárias
│   ├── sequencial/    # Implementação sequencial
│   │   └── convolucao_seq.c
│   ├── threads/       # Implementação com pthreads
│   │   └── convolucao_threads.c
│   ├── openmp/        # Implementação com OpenMP
│   │   └── convolucao_omp.c
│   └── main.c         # Ponto de entrada do programa
├── imagens/           # Diretório para imagens de entrada/saída
├── logs/              # Arquivos de log
└── Makefile
```

## 🛠️ Compilação

### Compilar Todas as Versões

```bash
make all
```

### Compilar Versões Específicas

```bash
make sequencial  # Apenas versão sequencial
make threads     # Apenas versão com threads
make openmp      # Apenas versão com OpenMP
make gpu         # Apenas versão com GPU
```

### Limpar Arquivos de Build

```bash
make clean
```

## 📊 Utilização

Executar uma versão específica

```bash
./bin/convseq     # Versão sequencial
./bin/convthreads # Versão com threads
./bin/convomp     # Versão com OpenMP
./bin/convgpu     # Versão com GPU
```

## 🖼️ Formatos de Entrada

O projeto suporta:

- Imagem de teste interna: Gerada automaticamente com um padrão de gradiente

- Arquivos PGM: Formato de imagem em escala de cinza

- Arquivos personalizados: Qualquer imagem 20×20 que será redimensionada
