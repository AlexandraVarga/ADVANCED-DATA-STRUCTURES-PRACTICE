#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
    Tema 8 - Varga Alexandra-Ioana

*/

/*

structuri pentru retinere graf

*/
struct Muchie
{
    int sursa, destinatie, pondere;// pondere == cost
};

struct Graf
{
    int V, M;//varfuri,muchii graf
    struct Muchie* muchie;
};

/*

creare graf + alocare dinamica memorie

*/

struct Graf* creareGraf(int V, int M)
{
    struct Graf* graf = (Graf*)malloc(sizeof(Graf));
    graf->V = V;
    graf->M = M;
    graf->muchie = (Muchie*)malloc(M * sizeof(Muchie));
    return graf;
}

// parinte + rang pentru subseturi
struct subset
{
    int parinte;
    int rang;
};

/*

initializare subset-uri
se creeaza o multime noua ce contine elemente date

*/
void MAKE_SET(struct subset subsets[], int n)
{
    for (int i = 0; i < n; i++)
    {
        subsets[i].parinte = i;
        subsets[i].rang = 0;
    }
}

/*

cauta multime in care se afla x

*/
int FIND_SET(struct subset subsets[], int x)
{
    if (subsets[x].parinte != x)
        subsets[x].parinte = FIND_SET(subsets, subsets[x].parinte);
    return subsets[x].parinte;
}

/*

uniune in functie de rang

*/

void UNION(struct subset subsets[], int x, int y)
{
    int radX = FIND_SET(subsets, x);
    int radY = FIND_SET(subsets, y);

    if (subsets[radX].rang < subsets[radY].rang)
        subsets[radX].parinte = radY;
    else if (subsets[radX].rang > subsets[radY].rang)
        subsets[radY].parinte = radX;
    else
    {
        ///daca rangul e acelasi => unul dintre ele devine parinte
        ///si ii marim rangul
        subsets[radY].parinte = radX;
        subsets[radX].rang++;
    }
}

///sortare in functie de ponderi
void sortare(Graf* G)
{
    for (int i = 0; i < G->M - 1; i++)
    {
        for (int j = 0; j < G->M - i - 1; j++)
            if (G->muchie[j].pondere > G->muchie[j + 1].pondere)
            {
                int auxS = G->muchie[j].sursa;
                G->muchie[j].sursa = G->muchie[j + 1].sursa;
                G->muchie[j + 1].sursa = auxS;

                int auxD = G->muchie[j].destinatie;
                G->muchie[j].destinatie = G->muchie[j + 1].destinatie;
                G->muchie[j + 1].destinatie = auxD;

                int auxP = G->muchie[j].pondere;
                G->muchie[j].pondere = G->muchie[j + 1].pondere;
                G->muchie[j + 1].pondere = auxP;
            }
    }
}

/*

    Kruskal
    Algoritm de construire a arborelui de acoperire minima

*/

void KRUSKAL(struct Graf* graf, int* eCopy, Muchie* rezultatCopy)
{
    int const V = graf->V;
    struct Muchie rezultat[4];
    int e = 0;
    int i = 0;

    sortare(graf);

    struct subset* subsets = (struct subset*)malloc(V * sizeof(struct subset));
    MAKE_SET(subsets, V);

    while (e < V - 1 && i < graf->M)
    {
        struct Muchie urmMuchie = graf->muchie[i++];

        int x = FIND_SET(subsets, urmMuchie.sursa);
        int y = FIND_SET(subsets, urmMuchie.destinatie);

        if (x != y)
        {
            rezultat[e++] = urmMuchie;
            UNION(subsets, x, y);
        }
    }
    ///pentru afisare rezultat
    *eCopy = e;
    for (int i = 0; i < e; i++)
        *(rezultatCopy + i) = *(rezultat + i);
}

void rezultat(int e, Muchie* rezultat)
{
    printf("\n KRUSKAL \n\n");
    int minK = 0;
    for (int i = 0; i < e; ++i)
    {
        printf(" Muchia %d -- %d cu ponderea %d\n\n", rezultat[i].sursa, rezultat[i].destinatie, rezultat[i].pondere);
        minK += rezultat[i].pondere;
    }
    printf("\n Cost minim al arborelui de acoperire: %d\n", minK);
}
void demo()
{
    //testare multimi disjuncte
    struct subset* subsets = (struct subset*)malloc(10*sizeof(struct subset));
    //crearea a 10 multimi disjuncte reprezentate prin vectorul de parinti
    MAKE_SET(subsets, 10);
    //multimile disjuncte dupa uniunea a 5 elemente
    UNION(subsets, 1, 3);
    UNION(subsets, 1, 0);
    UNION(subsets, 1, 5);
    UNION(subsets, 5, 7);
    UNION(subsets, 2, 6);
    //aplicarea FIND_SET pe 5 elemente
    for (int i = 5; i < 10; i++)
        printf("\n Reprezentantul nodului %d este %d \n", i, subsets[i].parinte);
}

void initializareMuchii(Graf* graf)
{
    graf->muchie[0].sursa = 0;
    graf->muchie[0].destinatie = 1;
    graf->muchie[0].pondere = 10;

    graf->muchie[1].sursa = 0;
    graf->muchie[1].destinatie = 2;
    graf->muchie[1].pondere = 6;

    graf->muchie[2].sursa = 0;
    graf->muchie[2].destinatie = 3;
    graf->muchie[2].pondere = 5;

    graf->muchie[3].sursa = 1;
    graf->muchie[3].destinatie = 3;
    graf->muchie[3].pondere = 15;

    graf->muchie[4].sursa = 2;
    graf->muchie[4].destinatie = 3;
    graf->muchie[4].pondere = 4;

}
void demoKruskal()
{
    /* Kruskal pentru urmatorul graf cu ponderi
               10
        0--------1
        | \ |
    6| 5\ |15
        | \ |
        2--------3
            4 */
    int V = 4;
    int M = 5;
    struct Graf* graf = creareGraf(V, M);
    ///initializez cu ponderi
    initializareMuchii(graf);
    ///aplicare algoritmp pentru determinare
    int e;
    struct Muchie Rezultat[4];
    KRUSKAL(graf, &e, Rezultat);
    ///rezultat dupa aplicare Kruskal
    rezultat(e, Rezultat);

}
int main()
{
    ///demo pentru MAKE_SET, UNION, FIND_SET
    demo();
    ///testare algoritm Kruskal
    demoKruskal();

    return 0;
}
