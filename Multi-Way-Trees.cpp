/*

	Tema 7 - Varga Alexandra-Ioana, grupa 30223 

*/
#include <stdio.h>
#include <stdlib.h>

//vector de parinti
int n = 9;
int parent[9] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };


//strctura folosita pentru arbore multi-cai
typedef struct node 
{
	node** child;// vector de copii - ai nodului
	int childNr; // numar de copii - ai nodului
	int key;// cheia - din nod
}
Node;

// structura pt transformare in reprezentare arbore multi-cai -> binara
typedef struct nodeB 
{
	nodeB* brother; // frate -- dreapta
	nodeB* child; // copil - stanga
	int key; // cheia - din nod
}
NodeB;

//gasesc radacina in vectorul de parinti
int find_root(int* parent, int n)
{
	for (int i = 0; i < n; i++)
		if (parent[i] == -1)
			return i;
}


//pretty print cu vector de parinti
void reprezentare_r1(int* parent, int n, int root, int x)//x=0
{
	for (int i = 1; i <= x; i++)
		printf("     ");

	printf("%d\n", root + 1);
	x++;

	for (int i = 0; i < n; i++)
	{
		if (parent[i] == root + 1)
			reprezentare_r1(parent, n, i, x);
	}
}

/// parent -> multi-way tree transofomation (T1)

Node* t1(int* parent, int n, int key) 
{
	int x = 0;
	
	//aloc memorie pt creare nod reprezentare T1
	Node *root = (Node*)malloc(n*sizeof(Node*));
	root->child = (Node**)malloc(n*sizeof(Node*));
	root->key = key;
	
	//O(n^2) ???
	for (int i = 0; i < n; i++) 
	{
		if (parent[i] == key) 
		{
			root->child[x] = t1(parent, n, i+1);
			x++;
		}
	}

	root->childNr = x;
	return root;

}

//pretty print pentru arbore multi-cai (t1)
void reprezentare_r2(Node* root, int x)//x=0
{	
	if (root) 
	{
		for (int i = 0; i < x; i++) 
			printf("     ");
		
		printf("%d\n", root->key);
		x++;

		for (int i = 0; i < root->childNr; i++) 
			reprezentare_r2(root->child[i], x);
	}
}

/// multi-way tree -> binary tree transormation (T2)

NodeB* t2 (Node* tree) 
{
		//alocare memorie nod pentru transformare t2
		int key = tree->key;		
		NodeB* root = (nodeB*)malloc(sizeof(nodeB*));
		root->brother = (nodeB*)malloc(sizeof(nodeB*));
		root->brother = NULL;
		root->child = (nodeB*)malloc(sizeof(nodeB*));
		root->child = NULL;
		root->key = key;
		NodeB* prevNode = NULL;
		// O(n^2) ??
		for (int i = 0; i < tree->childNr; i++) 
		{
			if (i!=0) 
			{
				prevNode->brother = t2(tree->child[i]);
				prevNode = prevNode->brother;
			}
			else 
			{
				root->child = t2(tree->child[i]);
				prevNode = root->child;
			}
		}

		return root;

}
//pretty print pentru arbore binar
void reprezentare_r3(NodeB* root, int x) //x=0
{
	
	if (root) 
	{
		for (int i = 1; i <= x; i++) 
			printf("     ");
		
		printf("%d\n", root->key);

		reprezentare_r3(root->child, x + 1);
		reprezentare_r3(root->brother, x);
	}
}

void demo()
{
	//radacina 
	int root = find_root(parent, n);
	//printf("%d\n", root);
	printf("Pretty print parent representation\n");
	//pretty print vectori de parinti
	reprezentare_r1(parent, n, root, 0);

	printf("Pretty print multi-way tree represantation\n");
	//creeare arbore multi-cai
	Node* tree = t1(parent, n, root + 1);
	//pretty print arbore multi-cai
	reprezentare_r2(tree, 0);

	printf("Pretty print binary representation\n");
	//creare arbore binar frate stanga - copii dreapta
	nodeB* treeB = t2(tree);
	//pretty print arbore binar
	reprezentare_r3(treeB, 0);

}
int main()
{
	demo();

	return 0;
}