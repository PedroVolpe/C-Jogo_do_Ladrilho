/*
Pedro Loureiro Morone Branco Volpe 
*/

//----------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//----------------------------------------------------------------

// STRUCT PARA ARMAZENAR O INDEX DE UM DADO ELEMENTO(PEÇA) DO LADRILHO(MATRIZ)
typedef struct {
  int linha;
  int coluna;

} Index;
//----------------------------------------------------------------

// FUNÇÃO DE PREENCHIMENTO DO LADRILHO(MATRIZ) COM AS INFORMAÇÕES DO ARQUIVO
int **preencheMatriz(FILE *ladrilho, int linha, int coluna){
  int valor;
  int **M = (int**) malloc(linha * sizeof(int *));
  for (int i = 0; i < linha; i++){
    M[i] = (int*) malloc(coluna*sizeof(int));
  }
  for (int j = 0; j < linha; j++){
    for (int k = 0; k < coluna; k++){
      fscanf(ladrilho, "%d", &valor);
      M[j][k] = valor; 
    }
  }
  return M;
}
//----------------------------------------------------------------

// FUNÇÃO DE IMPRESSÃO DO LADRILHO
void imprimeMatriz(int **M, int linha, int coluna){
  for(int i =0; i < linha;i++){
    for(int j = 0;j < coluna;j++){
      printf("%d\t ", M[i][j]);
    }
  printf("\n");
  }
  printf("\n");
}
// FUNÇÃO DE DESALOCAÇÃO DO LADRILHO (MATRIZ DINAMICA)
//----------------------------------------------------------------

void freeMatriz(int **M,int linha){
  for (int i = 0; i < linha; i++){
    free(M[i]);
    }
  free(M);
}
//---------------------------------------------------------------- 

// FUNÇÃO DE BUSCA DO INDEX DE DADO ELEMENTO (PEÇA), E O ARMAZENA NA STRUCT INDEX
Index buscaIndex(int **M, int tamanho, int num){
  Index index = {-1,-1};
  for (int i = 0; i < tamanho; i++) {
    for (int j = 0; j < tamanho; j++){
      if (M[i][j] == num){
        index.linha = i; 
        index.coluna = j;
        return index;
      }  
    }
  }
  return index; // ERROR, num não presente na matriz
}
//---------------------------------------------------------------- 

// FUNÇÃO DE VALIDAÇÃO DOS COMANDOS DADOS
// IMPEDIRA QUE O USUARIO FAÇA COMANDOS PROIBIDOS PARA CADA CASO
bool valida(int **M, int tamanho, int peca, char comando){
  Index index;
  index = buscaIndex(M, tamanho, peca);
  
  switch ( comando ){
    // PROIBIDO: SAIR DA AREA DO LADRILHO OU MOVIMENTAR PARA UM ESPAÇO NÃO NULO (!= 0)
      case 'c' :
        if ((index.linha - 1) == -1 || (M[index.linha - 1][index.coluna]) != 0)
          return false;
        break;
    
      case 'b':
        if ((index.linha + 1) == tamanho || (M[index.linha + 1][index.coluna]) != 0)
          return false;
        break;
    
      case 'd' :
        if ((index.coluna + 1) == tamanho || (M[index.linha][index.coluna + 1]) != 0)
          return false;
        break;
    
      case 'e' :
        if ((index.coluna - 1) == -1 || (M[index.linha][index.coluna - 1]) != 0)
          return false;
        break;

      default:
        printf("Comando %d de movimento invalido", comando);
  }
  return true;
  
}
//----------------------------------------------------------------

// FUNÇÃO DE MOVIMENTAÇÃO DE DADA PEÇA NO LADRILHO PARA CADA COMANDO
void movimenta_peca(int **M,int tamanho, int peca, char comando){
  Index index;
  index = buscaIndex(M, tamanho, peca);

  switch ( comando ){
      case 'c' :
        M[index.linha - 1][index.coluna] = peca;
        M[index.linha][index.coluna] = 0;
        break;
    
      case 'b':
        M[index.linha + 1][index.coluna] = peca;
        M[index.linha][index.coluna] = 0;
        break;
    
      case 'd' :
        M[index.linha][index.coluna + 1] = peca;
        M[index.linha][index.coluna] = 0;
        break;
    
      case 'e' :
        M[index.linha][index.coluna -1] = peca;
        M[index.linha][index.coluna] = 0;
        break;
  }
  
}
//----------------------------------------------------------------

// FUNÇÃO QUE VERIFICA SE O LADRILHO ESTÁ ORDENADO, ISTO É, SE O JOGADOR GANHOU
// PARA ISSO O VALOR NULO DEVERÁ ESTAR, OU NA PRIMEIRA POSIÇÃO OU NA ULTIMA POSIÇÃO DO LADRILHO

