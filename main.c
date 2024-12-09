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
#include <stdint.h>
#include <string.h>

typedef struct{
   uint8_t **atual;
   int linhas;
   int colunas;
} t_estado;

void imprimeMatriz(uint8_t** matriz, int linhas, int colunas, char *nome_arquivo){

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

uint8_t** lerMatriz(int* linhas, int* colunas){
   scanf("%d %d", linhas, colunas);

   uint8_t **matriz = malloc(*linhas * sizeof(uint8_t*));

   for(int i = 0; i < *linhas; i++) {
      matriz[i] = malloc(*colunas * sizeof(uint8_t));
   }
   
   for(int i = 0; i < *linhas; i++) {
      for(int j = 0; j < *colunas; j++) {
         scanf("%hhd", &matriz[i][j]);
      }
   }
   return matriz;
}

void freeMatriz(uint8_t** matriz, int linhas){
   for(int i = 0; i < linhas; i++){
      free(matriz[i]);
   }
   free(matriz);
}

int contaVizinhosVivos(uint8_t **atual, int linhas, int colunas, int i, int j){
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

void geraProximoEstado(uint8_t **atual, uint8_t **proximo, int linhas, int colunas){
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

int contaCelulasVivas(uint8_t **matriz, int linhas, int colunas){
   int qtd_celulas_vivas = 0;
   for (int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++)
         if(matriz[i][j] == 1)
            qtd_celulas_vivas++;
   
   return qtd_celulas_vivas;
}

double calculaCusto(uint8_t **anterior, uint8_t** atual, int linhas, int colunas){
   uint8_t **proximo = malloc(linhas * sizeof(uint8_t*));
   for (int i = 0; i < linhas; i++)
      proximo[i] = malloc(colunas * sizeof(uint8_t));

   geraProximoEstado(anterior, proximo, linhas, colunas);

   const double PESO_DIF = 1.5;
   const double PESO_VIVAS = 0.5;     
   const double PESO_ESTABILIDADE = 1.0;
   const double PESO_PROXIMIDADE = 0.75;

   int diferencas = 0;
   int diferencas_adjacentes = 0;
   
   for(int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++){
         if(proximo[i][j] != atual[i][j]){
            diferencas++;
            
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
            
            for (int k = 0; k < 8; k++){
               int novo_x = i + dx[k];
               int novo_y = j + dy[k];
               
               if (novo_x >= 0 && novo_x < linhas && novo_y >= 0 && novo_y < colunas){
                  if (proximo[novo_x][novo_y] != atual[novo_x][novo_y])
                     diferencas_adjacentes++;
               }
            }
         }
      }

   int celulas_vivas = contaCelulasVivas(anterior, linhas, colunas);

   double estabilidade = 1.0 - ((double)diferencas / (linhas * colunas));

   double custo = ( (PESO_DIF * diferencas) + (PESO_VIVAS * celulas_vivas) + (PESO_ESTABILIDADE * estabilidade)
                                                                  + (PESO_PROXIMIDADE * diferencas_adjacentes));

   freeMatriz(proximo, linhas);

   return custo;   
}

void geraEstadoVizinho(uint8_t **base, uint8_t **vizinho, int linhas, int colunas, int mudancas){
   for (int i = 0; i < linhas; i++)
      for (int j = 0; j < colunas; j++)
         vizinho[i][j] = base[i][j];
   
   bool *selecionadas = calloc(linhas * colunas, sizeof(bool));
   
   for (int h = 0; h < mudancas; h++){
      int i = rand() % linhas;
      int j = rand() % colunas;
      int index = i * colunas + j;

      //Verifica se a celula já foi alterada
      if (!selecionadas[index]){
         vizinho[i][j] = 1 - vizinho[i][j]; //inverte a celula
         selecionadas[index] = true;
      }
   }
   free(selecionadas);
}

void configuraParametros(int linhas, int colunas, double *temperatura_inicial, double *temperatura_final,
                                                double *taxa_resfriamento, int *iteracoes, int *mudancas){
   int tam = linhas * colunas;
   if (tam <= 100){  //ate 10x10
      *temperatura_inicial = 500.0;   // ~ 0.97 de probabilidade da escolha de um pior com diff custo 10
      *temperatura_final = 0.0001;
      *taxa_resfriamento = 0.98;
      *iteracoes = 5 * tam;
      *mudancas = (tam / 20 > 5) ? 20 : 5;  
   }else if(tam <= 225){   //ate 15x15
      *temperatura_inicial = 600.0;   // ~ 0.98 de probabilidade da escolha de um pior com diff custo 10
      *temperatura_final = 0.0001;
      *taxa_resfriamento = 0.98;
      *iteracoes = 7 * tam;
      *mudancas = (tam / 20 > 10) ? 20 : 10;  
   }else if(tam <= 400){   //ate 20x20
      *temperatura_inicial = 800.0;   // ~ 0.98 de probabilidade da escolha de um pior com diff custo 10
      *temperatura_final = 0.001;
      *taxa_resfriamento = 0.98;
      *iteracoes = 8 * tam;
      *mudancas = (tam / 20 > 15) ? 20 : 15;  
   }else{
      *temperatura_inicial = 1000.0;   // ~ 0.99 de probabilidade da escolha de um pior com diff custo 10
      *temperatura_final = 0.01;
      *taxa_resfriamento = 0.99;
      *iteracoes = 10 * tam;
      *mudancas = (tam / 20 > 20) ? 20 : 20;  
   }
}

uint8_t** simulatedAnnealing(t_estado *estado){
   //Parametros da tempera
   double temperatura_inicial = 1.0;
   double temperatura_final = 0.1;
   double taxa_resfriamento = 0.1;
   int iteracoes = 1;
   int mudancas = 1;

   configuraParametros(estado->linhas, estado->colunas, &temperatura_inicial, &temperatura_final,
                                                      &taxa_resfriamento, &iteracoes, &mudancas);

   //printf("\nParametros:\n %f\n%f\n%f\n%d\n%d\n", temperatura_inicial, temperatura_final, taxa_resfriamento, iteracoes, mudancas);
   
   uint8_t **melhor_estado = malloc(estado->linhas * sizeof(uint8_t*));
   uint8_t **estado_atual = malloc(estado->linhas * sizeof(uint8_t*));
   uint8_t **estado_vizinho = malloc(estado->linhas * sizeof(uint8_t*));
   
   for(int i = 0; i < estado->linhas; i++){
      melhor_estado[i] = malloc(estado->colunas * sizeof(uint8_t));
      estado_atual[i] = malloc(estado->colunas * sizeof(uint8_t));
      estado_vizinho[i] = malloc(estado->colunas * sizeof(uint8_t));
   }

   for (int i = 0; i < estado->linhas; i++){
      for (int j = 0; j < estado->colunas; j++){ //Gera um estado aleatorio
            estado_atual[i][j] = (rand() % 2 == 0) ? 1 : 0;
            melhor_estado[i][j] = 0;
      }
   }

   double temperatura = temperatura_inicial;
   double melhor_custo = calculaCusto(estado_atual, estado->atual, estado->linhas, estado->colunas);

   while (temperatura > temperatura_final){
      for (int i = 0; i < iteracoes; i++){
         geraEstadoVizinho(estado_atual, estado_vizinho, estado->linhas, estado->colunas, mudancas);

         double custo_vizinho = calculaCusto(estado_vizinho, estado->atual, estado->linhas, estado->colunas);

         if( custo_vizinho < melhor_custo ||
            (rand() / (double)RAND_MAX) < exp((melhor_custo - custo_vizinho) / temperatura) ){ //Probabilidade de ceitar o pior
            for (int i = 0; i < estado->linhas; i++)
               for (int j = 0; j < estado->colunas; j++)
                  estado_atual[i][j] = estado_vizinho[i][j];

            melhor_custo = custo_vizinho;
             
            // Atualiza melhor estado
            for (int i = 0; i < estado->linhas; i++)
               for (int j = 0; j < estado->colunas; j++) 
                     melhor_estado[i][j] = estado_atual[i][j];   
         }
      }
      temperatura *= taxa_resfriamento;
   }

   freeMatriz(estado_atual, estado->linhas);
   freeMatriz(estado_vizinho, estado->linhas);
   
   return melhor_estado;
}

uint8_t** hillClimbing(uint8_t **estado_base, uint8_t **estado_objetivo, int linhas, int colunas) {
   uint8_t **melhor_estado = malloc(linhas * sizeof(uint8_t*));
   uint8_t **estado_atual = malloc(linhas * sizeof(uint8_t*));
   uint8_t **estado_vizinho = malloc(linhas * sizeof(uint8_t*));
   
   for(int i = 0; i < linhas; i++){
      melhor_estado[i] = malloc(colunas * sizeof(uint8_t));
      estado_atual[i] = malloc(colunas * sizeof(uint8_t));
      estado_vizinho[i] = malloc(colunas * sizeof(uint8_t));

      memcpy(estado_atual[i], estado_base[i], colunas * sizeof(uint8_t));
      memcpy(melhor_estado[i], estado_base[i], colunas * sizeof(uint8_t));
   } 
   
   int max_iteracoes = linhas * colunas * 10;
   int iteracoes_sem_melhora = 0;
   double melhor_custo = calculaCusto(estado_base, estado_objetivo, linhas, colunas);
   
   for (int iteracao = 0; iteracao < max_iteracoes; iteracao++) {
      int i_mudanca = rand() % linhas;
      int j_mudanca = rand() % colunas;
      
      for(int i = 0; i < linhas; i++)
         for(int j = 0; j < colunas; j++)
               estado_vizinho[i][j] = estado_atual[i][j];

      estado_vizinho[i_mudanca][j_mudanca] = 1 - estado_vizinho[i_mudanca][j_mudanca];

      double custo_vizinho = calculaCusto(estado_vizinho, estado_objetivo, linhas, colunas);
      
      if (custo_vizinho < melhor_custo) {
         for(int i = 0; i < linhas; i++)
            for(int j = 0; j < colunas; j++)
               estado_atual[i][j] = estado_vizinho[i][j];
         
         for(int i = 0; i < linhas; i++)
            for(int j = 0; j < colunas; j++)
               melhor_estado[i][j] = estado_vizinho[i][j];
         
         melhor_custo = custo_vizinho;
         iteracoes_sem_melhora = 0;
      } else {
         iteracoes_sem_melhora++;
      }

      if (iteracoes_sem_melhora > linhas * colunas) {
         break;
      }
   }
    
   freeMatriz(estado_atual, linhas);
   freeMatriz(estado_vizinho, linhas);
    
   return melhor_estado;
}

int setTentativas(int tam){
   if (tam <= 144)
      return 10;
   else if(tam <=225)
      return 5;
   else if(tam <= 350)
      return 3;
   else
      return 1;
}

int main() {
   srand(time(NULL));

   char *nome_arquivo = "teste.txt";

   int linhas, colunas;

   uint8_t **estado_anterior = NULL;
   uint8_t **estado_refinado = NULL;
   uint8_t** matriz = lerMatriz(&linhas, &colunas);

   //Criacao do estado atual
   t_estado estado_atual = {
      matriz,
      linhas,
      colunas
   }; 

   clock_t t;
   t = clock();

   uint8_t** melhor_estado_global = NULL;
   int menor_num_celulas_vivas = linhas * colunas;
   int num_celulas_vivas = linhas * colunas;
   int max_tentativas = setTentativas( (linhas * colunas) );

   for (int tentativa = 0; tentativa < max_tentativas; tentativa++) {
      
      estado_anterior = simulatedAnnealing(&estado_atual);

      //printf("\nTentativa: %i\n", tentativa+1);
      //printf("\nTempera:\n");
      //imprimeMatriz(estado_anterior, linhas, colunas, nome_arquivo);

      //printf("\nHill Climbing:\n");
      //imprimeMatriz(estado_refinado, linhas, colunas, nome_arquivo);

      num_celulas_vivas = contaCelulasVivas(estado_anterior, linhas, colunas);

      if(num_celulas_vivas < menor_num_celulas_vivas){
         if (melhor_estado_global != NULL)
            freeMatriz(melhor_estado_global, linhas);
      
         melhor_estado_global = estado_anterior;
         menor_num_celulas_vivas = num_celulas_vivas;
      } else {
         freeMatriz(estado_anterior, linhas);
      }
   }
   
   estado_refinado = hillClimbing(melhor_estado_global, matriz, linhas, colunas);
   imprimeMatriz(estado_refinado, linhas, colunas, nome_arquivo);

   t = clock() - t;
   double time_taken = ((double)t / CLOCKS_PER_SEC);

   printf("Tempo:\t %f\n", time_taken);

   system("gcc teste.c");
   system("./a.out < teste.txt");

   freeMatriz(matriz, linhas);
   freeMatriz(melhor_estado_global, linhas);
   
   return 0;
}