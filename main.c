#include <stdio.h>
#include <stdlib.h>

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

   // Aloca as linhas
   int **matriz = malloc(*linhas * sizeof(int*));

   // Aloca as colunas
   for(int i = 0; i < *linhas; i++) {
      matriz[i] = malloc(*colunas * sizeof(int));
   }
   
   // Lê a matriz
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

int main() {
   int linhas, colunas;

   int** matriz = lerMatriz(&linhas, &colunas);
   imprimeMatriz(matriz, linhas, colunas);

   freeMatriz(matriz, linhas);

   return 0;
}