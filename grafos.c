#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// Grafo usando Matriz de Ajacencia
typedef struct {
    int numVertices;
    int matriz[MAX_VERTICES][MAX_VERTICES];
} GrafoMatriz;

// Grafo usando Lista de Ajacencia
typedef struct Nodo {
    int vertice;
    struct Nodo* prox;
} Nodo;

typedef struct {
    int numVertices;
    Nodo* listaAdj[MAX_VERTICES];
} GrafoListaAdj;

// Funções para Grafo de Matriz de Ajacencia
GrafoMatriz* criarGrafoMatriz(int vertices) {
    GrafoMatriz* grafo = (GrafoMatriz*)malloc(sizeof(GrafoMatriz));
    grafo->numVertices = vertices;
    
    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            grafo->matriz[i][j] = 0;
    
    return grafo;
}

void adicionarArestaMatriz(GrafoMatriz* grafo, int u, int v) {
    grafo->matriz[u][v] = 1;
    grafo->matriz[v][u] = 1;
}

// Funções para Grafo de Lista de Ajacencia
GrafoListaAdj* criarGrafoListaAdj(int vertices) {
    GrafoListaAdj* grafo = (GrafoListaAdj*)malloc(sizeof(GrafoListaAdj));
    grafo->numVertices = vertices;
    
    for (int i = 0; i < vertices; i++)
        grafo->listaAdj[i] = NULL;
    
    return grafo;
}

void adicionarArestaLista(GrafoListaAdj* grafo, int u, int v) {
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->vertice = v;
    novoNodo->prox = grafo->listaAdj[u];
    grafo->listaAdj[u] = novoNodo;

    novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->vertice = u;
    novoNodo->prox = grafo->listaAdj[v];
    grafo->listaAdj[v] = novoNodo;
}

void imprimirGrafoLista(GrafoListaAdj* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        Nodo* temp = grafo->listaAdj[i];
        printf("Vertice %d: ", i);
        while (temp) {
            printf("%d -> ", temp->vertice);
            temp = temp->prox;
        }
        printf("NULL\n");
    }
}

// Função de Leitura de Arquivo e Carregamento do Grafo
void lerArquivoGrafo(const char* nomeArquivo, GrafoMatriz* grafoMatriz, GrafoListaAdj* grafoLista, const char* tipo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int u, v;
    while (fscanf(arquivo, "%d %d", &u, &v) != EOF) {
        if (tipo[0] == 'm') { // Matriz de Ajacencia
            adicionarArestaMatriz(grafoMatriz, u, v);
        } else { // Lista de Ajacencia
            adicionarArestaLista(grafoLista, u, v);
        }
    }

    fclose(arquivo);
}

// Função para imprimir o caminho do BFS
void imprimirCaminho(int antecessor[], int s, int t) {
    if (t == -1) {
        printf("Nao há caminho entre os vertices.\n");
        return;
    }
    if (s == t) {
        printf("%d ", s);
    } else {
        imprimirCaminho(antecessor, s, antecessor[t]);
        printf("%d ", t);
    }
}

// Busca em Largura (BFS) usando Lista de Ajacencia
void bfs(GrafoListaAdj* grafo, int s, int t) {
    bool visitado[MAX_VERTICES] = { false };
    int antecessor[MAX_VERTICES];
    for (int i = 0; i < grafo->numVertices; i++) {
        antecessor[i] = -1;
    }

    int fila[MAX_VERTICES];
    int inicio = 0, fim = 0;

    fila[fim++] = s;
    visitado[s] = true;

    while (inicio < fim) {
        int atual = fila[inicio++];

        Nodo* adj = grafo->listaAdj[atual];
        while (adj) {
            int vizinho = adj->vertice;
            if (!visitado[vizinho]) {
                fila[fim++] = vizinho;
                visitado[vizinho] = true;
                antecessor[vizinho] = atual;

                if (vizinho == t) {
                    imprimirCaminho(antecessor, s, t);
                    printf("\n");
                    return;
                }
            }
            adj = adj->prox;
        }
    }

    printf("Nao há caminho entre os vertices.\n");
}

// Busca em Profundidade (DFS) iterativa usando Lista de Ajacencia
void dfsIterativo(GrafoListaAdj* grafo, int s) {
    bool visitado[MAX_VERTICES] = { false };
    int pilha[MAX_VERTICES];
    int topo = -1;

    pilha[++topo] = s;

    while (topo >= 0) {
        int atual = pilha[topo--];

        if (!visitado[atual]) {
            visitado[atual] = true;
            printf("%d ", atual);

            Nodo* adj = grafo->listaAdj[atual];
            while (adj) {
                if (!visitado[adj->vertice]) {
                    pilha[++topo] = adj->vertice;
                }
                adj = adj->prox;
            }
        }
    }
    printf("\n");
}

// Função Principal
int main() {
    int numVertices = 5;

    // Criar Grafo como Lista de Ajacencia
    GrafoListaAdj* grafoLista = criarGrafoListaAdj(numVertices);
    adicionarArestaLista(grafoLista, 0, 1);
    adicionarArestaLista(grafoLista, 0, 2);
    adicionarArestaLista(grafoLista, 1, 3);
    adicionarArestaLista(grafoLista, 3, 4);

    printf("Lista de Ajacencia:\n");
    imprimirGrafoLista(grafoLista);

    printf("\nCaminho BFS de 0 a 4: ");
    bfs(grafoLista, 0, 4);

    printf("\nOrdem DFS iterativa a partir de 0: ");
    dfsIterativo(grafoLista, 0);

    return 0;
}
