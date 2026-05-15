#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define elementi 500000

/****REGOLE DEL RED-BLACK TREE****

-----1 OGNI NODO E ROSSO O NERO
-----2 LA RADICE (ROOT) E SEMPRE NERA
-----3 NESSUN CAMMINO DA RADICE A FOGLIA HA 2 NODI ROSSI CONSECUTIVI
-----4 OGNI CAMMINO DA RADICE A FOGLIA CONTIENE LO STESSO NUMERO DI NODI NERI (ALTEZZA NERA)
-----5 LE FOGLIE SONO NIL E SONO NERE
*/

typedef enum {RED, BLACK} color;

/*la struct del nostro nodo, 
ogni nodo punta ai figli e al genitore*/
typedef struct node {
    int value; // valore nodo
    color color;
    struct node *left;  //figlio sinistro
    struct node *right; //figlio destro
    struct node *parent; //genitore
} node;

/*in un red black tree le foglie vuote non sono NULL
ma nodi neri chiamati NIL */
node *NIL;

void initNIL() {
    NIL = malloc(sizeof(node));
    NIL -> color = BLACK;       //NIL è sempre nero
    NIL -> left = NIL -> right = NIL -> parent = NIL;
}//tutte le foglie puntano a NIL

//quando inseriamo il nodo è sempre rosso inizialmente
node *createnode(int value){
    node *n = malloc(sizeof(node));
    n -> value = value;
    n -> color = RED;
    n -> left = n-> right = n-> parent = NIL; // figli inizialmente sono NIL
    return n;
}

/*BST, scendiamo a sinistra o a destra,
il nuovo nodo punta al parent trovato,
i figli puntano a NIL,
colore iniziale RED
*/
node *root = NULL;

node *bst(node *root, node * newnode) {
    node *y = NIL;//genitore del nodo corrente
    node *x = root;//nodo corrente

    //scorre l'albero per trovare la posizione
    while(x != NIL) {
        y = x;
        if(newnode->value < x -> value)
            x = x -> left;
        else
            x = x -> right;
    }

    newnode->parent = y;

    if(y == NIL) //l'albero era vuoto
        root = newnode;
    else if (newnode -> value < y -> value)
        y -> left = newnode;
    else
        y -> right = newnode;

    newnode -> left = newnode -> right = NIL; //figli NIL
    newnode -> color = RED; //nodo appena inserito

    return root; // ritorna la nuova radice
}

/*bilanciamento red-black tree
dobbiamo garantire che la radice resti nera
che nessun nodo rosso abbia un genitore rosso
e che quindi un nodo rosso abbia solo figli neri*/
void leftrotate(node **root, node *x) {
    node *y = x -> right; //y diventa un nuovo nodo pivot
    x -> right = y -> left;
    if(y -> left != NIL)
        y -> left -> parent = x;
    
    y -> parent = x -> parent;
    if (x -> parent == NIL)
        *root = y;
    else if ( x == x -> parent -> left)
        x -> parent -> left = y;
    else
        x -> parent -> right = y;
    
    y -> left = x;
    x -> parent = y;
}

void rightrotate(node **root, node *x) {
    node *y = x -> left; //pivot anche qui
    x -> left = y -> right;
    if(y -> right != NIL)
        y -> right -> parent = x;
    
    y -> parent = x -> parent;
    if (x -> parent == NIL)
        *root = y;
    else if ( x == x -> parent -> right)
        x -> parent -> right = y;
    else
        x -> parent -> left = y;
    
    y -> right = x;
    x -> parent = y;
}

