#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- Constantes ---
#define MAX_FILA 5   // Capacidade máxima da Fila de Peças Futuras
#define MAX_PILHA 3  // Capacidade máxima da Pilha de Reserva

// --- Estruturas de Dados ---

// Estrutura para representar uma peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para a Fila Circular (FIFO)
typedef struct {
    Peca itens[MAX_FILA];
    int frente;   // Índice da frente (remoção)
    int tras;     // Índice do final (inserção)
    int contador; // Número atual de elementos
    int proximo_id; // Contador para gerar IDs únicos
} FilaPecas;

// Estrutura para a Pilha Linear (LIFO)
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;     // Índice do topo (-1 para pilha vazia)
} PilhaPecas;

// --- Protótipos das Funções ---

// Funções de Utilitários
Peca gerarPeca(FilaPecas *fila);
void exibirEstadoAtual(FilaPecas *fila, PilhaPecas *pilha);

// Funções da Fila (Queue - FIFO)
void inicializarFila(FilaPecas *fila);
bool estaCheiaFila(FilaPecas *fila);
bool estaVaziaFila(FilaPecas *fila);
void enqueue(FilaPecas *fila, Peca novaPeca);
Peca dequeue(FilaPecas *fila);

// Funções da Pilha (Stack - LIFO)
void inicializarPilha(PilhaPecas *pilha);
bool estaCheiaPilha(PilhaPecas *pilha);
bool estaVaziaPilha(PilhaPecas *pilha);
void push(PilhaPecas *pilha, Peca peca);
Peca pop(PilhaPecas *pilha);

// Funções da Lógica do Jogo
void jogarPeca(FilaPecas *fila);
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha);
void usarPecaReservada(PilhaPecas *pilha);

// --- Implementação das Funções Utilitárias e de Lógica ---

/**
 * @brief Gera uma nova peça com tipo e ID únicos.
 * @param fila Ponteiro para a FilaPecas para acessar e incrementar o proximo_id.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca(FilaPecas *fila) {
    Peca novaPeca;
    const char tipos[] = {'I', 'O', 'T', 'L'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Sorteia um tipo de peça
    novaPeca.nome = tipos[rand() % num_tipos];
    
    // Atribui e incrementa o ID único
    novaPeca.id = fila->proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Exibe o estado atual da Fila e da Pilha.
 * @param fila Ponteiro para a FilaPecas.
 * @param pilha Ponteiro para a PilhaPecas.
 */
void exibirEstadoAtual(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n=======================================================\n");
    printf("                  ESTADO ATUAL DO JOGO\n");
    printf("=======================================================\n");

    // --- Exibição da Fila (FIFO) ---
    printf("Fila de pecas futuras (Capacidade: %d | Ocupacao: %d):\n", MAX_FILA, fila->contador);
    printf("Fila (Frente -> Final): ");
    
    if (estaVaziaFila(fila)) {
        printf("[VAZIA]\n");
    } else {
        int i = fila->frente;
        int elementos_exibidos = 0;
        
        while (elementos_exibidos < fila->contador) {
            printf("[%c %d]", fila->itens[i].nome, fila->itens[i].id);
            if (elementos_exibidos < fila->contador - 1) {
                printf(" ");
            }
            i = (i + 1) % MAX_FILA; // Avança circularmente
            elementos_exibidos++;
        }
        printf("\n");
    }
    
    // --- Exibição da Pilha (LIFO) ---
    printf("\n");
    printf("Pilha de reserva (Capacidade: %d | Ocupacao: %d):\n", MAX_PILHA, pilha->topo + 1);
    printf("Pilha (Topo -> Base): ");
    
    if (estaVaziaPilha(pilha)) {
        printf("[VAZIA]\n");
    } else {
        // Exibe do topo (pilha->topo) até a base (0)
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d]", pilha->itens[i].nome, pilha->itens[i].id);
            if (i > 0) {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("-------------------------------------------------------\n");
}


// --- Funções da Fila (Queue - FIFO) ---

void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = MAX_FILA - 1; // Para manter o cálculo consistente no loop de inicialização
    fila->contador = 0;
    fila->proximo_id = 0;
    
    // Inicializa o gerador de números aleatórios
    srand((unsigned int)time(NULL));

    // Pré-popula a fila para que ela comece cheia (5 elementos)
    printf("--- Inicializando Fila com %d pecas ---\n", MAX_FILA);
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca(fila);
        // Enqueue direto na inicialização para garantir que começa cheio
        fila->itens[i] = p;
        fila->contador++;
    }
    fila->tras = MAX_FILA - 1; // Ajusta 'tras' para a posição correta após inicialização
    printf("Fila inicializada. Proximo ID a ser gerado: %d\n\n", fila->proximo_id);
}

bool estaCheiaFila(FilaPecas *fila) {
    return fila->contador == MAX_FILA;
}

bool estaVaziaFila(FilaPecas *fila) {
    return fila->contador == 0;
}

