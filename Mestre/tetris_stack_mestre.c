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

// Funções de Utilitários e Inicialização
Peca gerarPeca(FilaPecas *fila);
void exibirEstadoAtual(FilaPecas *fila, PilhaPecas *pilha);
void inicializarFila(FilaPecas *fila);
void inicializarPilha(PilhaPecas *pilha);

// Funções de Operações Básicas (Fila)
bool estaCheiaFila(FilaPecas *fila);
bool estaVaziaFila(FilaPecas *fila);
void enqueue(FilaPecas *fila, Peca novaPeca);
Peca dequeue(FilaPecas *fila);

// Funções de Operações Básicas (Pilha)
bool estaCheiaPilha(PilhaPecas *pilha);
bool estaVaziaPilha(PilhaPecas *pilha);
void push(PilhaPecas *pilha, Peca peca);
Peca pop(PilhaPecas *pilha);
int getTamanhoPilha(PilhaPecas *pilha);

// Funções de Lógica do Jogo (Ações do Usuário)
void jogarPeca(FilaPecas *fila);
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha);
void usarPecaReservada(PilhaPecas *pilha);
void trocarPecaSimples(FilaPecas *fila, PilhaPecas *pilha);
void trocarPecaMultipla(FilaPecas *fila, PilhaPecas *pilha);

// --- Implementação das Funções Utilitárias e de Inicialização ---

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
    printf("              ESTADO ATUAL (Nivel Mestre)\n");
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
    printf("Pilha de reserva (Capacidade: %d | Ocupacao: %d):\n", MAX_PILHA, getTamanhoPilha(pilha));
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

/**
 * @brief Inicializa a fila de peças, preenchendo-a com MAX_FILA peças iniciais.
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = MAX_FILA - 1; 
    fila->contador = 0;
    fila->proximo_id = 0;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca(fila);
        fila->itens[i] = p;
        fila->contador++;
    }
    printf("Fila inicializada com %d pecas.\n", fila->contador);
}

/**
 * @brief Inicializa a pilha de peças, definindo o topo como -1 (vazia).
 */
void inicializarPilha(PilhaPecas *pilha) {
    pilha->topo = -1;
    printf("Pilha de reserva inicializada.\n");
}

// --- Funções de Operações Básicas (Fila) ---

bool estaCheiaFila(FilaPecas *fila) { return fila->contador == MAX_FILA; }
bool estaVaziaFila(FilaPecas *fila) { return fila->contador == 0; }

void enqueue(FilaPecas *fila, Peca novaPeca) {
    if (estaCheiaFila(fila)) return;
    
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->itens[fila->tras] = novaPeca;
    fila->contador++;
}

Peca dequeue(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', -1}; 
    if (estaVaziaFila(fila)) return pecaRemovida;
    
    pecaRemovida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->contador--;
    
    return pecaRemovida;
}


// --- Funções de Operações Básicas (Pilha) ---

bool estaCheiaPilha(PilhaPecas *pilha) { return pilha->topo == MAX_PILHA - 1; }
bool estaVaziaPilha(PilhaPecas *pilha) { return pilha->topo == -1; }
int getTamanhoPilha(PilhaPecas *pilha) { return pilha->topo + 1; }

void push(PilhaPecas *pilha, Peca peca) {
    if (estaCheiaPilha(pilha)) return;
    
    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}

Peca pop(PilhaPecas *pilha) {
    Peca pecaRemovida = {'\0', -1};
    if (estaVaziaPilha(pilha)) return pecaRemovida;
    
    pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    
    return pecaRemovida;
}

// --- Funções de Lógica do Jogo (Ações) ---

/**
 * @brief Executa Dequeue na fila e Enqueue de nova peça.
 */
