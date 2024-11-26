#include <stdio.h>
#include <stdlib.h>

// Função para contar os vizinhos vivos de uma célula
int contarVizinhos(int **matriz, int linhas, int colunas, int x, int y) {
    int vizinhos = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Ignorar a própria célula
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < linhas && ny >= 0 && ny < colunas) {
                vizinhos += matriz[nx][ny];
            }
        }
    }
    return vizinhos;
}

// Função para calcular o próximo estado
void proximoEstado(int **atual, int **proximo, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int vizinhos = contarVizinhos(atual, linhas, colunas, i, j);
            if (atual[i][j] == 1) {
                proximo[i][j] = (vizinhos == 2 || vizinhos == 3) ? 1 : 0;
            } else {
                proximo[i][j] = (vizinhos == 3) ? 1 : 0;
            }
        }
    }
}

int main() {
    int linhas, colunas;
    scanf("%d %d", &linhas, &colunas);

    // Alocar memória para as matrizes
    int **atual = (int **)malloc(linhas * sizeof(int *));
    int **proximo = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        atual[i] = (int *)malloc(colunas * sizeof(int));
        proximo[i] = (int *)malloc(colunas * sizeof(int));
    }

    // Ler o estado inicial
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf("%d", &atual[i][j]);
        }
    }
    
    // Calcular o próximo estado
    proximoEstado(atual, proximo, linhas, colunas);

    // Imprimir o próximo estado
    printf("\nPróximo estado:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d ", proximo[i][j]);
        }
        printf("\n");
    }

    // Liberar memória
    for (int i = 0; i < linhas; i++) {
        free(atual[i]);
        free(proximo[i]);
    }
    free(atual);
    free(proximo);

    return 0;
}