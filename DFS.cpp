#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
/*

    Tema 10, Varga Alexandra-Ioana 30223

*/

typedef struct nod
{
    int val;
    struct nod* next;
} NodeT;

typedef struct
{
    int n;      /// numar de varfuri
    NodeT** t;  /// tabloul de liste (va fi alocat dinamic)
    int* pi; ///vectorul de parinti
    int* d; ///timpii de descoperire
    int* f; ///timpii de finalizare
    int* color; ///informatia de culoare
} Graf;     /// structura pentru memorarea grafului prin liste de adiacenta dinamice; tine si informatia de parcurgere

enum { WHITE, GRAY, BLACK };

/* returneaza 1 daca stiva este goala */
int empty(NodeT* stiva)
{
    return stiva == NULL;
}
//initializare stiva
void init(NodeT** stiva)
{
    *stiva = NULL;
}

void printErr()
{
    printf("Memorie insuficienta!\n");
    exit(1);
}

void push(NodeT** stiva, int val)   //insert first in a linked list
{

    NodeT* el = (NodeT*)malloc(sizeof(NodeT));
    el->val = val;
    el->next = (*stiva);
    *stiva = el;
}

/* sterge elementul din varful stivei si returneaza valoarea lui */
int pop(NodeT** stiva)
{
    int n = -1;
    if (*stiva != NULL)
    {
        NodeT* firstElement = (*stiva);
        *stiva = (*stiva)->next;
        n = firstElement->val;
        free(firstElement);
    }
    return n;
}

void citesteGraf(FILE* f, Graf* pG)
{
    fscanf_s(f, "%d", &pG->n);  // citeste nr. de varfuri
    pG->t = (NodeT**)calloc(pG->n, sizeof(NodeT*));
    if (pG->t == NULL)
        printErr();  // alocare esuata

    pG->pi = (int*)calloc(pG->n, sizeof(int));
    pG->d = (int*)calloc(pG->n, sizeof(int));
    pG->f = (int*)calloc(pG->n, sizeof(int));
    pG->color = (int*)calloc(pG->n, sizeof(int));

    int i;
    for (i = 0; i < pG->n; i++)
    {
        pG->t[i] = NULL;
        pG->pi[i] = -1;
    }

    int v, w;
    while (fscanf_s(f, "%d%d", &v, &w) == 2)
    {
        //graful va fi neorientat, se adauga atat arcul (v,w) cat si (w,v)
        push(&pG->t[w], v);
        push(&pG->t[v], w);
    }
}

/// TODO: Implementati dfs recursiv cu colorarea nodurilor si marcaje de timp
void dfs_visit(int v, Graf* G, int* time)
{
    (*time)++;
    G->d[v] = (*time);
    G->color[v] = GRAY;
    printf("%d ", v);
    NodeT* w = G->t[v];
    while (w != NULL)
    {
        if (G->color[w->val] == WHITE)
        {
            G->pi[w->val] = v;
            dfs_visit(w->val, G, time);
        }
        w = w->next;
    }
    G->color[v] = BLACK;
    (*time)++;
    G->f[v] = (*time);
}

void dfs_recursiv(Graf* G)
{

    int time = 0, i;
    for (i = 0; i < G->n; i++)
    {
        if (G->color[i] == WHITE)
            dfs_visit(i, G, &time);
    }
}

