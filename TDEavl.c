#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura do Nodo da Árvore AVL para Produtos
struct nodo {
    char nome[100];
    int quantidade;
    struct nodo *esq, *dir;
    int verificaAltura;
};

// Estrutura do Nodo da Lista Encadeada de Letras
struct letraNodo {
    char letra;
    struct nodo *arvore;
    struct letraNodo *prox;
};

// Função para obter a altura de um nodo
int verificaAltura(struct nodo *aux) {
    if (aux == NULL)
        return 0;
    return 1 + max(verificaAltura(aux->esq), verificaAltura(aux->dir));
}

// Função para obter o máximo de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nodo de produto
struct nodo* novoNodo(char *nome, int quantidade) {
    struct nodo* novo = (struct nodo*)malloc(sizeof(struct nodo));
    if (novo == NULL) {
        perror("Erro ao alocar memória para novo nodo");
        exit(EXIT_FAILURE);
    }
    strcpy(novo->nome, nome);
    novo->quantidade = quantidade;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->verificaAltura = 1;  // novo nodo é inicialmente adicionado como folha
    return novo;
}

// Função para criar um novo nodo de letra
struct letraNodo* newletraNodo(char letra) {
    struct letraNodo* novo = (struct letraNodo*)malloc(sizeof(struct letraNodo));
    if (novo == NULL) {
        perror("Erro ao alocar memória para novo nodo de letra");
        exit(EXIT_FAILURE);
    }
    novo->letra = letra;
    novo->arvore = NULL;
    novo->prox = NULL;
    return novo;
}

// Função para fazer rotação à direita
struct nodo *rotacaoDir(struct nodo *y) {
    struct nodo *x = y->esq;
    struct nodo *T2 = x->dir;

    // Realizar rotação
    x->dir = y;
    y->esq = T2;

    // Atualizar alturas
    y->verificaAltura = verificaAltura(y);
    x->verificaAltura = verificaAltura(x);

    // Retornar nova raiz
    return x;
}

// Função para fazer rotação à esquerda
struct nodo *rotacaoEsq(struct nodo *x) {
    struct nodo *y = x->dir;
    struct nodo *T2 = y->esq;

    // Realizar rotação
    y->esq = x;
    x->dir = T2;

    // Atualizar alturas
    x->verificaAltura = verificaAltura(x);
    y->verificaAltura = verificaAltura(y);

    // Retornar nova raiz
    return y;
}

// Função para obter o fator de balanceamento de um nodo
int Balanceamento(struct nodo *aux) {
    if (aux == NULL)
        return 0;
    return verificaAltura(aux->esq) - verificaAltura(aux->dir);
}

// Função recursiva para inserir um produto na AVL
struct nodo* insertProduto(struct nodo* novo, char *nome, int quantidade) {
    // Inserção normal de BST
    if (novo == NULL)
        return novoNodo(nome, quantidade);

    if (strcmp(nome, novo->nome) < 0)
        novo->esq = insertProduto(novo->esq, nome, quantidade);
    else if (strcmp(nome, novo->nome) > 0)
        novo->dir = insertProduto(novo->dir, nome, quantidade);
    else // Nomes iguais não são permitidos
        return novo;

    // Atualizar altura deste ancestral
    novo->verificaAltura = verificaAltura(novo);

    // Obter o fator de balanceamento deste ancestral
    int balance = Balanceamento(novo);

    // Se o nodo ficar desbalanceado, então há 4 casos

    // Caso Esquerda-Esquerda
    if (balance > 1 && strcmp(nome, novo->esq->nome) < 0)
        return rotacaoDir(novo);

    // Caso Direita-Direita
    if (balance < -1 && strcmp(nome, novo->dir->nome) > 0)
        return rotacaoEsq(novo);

    // Caso Esquerda-Direita
    if (balance > 1 && strcmp(nome, novo->esq->nome) > 0) {
        novo->esq = rotacaoEsq(novo->esq);
        return rotacaoDir(novo);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && strcmp(nome, novo->dir->nome) < 0) {
        novo->dir = rotacaoDir(novo->dir);
        return rotacaoEsq(novo);
    }

    // Retornar o ponteiro do nodo (inalterado)
    return novo;
}

// Função recursiva para buscar um produto na AVL
struct nodo* buscarProduto(struct nodo* novo, char *nome) {
    if (novo == NULL || strcmp(nome, novo->nome) == 0)
        return novo;

    if (strcmp(nome, novo->nome) < 0)
        return buscarProduto(novo->esq, nome);

    return buscarProduto(novo->dir, nome);
}

// Função recursiva para obter o nodo com o menor valor (usado na remoção)
struct nodo* nodoMinimo(struct nodo* novo) {
    struct nodo* atual = novo;

    while (atual->esq != NULL)
        atual = atual->esq;

    return atual;
}

// Função recursiva para remover um produto da AVL
struct nodo* removerProduto(struct nodo* novo, char *nome) {
    if (novo == NULL)
        return novo;

