#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*

    Tema 7 - Varga Alexandra-Ioana, 30223


*/
using namespace std;

typedef struct Node
{
    Node* left;
    Node* right;
    int key;
    //size1 == numarul de noduri din subarborele cu radacina in acel nod
    int size1;
};

Node* createNode(int key, int size1)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->left = NULL;
    node->right = NULL;
    node->size1 = size1;
    node->key = key;
    return node;
}

//divide-et-impera - sirul este deja sortat crescator + determinare size-ul nodului

Node* BUILD_TREE(int* a, int left, int right)
{
    if (left > right)
        return NULL;
    else
    {
        int mid = left + (right - left) / 2;
        Node* root = createNode(a[mid], right - left + 1);
        root->left = BUILD_TREE(a, left, mid - 1);
        root->right = BUILD_TREE(a, mid + 1, right);
        return root;
    }
}
/*

pretty print arbore 

*/
void reprezentare(Node* root, int x)
{
    if (root == NULL)
        return;

    x += 10;
    reprezentare(root->right, x);
    printf("\n");

    for (int i = 10; i < x; i++)
        printf(" ");
    printf("%d s: %d\n", root->key, root->size1);

    reprezentare(root->left, x);
}

Node* OS_SELECT(Node* root, int i, int n)
{
    int r = 1;
    if (root->left != NULL)
        r = root->left->size1 + 1;
    if (i == r)
        return root;
    else if (i < r)
        return OS_SELECT(root->left, i, n);
    else
        return OS_SELECT(root->right, i - r, n);
}

Node* findMin(Node* root)
{
    if (root == NULL)
        return NULL;
    else
    {
        if (root->left != NULL)
            return findMin(root->left);
        else
            return root;
    }
}

Node* OS_DELETE(Node* root, int key)
{
    if (root == NULL)
        return root;

    Node* p;

    if (key < root->key)
    {
        root->left = OS_DELETE(root->left, key);
        root->size1--;
    }
    else if (key > root->key)
    {
        root->right = OS_DELETE(root->right, key);
        root->size1--;
    }
    else
    {
        if (root->left == NULL)
        {
            p = root->right;
            free(root);
            return p;
        }
        else if (root->right == NULL)
        {
            p = root->left;
            free(root);
            return p;
        }

        p = findMin(root->right);

        root->key = p->key;
        root->right = OS_DELETE(root->right, p->key);
        root->size1--;
    }
    return root;
}

void demo()
{
    /*
        demo BUID_TREE
    */
    ///construiesc arbore binar de cautare perfect echilibrat
    Node* root = NULL;
    int a[11] = { 1,2,3,4,5,6,7,8,9,10,11 };
    root = BUILD_TREE(a, 0, 10);
    reprezentare(root, 0);

    /*
        demo OS_SELECT
    */
    //for (int i = 1; i <= 11; i++)
        //printf("Al %d-lea cel mai mic element este %d\n", i, OS_SELECT(root, i, 10)->key);


    /*
        demo OS_DELETE
    */
    int n = 11;
    srand(time(0));
    for (int i = 0; i < 3; i++) 
    {
        int rank = rand() % n + 1; // indice aleator din intervalul [1, n]

        Node* select = OS_SELECT(root, rank, 10);//nodul cu cea mai mica cheie
        //indice, root, nr de noduri din subarborele respectiv
        //printf("key = %d, size = %d\n", select->key, select->size1); 
        printf("Al %d-lea cel mai mic element este %d\n", rank, OS_SELECT(root, rank, 10)->key);
        //OS_DELETE(root, rank, 11);
        OS_DELETE(root, select->key);
        reprezentare(root,0);
        n--;
    }

    ///sterg nood cheie 2 + afisez arbore dupa stergere
    //printf("Arbore dupa stergere nod cheie 2\n");
    //root = OS_DELETE(root, 2);
    //reprezentare(root, 0);

    ///sterg nod cheie 6 + afisez arbore dupa stergere
    //printf("Arbore dupa stergere nod cheie 6\n");
    //root = OS_DELETE(root, 6);
    //reprezentare(root, 0);

}
int main()
{
    demo();

    return 0;
}