void jogarPeca(FilaPecas *fila) {
    if (estaVaziaFila(fila)) {
        printf("\nAVISO: Nao e possivel jogar. A fila esta vazia.\n");
        return;
    }

    Peca pecaJogada = dequeue(fila);
    printf("\nAcao 1: Jogando peca [%c %d] (dequeue da fila).\n", pecaJogada.nome, pecaJogada.id);
    
    // Reposicao automatica
    Peca novaPeca = gerarPeca(fila);
    enqueue(fila, novaPeca);
    printf("--> Peça de reposicao [%c %d] gerada e inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Move a peça da frente da fila para o topo da pilha.
 * Ação: Dequeue da Fila + Push na Pilha + Enqueue de nova peça.
 */
void reservarPeca(FilaPecas *fila, PilhaPecas *pilha) {
    if (estaCheiaPilha(pilha)) {
        printf("\nAVISO: Pilha de reserva cheia! Nao e possivel reservar mais pecas.\n");
        return;
    }
    if (estaVaziaFila(fila)) {
        printf("\nAVISO: Fila vazia! Nao ha pecas para reservar.\n");
        return;
    }

    Peca pecaReservar = dequeue(fila);
    printf("\nAcao 2: Reservando peca [%c %d] (Fila -> Pilha).\n", pecaReservar.nome, pecaReservar.id);
    
    push(pilha, pecaReservar);
    
    // Reposicao automatica
    Peca novaPeca = gerarPeca(fila);
    enqueue(fila, novaPeca);
    printf("--> Peça de reposicao [%c %d] gerada e inserida no final da fila.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Executa Pop na pilha.
 */
void usarPecaReservada(PilhaPecas *pilha) {
    if (estaVaziaPilha(pilha)) {
        printf("\nAVISO: Nao e possivel usar. A pilha de reserva esta vazia.\n");
        return;
    }

    Peca pecaUsada = pop(pilha);
    printf("\nAcao 3: Usando peca reservada [%c %d] (pop da Pilha).\n", pecaUsada.nome, pecaUsada.id);
}

/**
 * @brief Troca a peça da FRENTE da fila com a peça do TOPO da pilha.
 */
void trocarPecaSimples(FilaPecas *fila, PilhaPecas *pilha) {
    if (estaVaziaFila(fila) || estaVaziaPilha(pilha)) {
        printf("\nAVISO: Troca Simples nao pode ser realizada. Fila ou Pilha estao vazias.\n");
        return;
    }
    
    // Pega as peças a serem trocadas
    Peca pecaFila = fila->itens[fila->frente];
    Peca pecaPilha = pilha->itens[pilha->topo];
    
    // Realiza a troca:
    // 1. A peça da pilha vai para a frente da fila
    fila->itens[fila->frente] = pecaPilha;
    // 2. A peça da fila vai para o topo da pilha
    pilha->itens[pilha->topo] = pecaFila;
    
    printf("\nAcao 4: Troca Simples realizada.\n");
    printf("   [Fila] %c %d <--> [Pilha] %c %d\n", pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
    
    // Nenhuma reposição é necessária pois não há remoção
}

/**
 * @brief Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 * (Requer que ambas tenham no mínimo 3 elementos)
 */
void trocarPecaMultipla(FilaPecas *fila, PilhaPecas *pilha) {
    const int N_TROCA = 3;

    if (fila->contador < N_TROCA || getTamanhoPilha(pilha) < N_TROCA) {
        printf("\nAVISO: Troca Multipla nao pode ser realizada.\n");
        printf("   Requer %d pecas na Fila (atual: %d) e %d na Pilha (atual: %d).\n", 
               N_TROCA, fila->contador, N_TROCA, getTamanhoPilha(pilha));
        return;
    }
    
    printf("\nAcao 5: Troca Multipla (Bloco) de %d pecas realizada.\n", N_TROCA);
    
    // A troca é realizada movendo-se 3 elementos de cada estrutura
    for (int i = 0; i < N_TROCA; i++) {
        // Índice da fila (circular)
        int idx_fila = (fila->frente + i) % MAX_FILA;
        // Índice da pilha (linear, do topo para baixo)
        int idx_pilha = pilha->topo - i;
        
        // Troca os valores (swap)
        Peca temp = fila->itens[idx_fila];
        fila->itens[idx_fila] = pilha->itens[idx_pilha];
        pilha->itens[idx_pilha] = temp;
        
        printf("   Bloco #%d: Fila [%c %d] <--> Pilha [%c %d]\n", 
               i+1, pilha->itens[idx_pilha].nome, pilha->itens[idx_pilha].id, 
               fila->itens[idx_fila].nome, fila->itens[idx_fila].id);
    }
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
    printf("  2    | Enviar peca (Fila -> Pilha)\n");
    printf("  3    | Usar peca (pop da Pilha)\n");
    printf("  4    | Trocar peca simples (Frente Fila <-> Topo Pilha)\n");
    printf("  5    | Trocar multipla (3 Fila <-> 3 Pilha)\n");
    printf("  0    | Sair\n");
    printf("----------------------------------\n");
    printf("Digite o codigo da acao: ");
}

// --- Função Principal ---

int main() {
    FilaPecas filaPrincipal;
    PilhaPecas pilhaReserva;
    int opcao = -1;

    // 1. Inicializa as estruturas
    inicializarPilha(&pilhaReserva);
    inicializarFila(&filaPrincipal);
    
    do {
        // 2. Exibe o estado atual
        exibirEstadoAtual(&filaPrincipal, &pilhaReserva);
        
        // 3. Exibe o menu e solicita a opção
        exibirMenu();
        
        // Leitura e validação básica da entrada
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); 
            opcao = -1; 
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
            case 4:
                trocarPecaSimples(&filaPrincipal, &pilhaReserva);
                break;
            case 5:
                trocarPecaMultipla(&filaPrincipal, &pilhaReserva);
                break;
            case 0:
                printf("\nSaindo do simulador Mestre. O gerenciamento de pecas foi um sucesso!\n");
                break;
            default:
                printf("\nOPCAO INVALIDA. Por favor, digite 1, 2, 3, 4, 5 ou 0.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}