#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "graph.h"

enum State {S, I, R};

typedef struct Event {

    char nome[30];

    int my_state, neighbour_state; // Condições
    int action_state; // Ação

    float probability, timedelay;
    bool repeat;

} Event;

const int QUANT_EVENT = 2;

Event event_queue[] = {
    { "recovery", I, -1, R, 0.7, 5, true }, // Recovery
    { "infection", S, I, I, 0.3, 3, true } // Infection
};

Event *ponteiro_event_queue = event_queue;

void executeEvent(Graph *g, Event *e, int tempo) {
    // Para cada vértice do grafo:
    for(int n = 0; n < QUANT_EVENT; n++) {
        for (int i = 0; i < g->count; i++) {
        Vertex *v = g->vertices[i];

        if((!(strcmp(e[n].nome, "infection"))) && (((tempo - 1) % (int)(e[n].timedelay) == 0) || (tempo == 1))) {
            // Se o estado do vértice atual é o mesmo que 'my_state'
            if (e[n].my_state == v->state) {
                bool has_neighbour_state = false;
                int who;

                // Procure ao menos um vizinho que possua estado igual ao 'neighbour_state'
                for (int j = 0; j < v->count && !has_neighbour_state; j++) {
                    if (v->neighbours[j]->state == e[n].neighbour_state) {
                        has_neighbour_state = true;
                        who = v->neighbours[j]->index;
                    }
                }
                // Se esse vizinho foi encontrado E rodamos o dado e passou a probabilidade
                if ((has_neighbour_state) && (rand()%100 < e[n].probability * 100)) {
                    // Atualize o estado do vértice atual com o estado-ação do evento
                    v->state = e[n].action_state;
                    printf("Vertice %d infectado por %d\n", v->index, who);
                }
            }
        } else if((!(strcmp(e[n].nome, "recovery"))) && (((tempo - 1) % (int)(e[n].timedelay) == 0) || (tempo == 1))) {
            if ((e[n].my_state == v->state) && (rand()%100 < e[n].probability * 100)) {
                // Atualize o estado do vértice atual com o estado-ação do evento
                v->state = e[n].action_state;
                printf("Vertice %d recuperado\n", v->index);
            }
        }
    }
}
}

// O método de ordenação utilizado foi o bubbleSort
void ordenaHeap(Event *ponteiro_event_queue) {
    int continua, fim = QUANT_EVENT;
    Event evento_aux = {0};

    do {
        continua = 0;
        for(int i = 0; i < fim - 1; i++) {
            if(ponteiro_event_queue[i].timedelay > ponteiro_event_queue[i+1].timedelay) {
                strcpy(evento_aux.nome, ponteiro_event_queue[i].nome);
                evento_aux.my_state = ponteiro_event_queue[i].my_state;
                evento_aux.neighbour_state = ponteiro_event_queue[i].neighbour_state;
                evento_aux.action_state = ponteiro_event_queue[i].action_state;
                evento_aux.probability = ponteiro_event_queue[i].probability;
                evento_aux.timedelay = ponteiro_event_queue[i].timedelay;
                evento_aux.repeat = ponteiro_event_queue[i].repeat;

                strcpy(ponteiro_event_queue[i].nome, ponteiro_event_queue[i+1].nome);
                ponteiro_event_queue[i].my_state = ponteiro_event_queue[i+1].my_state;
                ponteiro_event_queue[i].neighbour_state = ponteiro_event_queue[i+1].neighbour_state;
                ponteiro_event_queue[i].action_state = ponteiro_event_queue[i+1].action_state;
                ponteiro_event_queue[i].probability = ponteiro_event_queue[i+1].probability;
                ponteiro_event_queue[i].timedelay = ponteiro_event_queue[i+1].timedelay;
                ponteiro_event_queue[i].repeat = ponteiro_event_queue[i+1].repeat;

                strcpy(ponteiro_event_queue[i+1].nome, evento_aux.nome);
                ponteiro_event_queue[i+1].my_state = evento_aux.my_state;
                ponteiro_event_queue[i+1].neighbour_state = evento_aux.neighbour_state;
                ponteiro_event_queue[i+1].action_state = evento_aux.action_state;
                ponteiro_event_queue[i+1].probability = evento_aux.probability;
                ponteiro_event_queue[i+1].timedelay = evento_aux.timedelay;
                ponteiro_event_queue[i+1].repeat = evento_aux.repeat;

                continua = i;
            }
        }
        fim--;
    } while(continua != 0);
}

void calculaPorcentagem(Graph *g) {

    float suscetivel = 0.0, infectado = 0.0, recuperado = 0.0;

    for (int i = 0; i < g->count; i++) {
        switch (g->vertices[i]->state) {
            case S:
                suscetivel++;
                break;
            case I:
                infectado++;
                break;
            case R:
                recuperado++;
                break;
            default:
                break;
            }
    }

    printf("\nPorcentagem final de suscetiveis: %.2f%%\n", ((suscetivel/(float)(g->count)) * 100));
    printf("\nPorcentagem final de infectados: %.2f%%\n", ((infectado/(float)(g->count)) * 100));
    printf("\nPorcentagem final de recuperado: %.2f%%\n", ((recuperado/(float)(g->count)) * 100));
}

int main() {

    srand(time(NULL));

    int i;
    Graph *g = newGraph();

    #define GRID 10
    // Inicializa os vértices
    for (i = 0; i < GRID*GRID; i++) {
        float prob = rand()%100;
        newVertex(g, (prob < 40) ? I : S);
    }

    // Cria vizinhança de grade
    for (i = 0; i < GRID*GRID; i++) {

        if ((i + 1) % GRID != 0)
            makeNeighbours(g, i, i+1);

        if (i < GRID * (GRID - 1))
            makeNeighbours(g, i, i+GRID);

    }

    printAdj(g);
    printf("--------------------------------------------\n");
    printf("\nTempo = 0\n\n");
    printGraph(g);
    printf("--------------------------------------------\n");

    ordenaHeap(ponteiro_event_queue);

    int dias;

    printf("\nInforme o passo de tempo (em dias): ");
    scanf("%d", &dias);

    for(int i = 1; i <= dias; i++) {
        // for cada passo de tempo
        // pega o evento no topo da lista
        // passa o evento, passa o tempo
        printf("\nTempo = %d\n\n", i);
        executeEvent(g, event_queue, i);

        printGraph(g);
        printf("-------\n");
    }

    printf("\nEstado final do grafo:\n");
    printGraph(g);

    calculaPorcentagem(g);

    freeGraph(g);

    return 0;
}