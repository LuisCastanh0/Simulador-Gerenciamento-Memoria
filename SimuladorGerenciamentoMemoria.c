#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura para representar blocos de memória livre
typedef struct _tipo_MemLivre {
  int End_i;                   // Endereço inicial do bloco de memória livre
  int tam;                     // Tamanho do bloco de memória livre
  struct _tipo_MemLivre *prox; // Ponteiro para o próximo bloco de memória livre
} tipo_MemLivre;

// Definição da estrutura para representar blocos de memória alocada
typedef struct _tipo_MemAloc {
  int NProcesso; // Número do processo alocado
  int End_i;     // Endereço inicial do bloco de memória alocada
  int tam;       // Tamanho do bloco de memória alocada
  struct _tipo_MemAloc
      *prox; // Ponteiro para o próximo bloco de memória alocada
} tipo_MemAloc;

// Declaração de ponteiros para controlar as listas de memória livre e alocada
tipo_MemLivre *mem_livre = NULL;
tipo_MemAloc *mem_alocada = NULL;

// Função para inicializar as listas de memória livre e alocada
void inicia(void) {
  mem_livre = NULL;
  mem_alocada = NULL;
}

// Função para inserir um bloco de memória alocada na lista de alocada
void InsereBlocoAloc(int nProcesso, int end_i, int tam) {
  tipo_MemAloc *novo_bloco = (tipo_MemAloc *)calloc(1, sizeof(tipo_MemAloc));
  novo_bloco->NProcesso = nProcesso;
  novo_bloco->End_i = end_i;
  novo_bloco->tam = tam;
  novo_bloco->prox = mem_alocada;
  mem_alocada = novo_bloco;
}

// Função para inserir um bloco de memória livre na lista de livre
void InsereBlocoMemLivre(int end_i, int tam) {
  tipo_MemLivre *novo_bloco = (tipo_MemLivre *)calloc(1, sizeof(tipo_MemLivre));
  novo_bloco->End_i = end_i;
  novo_bloco->tam = tam;
  novo_bloco->prox = mem_livre;

  tipo_MemLivre *atual = mem_livre;
  tipo_MemLivre *anterior = NULL;
  while (atual != NULL) {
    if (end_i + tam == atual->End_i) {
      // Se há blocos contíguos, unificar esses blocos
      novo_bloco->tam += atual->tam;
      novo_bloco->prox = atual->prox;
      free(atual);
      if (anterior == NULL) {
        mem_livre = novo_bloco;
      } else {
        anterior->prox = novo_bloco;
      }
      return;
    } else if (atual->End_i + atual->tam == end_i) {
      // Se há blocos contíguos, unificar esses blocos
      novo_bloco->End_i = atual->End_i;
      novo_bloco->tam += atual->tam;
      novo_bloco->prox = atual->prox;
      free(atual);
      if (anterior == NULL) {
        mem_livre = novo_bloco;
      } else {
        anterior->prox = novo_bloco;
      }
      return;
    }
    anterior = atual;
    atual = atual->prox;
  }

  mem_livre = novo_bloco;
}

// Função para organizar blocos de memória livre
void OrganizaBlocoMemLivre() {
  tipo_MemLivre *atual = mem_livre;
  tipo_MemLivre *anterior = NULL;

  while (atual != NULL) {
    tipo_MemLivre *proximo = atual->prox;

    if (proximo != NULL && atual->End_i + atual->tam == proximo->End_i) {
      // Fundir blocos livres contíguos
      atual->tam += proximo->tam;
      atual->prox = proximo->prox;
      free(proximo);
      proximo = atual->prox;
    } else {
      anterior = atual;
    }

    if (proximo != NULL) {
      atual = proximo;
    } else {
      break;
    }
  }
}

// Função para fundir blocos de memória livre
void FusaoBlocosMemLivre() {
  tipo_MemLivre *atual = mem_livre;
  tipo_MemLivre *anterior = NULL;

  while (atual != NULL) {
    tipo_MemLivre *proximo = atual->prox;

    if (proximo != NULL && atual->End_i + atual->tam == proximo->End_i) {
      // Fundir blocos livres contíguos
      atual->tam += proximo->tam;
      atual->prox = proximo->prox;
      free(proximo);
    } else {
      anterior = atual;
    }

    if (proximo != NULL) {
      atual = proximo;
    } else {
      break;
    }
  }
}

// Função para buscar espaço disponível para alocar um processo
int BuscaEspacoDisp(int tam) {
  tipo_MemLivre *atual = mem_livre;
  while (atual != NULL) {
    if (atual->tam >= tam) {
      return atual->End_i;
    }
    atual = atual->prox;
  }
  return -1; // Espaço disponível não encontrado
}