void enqueue(FilaPecas *fila, Peca novaPeca) {
    // Note: No contexto deste desafio, esta função é chamada após um dequeue,
    // garantindo que a fila não esteja cheia, mas mantemos a validação.
    if (estaCheiaFila(fila)) {
        printf("\nERRO: Fila cheia. Impossivel enfileirar.\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->itens[fila->tras] = novaPeca;
    fila->contador++;
}

Peca dequeue(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', -1}; 
    
    if (estaVaziaFila(fila)) {
        printf("\nERRO: Fila vazia. Impossivel desenfileirar.\n");
        return pecaRemovida;
    }
    
    pecaRemovida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->contador--;
    
    return pecaRemovida;
}


// --- Funções da Pilha (Stack - LIFO) ---

void inicializarPilha(PilhaPecas *pilha) {
    pilha->topo = -1; // Pilha começa vazia
}

bool estaCheiaPilha(PilhaPecas *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

bool estaVaziaPilha(PilhaPecas *pilha) {
    return pilha->topo == -1;
}

void push(PilhaPecas *pilha, Peca peca) {
    if (estaCheiaPilha(pilha)) {
        printf("\nERRO: Pilha de reserva cheia. Impossivel reservar a peca [%c %d].\n", peca.nome, peca.id);
        return;
    }
    
    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
    printf("\nSUCESSO: Peça [%c %d] reservada (PUSH) para a Pilha.\n", peca.nome, peca.id);
}

Peca pop(PilhaPecas *pilha) {
    Peca pecaRemovida = {'\0', -1};
    
    if (estaVaziaPilha(pilha)) {
        printf("\nERRO: Pilha de reserva vazia. Nao ha pecas para usar.\n");
        return pecaRemovida;
    }
    
    pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    
    return pecaRemovida;
}

// --- Funções de Lógica do Jogo (Ações) ---

/**
 * @brief Remove a peça da frente da fila, simulando o uso imediato.
 * * Ação: Dequeue + Enqueue de nova peça (para manter a fila cheia).
 * @param fila Ponteiro para a FilaPecas.
 */
void jogarPeca(FilaPecas *fila) {
    if (estaVaziaFila(fila)) {
        printf("\nAVISO: Nao e possivel jogar. A fila esta vazia.\n");
        return;
    }

    Peca pecaJogada = dequeue(fila);
    if (pecaJogada.id != -1) {
        printf("Acao 1: Jogando peca [%c %d].\n", pecaJogada.nome, pecaJogada.id);
        
        // Regra do jogo: A cada remocao da fila, uma nova peca e gerada e inserida
        Peca novaPeca = gerarPeca(fila);
        enqueue(fila, novaPeca);
        printf("--> Peça de reposicao [%c %d] gerada e inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

/**
 * @brief Move a peça da frente da fila para a pilha de reserva.
 * * Ação: Dequeue da Fila + Push na Pilha + Enqueue de nova peça.
 * @param fila Ponteiro para a FilaPecas.
 * @param pilha Ponteiro para a PilhaPecas.
 */
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha) {
    if (estaCheiaPilha(pilha)) {
        printf("\nAVISO: Pilha de reserva cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    if (estaVaziaFila(fila)) {
        printf("\nAVISO: Nao e possivel reservar. A fila esta vazia.\n");
        return;
    }

    Peca pecaReservar = dequeue(fila);
    
    if (pecaReservar.id != -1) {
        printf("Acao 2: Reservando peca [%c %d] da frente da fila.\n", pecaReservar.nome, pecaReservar.id);
        push(pilha, pecaReservar); // Push na pilha
        
        // Regra do jogo: Reposicao da fila
        Peca novaPeca = gerarPeca(fila);
        enqueue(fila, novaPeca);
        printf("--> Peça de reposicao [%c %d] gerada e inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
    }
}

/**
 * @brief Remove a peça do topo da pilha, simulando seu uso posterior.
 * * Ação: Pop da Pilha.
 * @param pilha Ponteiro para a PilhaPecas.
 */
void usarPecaReservada(PilhaPecas *pilha) {
    if (estaVaziaPilha(pilha)) {
        printf("\nAVISO: Nao e possivel usar. A pilha de reserva esta vazia.\n");
        return;
    }

    Peca pecaUsada = pop(pilha);
    if (pecaUsada.id != -1) {
        printf("Acao 3: Usando peca reservada [%c %d] (POP) do topo da pilha.\n", pecaUsada.nome, pecaUsada.id);
    }
    // Obs: Esta acao NAO gera uma nova peça para a fila.
}

/**
 * @brief Exibe o menu de acoes para o usuario.
 */
void exibirMenu(void) {
    printf("\n==================================\n");
    printf("        Opcoes de Acao\n");
    printf("==================================\n");
    printf("Codigo | Acao\n");
    printf("----------------------------------\n");
    printf("  1    | Jogar peca (dequeue da Fila)\n");
    printf("  2    | Reservar peca (Fila -> Pilha)\n");
    printf("  3    | Usar peca reservada (pop da Pilha)\n");
    printf("  0    | Sair\n");
    printf("----------------------------------\n");
    printf("Digite o codigo da acao: ");
}

// --- Função Principal ---

int main() {
    FilaPecas filaPrincipal;
    PilhaPecas pilhaReserva;
    int opcao = -1;

    // 1. Inicializa a Fila e a Pilha
    inicializarPilha(&pilhaReserva);
    inicializarFila(&filaPrincipal);
    
    do {
        // 2. Exibe o estado atual (Fila e Pilha)
        exibirEstadoAtual(&filaPrincipal, &pilhaReserva);
        
        // 3. Exibe o menu e solicita a opção
        exibirMenu();
        
        // Leitura e validacao basica da entrada
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer em caso de erro de leitura (entrada nao numerica)
            while (getchar() != '\n'); 
            opcao = -1; // Define uma opcao invalida
            printf("\nERRO: Entrada invalida. Por favor, digite um numero.\n");
            continue;
        }
        
        // 4. Executa a ação escolhida
        switch (opcao) {
            case 1:
                jogarPeca(&filaPrincipal);
                break;
            case 2:
                reservarPeca(&filaPrincipal, &pilhaReserva);
                break;
            case 3:
                usarPecaReservada(&pilhaReserva);
                break;
            case 0:
                printf("\nSaindo do simulador. Boa sorte no Tetris Stack!\n");
                break;
            default:
                printf("\nOPCAO INVALIDA. Por favor, digite 1, 2, 3 ou 0.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}