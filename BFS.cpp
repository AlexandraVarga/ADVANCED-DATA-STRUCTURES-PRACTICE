#   define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "bfs.h"

//structura pentru coada

typedef struct {
    int size, head, tail;
    Node** vect;
}Queue;

//verificare coordonate vecini daca nu ies in afara matricei

int verificare (const Grid* grid, Point p) 
{
    if (p.col >= 0 && p.col < grid->cols && p.row >= 0 && p.row < grid->rows)
        return 1;
    return 0;
}


int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int cnt = 0;
    Point pVecin{};
    // stanga
    pVecin.col = p.col - 1;
    pVecin.row = p.row;
    if (verificare(grid, pVecin) && grid->mat[pVecin.row][pVecin.col] == 0) {
        neighb[cnt++] = pVecin;
    }

    // sus
    pVecin.col = p.col;
    pVecin.row = p.row - 1;
    if (verificare(grid, pVecin) && grid->mat[pVecin.row][pVecin.col] == 0) {
        neighb[cnt++] = pVecin;
    }

    // jos
    pVecin.col = p.col;
    pVecin.row = p.row + 1;
    if (verificare(grid, pVecin) && grid->mat[pVecin.row][pVecin.col] == 0) {
        neighb[cnt++] = pVecin;
    }

    // dreapta
    pVecin.col = p.col + 1;
    pVecin.row = p.row;
    if (verificare(grid, pVecin) && grid->mat[pVecin.row][pVecin.col] == 0) {
        neighb[cnt++] = pVecin;
    }

    return cnt;
}

void grid_to_graph(const Grid* grid, Graph* graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node* nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }
            else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}


// Aloca memorie pentru o coada cu maxim n elemente

Queue * create_empty_queue(int n) 
{
    Queue* Q = (Queue*)malloc(sizeof(Queue));
    (*Q).size = 0;
    (*Q).head = 0;
    (*Q).tail = 0;
    (*Q).vect = (Node**)malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) 
    {
        (*Q).vect[i] = (Node*)malloc(sizeof(Node));
        memset((*Q).vect[i], 0, sizeof(Node*)); //setez elementele pe 0
    }
    return Q;
}

// verificare coada daca este goala

boolean is_empty(Queue Q)
{
    return Q.size == 0;
}

//adaugare in coada

void enqueue(Queue* Q, Node* x) {
    
    (*Q).vect[(*Q).tail] = x;
    (*Q).tail++;
    (*Q).size++;
}

// scoatere din coada

Node* dequeue(Queue* Q) 
{
    Node* ret = (*Q).vect[(*Q).head];
    (*Q).head++;
    (*Q).size--;
    return ret;
}


void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    
    //toate nodurile sunt albe
    for (int i = 0; i < (*graph).nrNodes; i++) 
    {
        if ((*graph).v[i] != s) 
        {
            (*graph).v[i]->color = COLOR_WHITE;
            (*graph).v[i]->dist = INT_MAX;
            (*graph).v[i]->parent = NULL;
        }
    }
   
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    Queue* Q = create_empty_queue((*graph).nrNodes);
    enqueue(Q, s);
    
    while (!is_empty(*Q)) 
    {
        Node* u = dequeue(Q);
        for (int i = 0; i < u->adjSize; i++) 
        {    
            if (u->adj[i]->color == COLOR_WHITE) {
                u->adj[i]->color = COLOR_GRAY;
                u->adj[i]->dist = u->dist + 1;
                u->adj[i]->parent = u;
                enqueue(Q, u->adj[i]);
            }
        }
        //dupa parcurgere setez culoare nod pe negru
        u->color = COLOR_BLACK;
    }
}

//reprezentare prin vector de parinti pentru pretty print

void reprezentare_R1(int* p, Point* repr, int n, int root, int x) 
{
    for (int i = 0; i < x; i++) 
        printf("        ");
    
    printf("(%d, %d)\n", repr[root].row, repr[root].col);

    for (int i = 0; i < n; i++) 
    {
        if (p[i] == root) 
            reprezentare_R1(p, repr, n, i, x + 1);
    }
}

void print_bfs_tree(Graph* graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int* p = NULL; //the parent array
    Point* repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        }
        else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) 
    {
        if (graph->v[i]->color == COLOR_BLACK) 
        {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            }
            else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                }
                else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        
        int root;
        for (int i = 0; i < n; i++) 
        {
            if (p[i] == -1) 
            {
                root = i;
                break;
            }
        }
        reprezentare_R1(p, repr, n, root, 0);
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}


int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    return -1;
}


void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}

