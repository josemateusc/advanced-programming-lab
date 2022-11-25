#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>

typedef struct lista_vizinhos_t{
    int id;
    struct lista_vizinhos_t *prox;
}lista_vizinhos_t;

typedef struct no_t{
    int id;
    double x, y;
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

void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
    while (lista!=NULL)
    {
        printf("%d ",lista->id);
        lista = lista->prox;
    }
    
}

void grafo_imprimir(int tam, grafo_t grafo){
    for(int i=0; i<tam; i++){
        printf("NO %d: ",grafo[i].id);
        lista_vizinhos_imprimir(grafo[i].lista_vizinhos);
        printf("\n");
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

    grafo_imprimir(tam, grafo);

    fclose(f);
}