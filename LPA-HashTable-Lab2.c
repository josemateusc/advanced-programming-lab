#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

int VETOR_MAX;

typedef struct pessoa_t{
    char nome[50];
    long long int cpf;
    int idade;
} pessoa_t;

typedef struct lista_pessoas_t{
    pessoa_t *pessoa;
    struct lista_pessoas_t *proximo;
} lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t;

bool lista_pessoa_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista){
    lista_pessoas_t *new_person = (lista_pessoas_t*) malloc(sizeof(lista_pessoas_t));
    if (new_person == NULL) return false;
    new_person->pessoa = pessoa; 
    new_person->proximo = *lista;
    *lista = new_person;
    return true;
}

void lista_pessoa_listar(lista_pessoas_t *lista){    
    while(lista!=NULL){
        printf("- %s\t%lld\t%d\n", lista->pessoa->nome, lista->pessoa->cpf, lista->pessoa->idade);
        lista = lista->proximo;
    }
}

tabela_hash_t tabela_hash_pessoas_criar(){
    tabela_hash_t hash_table = (tabela_hash_t) malloc(VETOR_MAX*sizeof(lista_pessoas_t*));
    
    for(int i=0; i<VETOR_MAX;i++){
        hash_table[i] = NULL;
    }

    return hash_table;
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa){
    return (pessoa->cpf % VETOR_MAX);
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash){
    int hash = tabela_hash_pessoas_funcao(pessoa);

    

    return lista_pessoa_adicionar(pessoa,&tabela_hash[hash]);
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
    for(int i=0; i<VETOR_MAX;i++){
        printf("\nPOSIÇÃO %d DA TABELA HASH:\n",i);
        lista_pessoas_t *lista = tabela_hash[i];
        lista_pessoa_listar(lista);
    }
}

int main(int argc, char **argv){

    sscanf(argv[1],"%d",&VETOR_MAX);
    FILE *f = fopen(argv[2], "r");

    if(f == NULL){
        printf("ERROR!\n");
        return 0;
    }

    tabela_hash_t hash_table = tabela_hash_pessoas_criar();
    

    
    while(!feof(f)){
        char nome[50];
        long long int cpf;
        int idade;

        fscanf(f,"%50[^\t]\t%lld\t%d\n",&nome,&cpf,&idade);

        pessoa_t *new_person = (pessoa_t*) malloc(sizeof(pessoa_t));
        strcpy(new_person->nome,nome);
        new_person->cpf = cpf;
        new_person->idade = idade;


        //Adiciona na Tabela;
        if(! tabela_hash_pessoas_adicionar(new_person, hash_table)){
            printf("Erro ao Adicionar o %s",new_person->nome);
        }

    }

    //Print Tabela
    tabela_hash_pessoas_listar(hash_table);

    fclose(f);
}