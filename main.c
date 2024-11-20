/*
#################################################
#  Raphael A. Surmacz (GRR20213405)
#
#  Isabela Barata (GRR2021)
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
   double temperatura_final = 0.001;
   double taxa_resfriamento = 0.98;
   double iteracoes = estado->linhas * estado->colunas * 2;
   
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