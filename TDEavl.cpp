#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura do Nodo da Árvore AVL para Produtos
struct ProdutoNode {
    char nome[100];
    int quantidade;
    struct ProdutoNode *left, *right;
    int height;
};

// Estrutura do Nodo da Lista Encadeada de Letras
struct LetraNode {
    char letra;
    struct ProdutoNode *arvore_produtos;
    struct LetraNode *prox;
};

// Função para obter a altura de um nodo
int height(struct ProdutoNode *N) {
    if (N == NULL)
        return 0;
    return 1 + max(height(N->left), height(N->right));
}

// Função para obter o máximo de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nodo de produto
struct ProdutoNode* newProdutoNode(char *nome, int quantidade) {
    struct ProdutoNode* node = (struct ProdutoNode*)malloc(sizeof(struct ProdutoNode));
    strcpy(node->nome, nome);
    node->quantidade = quantidade;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // novo nodo é inicialmente adicionado como folha
    return node;
}

// Função para criar um novo nodo de letra
struct LetraNode* newLetraNode(char letra) {
    struct LetraNode* node = (struct LetraNode*)malloc(sizeof(struct LetraNode));
    node->letra = letra;
    node->arvore_produtos = NULL;
    node->prox = NULL;
    return node;
}

// Função para fazer rotação à direita
struct ProdutoNode *rightRotate(struct ProdutoNode *y) {
    struct ProdutoNode *x = y->left;
    struct ProdutoNode *T2 = x->right;

    // Realizar rotação
    x->right = y;
    y->left = T2;

    // Atualizar alturas
    y->height = height(y);
    x->height = height(x);

    // Retornar nova raiz
    return x;
}

// Função para fazer rotação à esquerda
struct ProdutoNode *leftRotate(struct ProdutoNode *x) {
    struct ProdutoNode *y = x->right;
    struct ProdutoNode *T2 = y->left;

    // Realizar rotação
    y->left = x;
    x->right = T2;

    // Atualizar alturas
    x->height = height(x);
    y->height = height(y);

    // Retornar nova raiz
    return y;
}

