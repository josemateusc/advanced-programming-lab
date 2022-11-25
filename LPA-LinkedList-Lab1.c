#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct evento_t{
    double tempo;
    int alvo;
    int tipo;
} evento_t;

typedef struct lista_eventos_t{
    evento_t *evento;
    struct lista_eventos_t *proximo;
} lista_eventos_t;

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *new_event = (lista_eventos_t*) malloc(sizeof(lista_eventos_t));
    if (new_event == NULL) return false;
    new_event->evento = evento; 
    new_event->proximo = *lista;
    *lista = new_event;
    return true;
}

void lista_eventos_listar(lista_eventos_t *lista){    
    printf("Lista do Eventos: \n");

    while(lista!=NULL){
        printf("\nTempo: %3.6lf\n", lista->evento->tempo);
        printf("Alvo: %d\n", lista->evento->alvo);
        printf("Tipo: %d\n", lista->evento->tipo);
        
        lista = lista->proximo;
    }
}

//Questão 2 - insere elemento no fim
bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *new_event = (lista_eventos_t*) malloc(sizeof(lista_eventos_t));

    if (new_event == NULL) return false;

    lista_eventos_t *aux;

    new_event->evento = evento;
    new_event->proximo = NULL;

    //Lista Vazia
    if(*lista == NULL) *lista = new_event;

    else{
        aux = *lista;
        while(aux->proximo) aux = aux->proximo;
        aux->proximo = new_event;
    }
    return true;
}

//Questão 3 - insere evento ordenado
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

int main(int argc, char **argv){

    FILE *f = fopen(argv[1], "r");

    if(f == NULL){
        printf("ERROR!\n");
        return 0;
    }

    lista_eventos_t *lista1 = NULL;
    lista_eventos_t *lista2 = NULL;
    lista_eventos_t *lista3 = NULL;

    while(!feof(f)){
        double tempo;
        int alvo;
        int tipo;

        fscanf(f,"%lf\t%d\t%d\n",&tempo,&alvo,&tipo);

        evento_t *new_event = (evento_t*) malloc(sizeof(evento_t));
        new_event->tempo = tempo;
        new_event->alvo = alvo;
        new_event->tipo = tipo;

        lista_eventos_adicionar_ordenado(new_event,&lista3);
        lista_eventos_adicionar_fim(new_event,&lista2);
        lista_eventos_adicionar_inicio(new_event,&lista1);
    }


    printf("#### Primeira Questão - Inserir no Início ####\n");
    lista_eventos_listar(lista1);
    printf("\n#### Segunda Questão - Inserir no Final ####\n");
    lista_eventos_listar(lista2);
    printf("\n#### Terceira Questão - Inserir Ordenado ####\n");
    lista_eventos_listar(lista3);

    fclose(f);
}