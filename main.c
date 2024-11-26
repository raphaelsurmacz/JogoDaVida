/*
#################################################
#  Raphael A. Surmacz (GRR20213405)
#
#  Isabela Barata (GRR20213395)
#################################################
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct{
   int **atual;
   int linhas;
   int colunas;
} t_estado;

void imprimeMatriz(int** matriz, int linhas, int colunas, char *nome_arquivo){

   FILE *arquivo = fopen(nome_arquivo, "w");
   fprintf(arquivo, "%d %d\n", linhas, colunas);
   fclose(arquivo);


   FILE *arquivo2 = fopen(nome_arquivo, "a");

   for(int i = 0; i < linhas; i++){
      for(int j = 0; j < colunas; j++) {
         printf("%d ", matriz[i][j]);
      }
      printf("\n");
   }

   for(int i = 0; i < linhas; i++){
      for(int j = 0; j < colunas; j++) {
         fprintf(arquivo2, "%d ", matriz[i][j]);
      }
      fprintf(arquivo2, "\n");
   }

   fclose(arquivo2);
}

int** lerMatriz(int* linhas, int* colunas){
   scanf("%d %d", linhas, colunas);

   int **matriz = malloc(*linhas * sizeof(int*));

   for(int i = 0; i < *linhas; i++) {
      matriz[i] = malloc(*colunas * sizeof(int));
   }
   
   for(int i = 0; i < *linhas; i++) {
      for(int j = 0; j < *colunas; j++) {
         scanf("%d", &matriz[i][j]);
      }
   }
   return matriz;
}

void freeMatriz(int** matriz, int linhas){
   for(int i = 0; i < linhas; i++){
      free(matriz[i]);
   }
   free(matriz);
}

int contaVizinhosVivos(int **atual, int linhas, int colunas, int i, int j){
   int vizinhos = 0;
   int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
   int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

   for (int h = 0; h < 8; h++){
      int nova_linha = i + dx[h];
      int nova_coluna = j + dy[h];

      if ( ((nova_linha >= 0) && (nova_linha < linhas)) && ((nova_coluna >=0) && (nova_coluna < colunas)) )
         vizinhos += atual[nova_linha][nova_coluna];
   }
   return vizinhos;
}

void geraProximoEstado(int **atual, int **proximo, int linhas, int colunas){
   for (int i = 0; i < linhas; i++){
      for (int j = 0; j < colunas; j++){
         int vizinhos = contaVizinhosVivos(atual, linhas, colunas, i, j);

         if(atual[i][j] == 1)
            proximo[i][j] = ( (vizinhos == 2) || (vizinhos == 3) ) ? 1 : 0;
         else
            proximo[i][j] = (vizinhos == 3) ? 1 : 0;
      }
   } 
}

int contaCelulasVivas(int **matriz, int linhas, int colunas){
   int qtd_celulas_vivas = 0;
   for (int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++)
         if(matriz[i][j] == 1)
            qtd_celulas_vivas++;
   
   return qtd_celulas_vivas;
}

int calculaCusto(int **anterior, int** atual, int linhas, int colunas){
   double custo = 0;
   
   int **proximo = malloc(linhas * sizeof(int*));
   for (int i = 0; i < linhas; i++)
      proximo[i] = malloc(colunas * sizeof(int));
   

   geraProximoEstado(anterior, proximo, linhas, colunas);

   const double PESO_DIF = 1.0;
   const double PESO_VIVAS = 0.7 ;

   int diferencas = 0;
   for(int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++)
         if(proximo[i][j] != atual[i][j])
            diferencas++;

   int celulas_vivas = contaCelulasVivas(anterior, linhas, colunas);
   custo = ( (PESO_DIF * diferencas) + (PESO_VIVAS * celulas_vivas) );

   for (int i = 0; i < linhas; i++)
      free(proximo[i]);
   free(proximo);

   return custo;   
}

void geraEstadoVizinho(int **base, int **vizinho, int linhas, int colunas){
   for (int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++)
         vizinho[i][j] = base[i][j];
      
   int mudancas = rand() % (linhas * colunas / 3 + 1) + 1;
   for (int h = 0; h < mudancas; h++){
      int i = rand() % linhas;
      int j = rand() % colunas;

      vizinho[i][j] = 1 - vizinho[i][j]; //inverte a celula
   }
}


int** simulatedAnnealing(t_estado *estado){
   //Parametros da tempera
   double temperatura_inicial = 1000.0;
   double temperatura_final = 0.001;
   double taxa_resfriamento = 0.95;
   double iteracoes = estado->linhas * estado->colunas * 5;
   
   int **melhor_estado = malloc(estado->linhas * sizeof(int*));
   int **estado_atual = malloc(estado->linhas * sizeof(int*));
   int **estado_vizinho = malloc(estado->linhas * sizeof(int*));
   
   for(int i = 0; i < estado->linhas; i++){
      melhor_estado[i] = malloc(estado->colunas * sizeof(int));
      estado_atual[i] = malloc(estado->colunas * sizeof(int));
      estado_vizinho[i] = malloc(estado->colunas * sizeof(int));

      for (int j = 0; j < estado->colunas; j++){ //Gera um estado aleatorio
         estado_atual[i][j] = (rand() % 3 == 0) ? 1 : 0;
         melhor_estado[i][j] = (rand() % 3 == 0) ? 1 : 0;
      }
   }

   double temperatura = temperatura_inicial;
   double melhor_custo = calculaCusto(estado_atual, estado->atual, estado->linhas, estado->colunas);
   //int melhor_num_vivas = contaCelulasVivas(estado_atual, estado->linhas, estado->colunas);

   while (temperatura > temperatura_final){
      for (int i = 0; i < iteracoes; i++){
         geraEstadoVizinho(estado_atual, estado_vizinho, estado->linhas, estado->colunas);

         double custo_vizinho = calculaCusto(estado_vizinho, estado->atual, estado->linhas, estado->colunas);
         //int num_vivas_vizinho = contaCelulasVivas(estado_vizinho, estado->linhas, estado->colunas);

         if( custo_vizinho < melhor_custo || (rand() / (double)RAND_MAX) < exp((melhor_custo - custo_vizinho) / temperatura) ){ //Probabilidade de ceitar o pior
            for (int i = 0; i < estado->linhas; i++)
               for (int j = 0; j < estado->colunas; j++)
                  estado_atual[i][j] = estado_vizinho[i][j];
         }
         if(custo_vizinho < melhor_custo){
            melhor_custo = custo_vizinho;
            // Atualiza melhor estado
            for (int i = 0; i < estado->linhas; i++)
               for (int j = 0; j < estado->colunas; j++) 
                     melhor_estado[i][j] = estado_atual[i][j];    
         }
      }
      temperatura *= taxa_resfriamento;
   }
   

   for(int i = 0; i < estado->linhas; i++){
         free(estado_atual[i]);
         free(estado_vizinho[i]);
   }
      free(estado_atual);
      free(estado_vizinho);
   
   return melhor_estado;
}

int main() {
   srand(time(NULL));

   int linhas, colunas;
   int** matriz = lerMatriz(&linhas, &colunas);
   char *nome_arquivo = "teste.txt";

   // Criacao do estado atual
   t_estado estado_atual = {
      matriz,
      linhas,
      colunas
   }; 

   int **estado_anterior = simulatedAnnealing(&estado_atual);
   imprimeMatriz(estado_anterior, linhas, colunas, nome_arquivo);


   system("gcc teste.c");
   system("./a.out < teste.txt");

   
   freeMatriz(matriz, linhas);
   freeMatriz(estado_anterior, linhas);
   
   return 0;
}