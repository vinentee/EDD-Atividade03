#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Livro {
    char nome[100];
    char autor[100];
};

struct NodeFila {
    struct Livro livro;
    struct NodeFila* proximo;
};

struct NodePilha {
    struct Livro livro;
    struct NodePilha* proximo;
};

int lerLivro(FILE* arquivo, struct Livro* livro) {
    char linha[201];
    if (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char* token = strtok(linha, ";");
        if (token != NULL) {
            strcpy(livro->nome, token);
            token = strtok(NULL, ";");
            if (token != NULL) {
                strcpy(livro->autor, token);
                return 1; 
            }
        }
    }
    return 0;
}

void adicionarLivro(FILE* arquivo, struct Livro livro) {
    fprintf(arquivo, "%s;%s\n", livro.nome, livro.autor);
}

void mostrarFila(struct NodeFila* inicio) {
    struct NodeFila* atual = inicio;
    printf("Fila de Livros:\n");
    while (atual != NULL) {
        printf("%s (%s)\n", atual->livro.nome, atual->livro.autor);
        atual = atual->proximo;
    }
}

void mostrarPilha(struct NodePilha* topo) {
    struct NodePilha* atual = topo;
    printf("Pilha de Livros:\n");
    while (atual != NULL) {
        printf("%s (%s)\n", atual->livro.nome, atual->livro.autor);
        atual = atual->proximo;
    }
}

void adicionarFila(struct NodeFila** inicio, struct Livro livro, FILE* arquivo) {
    struct NodeFila* novo = (struct NodeFila*)malloc(sizeof(struct NodeFila));
    novo->livro = livro;
    novo->proximo = NULL;

    if (*inicio == NULL) {
        *inicio = novo;
    } else {
        struct NodeFila* atual = *inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }

    adicionarLivro(arquivo, livro);
    printf("Livro adicionado na fila: %s (%s)\n", livro.nome, livro.autor);
}

void adicionarPilha(struct NodePilha** topo, struct Livro livro, FILE* arquivo) {
    struct NodePilha* novo = (struct NodePilha*)malloc(sizeof(struct NodePilha));
    novo->livro = livro;
    novo->proximo = *topo;
    *topo = novo;

    adicionarLivro(arquivo, livro);
    printf("Livro adicionado na pilha: %s (%s)\n", livro.nome, livro.autor);
}

void removerFila(struct NodeFila** inicio) {
    if (*inicio == NULL) {
        printf("Não há livros na fila.\n");
        return;
    }

    struct NodeFila* temp = *inicio;
    *inicio = (*inicio)->proximo;
    free(temp);
    printf("Livro removido com sucesso.\n");
}

void removerPilha(struct NodePilha** topo) {
    if (*topo == NULL) {
        printf("Não há livros na pilha.\n");
        return;
    }

    struct NodePilha* temp = *topo;
    *topo = (*topo)->proximo;
    free(temp);
    printf("Livro removido com sucesso.\n");
}

int main() {
    FILE* fila_arquivo = fopen("fila_de_livros.txt", "r");
    FILE* pilha_arquivo = fopen("pilha_de_livros.txt", "r");

    if (fila_arquivo == NULL || pilha_arquivo == NULL) {
        printf("Não foi possível abrir um ou mais arquivos.\n");
        return 1;
    }

    struct NodeFila* fila_inicio = NULL;
    struct NodePilha* pilha_topo = NULL;

    struct Livro livro;

    while (lerLivro(fila_arquivo, &livro)) {
        adicionarFila(&fila_inicio, livro, fila_arquivo);
    }

    rewind(fila_arquivo); 
    while (lerLivro(pilha_arquivo, &livro)) {
        adicionarPilha(&pilha_topo, livro, pilha_arquivo);
    }

    rewind(pilha_arquivo); 

    int escolha;
    do {
        printf("\nMenu:\n");
        printf("1. Mostrar fila de livros\n");
        printf("2. Mostrar pilha de livros\n");
        printf("3. Adicionar livro na fila\n");
        printf("4. Adicionar livro na pilha\n");
        printf("5. Remover livro da fila\n");
        printf("6. Remover livro da pilha\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                mostrarFila(fila_inicio);
                break;
            case 2:
                mostrarPilha(pilha_topo);
                break;
            case 3:
                printf("Digite o nome do livro: ");
                scanf(" %[^\n]", livro.nome);
                printf("Digite o autor do livro: ");
                scanf(" %[^\n]", livro.autor);
                adicionarFila(&fila_inicio, livro, fila_arquivo);
                break;
            case 4:
                printf("Digite o nome do livro: ");
                scanf(" %[^\n]", livro.nome);
                printf("Digite o autor do livro: ");
                scanf(" %[^\n]", livro.autor);
                adicionarPilha(&pilha_topo, livro, pilha_arquivo);
                break;
            case 5:
                removerFila(&fila_inicio);
                break;
            case 6:
                removerPilha(&pilha_topo);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (escolha != 0);

    fclose(fila_arquivo);
    fclose(pilha_arquivo);

    while (fila_inicio != NULL) {
        struct NodeFila* temp = fila_inicio;
        fila_inicio = fila_inicio->proximo;
        free(temp);
    }

    while (pilha_topo != NULL) {
        struct NodePilha* temp = pilha_topo;
        pilha_topo = pilha_topo->proximo;
        free(temp);
    }

    return 0;
}