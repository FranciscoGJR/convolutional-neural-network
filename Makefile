# Makefile para projeto de Convolução 2D

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O0 -g
LDFLAGS = -lm

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

# Criação de diretórios necessários
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Versão Sequencial
SRCS_SEQ = $(SRC_DIR)/main.c \
          $(SRC_DIR)/comum/imagem.c \
          $(SRC_DIR)/comum/util.c \
          $(SRC_DIR)/sequencial/convolucao_seq.c

OBJS_SEQ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_SEQ))
TARGET_SEQ = $(BIN_DIR)/convseq

# Regras
all: sequencial

sequencial: $(TARGET_SEQ)

$(TARGET_SEQ): $(OBJS_SEQ)
	@echo "Compilando versão sequencial..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compilação concluída! Execute com: ./$(TARGET_SEQ)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* logs/*

help:
	@echo "Uso do Makefile:"
	@echo "  make all        - Compila todas as versões"
	@echo "  make sequencial - Compila apenas a versão sequencial"
	@echo "  make clean      - Remove arquivos compilados"

.PHONY: all sequencial clean help
