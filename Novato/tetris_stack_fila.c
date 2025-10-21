#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Definição do tamanho máximo da fila de peças
#define MAX_SIZE 5

// --- Estrutura de Dados ---

// Estrutura para representar uma peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca itens[MAX_SIZE];
    int frente;   // Índice da frente da fila (onde a remoção ocorre)
    int tras;     // Índice do final da fila (onde a inserção ocorre)
    int contador; // Número atual de elementos na fila
    int proximo_id; // Contador para gerar IDs únicos
} FilaPecas;

// --- Protótipos das Funções ---

void inicializarFila(FilaPecas *fila);
Peca gerarPeca(FilaPecas *fila);
bool estaCheia(FilaPecas *fila);
bool estaVazia(FilaPecas *fila);
void enqueue(FilaPecas *fila, Peca novaPeca);
Peca dequeue(FilaPecas *fila);
void exibirFila(FilaPecas *fila);
void exibirMenu(void);
void executarAcao(FilaPecas *fila, int opcao);

// --- Implementação das Funções ---

/**
 * @brief Inicializa a fila de peças.
 * * Define os índices de frente, trás e o contador de elementos como zero.
 * Pré-popula a fila com peças geradas automaticamente.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1; // -1 indica que a fila está vazia (ou pode ser 0 e usar contador)
    fila->contador = 0;
    fila->proximo_id = 0;
    
    // Inicializa o gerador de números aleatórios para o tipo da peça
    srand((unsigned int)time(NULL));

    // Pré-popula a fila com um número fixo de elementos (MAX_SIZE)
    printf("--- Inicializacao da Fila ---\n");
    for (int i = 0; i < MAX_SIZE; i++) {
        Peca p = gerarPeca(fila);
        // O enqueue aqui é simplificado para a inicialização
        fila->tras = (fila->tras + 1) % MAX_SIZE;
        fila->itens[fila->tras] = p;
        fila->contador++;
        printf("Peca inicial gerada e inserida: [%c %d]\n", p.nome, p.id);
    }
    printf("Fila de pecas inicializada com %d elementos.\n\n", fila->contador);
}

/**
 * @brief Gera uma nova peça com tipo e ID únicos.
 * * O tipo da peça é escolhido aleatoriamente entre 'I', 'O', 'T', 'L'.
 * O ID é gerado sequencialmente.
 * * @param fila Ponteiro para a estrutura FilaPecas para acessar o proximo_id.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca(FilaPecas *fila) {
    Peca novaPeca;
    
    // Tipos de peças disponíveis
    const char tipos[] = {'I', 'O', 'T', 'L'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Sorteia um tipo de peça
    novaPeca.nome = tipos[rand() % num_tipos];
    
    // Atribui e incrementa o ID único
    novaPeca.id = fila->proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Verifica se a fila está cheia.
 * * Na implementação circular com contador, a fila está cheia se o contador
 * for igual ao tamanho máximo.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 * @return true se a fila estiver cheia, false caso contrário.
 */
bool estaCheia(FilaPecas *fila) {
    return fila->contador == MAX_SIZE;
}

/**
 * @brief Verifica se a fila está vazia.
 * * A fila está vazia se o contador de elementos for zero.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 * @return true se a fila estiver vazia, false caso contrário.
 */
bool estaVazia(FilaPecas *fila) {
    return fila->contador == 0;
}

/**
 * @brief Insere uma nova peça no final da fila (enqueue).
 * * Se a fila não estiver cheia, o índice 'tras' é atualizado circularmente,
 * a peça é inserida e o contador é incrementado.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 * @param novaPeca A peça a ser inserida.
 */
