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

typedef struct{
   int **atual;
   int linhas;
   int colunas;
} t_estado;

void imprimeMatriz(int** matriz, int linhas, int colunas){
   for(int i = 0; i < linhas; i++) {
      for(int j = 0; j < colunas; j++) {
         printf("%d ", matriz[i][j]);
      }
      printf("\n");
   }
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

t_estado **simulatedAnnealing(t_estado *estado){
   //Parametros da tempera
   double temperatura_inicial = 1000.0;
   double temperatura_final = 0.0001;
   double taxa_resfriamento = 0.98;
   double iteracoes = estado->linhas * estado->colunas * 2;
   
   int **melhor_estado = malloc(estado->linhas * sizeof(int*));
   int **estado_atual = malloc(estado->linhas * sizeof(int*));
   int **estado_vizinho = malloc(estado->linhas * sizeof(int*));
   
   for(int i = 0; i < sizeof(estado->colunas); i++){
      melhor_estado[i] = malloc(estado->colunas * sizeof(int));
      estado_atual[i] = malloc(estado->colunas * sizeof(int));
      estado_vizinho[i] = malloc(estado->colunas * sizeof(int));

      for (int j = 0; j < estado->colunas; j++){ //Gera um estado aleatorio
         estado_atual[i][j] = rand() % 2;
      }   
   }

   double temperatura = temperatura_inicial;
   double melhor_custo = calculaCusto(estado_atual, estado->atual, estado->linhas, estado->colunas);
   int melhor_num_vivas = contaCelulasVivas(estado_atual, estado->linhas, estado->colunas);

   while (temperatura > temperatura_final){
      for (int i = 0; i < iteracoes; i++){
         geraEstadoVizinho(estado_atual, estado_vizinho, estado->linhas, estado->colunas);

         double custo_vizinho = calculaCusto(estado_vizinho, estado->atual, estado->linhas, estado->colunas);
         int num_vivos_vizinho = contaCelulasVivas(estado_vizinho, estado->linhas, estado->colunas);
         
         int aceitar = 0;
         if(custo_vizinho < melhor_custo)
            aceitar = 1;
         else if( (custo_vizinho == melhor_custo) && (num_vivos_vizinho < melhor_num_vivas) )
            aceitar = 1;
         else if( (rand() / (double)RAND_MAX) < exp((melhor_custo - custo_vizinho) / temperatura) ) //Probabilidade de ceitar o pior
            aceitar = 1;

         if(aceitar){
            for (int i = 0; i < estado->linhas; i++)
               for (int j = 0; j < estado->colunas; j++)
                  estado_atual[i][j] = estado_vizinho[i][j];

            if( (custo_vizinho <= melhor_custo) && (num_vivos_vizinho <= melhor_num_vivas) ){
               melhor_custo = custo_vizinho;
               melhor_num_vivas = num_vivos_vizinho;
            }
         }
      }
      temperatura += taxa_resfriamento;

      for (int i = 0; i < estado->linhas; i++){
         free(estado_atual[i]);
         free(estado_vizinho[i]);
      }
      free(estado_atual);
      free(estado_vizinho);
      
      return melhor_estado;
   }
   

}

int main() {
   int linhas, colunas;
   int** matriz = lerMatriz(&linhas, &colunas);

   // Criacao do estado atual
   t_estado estado_atual = {
      matriz,
      linhas,
      colunas
   }; 

   int **estado_anterior = simulatedAnnealing(&estado_atual);

   imprimeMatriz(estado_anterior, linhas, colunas);
   freeMatriz(matriz, linhas);
   return 0;
}