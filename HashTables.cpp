/*

       Tema Varga Alexandra-Ioana, grupa 30223

*/

#include "Profiler.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Profiler p("Hash Tables");

/*

Din tabelul rezultat se observa ca:
Cu cat factorul de umplere este mai mare => cu atat este mai mare si efortul de cautare

*/

int efort = 0;///in efort o sa stochez pe rand numarul de accesari la tabela pentru diferite elemente
int maxim = 0;///in maxim o sa stochez pe rand efortul maxim de accesari la tabela pentru un anumit element

/*

golesc tabela - o setez pe -1

*/

void set_table_free(int hashTable[], int n)
{
    for (int i = 0; i < n; i++)
    {
        hashTable[i] = -1;
    }
}

/*

se probeaza un anumit hashIndex

*/

int quadratic_probing(int k, int m, int i)
{
    if (k > 0)
        return (k + i + i * i) % m;
    else
        return (k + i + i * i) % m + m;
    return 0;
}
/*

inserare in hash table cu quadric probing

*/
void insert_key(int hashTable[], int n, int k)
{
    int hashIndex;
    for (int i = 0; i < n; i++)
    {
        hashIndex = quadratic_probing(k, n, i);
        if (hashTable[hashIndex] == -1)
        {
            hashTable[hashIndex] = k;//adaug cheia
            break;
        }
    }
}
/*

cautare cheia k - determinare efort de cautare = nr de cautari pana la gasire cheii in tabela

*/
void search_key(int hashTable[], int n, int k)
{
    int i = 0;
    int hashIndex = quadratic_probing(k, n, i);
    int j = hashIndex;
    if (hashTable[hashIndex] == k)//am gasit cheia
    {
        efort++;
        return;
    }
    else
    {
        //caut mai departe in tabela
        i++;
        hashIndex = quadratic_probing(k, n, i);
        while (hashTable[hashIndex] != -1 && hashIndex != j)
        {
            efort++;
            hashIndex = quadratic_probing(k, n, i);
            if (hashTable[hashIndex] == k)
            {
                efort++;
                break;
            }
            i++;
        }
    }
}

void zone_cautare_dreapta_mediu(int hashTable[], int n, int r, int i)
{
    while (r > 0)
    {
        if (hashTable[i] != -1)
        {
            search_key(hashTable, n, hashTable[i]);
        }
        i++;
        r--;
    }
}
void zone_cautare_stanga_mediu(int hashTable[], int n, int r, int i)
{
    while (r > 0)
    {
        if (hashTable[i] != -1)
        {
            search_key(hashTable, n, hashTable[i]);
        }
        i--;
        r--;
    }
}
/*

Pentru o cautare cat mai uniforma
se iau cate 500 de elemente de la inceput, mijloc si finalul tabelei
si se afla nr de accesari are tabelei

*/
void efort_mediu(int hashTable[], int n, int size1, int a[])
{
    efort = 0;
    //5 teste
    for (int k = 0; k < 5; k++)
    {
        set_table_free(hashTable, n);
        for (int i = 0; i < size1; i++)
            insert_key(hashTable, n, a[i]);

        ///cautare in intervale cat mai uniforme

        //caut la inceputul tabelei
        zone_cautare_dreapta_mediu(hashTable, n, 500, 0);// 500 de elemente de la poz 0 la poz 500 in tabela
        //caut la finalul tabelei
        zone_cautare_stanga_mediu(hashTable, n, 500, size1); // 500 de elemente de la poz 10 000 la poz 9500
        //caut in mijlocul tabelei
        zone_cautare_stanga_mediu(hashTable, n, 250, size1 / 2);// 250 de elemente de la poz 5000 la poz 4500
        zone_cautare_dreapta_mediu(hashTable, n, 250, size1 / 2);//250 de elemente de la poz 5000 la poz 5500

    }
    efort = efort / 5;
}

void zone_cautare_dreapta_maxim(int hashTable[], int n, int r, int i)
{
    while (r > 0)
    {
        if (hashTable[i] != -1)
        {
            search_key(hashTable, n, hashTable[i]);
            //determin nr maxim de accesari posibile
            if (maxim < efort)
                maxim = efort;
            efort = 0;
        }
        i++;
        r--;
    }
}