void enqueue(FilaPecas *fila, Peca novaPeca) {
    if (estaCheia(fila)) {
        printf("\nERRO: A fila esta cheia! Nao e possivel adicionar mais pecas.\n");
        return;
    }
    
    // Move o índice 'tras' circularmente
    fila->tras = (fila->tras + 1) % MAX_SIZE;
    fila->itens[fila->tras] = novaPeca;
    fila->contador++;
    
    printf("\nSUCESSO: Peça [%c %d] inserida (enqueue) no final da fila.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * * Se a fila não estiver vazia, a peça na 'frente' é removida, o índice 'frente' 
 * é atualizado circularmente e o contador é decrementado.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 * @return Peca A peça removida (a ser "jogada"). Se vazia, retorna uma peça vazia.
 */
Peca dequeue(FilaPecas *fila) {
    Peca pecaRemovida = {'\0', -1}; // Peça de retorno padrão para erro
    
    if (estaVazia(fila)) {
        printf("\nERRO: A fila esta vazia! Nao ha pecas para jogar (dequeue).\n");
        return pecaRemovida;
    }
    
    // Pega a peça na frente
    pecaRemovida = fila->itens[fila->frente];
    
    // Atualiza o índice 'frente' circularmente
    fila->frente = (fila->frente + 1) % MAX_SIZE;
    fila->contador--;
    
    printf("\nSUCESSO: Peça [%c %d] jogada (dequeue) da frente da fila.\n", pecaRemovida.nome, pecaRemovida.id);
    return pecaRemovida;
}

/**
 * @brief Exibe o estado atual da fila de peças.
 * * Mostra o tipo e o ID de cada peça, respeitando a ordem FIFO da fila circular.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 */
void exibirFila(FilaPecas *fila) {
    printf("\n--- Visualizacao da Fila de Pecas (Capacidade: %d | Ocupacao: %d) ---\n", MAX_SIZE, fila->contador);
    
    if (estaVazia(fila)) {
        printf("A fila esta vazia.\n");
        return;
    }
    
    // O loop começa na 'frente' e avança 'contador' vezes
    printf("Fila de pecas: ");
    int i = fila->frente;
    int elementos_exibidos = 0;
    
    while (elementos_exibidos < fila->contador) {
        printf("[%c %d]", fila->itens[i].nome, fila->itens[i].id);
        
        // Se não for o último elemento, adiciona um espaço
        if (elementos_exibidos < fila->contador - 1) {
            printf(" ");
        }
        
        // Avança para o próximo índice circular
        i = (i + 1) % MAX_SIZE;
        elementos_exibidos++;
    }
    printf("\n");
}

/**
 * @brief Exibe as opções de ação para o usuário.
 */
void exibirMenu(void) {
    printf("\n==================================\n");
    printf("        Opcoes de Acao\n");
    printf("==================================\n");
    printf("Codigo | Acao\n");
    printf("----------------------------------\n");
    printf("  1    | Jogar peca (dequeue)\n");
    printf("  2    | Inserir nova peca (enqueue)\n");
    printf("  0    | Sair\n");
    printf("----------------------------------\n");
    printf("Digite o codigo da acao: ");
}

/**
 * @brief Executa a ação escolhida pelo usuário.
 * * @param fila Ponteiro para a estrutura FilaPecas.
 * @param opcao O código da ação escolhida.
 */
void executarAcao(FilaPecas *fila, int opcao) {
    switch (opcao) {
        case 1: // Jogar peça (dequeue)
            dequeue(fila);
            break;
        case 2: { // Inserir nova peça (enqueue)
            Peca novaPeca = gerarPeca(fila);
            enqueue(fila, novaPeca);
            break;
        }
        case 0: // Sair
            printf("\nSaindo do simulador. O jogo Tetris Stack espera por voce!\n");
            break;
        default:
            printf("\nOPCAO INVALIDA. Por favor, digite 1, 2 ou 0.\n");
            break;
    }
}

// --- Função Principal ---

int main() {
    // Declaração da variável que irá armazenar a fila
    FilaPecas filaPrincipal;
    int opcao = -1; // Inicializa com valor diferente de 0

    // 1. Inicializa a estrutura da fila e a pré-popula
    inicializarFila(&filaPrincipal);
    
    do {
        // 2. Exibe o estado atual da fila
        exibirFila(&filaPrincipal);
        
        // 3. Exibe o menu e solicita a opção
        exibirMenu();
        
        // Limpa o buffer de entrada antes de ler
        // (necessário em alguns sistemas após o scanf de números)
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer se a entrada for inválida (não numérica)
            while (getchar() != '\n');
            opcao = -1; // Define uma opção inválida para não sair do loop
            printf("\nERRO: Entrada invalida. Por favor, digite um numero.\n");
            continue; // Pula para a próxima iteração do loop
        }
        
        // 4. Executa a ação escolhida
        executarAcao(&filaPrincipal, opcao);

    } while (opcao != 0);

    return 0;
}