/*----------------fix up dei 3 casi-----------------------
---------caso 1 zio rosso --> ricolorazione
---------caso 2 zio nero e nodo triangolo -> rotazione e ricolorazione
---------caso 3 zio nero e nodo linea -> rotazione e ricolorazione
---------si ripete finche le regole del RBT non sono soddisfatte
*/
void fixup(node **root, node *z) {
    while(z->parent != NIL && z->parent->color == RED) {//finche il genitore e rosso
        if(z->parent == z->parent->parent->left) {//z sottalbero sinistro
            node *y = z->parent->parent->right; //zio
            if (y->color == RED) {    //caso 1: zio rosso
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {  //caso 2 e 3: zio nero
                if(z == z->parent->right) {//caso 2, zio nero e triangolo
                    z = z->parent;
                    leftrotate(root, z);
                }
                // caso 3 zio nero e linea
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightrotate(root, z->parent->parent);
            }
        } else {//speculare: z e nel sottalbero destro
            node *y = z->parent->parent->left; //zio
            if (y->color == RED) {    //caso 1: zio rosso
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {  //caso 2 e 3: zio nero
                if(z == z->parent->left) {//caso 2, zio nero e triangolo
                    z = z->parent;
                    rightrotate(root, z);
                }
                // caso 3 zio nero e linea
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftrotate(root, z->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;//la radice è sempre nera
}

//inserimento
node *insert(node *root, int value) {
    node *newnode = createnode(value);
    root = bst(root, newnode); //inseriamo come BST
    fixup(&root, newnode); // bilanciamento
    return root;
}

//funzione di stampa in-order
void inorder(node *root) {
    if(root == NIL) return; //foglia NIL -> stop
    inorder(root -> left); //visita sottalbero sinistro
    printf("%d", root -> value); //valore
    printf("%s",root -> color == RED ? "(R) " : "(B) "); // stampa colore
    inorder(root -> right); //visita il sottalbero destro
}

//stampiamo l'albero visivamente
void print_TREE(node *root, int space) {
    if(root == NIL) return;

    space = space + 5; //distanza tra livelli

    print_TREE(root -> right, space);//stampiamo il sottalbero destro

    printf("\n");
    for(int i = 5; i < space; i++)
        printf(" ");
    printf("%d", root -> value);
    printf(root -> color == RED ? "(R)" : "(B)");

    print_TREE(root -> left, space);//stampiamo il sottalbero sinistro
}

//per liberare la memoria dell'albero
void freeTree(node *root){
    if(root == NIL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void){
    initNIL(); //inizializza nodo NIL
    
    //----------------ARRAY DI PROVA PER VEDERE LA GENERAZIONE DEL RBT----------------------
    node *root_values = NIL;
    int values[] = {7, 7, 20, 30, 15, 25, 5, 12, 33, 39, 40, 1, 25, 100}; //Array demo
    int n = sizeof(values)/sizeof(values[0]);

    for(int i = 0; i < n; i++){
        root_values = insert(root_values, values[i]);
    }
    //--------------------------------------------------------------------------------------

    node *root = NIL; // albero vuoto
    
    int *array = malloc(elementi * sizeof(int));
    srand(time(NULL));
    for(int i = 0; i < elementi; i++){
        array[i] = rand();
    }

    printf("inserimento di %d elementi nel RBT,,,\n", elementi);

    clock_t start = clock();
    for(int i = 0; i < elementi; i++){
        root = insert(root, array[i]);
    }
    clock_t end = clock();
    
    double tempo_inserimento = (double)(end - start)/CLOCKS_PER_SEC;
    

    printf("albero RED-BLACK in ordine crescente: \n");  
    inorder(root_values);//............................SCONSIGLIATO USARE CON GRANDI NUMERI DI ELEMENTI
    printf("\nALBERO RED-BLACK GRAFICO:\n");
    print_TREE(root_values, 0);//......................SCONSIGLIATO USARE CON GRANDI NUMERI DI ELEMENTI
    printf("\n");
    printf("tempo impiegato per l'inserimento di %d elementi: %.4f secondi\n", elementi, tempo_inserimento);

    //liberiamo la memoria
    freeTree(root_values);
    freeTree(root);
    free(NIL);
    free(array);

    return 0;
}