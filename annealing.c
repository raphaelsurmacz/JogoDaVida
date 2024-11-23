#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Estrutura para armazenar os parâmetros do problema
typedef struct {
    int **atual;
    int linhas;
    int colunas;
} GameState;

int contarCelulasVivas(int **matriz, int linhas, int colunas){
    int vivas = 0;
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            vivas += matriz[i][j];
        }
    }
    return vivas;
}

// Função para contar vizinhos vivos
int contarVizinhosVivos(int **matriz, int linhas, int colunas, int linha, int coluna) {
    int vizinhos = 0;
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int k = 0; k < 8; k++) {
        int novaLinha = linha + dx[k];
        int novaColuna = coluna + dy[k];
        
        if (novaLinha >= 0 && novaLinha < linhas && 
            novaColuna >= 0 && novaColuna < colunas) {
            vizinhos += matriz[novaLinha][novaColuna];
        }
    }
    
    return vizinhos;
}

// Função para aplicar as regras do Jogo da Vida
bool gerarProximoEstado(int **atual, int **proximo, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int vizinhos = contarVizinhosVivos(atual, linhas, colunas, i, j);
            
            if (atual[i][j] == 1) {
                proximo[i][j] = (vizinhos == 2 || vizinhos == 3) ? 1 : 0;
            } else {
                proximo[i][j] = (vizinhos == 3) ? 1 : 0;
            }
        }
    }
    
    // Verifica se o próximo estado é igual ao estado atual
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (atual[i][j] != proximo[i][j]) {
                return false;
            }
        }
    }
    
    return true;
}

// Função para calcular o custo (diferença entre estados)
double calcularCusto(int **anterior, int **atual, int linhas, int colunas) {
    double custo = 0;
    int **proximo = malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        proximo[i] = malloc(colunas * sizeof(int));
    }
    
    // Gera o próximo estado a partir do anterior
    gerarProximoEstado(anterior, proximo, linhas, colunas);
    
    const double PESO_DIF = 1.0;
    const double PESO_VIVAS = 0.3;
    
    // Calcula a diferença entre o próximo estado e o estado atual
    int diferencas = 0;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (proximo[i][j] != atual[i][j]) {
                diferencas++;
                //custo += 1.0;
            }
        }
    }
    
    int celulas_vivas = contarCelulasVivas(anterior, linhas, colunas);
    custo = (PESO_DIF * diferencas) + (PESO_VIVAS * celulas_vivas);

    // Libera memória
    for (int i = 0; i < linhas; i++) {
        free(proximo[i]);
    }
    free(proximo);
    
    return custo;
}

// Função para gerar um estado vizinho
void gerarEstadoVizinho(int **base, int **vizinho, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            vizinho[i][j] = base[i][j];
        }
    }
    
    // Faz algumas mudanças aleatórias
    int mudancas = rand() % (linhas * colunas / 4 + 1) + 1;
    for (int k = 0; k < mudancas; k++) {
        int i = rand() % linhas;
        int j = rand() % colunas;
        vizinho[i][j] = 1 - vizinho[i][j];  // Inverte o estado
    }
}

// Função principal de Simulated Annealing
int** simulated_annealing(GameState *estado) {
    // Parâmetros do Simulated Annealing
    double temperatura_inicial = 1000.0;
    double taxa_resfriamento = 0.99;
    double temperatura_final = 0.0001;
    int iteracoes_por_temperatura = estado->linhas * estado->colunas * 2;
    
    // Aloca memória para o melhor estado
    int **melhor_estado = malloc(estado->linhas * sizeof(int*));
    int **estado_atual = malloc(estado->linhas * sizeof(int*));
    int **estado_vizinho = malloc(estado->linhas * sizeof(int*));
    
    for (int i = 0; i < estado->linhas; i++) {
        melhor_estado[i] = malloc(estado->colunas * sizeof(int));
        estado_atual[i] = malloc(estado->colunas * sizeof(int));
        estado_vizinho[i] = malloc(estado->colunas * sizeof(int));
        
        // Inicializa com estado aleatório
        for (int j = 0; j < estado->colunas; j++) {
            estado_atual[i][j] = rand() % 2;
        }
    }
    
    double temperatura = temperatura_inicial;
    double melhor_custo = calcularCusto(estado_atual, estado->atual, estado->linhas, estado->colunas);
    
    while (temperatura > temperatura_final) {
        for (int iter = 0; iter < iteracoes_por_temperatura; iter++) {
            // Gera estado vizinho
            gerarEstadoVizinho(estado_atual, estado_vizinho, estado->linhas, estado->colunas);
            
            // Calcula o custo do estado vizinho
            double custo_vizinho = calcularCusto(estado_vizinho, estado->atual, estado->linhas, estado->colunas);
            
            // Decide se aceita o novo estado
            if (custo_vizinho < melhor_custo || 
                (rand() / (double)RAND_MAX) < exp((melhor_custo - custo_vizinho) / temperatura)) {
                
                // Atualiza estado atual
                for (int i = 0; i < estado->linhas; i++) {
                    for (int j = 0; j < estado->colunas; j++) {
                        estado_atual[i][j] = estado_vizinho[i][j];
                    }
                }
                
                // Atualiza melhor custo
                if (custo_vizinho < melhor_custo) {
                    melhor_custo = custo_vizinho;
                    
                    // Atualiza melhor estado
                    for (int i = 0; i < estado->linhas; i++) {
                        for (int j = 0; j < estado->colunas; j++) {
                            melhor_estado[i][j] = estado_atual[i][j];
                        }
                    }
                }
            }
        }
        
        // Resfria a temperatura
        temperatura *= taxa_resfriamento;
    }
    
    // Libera memória dos estados temporários
    for (int i = 0; i < estado->linhas; i++) {
        free(estado_atual[i]);
        free(estado_vizinho[i]);
    }
    free(estado_atual);
    free(estado_vizinho);
    
    return melhor_estado;
}

// Função para imprimir a matriz
void imprimeMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Função para liberar matriz
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int main() {
    // Inicializa semente aleatória
    srand(time(NULL));
    
    int linhas, colunas;
    
    // Lê a matriz atual
    scanf("%d %d", &linhas, &colunas);
    
    // Aloca a matriz atual
    int **atual = malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        atual[i] = malloc(colunas * sizeof(int));
    }
    
    // Lê a matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            scanf("%d", &atual[i][j]);
        }
    }
    
    // Prepara o estado do jogo
    GameState estado = {atual, linhas, colunas};
    
    // Encontra o estado anterior
    int **estado_anterior = simulated_annealing(&estado);
    
    // Imprime o estado anterior
    printf("Estado anterior encontrado:\n");
    imprimeMatriz(estado_anterior, linhas, colunas);
    
    // Libera memória
    liberarMatriz(atual, linhas);
    liberarMatriz(estado_anterior, linhas);
    
    return 0;
}