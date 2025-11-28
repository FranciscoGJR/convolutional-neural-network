CC = gcc
CFLAGS = -Wall -Wextra -O0 -g
LDFLAGS = -lm

PTHREAD_FLAGS = -pthread
OPENMP_FLAGS = -fopenmp
OPENMP_TARGET_FLAGS = -fopenmp

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = obj

$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))
$(shell mkdir -p $(OBJ_DIR)/sequencial $(OBJ_DIR)/threads $(OBJ_DIR)/openmp $(OBJ_DIR)/gpu $(OBJ_DIR)/comum)

# Versão Sequencial
SRCS_SEQ = $(SRC_DIR)/main.c \
          $(SRC_DIR)/comum/imagem.c \
          $(SRC_DIR)/comum/util.c \
          $(SRC_DIR)/comum/kernel.c \
          $(SRC_DIR)/sequencial/convolucao_seq.c

OBJS_SEQ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_SEQ))
TARGET_SEQ = $(BIN_DIR)/conv

# Versão com Threads (pthreads)
SRCS_THR = $(SRC_DIR)/main.c \
          $(SRC_DIR)/comum/imagem.c \
          $(SRC_DIR)/comum/util.c \
          $(SRC_DIR)/comum/kernel.c \
          $(SRC_DIR)/threads/convolucao_threads.c

OBJS_THR = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%-thr.o, $(SRCS_THR))
TARGET_THR = $(BIN_DIR)/convthreads

# Versão com OpenMP
SRCS_OMP = $(SRC_DIR)/main.c \
          $(SRC_DIR)/comum/imagem.c \
          $(SRC_DIR)/comum/util.c \
          $(SRC_DIR)/comum/kernel.c \
          $(SRC_DIR)/openmp/convolucao_omp.c

OBJS_OMP = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%-omp.o, $(SRCS_OMP))
TARGET_OMP = $(BIN_DIR)/convomp

# Versão com OpenMP Target para GPU
SRCS_GPU = $(SRC_DIR)/main.c \
          $(SRC_DIR)/comum/imagem.c \
          $(SRC_DIR)/comum/util.c \
          $(SRC_DIR)/comum/kernel.c \
          $(SRC_DIR)/gpu/convolucao_gpu.c

OBJS_GPU = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%-gpu.o, $(SRCS_GPU))
TARGET_GPU = $(BIN_DIR)/convgpu

# Regras
all: sequencial threads openmp gpu

sequencial: $(TARGET_SEQ)

threads: $(TARGET_THR)

openmp: $(TARGET_OMP)

gpu: $(TARGET_GPU)

# Compilação da versão sequencial
$(TARGET_SEQ): $(OBJS_SEQ)
	@echo "Compilando versão sequencial..."
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compilação concluída! Execute com: ./$(TARGET_SEQ)"

# Compilação da versão com threads
$(TARGET_THR): $(OBJS_THR)
	@echo "Compilando versão com threads..."
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compilação concluída! Execute com: ./$(TARGET_THR)"

# Compilação da versão com OpenMP
$(TARGET_OMP): $(OBJS_OMP)
	@echo "Compilando versão com OpenMP..."
	$(CC) $(CFLAGS) $(OPENMP_FLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compilação concluída! Execute com: ./$(TARGET_OMP)"

# Compilação da versão com GPU
	$(TARGET_GPU): $(OBJS_GPU)
	@echo "Compilando versão para GPU..."
	$(CC) $(CFLAGS) $(OPENMP_TARGET_FLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compilação concluída! Execute com: ./$(TARGET_GPU)"

# Regras para compilação dos objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/%-thr.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(PTHREAD_FLAGS) -DUSE_THREADS -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/%-omp.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OPENMP_FLAGS) -DUSE_OPENMP -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/%-gpu.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OPENMP_TARGET_FLAGS) -DUSE_GPU -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/* logs/*

# Mostra ajuda
help:
	@echo "Uso do Makefile:"
	@echo "  make all            - Compila todas as versões"
	@echo "  make sequencial     - Compila apenas a versão sequencial"
	@echo "  make threads        - Compila apenas a versão com threads"
	@echo "  make openmp         - Compila apenas a versão com OpenMP"
	@echo "  make gpu            - Compila apenas a versão com GPU"
	@echo "  make clean          - Remove arquivos compilados"
	@echo ""

.PHONY: all sequencial threads openmp gpu clean help