// Função para alocar memória para um processo
void AlocaMemoria(int nProcesso, int tam) {
  int end_disponivel = BuscaEspacoDisp(tam);
  if (end_disponivel != -1) {
    InsereBlocoAloc(nProcesso, end_disponivel, tam);

    tipo_MemLivre *atual = mem_livre;
    tipo_MemLivre *anterior = NULL;
    while (atual != NULL) {
      if (atual->End_i == end_disponivel) {
        if (atual->tam == tam) {
          // Remover completamente o bloco de memória livre
          if (anterior == NULL) {
            mem_livre = atual->prox;
          } else {
            anterior->prox = atual->prox;
          }
          free(atual);
        } else {
          // Reduzir o tamanho do bloco de memória livre
          atual->End_i += tam;
          atual->tam -= tam;
        }
        break;
      }
      anterior = atual;
      atual = atual->prox;
    }
    OrganizaBlocoMemLivre();
    FusaoBlocosMemLivre();
  } else {
    printf("Espaço insuficiente para alocar o processo %d com tamanho %d.\n",
           nProcesso, tam);
  }
}

// Função para finalizar um processo e liberar a memória alocada
void FinalizaProcesso(int n) {
  tipo_MemAloc *atual = mem_alocada;
  tipo_MemAloc *anterior = NULL;
  while (atual != NULL) {
    if (atual->NProcesso == n) {
      if (anterior == NULL) {
        mem_alocada = atual->prox;
      } else {
        anterior->prox = atual->prox;
      }
      // Recupere o bloco de memória ocupada para a lista de memória livre
      InsereBlocoMemLivre(atual->End_i, atual->tam);
      free(atual);
      OrganizaBlocoMemLivre(); // Reorganize a lista de memória livre
      FusaoBlocosMemLivre();
      return;
    }
    anterior = atual;
    atual = atual->prox;
  }
  printf("Processo %d não encontrado.\n", n);
}

// Função para liberar toda a memória alocada
void LiberaLista() {
  tipo_MemLivre *atual_livre = mem_livre;
  while (atual_livre != NULL) {
    tipo_MemLivre *prox_livre = atual_livre->prox;
    free(atual_livre);
    atual_livre = prox_livre;
  }

  tipo_MemAloc *atual_alocada = mem_alocada;
  while (atual_alocada != NULL) {
    tipo_MemAloc *prox_alocada = atual_alocada->prox;
    free(atual_alocada);
    atual_alocada = prox_alocada;
  }
}

int main() {
  int opcao = 0;
  int processo, tamanho; // Variáveis para caso 3
  int processo_remover;  // Variável para caso 4

  int memoria_total = 4000;

  inicia();
  InsereBlocoMemLivre(0, memoria_total);
  while (opcao != 5) {
    // Exibe o menu
    printf("\nMenu:\n");
    printf("1. Listar memória livre\n");
    printf("2. Listar memória ocupada\n");
    printf("3. Inserir processo\n");
    printf("4. Remover processo\n");
    printf("5. Sair\n");

    // Solicita a escolha do usuário
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      printf("Memória livre:\n");
      tipo_MemLivre *livre = mem_livre;
      while (livre != NULL) {
        printf("Endereço inicial: %d, Tamanho: %d\n", livre->End_i, livre->tam);
        livre = livre->prox;
      }
      break;
    case 2:
      printf("Memória ocupada:\n");
      tipo_MemAloc *alocada = mem_alocada;
      while (alocada != NULL) {
        printf("Processo: %d, Endereço inicial: %d, Tamanho: %d\n",
               alocada->NProcesso, alocada->End_i, alocada->tam);
        alocada = alocada->prox;
      }
      break;
    case 3:
      // Simula a inserção de um processo
      printf("Informe o número do processo: ");
      scanf("%d", &processo);
      printf("Informe o tamanho do processo: ");
      scanf("%d", &tamanho);
      AlocaMemoria(processo, tamanho);
      break;
    case 4:
      // Simula a remoção de um processo
      printf("Informe o número do processo a ser removido: ");
      scanf("%d", &processo_remover);
      FinalizaProcesso(processo_remover);
      break;
    case 5:
      printf("Saindo do programa. Até logo!\n");
      LiberaLista(); // Libera a memória alocada antes de sair
      break;
    default:
      printf("Opção inválida. Escolha uma opção válida (1-5).\n");
    }
  }

  return 0;
}
