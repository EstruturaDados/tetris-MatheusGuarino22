#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ------------------------------------------------------
//                 DEFINIÇÃO DAS ESTRUTURAS
// ------------------------------------------------------

typedef struct {
    char tipo;
    int id;
} Peca;

// --------- FILA CIRCULAR ---------
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// --------- PILHA LINEAR ---------
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;


// ------------------------------------------------------
//                    FUNÇÕES DA FILA
// ------------------------------------------------------

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

bool filaVazia(Fila *f) { return f->tamanho == 0; }
bool filaCheia(Fila *f) { return f->tamanho == MAX_FILA; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

Peca dequeue(Fila *f) {
    Peca removida = {'X', -1};
    if (filaVazia(f)) return removida;

    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

void mostrarFila(Fila *f) {
    printf("\n[Fila de Próximas Peças]\n");

    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        return;
    }

    int idx = f->frente;
    for (int i = 0; i < f->tamanho; i++) {
        printf("Pos %d → Tipo: %c | ID: %d\n",
               idx, f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
}


// ------------------------------------------------------
//                    FUNÇÕES DA PILHA
// ------------------------------------------------------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

bool pilhaVazia(Pilha *p) { return p->topo == -1; }
bool pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }

void push(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = x;
    }
}

Peca pop(Pilha *p) {
    Peca removida = {'X', -1};
    if (!pilhaVazia(p)) {
        removida = p->itens[p->topo--];
    }
    return removida;
}

void mostrarPilha(Pilha *p) {
    printf("\n[Pilha de Reserva]\n");

    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("Topo-%d → Tipo: %c | ID: %d\n",
               i, p->itens[i].tipo, p->itens[i].id);
    }
}


// ------------------------------------------------------
//             FUNÇÃO DE GERAR NOVA PEÇA
// ------------------------------------------------------

char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};

Peca gerarPeca(int id) {
    Peca p;
    p.tipo = tipos[rand() % 7];
    p.id = id;
    return p;
}


// ------------------------------------------------------
//        NÍVEL MESTRE – FUNÇÕES DE TROCA
// ------------------------------------------------------

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


void trocarTres(Fila *fila, Pilha *pilha) {

    if (pilha->topo != 2) {
        printf("\nPara trocar 3 pecas, a pilha precisa ter exatamente 3 elementos.\n");
        return;
    }

    if (fila->tamanho < 3) {
        printf("\nA fila nao possui 3 pecas para realizar a troca.\n");
        return;
    }

    int idx = fila->frente;

    for (int i = 0; i < 3; i++) {
        Peca temp = fila->itens[idx];
        fila->itens[idx] = pilha->itens[i];
        pilha->itens[i] = temp;

        idx = (idx + 1) % MAX_FILA;
    }

    printf("\nTroca de 3 pecas entre fila e pilha realizada com sucesso.\n");
}


// ------------------------------------------------------
//                     PROGRAMA PRINCIPAL
// ------------------------------------------------------

int main() {

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    srand(time(NULL));

    int idGlobal = 1;
    int opcao;

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
        scanf("%d", &opcao);

        if (opcao == 1) {
            Peca jogada = dequeue(&fila);
            if (jogada.id != -1) {
                printf("\nPeca jogada: %c | ID: %d\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca(idGlobal++));
            }

        } else if (opcao == 2) {
            if (pilhaCheia(&pilha)) {
                printf("\nPilha cheia.\n");
            } else {
                Peca enviada = dequeue(&fila);
                if (enviada.id != -1) {
                    printf("\nPeca enviada para reserva: %c | ID: %d\n", enviada.tipo, enviada.id);
                    push(&pilha, enviada);
                    enqueue(&fila, gerarPeca(idGlobal++));
                }
            }

        } else if (opcao == 3) {
            Peca usada = pop(&pilha);
            if (usada.id != -1) {
                printf("\nPeca usada da pilha: %c | ID: %d\n", usada.tipo, usada.id);
            }

        } else if (opcao == 4) {
            trocarFrenteTopo(&fila, &pilha);

        } else if (opcao == 5) {
            trocarTres(&fila, &pilha);
        }

    } while (opcao != 0);

    printf("\nEncerrando...\n");
    return 0;
}