void print(int* arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void reInitTraversalData(Graf* G)
{
    for (int i = 0; i < G->n; i++)
    {
        G->d[i] = -1;
        G->pi[i] = -1;
        G->f[i] = -1;
        G->color[i] = WHITE;
    }

}
void printList(NodeT* first)
{
    NodeT* p = first;
    while (p != NULL)
    {
        printf("%d ", p->val);
        p = p->next;
    }
    printf("\n");
}

void sort(int nod, int* vizitat, NodeT** stiva, Graf* G)
{
    vizitat[nod] = 1;
    while (!empty(G->t[nod]))
    {
        int i = pop(&G->t[nod]);
        if (vizitat[i] == 0)
            sort(i, vizitat, stiva, G);
    }
    push(stiva, nod);
}

void sortare_topologica(Graf* G)
{
    int* vizitat = (int*)calloc(G->n, sizeof(int));

    NodeT** stiva = (NodeT**)calloc(G->n, sizeof(NodeT*));
    init(stiva);

    for (int j = 0; j < G->n; j++)
        if (vizitat[j] == 0)
            sort(j, vizitat, stiva, G);

    printf(" Sortare topologica: ");
    while (!empty(*stiva))
    {
        int x = pop(stiva);
        printf("%d ", x);
    }
    printf("\n");
}

void print(NodeT* lista)
{
    while (lista != NULL)
    {
        printf("%d ", lista->val);
        lista = lista->next;
    }
    printf("\n");
}

int minim(int a, int b)
{
    if (a <= b)
        return a;
    else
        return b;
}

void SCCUtil(Graf* G, int u, int* disc, int* low, stack<int>* st, int* sMember, int* time)
{
    (*time)++;
    disc[u] = *time;
    low[u] = *time;
    st->push(u);
    sMember[u] = 1;
    int z = 0;
    while (G->t[u] != NULL)
    {
        int v = G->t[u]->val;

        if (disc[v] == -1)
        {
            SCCUtil(G, v, disc, low, st, sMember, time);
            low[u] = minim(low[u], low[v]);
        }
        else if (sMember[v] == 1)
            low[u] = minim(low[u], disc[v]);

        G->t[u] = G->t[u]->next;
    }
    if (low[u] == disc[u])
    {
        while (st->top() != u)
        {
            z = (int)st->top();
            printf(" %d ", z);
            sMember[z] = 0;
            st->pop();
        }
        z = (int)st->top();
        printf(" %d\n", z);
        sMember[z] = 0;
        st->pop();
    }
}

void Tarjan(Graf* G)
{
    int time = 0;
    int* disc = (int*)calloc(G->n, sizeof(int));
    int* low = (int*)calloc(G->n, sizeof(int));
    int* sMember = (int*)calloc(G->n, sizeof(int));
    stack<int>* st = new stack<int>();
    for (int i = 0; i < G->n; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        sMember[i] = 0;
    }
    for (int i = 0; i < G->n; i++)
    {
        if (disc[i] == -1)
            SCCUtil(G, i, disc, low, st, sMember, &time);
    }
}

void demo(Graf G)
{
    FILE* f; 
    fopen_s(&f,"graf.txt", "r");
    if (f == NULL)
    {
        perror("graf.txt");
        exit(1);
    }
    citesteGraf(f, &G);
    fclose(f);

    ///afisare liste de adiacenta graf
    printf("Liste de adiacenta ale grafului:\n\n");
    for (int i = 0; i < G.n; i++)
    {
        printf("L(%d): ", i);
        printList(G.t[i]);
    }

    ///DFS - varianta recursiva
    printf("\n DFS varianta recursiva: ");
    dfs_recursiv(&G);
    printf(" Vectorul de parinti: ");
    print(G.pi, G.n);
    printf(" Timpii de descoperire: ");
    print(G.d, G.n);
    printf(" Timpii de finalizare: ");
    print(G.f, G.n);
    reInitTraversalData(&G);
    ///SORTARE TOPOLOGICA GRAF
    //sortare_topologica(&G);
    ///ALGORITMUL LUI TARJAN
    printf(" Algoritmul lui Tarjan:\n");
    Tarjan(&G);

    free(G.color);
    free(G.d);
    free(G.f);

    int i;
    for (i = 0; i < G.n; i++)
    {
        /// TODO: eliberati nodurile din lista de adiacenta a lui i, daca e nevida
        if (G.t[i] != NULL)
            free(G.t[i]);
    }

    free(G.t);
}
int main()
{
    Graf G{};
    /// demo DFS + sortare topologica + algoritm Tarjan
    demo(G);
    
    return 0;
}