    if (strcmp(nome, novo->nome) < 0)
        novo->esq = removerProduto(novo->esq, nome);
    else if (strcmp(nome, novo->nome) > 0)
        novo->dir = removerProduto(novo->dir, nome);
    else {
        if ((novo->esq == NULL) || (novo->dir == NULL)) {
            struct nodo *temp = novo->esq ? novo->esq : novo->dir;

            if (temp == NULL) {
                temp = novo;
                novo = NULL;
            } else
                *novo = *temp;

            free(temp);
        } else {
            struct nodo* temp = nodoMinimo(novo->dir);

            strcpy(novo->nome, temp->nome);
            novo->quantidade = temp->quantidade;

            novo->dir = removerProduto(novo->dir, temp->nome);
        }
    }

    if (novo == NULL)
        return novo;

    novo->verificaAltura = verificaAltura(novo);

    int balance = Balanceamento(novo);

    if (balance > 1 && Balanceamento(novo->esq) >= 0)
        return rotacaoDir(novo);

    if (balance > 1 && Balanceamento(novo->esq) < 0) {
        novo->esq = rotacaoEsq(novo->esq);
        return rotacaoDir(novo);
    }

    if (balance < -1 && Balanceamento(novo->dir) <= 0)
        return rotacaoEsq(novo);

    if (balance < -1 && Balanceamento(novo->dir) > 0) {
        novo->dir = rotacaoDir(novo->dir);
        return rotacaoEsq(novo);
    }

    return novo;
}

// Função para somar a quantidade total de produtos na AVL
// Função para somar todas as quantidades de produtos
int somarQuantidades(struct letraNodo* lista) {
    if (lista == NULL) {
        return 0;
    }

    int soma = 0;

    // Percorre a lista de letras
    struct letraNodo *atual = lista;
    while (atual != NULL) {
        // Percorre a árvore de produtos para cada letra
        struct nodo *raiz = atual->arvore;
        while (raiz != NULL) {
            soma += raiz->quantidade;
            soma += somarQuantidades(raiz->esq);  // Soma quantidades na subárvore esquerda
            raiz = raiz->dir;  // Move para o próximo nodo na árvore
        }
        atual = atual->prox;  // Move para a próxima letra na lista de letras
    }

    return soma;
}

// Função para exibir a árvore de produtos em ordem alfabética
void exibirProdutosOrdem(struct nodo *raiz) {
    if (raiz != NULL) {
        exibirProdutosOrdem(raiz->esq);
        printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosOrdem(raiz->dir);
    }
}

// Função para exibir a árvore de produtos em ordem alfabética inversa
void exibirProdutosOrdemInversa(struct nodo *raiz) {
    if (raiz != NULL) {
        exibirProdutosOrdemInversa(raiz->dir);
        printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosOrdemInversa(raiz->esq);
    }
}

