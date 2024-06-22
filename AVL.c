#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct nodo{
    char produto[30];
    int quantidade;
    struct nodo *esq;
    struct nodo *dir;
    int altura;
    int repeticoes;
};

struct letraNodo {
    char letra;
    struct nodo *arvore;
    struct letraNodo *prox;
};


struct nodo* novoNodo(char *produto, int quantidade)
{
    struct nodo* nodo = (struct nodo*) malloc(sizeof(struct nodo));
    nodo->produto[30] = produto[30];
    nodo->quantidade = quantidade;
    nodo->esq = NULL;
    nodo->dir = NULL;
    nodo->altura = NULL;
    nodo->repeticoes = 1;
    return nodo;

}

struct letraNodo* novaLetra(char letra){

    struct letraNodo* novaLetra = (struct letraNodo*) malloc(sizeof(struct letraNodo));
    novaLetra->letra = letra;
    novaLetra->arvore = NULL;
    novaLetra->prox = NULL;
    return novaLetra;
}

 struct nodo* insere(struct nodo* aux, char *nome, int quantidade){
    if(aux == NULL){
        return novoNodo(nome, quantidade);
    }
    /*
    Se o valor do return for < 0 indica que a str1 é menor que str2
    se o valor do return for > 0 indica que a str2 é menos que str1
    se o valor do return for = 0 indica que a str1 e a str2 são iguais
    */
    
    if(strcmp(nome, aux->produto) < 0){
        aux->esq = insere(aux->esq, nome, quantidade);
    } else if (strcmp(nome, aux->produto) > 0){
        aux->dir = insere(aux->dir, nome, quantidade);
    } else {
        aux->repeticoes++;
    }

    aux->altura = verificaAltura(aux);

    int balanco = balanceamento(aux);

    if (balanco > 1 && strcmp(nome, aux->esq->produto) < 0){
        return rotacaoDir(aux);
    }

    if(balanco < -1 && strcmp(nome, aux->dir->produto) > 0) {
        return rotacaoEsq(aux);
    }

    if(balanco > 1 && strcmp(nome, aux->esq->produto) > 0) {
        aux->dir = rotacaoEsq(aux->esq);
        return rotacaoDir(aux);
    }

    if(balanco < -1 && strcmp(nome, aux->dir->produto) < 0) {
        aux->dir = rotacaoDir(aux->dir);
        return rotacaoEsq(aux);
    }

    return aux;


}

struct nodo* rotacaoDir(struct nodo *y){
    struct nodo *x = y->esq;
    struct nodo *aux = x->dir;

    x->dir = y;
    y->esq = aux;

    y->altura = verificaAltura(y);
    x->altura = verificaAltura(x);

    return x;
}

struct nodo* rotacaoEsq(struct nodo *x){
    struct nodo *y = x->dir;
    struct nodo *aux = y->esq;

    y->esq = x;
    x->dir = aux;


    y->altura = verificaAltura(y);
    x->altura = verificaAltura(x);

    return y;
}


int verificaAltura(struct nodo *aux){
    if(aux == NULL){
        return 0;
    } else {
        return 1 + max(verificaAltura(aux->esq), verificaAltura(aux->dir));
    }
}

int max(int a, int b){
    retunr (a > b) ? a : b;
}

int balanceamento(struct nodo *aux){
    if (aux == NULL){
        return 0;
    } else {
        return verficaAltura(aux->esq) - verificaAltura(aux->dir);
    }
}

int main () {
    struct letraNodo *letraInicio = NULL;
    int opcao, quantidade;
    char letras, nome[30];

    while (opcao != -1){

        printf("0 - Sair\n");
        printf("1 - Insere produto\n");
        scanf("%d", &opcao);
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

     switch (opcao){
            case 0:
                return 0;
            case 1:
                
                printf("Digite o nome do produto:\n");
                scanf("%s", &nome);
                /*printf("Digite a quantidade do produto:\n");
                scanf("%d", &quantidade);
                insere(&letraInicio, nome, quantidade);*/
                break;
        }
    }

}