void zone_cautare_stanga_maxim(int hashTable[], int n, int r, int i)
{
    while (r > 0)
    {
        if (hashTable[i] != -1)
        {
            search_key(hashTable, n, hashTable[i]);
            if (maxim < efort)
                maxim = efort;
            efort = 0;
        }
        i--;
        r--;
    }

}
/*

se iau cate 500 de elemente de la inceput, mijloc si finalul tabelei
si se afla nr MAXIM de accesari are tabelei

*/
void efort_max(int hashTable[], int n, int size1, int a[])
{
    efort = 0;
    for (int k = 0; k < 5; k++)
    {
        maxim = 0;
        set_table_free(hashTable, n);
        for (int i = 0; i < size1; i++)
            insert_key(hashTable, n, a[i]);

        ///cautare in intervale cat mai uniforme - se calculeaza efort

        //caut la inceputul tabelei
        zone_cautare_dreapta_maxim(hashTable, n, 500, 0);// 500 de elemente de la poz 0 la poz 500 in tabela

        //caut la finalul tabelei
        zone_cautare_stanga_maxim(hashTable, n, 500, size1); // 500 de elemente de la poz 10 000 la poz 9500

        //caut in mijlocul tabelei
        zone_cautare_stanga_maxim(hashTable, n, 250, size1 / 2);// 250 de elemente de la poz 5000 la poz 4500
        zone_cautare_dreapta_maxim(hashTable, n, 250, size1 / 2);//250 de elemente de la poz 5000 la poz 5500

    }
    efort = efort / 5;
}
/*

 calculez efort pentru factor de umplere 80%, 85%, 90%, 95%, 99%
 efort maxim negasite= maximul eforturilor efectuate pe tabela ce contine chei ce nu pot fi gasite
 efort mediu negasite = (efort maxim negasite)/1500 elemente
 efort maxim gasite = maximul eforturilor efectuate pe tabela ce contine chei ce pot fi gasite
 efort mediu gasite =(efort maxim gasite)/1500 elemente


*/
void calculeaza_pt_factor(double factor, int hashTable[], int n, int a[], int nuInTabela[])
{
    //efort maxim negasite == nr de accesari pt tabela cu elemente ce nu pot fi gasite
    //efort mediu  negasite == efort maxim / 1500 de elemente din tabela cu elemente ce nu pot fi gasite
    int size1 = factor * n; // aprox 10 000

    efort_mediu(hashTable, n, size1, a);//efort total al cheilor ce pot fi gasite
    double efort_gasite = efort / 1500.0; //efort mediu gasite

    efort = 0;
    //efort med negasite
    for (int i = 0; i < 1500; i++)
    {
        search_key(hashTable, n, nuInTabela[i]);//efort total al cheilor ce nu pot fi gasite
        if (maxim < efort)
            maxim = efort;//efort maxim al cheilor negasite
        efort = 0;
    }

    int m_negasite = maxim;//efort maxim negasite
    for (int i = 0; i < 1500; i++)
        search_key(hashTable, n, nuInTabela[i]);

    double efort_negasite = efort / 1500.0;//efort med negasite

    efort_max(hashTable, n, size1, a);

    int m = maxim;//efort maxim gasite
    printf("%f ----- %f ----- %d ----- %d \n", efort_gasite, efort_negasite, m, m_negasite);

}
void generare_tabel(int hashTable[], int n, int a[], int nuInTabela[])
{

    printf("Factor  med gasite  med negasite  max gasite  max negasite\n");

    printf("80 ----- ");
    calculeaza_pt_factor(0.8, hashTable, n, a, nuInTabela);

    printf("85 ----- ");
    calculeaza_pt_factor(0.85, hashTable, n, a, nuInTabela);

    printf("90 ----- ");
    calculeaza_pt_factor(0.9, hashTable, n, a, nuInTabela);

    printf("95 ----- ");
    calculeaza_pt_factor(0.95, hashTable, n, a, nuInTabela);

    printf("99 ----- ");
    calculeaza_pt_factor(0.99, hashTable, n, a, nuInTabela);
}
int main()
{
    int n = 9973;
    int hashTable[9973]; // dimensiune aprox 10000
    int a[9973];
    int nuInTabela[1500];

    FillRandomArray(a, n, 0, 100000);//elemente ce pot fi gasite cu valori 0-100000
    FillRandomArray(nuInTabela, 1500, 100001, 1000000);//elemente ce nu pot fi gasite cu valori > 100000

    generare_tabel(hashTable, n, a, nuInTabela);

    return 0;
}
