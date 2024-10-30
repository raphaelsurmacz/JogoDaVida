# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra

# Nome do executável
TARGET = programa

# Arquivo fonte
SRC = main.c

# Arquivo objeto
OBJ = main.o

# Regra padrão
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Regra para criar o arquivo objeto
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# Regra para limpar arquivos compilados
clean:
	rm -f $(OBJ) $(TARGET)

# Regra para recompilar tudo
rebuild: clean all

# Define que estes alvos não são arquivos
.PHONY: all clean rebuild
