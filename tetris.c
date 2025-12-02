#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

/* Estrutura que representa uma peça */
typedef struct {
    char tipo;
    int id;
} Peca;

/* Fila circular */
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

/* Pilha linear */
typedef struct {
    Peca itens[MAX_PILHA];
    int topo; /* -1 quando vazia; topo é índice do último elemento */
} Pilha;


/* ---------- Funções da fila ---------- */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

bool filaVazia(const Fila *f) { return f->tamanho == 0; }
bool filaCheia(const Fila *f) { return f->tamanho == MAX_FILA; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Erro: tentando enfileirar em fila cheia.\n");
        return;
    }
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

/* Retorna peça removida; se fila vazia, retorna id = -1 */
Peca dequeue(Fila *f) {
    Peca removida = {'X', -1};
    if (filaVazia(f)) return removida;

    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

void mostrarFila(const Fila *f) {
    printf("\n[Fila de Próximas Peças]\n");
    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }
    int idx = f->frente;
    for (int i = 0; i < f->tamanho; i++) {
        printf("Pos %d -> Tipo: %c | ID: %d\n", idx, f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
}


/* ---------- Funções da pilha ---------- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

bool pilhaVazia(const Pilha *p) { return p->topo == -1; }
bool pilhaCheia(const Pilha *p) { return p->topo == MAX_PILHA - 1; }

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("Erro: tentando empurrar em pilha cheia.\n");
        return;
    }
    p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    Peca removida = {'X', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo--];
    return removida;
}

void mostrarPilha(const Pilha *p) {
    printf("\n[Pilha de Reserva]\n");
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("Indice %d -> Tipo: %c | ID: %d\n", i, p->itens[i].tipo, p->itens[i].id);
    }
}


/* ---------- Geração de peças ---------- */
char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

Peca gerarPeca(int id) {
    Peca p;
    p.tipo = tipos[rand() % (sizeof(tipos) / sizeof(tipos[0]))];
    p.id = id;
    return p;
}


/* ---------- Operações avançadas (Nível Mestre) ---------- */

/* Troca a peça da frente da fila com o topo da pilha.
   Validações: fila não vazia e pilha com pelo menos 1 peça.
*/
void trocarFrenteTopo(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nNao e possivel trocar: a fila esta vazia.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("\nNao e possivel trocar: a pilha esta vazia.\n");
        return;
    }

    int idxFrente = fila->frente;
    Peca temp = fila->itens[idxFrente];
    fila->itens[idxFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

/* Troca os 3 primeiros da fila com as 3 peças da pilha.
   Validações:
     - pilha deve ter exatamente MAX_PILHA elementos (topo == MAX_PILHA - 1)
     - fila deve ter ao menos 3 elementos (tamanho >= 3)
   Usa índice circular para acessar os 3 primeiros da fila.
*/
void trocarTres(Fila *fila, Pilha *pilha) {
    if (pilha->topo != (MAX_PILHA - 1)) {
        printf("\nPara trocar 3 pecas, a pilha precisa ter exatamente %d elementos.\n", MAX_PILHA);
        return;
    }
    if (fila->tamanho < 3) {
        printf("\nA fila nao possui 3 pecas para realizar a troca (tamanho atual: %d).\n", fila->tamanho);
        return;
    }

    int idx = fila->frente;
    for (int i = 0; i < 3; i++) {
        /* Troca fila[idx] <-> pilha->itens[i]
           Observação: pilha->itens[0] é o "fundo" e pilha->itens[topo] é o topo.
           Aqui trocamos pilha->itens[0..2] conforme a especificação (os 3 elementos da pilha).
        */
        Peca temp = fila->itens[idx];
        fila->itens[idx] = pilha->itens[i];
        pilha->itens[i] = temp;

        idx = (idx + 1) % MAX_FILA;
    }

    printf("\nTroca de 3 pecas entre fila e pilha realizada com sucesso.\n");
}


/* ---------- Programa principal ---------- */
int main(void) {
    Fila fila;
    Pilha pilha;
    int idGlobal = 1;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand((unsigned int) time(NULL));

    /* Preenche a fila inicialmente com MAX_FILA peças */
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(idGlobal++));
    }

    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("\nMenu:\n");
        printf("1 - Jogar peca (usar peca da fila)\n");
        printf("2 - Enviar peca da fila para pilha (reserva)\n");
        printf("3 - Usar peca da pilha (pop)\n");
        printf("4 - Trocar peca da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            /* Entrada invalida: limpa stdin e continua */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Digite um numero.\n");
            continue;
        }

        if (opcao == 1) {
            /* Jogar peça: remove da frente da fila e repoe uma nova peça */
            Peca jogada = dequeue(&fila);
            if (jogada.id != -1) {
                printf("\nPeca jogada: Tipo %c | ID %d\n", jogada.tipo, jogada.id);
                /* Repor a fila para manter sempre MAX_FILA */
                enqueue(&fila, gerarPeca(idGlobal++));
            } else {
                printf("\nNao ha pecas na fila para jogar.\n");
            }

        } else if (opcao == 2) {
            /* Enviar peça da fila para a pilha (reserva) */
            if (pilhaCheia(&pilha)) {
                printf("\nPilha cheia. Nao e possivel enviar.\n");
            } else if (filaVazia(&fila)) {
                printf("\nFila vazia. Nao ha pecas para enviar.\n");
            } else {
                Peca enviada = dequeue(&fila);
                if (enviada.id != -1) {
                    push(&pilha, enviada);
                    printf("\nPeca enviada para reserva: Tipo %c | ID %d\n", enviada.tipo, enviada.id);
                    /* Repor fila */
                    enqueue(&fila, gerarPeca(idGlobal++));
                }
            }

        } else if (opcao == 3) {
            /* Usar peça da pilha */
            if (pilhaVazia(&pilha)) {
                printf("\nPilha vazia. Nao ha pecas para usar.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nPeca usada da pilha: Tipo %c | ID %d\n", usada.tipo, usada.id);
            }

        } else if (opcao == 4) {
            /* Trocar frente da fila com topo da pilha */
            trocarFrenteTopo(&fila, &pilha);

        } else if (opcao == 5) {
            /* Trocar 3 primeiros da fila com os 3 da pilha */
            trocarTres(&fila, &pilha);

        } else if (opcao == 0) {
            /* Sair */
            printf("\nEncerrando...\n");
        } else {
            printf("\nOpcao desconhecida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
