#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

typedef struct evento_t{
    double tempo;
    int alvo;
    int tipo;
} evento_t;

typedef struct lista_eventos_t{
    evento_t *evento;
    struct lista_eventos_t *proximo;
} lista_eventos_t;

typedef struct lista_vizinhos_t{
    int id;
    struct lista_vizinhos_t *prox;
}lista_vizinhos_t;

typedef struct no_t{
    int id;
    double x, y;
    bool pacote_enviado;
    lista_vizinhos_t *lista_vizinhos;
} no_t;

typedef no_t *grafo_t;

grafo_t criar_grafo(int tam){
    grafo_t grafo = (grafo_t) malloc(tam * sizeof(no_t));
    return grafo;
}

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){
    lista_vizinhos_t *new_neighbor = (lista_vizinhos_t*) malloc(sizeof(lista_vizinhos_t));
    if(new_neighbor == NULL) return false;
    new_neighbor->id = vizinho;
    new_neighbor->prox = *lista;
    *lista = new_neighbor;
    return true;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo){
    for(int i=0; i<tam;i++){
        for(int j=0;j<tam;j++){
            if(grafo[i].id != grafo[j].id){
                double dist = sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2));
                if(dist < raio_comunicacao){
                    lista_vizinhos_adicionar(grafo[i].id,&grafo[j].lista_vizinhos);
                }
            }
        }
    }
}

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *new_event = malloc(sizeof(lista_eventos_t));
    
    //Lista Vazia
    if (new_event == NULL) return false;

    lista_eventos_t *aux;

    new_event->evento = evento;
    new_event->proximo = NULL;

    if(*lista == NULL) *lista = new_event;
    
    else{
        aux = *lista;
        if(new_event->evento->tempo  <  aux->evento->tempo) {
            new_event->proximo = *lista;
            *lista = new_event;
        }
        else {
            while((aux->proximo != NULL)  &&  (aux->proximo->evento->tempo < new_event->evento->tempo))
                aux = aux->proximo;
            new_event->proximo = aux->proximo;
            aux->proximo = new_event;
        }
    }

    return true;
}

double atualizar_tempo(double tempo, int id){
    return tempo + (0.1 + (id * 0.01));
}

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
    while(*lista != NULL){
        evento_t *event;
        lista_eventos_t *aux = (lista_eventos_t*) malloc(sizeof(lista_eventos_t));

        aux = *lista;
        event = aux->evento;
        *lista = aux->proximo;

        printf("[%3.6f] Nó %d recebeu pacote\n", event->tempo, event->alvo);

        if(grafo[event->alvo].pacote_enviado == false){
            lista_vizinhos_t *lista_vizinhos = grafo[event->alvo].lista_vizinhos;
            while(lista_vizinhos != NULL){
                evento_t *new_event = (evento_t*) malloc(sizeof(evento_t));
                new_event->alvo = lista_vizinhos->id;
                new_event->tempo = atualizar_tempo(event->tempo,lista_vizinhos->id);
                new_event->tipo = 1;
                lista_eventos_adicionar_ordenado(new_event,lista);
                
                printf("\t--> Repassando pacote para nó %d...\n",lista_vizinhos->id);

                lista_vizinhos = lista_vizinhos->prox;
            }
            grafo[event->alvo].pacote_enviado = true;
        }
    }
}

int main(int argc, char **argv){


    FILE *f = fopen(argv[1], "r");      //Arquivo com info do grafo

    if(f == NULL){
        printf("ERROR!\n");
        return 0;
    }

    double TAM_RAIO;
    int tam;

    fscanf(f,"%d\t%lf\n",&tam,&TAM_RAIO);

    grafo_t grafo = criar_grafo(tam); 

    int i=0;

    while(!feof(f)){
        fscanf(f,"%d\t%lf\t%lf\n",&grafo[i].id, &grafo[i].x, &grafo[i].y);
        grafo[i].lista_vizinhos = NULL;
        i++;
    }

    grafo_atualizar_vizinhos(tam,TAM_RAIO,grafo);

    lista_eventos_t *lista_eventos = NULL;
    evento_t *new_event = (evento_t*) malloc(sizeof(evento_t));
    new_event->alvo=0;
    new_event->tempo=1.0;
    new_event->tipo=1;
    lista_eventos_adicionar_ordenado(new_event,&lista_eventos);
    
    simulacao_iniciar(&lista_eventos,grafo);

    fclose(f);
}