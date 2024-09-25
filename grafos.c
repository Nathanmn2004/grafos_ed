#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NOS 200

// Estrutura de Lista de Adjacência
typedef struct No {
    int destino;
    int peso;
    struct No* proximo;
} No;

typedef struct {
    No* lista[MAX_NOS];  // Lista de adjacência
    int matriz[MAX_NOS][MAX_NOS];  // Matriz de adjacência
    int total_nos;
} Grafo;

// Inicializa o grafo para ambas as representações (matriz e lista)
void inicializaGrafo(Grafo* g, int total_nos) {
    g->total_nos = total_nos;
    
    // Inicializa a matriz com zeros
    for (int i = 0; i < total_nos; i++) {
        for (int j = 0; j < total_nos; j++) {
            g->matriz[i][j] = 0;
        }
    }
    
    // Inicializa as listas de adjacência como vazias
    for (int i = 0; i < total_nos; i++) {
        g->lista[i] = NULL;
    }
}

// Adiciona uma ligação no grafo para ambas as representações
void adicionaLigacao(Grafo* g, int origem, int destino, int peso) {
    if (peso > 0) {
        // Matriz de adjacência
        g->matriz[origem][destino] = peso;
        g->matriz[destino][origem] = peso;  // Grafo não direcionado

        // Lista de adjacência - adiciona nó na lista de origem
        No* novoNo = (No*)malloc(sizeof(No));
        novoNo->destino = destino;
        novoNo->peso = peso;
        novoNo->proximo = g->lista[origem];
        g->lista[origem] = novoNo;

        // Como é grafo não direcionado, adiciona também na lista do destino
        novoNo = (No*)malloc(sizeof(No));
        novoNo->destino = origem;
        novoNo->peso = peso;
        novoNo->proximo = g->lista[destino];
        g->lista[destino] = novoNo;
    }
}

// Função para imprimir a lista de adjacência
void imprimeListaAdjacencia(Grafo* g) {
    for (int i = 0; i < g->total_nos; i++) {
        printf("Vértice %d: ", i);
        No* atual = g->lista[i];
        while (atual != NULL) {
            printf("-> %d(peso %d) ", atual->destino, atual->peso);
            atual = atual->proximo;
        }
        printf("\n");
    }
}

// Busca em Largura (BFS) usando matriz de adjacência
void buscaLargura(Grafo* g, int origem, int destino) {
    bool visitado[MAX_NOS] = {false};
    int fila[MAX_NOS], caminho[MAX_NOS];
    int inicio = 0, fim = 0;

    for (int i = 0; i < g->total_nos; i++) caminho[i] = -1;

    fila[fim++] = origem;
    visitado[origem] = true;

    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < g->total_nos; i++) {
            if (g->matriz[atual][i] > 0 && !visitado[i]) {
                fila[fim++] = i;
                visitado[i] = true;
                caminho[i] = atual;

                if (i == destino) {
                    inicio = fim;
                    break;
                }
            }
        }
    }

    if (!visitado[destino]) {
        printf("Não há caminho entre %d e %d.\n", origem, destino);
    } else {
        printf("Caminho encontrado: ");
        int trajeto[MAX_NOS], tam = 0;
        for (int no = destino; no != -1; no = caminho[no]) {
            trajeto[tam++] = no;
        }

        for (int i = tam - 1; i >= 0; i--) {
            printf("%d ", trajeto[i]);
        }
        printf("\n");
    }
}

// Busca em Profundidade (DFS) usando matriz de adjacência
void buscaProfundidade(Grafo* g, int origem) {
    bool visitado[MAX_NOS] = {false};
    int pilha[MAX_NOS], topo = -1;

    pilha[++topo] = origem;

    while (topo >= 0) {
        int atual = pilha[topo--];

        if (!visitado[atual]) {
            printf("Visitando vértice %d\n", atual);
            visitado[atual] = true;

            for (int i = g->total_nos - 1; i >= 0; i--) {
                if (g->matriz[atual][i] > 0 && !visitado[i]) {
                    pilha[++topo] = i;
                }
            }
        }
    }
    printf("\n");
}

// Função para carregar arquivo e preencher o grafo
void carregaArquivo(Grafo* g, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int total_nos;
    fscanf(arquivo, "%d", &total_nos);
    inicializaGrafo(g, total_nos);

    for (int i = 0; i < total_nos; i++) {
        for (int j = 0; j < total_nos; j++) {
            int peso;
            fscanf(arquivo, "%d", &peso);
            adicionaLigacao(g, i, j, peso);
        }
    }

    fclose(arquivo);
}

int main() {
    Grafo g;
    int opcao, origem, destino;

    carregaArquivo(&g, "pcv50.txt");

    while (1) {
        printf("\n1. Busca em Largura (BFS)\n2. Busca em Profundidade (DFS)\n3. Imprimir Lista de Adjacência\n4. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o vértice de origem e destino: ");
                scanf("%d %d", &origem, &destino);
                buscaLargura(&g, origem, destino);
                break;
            case 2:
                printf("Digite o vértice inicial para DFS: ");
                scanf("%d", &origem);
                buscaProfundidade(&g, origem);
                break;
            case 3:
                imprimeListaAdjacencia(&g);
                break;
            case 4:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}