// Função para buscar uma letra na lista de letras
struct letraNodo* buscarLetra(struct letraNodo *lista, char letra) {
    struct letraNodo *atual = lista;
    while (atual != NULL) {
        if (atual->letra == letra)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Função para exibir produtos de uma letra específica em ordem alfabética
void exibirProdutosLetra(struct letraNodo *lista, char letra, int inversa) {
    struct letraNodo *nodoLetra = buscarLetra(lista, letra);
    if (nodoLetra != NULL) {
        if (inversa)
            exibirProdutosOrdemInversa(nodoLetra->arvore);
        else
            exibirProdutosOrdem(nodoLetra->arvore);
    }
}

// Função para exibir produtos acima de uma quantidade específica
void exibirProdutosQuantidadeAcima(struct nodo *raiz, int quantidade) {
    if (raiz != NULL) {
        exibirProdutosQuantidadeAcima(raiz->esq, quantidade);
        if (raiz->quantidade >= quantidade)
            printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosQuantidadeAcima(raiz->dir, quantidade);
    }
}

// Função para exibir produtos abaixo de uma quantidade específica
void exibirProdutosQuantidadeAbaixo(struct nodo *raiz, int quantidade) {
    if (raiz != NULL) {
        exibirProdutosQuantidadeAbaixo(raiz->esq, quantidade);
        if (raiz->quantidade <= quantidade)
            printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosQuantidadeAbaixo(raiz->dir, quantidade);
    }
}

// Função para contar o número de produtos na AVL
int contarProdutos(struct letraNodo* lista) {
    int count = 0;
    struct letraNodo *atual = lista;
    while (atual != NULL) {
        count += contarProdutosAux(atual->arvore);
        atual = atual->prox;
    }
    return count;
}

// Função auxiliar para contar os produtos em uma árvore
int contarProdutosAux(struct nodo* novo) {
    if (novo == NULL)
        return 0;
    
    return 1 + contarProdutosAux(novo->esq) + contarProdutosAux(novo->dir);
}

// Função para percorrer a árvore em pré-ordem
void preOrdem(struct nodo *novo) {
    if (novo != NULL) {
        printf("Produto: %s, Quantidade: %d\n", novo->nome, novo->quantidade);
        preOrdem(novo->esq);
        preOrdem(novo->dir);
    }
}

// Função para percorrer a árvore em pós-ordem
void posOrdem(struct nodo *novo) {
    if (novo != NULL) {
        posOrdem(novo->esq);
        posOrdem(novo->dir);
        printf("Produto: %s, Quantidade: %d\n", novo->nome, novo->quantidade);
    }
}

// Função para percorrer a árvore em ordem central
void central(struct nodo *novo) {
    if (novo != NULL) {
        central(novo->esq);
        printf("Produto: %s, Quantidade: %d\n", novo->nome, novo->quantidade);
        central(novo->dir);
    }
}

// Função para inserir um produto na lista de letras
void inserirProdutoLista(struct letraNodo **lista, char *nome, int quantidade) {
    char letra = tolower(nome[0]);
    struct letraNodo *nodoLetra = buscarLetra(*lista, letra);

    if (nodoLetra == NULL) {
        nodoLetra = newletraNodo(letra);
        nodoLetra->arvore = insertProduto(nodoLetra->arvore, nome, quantidade);
        nodoLetra->prox = *lista;
        *lista = nodoLetra;
    } else {
        nodoLetra->arvore = insertProduto(nodoLetra->arvore, nome, quantidade);
    }
}

// Função para consultar a quantidade de um produto
void consultarProduto(struct letraNodo *lista, char *nome) {
    char letra = tolower(nome[0]);
    struct letraNodo *nodoLetra = buscarLetra(lista, letra);
    if (nodoLetra != NULL) {
        struct nodo *nodoProduto = buscarProduto(nodoLetra->arvore, nome);
        if (nodoProduto != NULL) {
            printf("Produto: %s, Quantidade: %d\n", nodoProduto->nome, nodoProduto->quantidade);
        } else {
            printf("Produto %s não encontrado.\n", nome);
        }
    } else {
        printf("Produtos com a letra %c não encontrados.\n", letra);
    }
}

// Função para remover um produto da lista de letras
void removerProdutoLista(struct letraNodo **lista, char *nome) {
    char letra = tolower(nome[0]);
    struct letraNodo *nodoLetra = buscarLetra(*lista, letra);

    if (nodoLetra != NULL) {
        nodoLetra->arvore = removerProduto(nodoLetra->arvore, nome);
        if (nodoLetra->arvore == NULL) {
            struct letraNodo *atual = *lista, *prev = NULL;
            while (atual != NULL && atual->letra != letra) {
                prev = atual;
                atual = atual->prox;
            }
            if (atual != NULL) {
                if (prev != NULL)
                    prev->prox = atual->prox;
                else
                    *lista = atual->prox;
                free(atual);
            }
        }
    }
}

// Função para exibir todos os produtos em ordem alfabética ou inversa
void exibirProdutosTotal(struct letraNodo *lista, int inversa) {
    struct letraNodo *atual = lista;
    while (atual != NULL) {
        if (inversa)
            exibirProdutosOrdemInversa(atual->arvore);
        else
            exibirProdutosOrdem(atual->arvore);
        atual = atual->prox;
    }
}

// Função principal
int main() {
    struct letraNodo *listaLetras = NULL;
    int opcao, quantidade;
    char nome[100], letra;

    while (1) {
        printf("\n\n\n");
        printf("1. Inserir produto\n");
        printf("2. Consultar produto\n");
        printf("3. Remover produto\n");
        printf("4. Total de produtos\n");
        printf("5. Total de quantidades de produtos\n");
        printf("6. Exibir produtos em ordem\n");
        printf("7. Exibir produtos por letra\n");
        printf("8. Exibir produtos por quantidade acima\n");
        printf("9. Exibir produtos por quantidade abaixo\n");
        printf("10. Percurso pre-fixado\n");
        printf("11. Percurso pos-fixado\n");
        printf("12. Percurso central\n");
        printf("13. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1:
                printf("Digite o nome do produto: ");
                scanf("%s", nome);
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);
                inserirProdutoLista(&listaLetras, nome, quantidade);
                break;
            case 2:
                printf("Digite o nome do produto: ");
                scanf("%s", nome);
                consultarProduto(listaLetras, nome);
                break;
            case 3:
                printf("Digite o nome do produto: ");
                scanf("%s", nome);
                removerProdutoLista(&listaLetras, nome);
                break;
            case 4:
                printf("Total de produtos na árvore: %d\n", contarProdutos(listaLetras));
                break;
            case 5:
                printf("Total de quantidades de produtos: %d\n", somarQuantidades(listaLetras));
                break;
            case 6:
                exibirProdutosTotal(listaLetras, 0);
                break;
            case 7:
                printf("Digite a letra: ");
                scanf(" %c", &letra);
                exibirProdutosLetra(listaLetras, letra, 0);
                break;
            case 8:
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);
                exibirProdutosQuantidadeAcima(listaLetras->arvore, quantidade);
                break;
            case 9:
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);
                exibirProdutosQuantidadeAbaixo(listaLetras->arvore, quantidade);
                break;
            case 10:
                printf("Percurso pré-ordem:\n");
                preOrdem(listaLetras->arvore);
                break;
            case 11:
                printf("Percurso pós-ordem:\n");
                posOrdem(listaLetras->arvore);
                break;
            case 12:
                printf("Percurso em ordem central:\n");
                central(listaLetras->arvore);
                break;
            case 13:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