bool ganhou(int **M, int tamanho){
  
  int valor, cont = 0;
  // CASO ONDE O VALOR NULO ESTA NA POSIÇÃO INICIAL DO LADRILHO ( M[0][0] )
  if (M[0][0] == 0)
    valor = 1;
  // CASO ONDE O VALOR NULO ESTA NA POSIÇÃO FINAL DO LADRILHO  ( M[TAMANHO - 1][TAMANHO - 1] )
  if (M[tamanho - 1][tamanho - 1] == 0)
    valor = 2;
  
  switch(valor){
    
    // CASO VALOR NULO 1° POSIÇÃO
    case 1:
      
      for (int i = 0; i < tamanho; i++){
        if (i == 0){ // IGNORA A PRIMEIRA POSIÇÃO DO LADRILHO, POIS ESTÁ COMPORTA O VALOR NULO E VERIFICA O RESTANTE DA LINHA
          for (int j = 1; j < (tamanho); j++){
            if (M[i][j-1] < M[i][j]){
              cont++;
            }
          } 
        }  
        // VERIFICA O RESTANTE DO LADRILHO
        else{
          for (int j = 0; j < (tamanho); j++){
            if (M[i][j-1] < M[i][j]){
              cont++;  
            } 
          }
        }
        
      }
      goto verifica_ganhou; // VAI PARA A LINHA DE VERIFICAÇÃO DE ORDENAÇÃO
      break;
    
    // CASO VALOR NULO NA ULTIMA POSIÇÃO
    case 2:
    
      for (int i = 0; i < tamanho; i++){
        for (int j = 0; j < (tamanho); j++){
          if (i == 3 && j == 3){ // IGNORA A ULTIMA POSIÇÃO DO LADRILHO, POIS ESTÁ COMPORTA O VALOR NULO
            goto verifica_ganhou; // VAI PARA A LINHA DE VERIFICAÇÃO DE ORDENAÇÃO
          }
          if (M[i][j-1] < M[i][j]){
            cont++;          
          } 
        }
      }
      // LINHA DE VERIFICAÇÃO DE ORDENAÇÃO 
      verifica_ganhou:
      if (cont == (tamanho*tamanho) -1)
        return true; // ORDENADO (GANHOU)
      else
        return false; // AINDA DESORDENADO
      break;

    default:
      return false; // AINDA DESORDENADO 
  }
}

// FUNÇÃO DE EXECUÇÃO PRINCIPAL DO JOGO
int func_ladrilho(){
  //----------------------------------------------------------------
  
  // LEITURA DO ARQUIVO
  FILE *ladrilho;
  ladrilho = fopen("ladrilho.txt","r");
  
  int **M; //Matriz gerada
  int tamanho; //Tamanho da matriz
  fscanf(ladrilho, "%d", &tamanho);
  //----------------------------------------------------------------
  
  //CHAMADA DE FUNÇÕES E COMEÇO DO JOGO
  M = preencheMatriz(ladrilho, tamanho, tamanho);
  printf("------------------JOGO DO LADRILHO------------------\n\n");
  printf("------------------Ladrilho inicial------------------\n\n");
  imprimeMatriz(M,tamanho,tamanho);
  
  int movs; //Quantidade de movimentos permitidos ao usuário
  fscanf(ladrilho, "%d", &movs);
  
  printf("QUANTIDADE DE JOGADAS DISPONIVEIS: %d\n",movs);
  
  int rodada = 1;
  //----------------------------------------------------------------
  
  //COMEÇO DA CONTAGEM DE RODADAS
  printf("\nDigite ENTER para continuar");
  getchar();
  while(movs > 0) {
    int peca;
    char comando;
    fscanf(ladrilho, "%d %c", &peca, &comando);
    
    if (valida(M, tamanho, peca, comando)){
      movimenta_peca(M, tamanho, peca, comando);
    }
    else{
      printf("Moviemento invalido");
    }
    printf("------------------------------------\n\n");
    printf("%d° Rodada\n\n",rodada);
    rodada++;
    printf("Ladrilho movimentado: %d\nDireção: %c\n\n", peca, comando);
    
    imprimeMatriz(M, tamanho, tamanho);
    movs--;
    
    if (ganhou(M,tamanho)){
      printf("Ladrilho ordenado!!!\nGANHOU\n\n");
      break;
    }
    else{
      printf("Ladrilho ainda desordenado\n");
      printf("QUANTIDADE DE JOGADAS RESTANTES: %d\n\n", movs); 
      printf("------------------------------------\n\n");
      printf("Digite ENTER para continuar");
      getchar();
    }
  }
  printf("------------------Ladrilho finalizado------------------\n\n");
  freeMatriz(M, tamanho);
  fclose(ladrilho);
  return 0;
}
//----------------------------------------------------------------

int main(){
  
  func_ladrilho();

  return 0;
}