// Função para obter o fator de balanceamento de um nodo
int getBalance(struct ProdutoNode *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Função recursiva para inserir um produto na AVL
struct ProdutoNode* insertProduto(struct ProdutoNode* node, char *nome, int quantidade) {
    // Inserção normal de BST
    if (node == NULL)
        return newProdutoNode(nome, quantidade);

    if (strcmp(nome, node->nome) < 0)
        node->left = insertProduto(node->left, nome, quantidade);
    else if (strcmp(nome, node->nome) > 0)
        node->right = insertProduto(node->right, nome, quantidade);
    else // Nomes iguais não são permitidos
        return node;

    // Atualizar altura deste ancestral
    node->height = height(node);

    // Obter o fator de balanceamento deste ancestral
    int balance = getBalance(node);

    // Se o nodo ficar desbalanceado, então há 4 casos

    // Caso Esquerda-Esquerda
    if (balance > 1 && strcmp(nome, node->left->nome) < 0)
        return rightRotate(node);

    // Caso Direita-Direita
    if (balance < -1 && strcmp(nome, node->right->nome) > 0)
        return leftRotate(node);

    // Caso Esquerda-Direita
    if (balance > 1 && strcmp(nome, node->left->nome) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && strcmp(nome, node->right->nome) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Retornar o ponteiro do nodo (inalterado)
    return node;
}

// Função recursiva para buscar um produto na AVL
struct ProdutoNode* buscarProduto(struct ProdutoNode* node, char *nome) {
    if (node == NULL || strcmp(nome, node->nome) == 0)
        return node;

    if (strcmp(nome, node->nome) < 0)
        return buscarProduto(node->left, nome);

    return buscarProduto(node->right, nome);
}

// Função recursiva para obter o nodo com o menor valor (usado na remoção)
struct ProdutoNode* nodoMinimo(struct ProdutoNode* node) {
    struct ProdutoNode* atual = node;

    while (atual->left != NULL)
        atual = atual->left;

    return atual;
}

// Função recursiva para remover um produto da AVL
struct ProdutoNode* removerProduto(struct ProdutoNode* root, char *nome) {
    if (root == NULL)
        return root;

    if (strcmp(nome, root->nome) < 0)
        root->left = removerProduto(root->left, nome);
    else if (strcmp(nome, root->nome) > 0)
        root->right = removerProduto(root->right, nome);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct ProdutoNode *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            struct ProdutoNode* temp = nodoMinimo(root->right);

            strcpy(root->nome, temp->nome);
            root->quantidade = temp->quantidade;

            root->right = removerProduto(root->right, temp->nome);
        }
    }

    if (root == NULL)
        return root;

    root->height = height(root);

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Função para contar o número de produtos na AVL
int contarProdutos(struct ProdutoNode* node) {
    if (node == NULL)
        return 0;
    return 1 + contarProdutos(node->left) + contarProdutos(node->right);
}

// Função para somar a quantidade total de produtos na AVL
int somarQuantidades(struct ProdutoNode* node) {
    if (node == NULL)
        return 0;
    return node->quantidade + somarQuantidades(node->left) + somarQuantidades(node->right);
}

// Função para exibir a árvore de produtos em ordem alfabética
void exibirProdutosOrdem(struct ProdutoNode *raiz) {
    if (raiz != NULL) {
        exibirProdutosOrdem(raiz->left);
        printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosOrdem(raiz->right);
    }
}

// Função para exibir a árvore de produtos em ordem alfabética inversa
void exibirProdutosOrdemInversa(struct ProdutoNode *raiz) {
    if (raiz != NULL) {
        exibirProdutosOrdemInversa(raiz->right);
        printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosOrdemInversa(raiz->left);
    }
}

// Função para buscar uma letra na lista de letras
struct LetraNode* buscarLetra(struct LetraNode *lista, char letra) {
    struct LetraNode *atual = lista;
    while (atual != NULL) {
        if (atual->letra == letra)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

// Função para exibir produtos de uma letra específica em ordem alfabética
void exibirProdutosLetra(struct LetraNode *lista, char letra, int inversa) {
    struct LetraNode *nodoLetra = buscarLetra(lista, letra);
    if (nodoLetra != NULL) {
        if (inversa)
            exibirProdutosOrdemInversa(nodoLetra->arvore_produtos);
        else
            exibirProdutosOrdem(nodoLetra->arvore_produtos);
    }
}

// Função para exibir produtos acima de uma quantidade específica
void exibirProdutosQuantidadeAcima(struct ProdutoNode *raiz, int quantidade) {
    if (raiz != NULL) {
        exibirProdutosQuantidadeAcima(raiz->left, quantidade);
        if (raiz->quantidade >= quantidade)
            printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosQuantidadeAcima(raiz->right, quantidade);
    }
}

// Função para exibir produtos abaixo de uma quantidade específica
void exibirProdutosQuantidadeAbaixo(struct ProdutoNode *raiz, int quantidade) {
    if (raiz != NULL) {
        exibirProdutosQuantidadeAbaixo(raiz->left, quantidade);
        if (raiz->quantidade <= quantidade)
            printf("Produto: %s, Quantidade: %d\n", raiz->nome, raiz->quantidade);
        exibirProdutosQuantidadeAbaixo(raiz->right, quantidade);
    }
}

// Função para percorrer a árvore em pré-ordem
void preOrder(struct ProdutoNode *root) {
    if (root != NULL) {
        printf("Produto: %s, Quantidade: %d\n", root->nome, root->quantidade);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Função para inserir um produto na lista de letras
void inserirProdutoLista(struct LetraNode **lista, char *nome, int quantidade) {
    char letra = tolower(nome[0]);
    struct LetraNode *nodoLetra = buscarLetra(*lista, letra);

    if (nodoLetra == NULL) {
        nodoLetra = newLetraNode(letra);
        nodoLetra->arvore_produtos = insertProduto(nodoLetra->arvore_produtos, nome, quantidade);
        nodoLetra->prox = *lista;
        *lista = nodoLetra;
    } else {
        nodoLetra->arvore_produtos = insertProduto(nodoLetra->arvore_produtos, nome, quantidade);
    }
}

// Função para consultar a quantidade de um produto
void consultarProduto(struct LetraNode *lista, char *nome) {
    char letra = tolower(nome[0]);
    struct LetraNode *nodoLetra = buscarLetra(lista, letra);
    if (nodoLetra != NULL) {
        struct ProdutoNode *nodoProduto = buscarProduto(nodoLetra->arvore_produtos, nome);
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
void removerProdutoLista(struct LetraNode **lista, char *nome) {
    char letra = tolower(nome[0]);
    struct LetraNode *nodoLetra = buscarLetra(*lista, letra);

    if (nodoLetra != NULL) {
        nodoLetra->arvore_produtos = removerProduto(nodoLetra->arvore_produtos, nome);
        if (nodoLetra->arvore_produtos == NULL) {
            struct LetraNode *atual = *lista, *prev = NULL;
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
void exibirProdutosTotal(struct LetraNode *lista, int inversa) {
    struct LetraNode *atual = lista;
    while (atual != NULL) {
        if (inversa)
            exibirProdutosOrdemInversa(atual->arvore_produtos);
        else
            exibirProdutosOrdem(atual->arvore_produtos);
        atual = atual->prox;
    }
}

// Função principal
int main() {
    struct LetraNode *listaLetras = NULL;
    int opcao, quantidade;
    char nome[100], letra;

    while (1) {
        printf("Menu:\n");
        printf("1. Inserir produto\n");
        printf("2. Consultar produto\n");
        printf("3. Remover produto\n");
        printf("4. Total de produtos\n");
        printf("5. Total de quantidades de produtos\n");
        printf("6. Exibir produtos em ordem\n");
        printf("7. Exibir produtos por letra\n");
        printf("8. Exibir produtos por quantidade acima\n");
        printf("9. Exibir produtos por quantidade abaixo\n");
        printf("10. Percurso pré-fixado\n");
        printf("11. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

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
                printf("Total de produtos: %d\n", contarProdutos(listaLetras->arvore_produtos));
                break;
            case 5:
                printf("Total de quantidades de produtos: %d\n", somarQuantidades(listaLetras->arvore_produtos));
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
                exibirProdutosQuantidadeAcima(listaLetras->arvore_produtos, quantidade);
                break;
            case 9:
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);
                exibirProdutosQuantidadeAbaixo(listaLetras->arvore_produtos, quantidade);
                break;
            case 10:
                preOrder(listaLetras->arvore_produtos);
                break;
            case 11:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }
    return 0;
}