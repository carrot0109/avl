#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>      // input/print
#include <stdlib.h>     // alloc

// Node structure
typedef struct Node {
    int key;        // key
    int height;     // depth
    int size;
    struct Node* left;      // left Node
    struct Node* right;     // right Node
    int bf;
}Node;

typedef struct CheckB {
    int rotateType;
    Node* a;
    Node* f;
} CheckB;

Node* getNodeBST(int key);
Node* getNodeAVL();
int insertBST(Node** T, int key);
int height(Node* node);
int size(Node* node);
Node* minNode(Node* n);
Node* maxNode(Node* n);
Node* searchBST(Node* T, int key);
Node* searchParent(Node* T, int key);
Node* eraseBST(Node** T, int key);
void inorder(Node* T);
void clear(Node* T);
int insertAVL(Node** T, int newKey);
CheckB checkBalance(Node** T, int newkey);
void rotateTree(Node** T, int rotationType, Node* p, Node* q);
int eraseAVL(Node** T, int deleteKey);


int main() {
    char ins;
    int key = 0;
    Node* T = NULL;

    while (scanf(" %c %d", &ins, &key) == 2) {
        if (ins == 'i') {
            if (insertAVL(&T, key))     // insert success
                inorder(T);
        }
        else if (ins == 'd') {
            if (eraseAVL(&T, key)) {   // delete success
                inorder(T);
            }
        }
        else {
            break;
        }
        printf("\n");
    }

    clear(T);        // free

    return 0;
}

Node* getNodeBST(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));    // alloc new Node
    newNode->height = 1;        // depth initialization
    newNode->key = key;     // key initialization
    newNode->bf = 0;
    newNode->left = newNode->right = NULL;      // both of Node init
    return newNode;     // return new Node
}

Node* getNodeAVL() {
    Node* newNode = (Node*)malloc(sizeof(Node));    // alloc new Node
    newNode->left = newNode->right = NULL;      // both of Node init
    newNode->bf = 0;
    return newNode;     // return new Node
}

int insertBST(Node** T, int key) {
    Node* q = NULL;     // parent Node
    Node* p = *T;       // child Node

    while (p != NULL) {     // stop if parent Node is NULL
        if (key == p->key) {
            printf("i %d: The key already exists", key);
            return 1;
        }
        q = p;      // new parent if key doesn't exist
        if (key < p->key) p = p->left;      // if key is less than parent --> move left
        else p = p->right;      // equal
    }

    Node* newNode = getNodeBST(key);        // newNode create   
    if (*T == NULL) *T = newNode;       // insert newNode into child
    else if (key < q->key) q->left = newNode;
    else q->right = newNode;

    return 0;
}

int height(Node* node) {
    if (node == NULL) return 0;     // no node anymore
    int LH = height(node->left);        // height of left nodes
    int RH = height(node->right);       // height of right nodes

    return ((LH > RH) ? (LH + 1) : (RH + 1));       // return height
}

int size(Node* node) {
    if (node == NULL) return 0;
    return (1 + size(node->left) + size(node->right));      // sum of right, left nodes
}

Node* minNode(Node* n) {        // find minimum key
    while (n->left != NULL) {
        n = n->left;
    }

    return n;
}

Node* maxNode(Node* n) {        // equal above
    while (n->right != NULL) {
        n = n->right;
    }

    return n;
}

Node* searchBST(Node* T, int key) {     // find node that has the key
    if (T == NULL) return NULL;
    if (key == T->key) return T;
    if (key < T->key) return searchBST(T->left, key);
    if (key > T->key) return searchBST(T->right, key);
    else return NULL;
}

Node* searchParent(Node* T, int key) {
    Node* p = T;       // child Node
    Node* q = NULL;     // parent Node 

    while ((p != NULL) && (p->key != key)) {
        q = p;
        if (key < p->key) p = p->left;
        else p = p->right;
    }

    return q;
}

Node* eraseBST(Node** T, int key) {
    Node* p = searchBST(*T, key);        // child
    Node* q = searchParent(*T, key);     // parent

    if (p == NULL) {
        printf("d %d: The key does not exist", key);
        return p;
    }

    if (p->left == NULL && p->right == NULL) {      // leaf node
        if (q == NULL) *T = NULL;
        else if (q->left == p) q->left = NULL;
        else q->right = NULL;
        free(p);
    }
    else if ((p->left == NULL || p->right == NULL)) {      // one child
        Node* c = NULL;
        if (p->left != NULL) {
            c = p->left;
        }
        else if (p->right != NULL) {
            c = p->right;
        }
        if (q == NULL) *T = c;      // move of child of p
        else if (q->left == p) q->left = c;     // if (left == p) ==> move
        else if (q->right == p) q->right = c;      // right ==> move
        free(p);
    }
    else if (p->left != NULL && p->right != NULL) {     // two children
        Node* r = NULL;
        int flag = 0;       // var to decide to remove left or right node

        if (height(p->left) > height(p->right)) {
            r = maxNode(p->left);
            flag = 0;
        }
        else if (height(p->left) < height(p->right)) {
            r = minNode(p->right);
            flag = 1;
        }
        else if (height(p->left) == height(p->right)) {
            if (size(p->left) >= size(p->right)) {
                r = maxNode(p->left); flag = 0;
            }
            else {
                r = maxNode(p->right); flag = 1;
            }
        }

        if (p != NULL) p->key = r->key;        // key copy
        if (flag == 0) eraseBST(&(p->left), r->key);
        else if (flag == 1) eraseBST(&(p->right), r->key);
    }

    return *T;
}

void inorder(Node* T) {
    if (T != NULL) {
        printf("<");
        inorder(T->left);
        printf(" %d ", T->key);
        inorder(T->right);
        printf(">");
    }
}

void clear(Node* T) {
    if (T == NULL) {
        return;
    }
    clear(T->left);
    clear(T->right);
    free(T);
    T = NULL;
}

int insertAVL(Node** T, int newKey) {
    Node* y;

    if (*T == NULL) {
        y = getNodeAVL();
        y->key = newKey;
        *T = y;
        return 1;
    }

    int found = insertBST(T, newKey);
    if (found) return 0;

    CheckB res = checkBalance(T, newKey);
    int rotationType = res.rotateType;
    Node* p = res.a;
    Node* q = res.f;

    if (rotationType != 0) {
        rotateTree(T, rotationType, p, q);
    }

    return 1;
}

CheckB checkBalance(Node** T, int newkey) {
    Node* f = NULL;
    Node* a = *T;
    Node* b = NULL;

    Node* p = *T;
    Node* q = NULL;
    int d;

    while (p != NULL) {
        if (p->bf != 0) {
            a = p;
            f = q;
        }

        q = p;
        if (newkey < p->key) {
            p = p->left;
        }
        else if (newkey > p->key) {
            p = p->right;
        }
        else {
            p = NULL;
            break;
        }
    }

    if (newkey < a->key) {
        p = a->left;
        b = p;
        d = 1;
    }
    else {
        p = a->right;
        b = p;
        d = -1;
    }

    while (p != q) {
        if (newkey < p->key) {
            p->bf = 1;
            p = p->left;
        }
        else {
            p->bf = -1;
            p = p->right;
        }
    }

    int unbalanced = 1;
    if (a->bf + d != 2 && a->bf + d != -2) {
        a->bf = a->bf + d;
        unbalanced = 0;
    }

    int rotateType = 0;
    if (unbalanced == 1) {
        if (d == 1) {
            if (b->bf == 1) rotateType = 11;
            else rotateType = 12;
        }
        else if (d == -1) {
            if (b->bf == -1) rotateType = 22;
            else rotateType = 21;
        }
    }

    CheckB res = { rotateType, a, f };

    return res;
}

void rotateTree(Node** T, int rotationType, Node* p, Node* q) {
    Node* a = p;
    Node* b = NULL;
    Node* c = NULL;

    if (rotationType == 11) {
        b = a->left;
        a->left = b->right;
        b->right = a;
        a->bf = 0;
        b->bf = 0;
    } 
    else if (rotationType == 12) {
        b = a->left;
        c = b->right;
        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;

        switch (c->bf) {
            case 0:
                b->bf = 0;
                a->bf = 0;
                break;
            case 1:
                a->bf = -1;
                b->bf = 0;
                break;
            case -1:
                a->bf = 0;
                b->bf = 1;
                break;
        }

        c->bf = 0;
        b = c;
    } 
    else if (rotationType == 22) {
        b = a->right;
        a->right = b->left;
        b->left = a;
        a->bf = 0;
        b->bf = 0;
    } 
    else if (rotationType == 21) {
        b = a->right;
        c = b->left;
        b->left = c->right;
        a->right = c->left;
        c->left = a;
        c->right = b;

        switch (c->bf) {
            case 0:
                a->bf = 0;
                b->bf = 0;
                break;
            case 1:
                a->bf = 0;
                b->bf = -1;
                break;
            case -1:
                a->bf = 1;
                b->bf = 0;
                break;
        }

        c->bf = 0;
        b = c;
    }

    if (q == NULL) {
        *T = b;
    } else if (a == q->left) {
        q->left = b;
    } else if (a == q->right) {
        q->right = b;
    }
}


int eraseAVL(Node** T, int deleteKey) {
    Node* p = *T;
    Node* q = NULL;
    Node* x = NULL;
    Node* f = NULL;
    Node* stack[100]; int t = -1;

    while (p != NULL && deleteKey != p->key) {
        q = p;
        stack[++t] = q;
        if (deleteKey < p->key) p = p->left;
        else p = p->right;
    }

    if (p == NULL) {
        printf("d %d: The key does not exist", deleteKey);
        return 0;
    }

    if (p->left != NULL && p->right != NULL) {
        stack[++t] = p;
        Node* tmpNode = p;

        if (height(p->left) < height(p->right) || (height(p->left) == height(p->right) && size(p->left) < size(p->right))) {
            p = p->right;
            while (p->left != NULL) {
                stack[++t] = p;
                p = p->left;
            }
        }
        else {
            p = p->left;
            while (p->right != NULL) {
                stack[++t] = p;
                p = p->right;
            }
        }

        tmpNode->key = p->key;
        q = stack[t];
    }

    if (p->left == NULL && p->right == NULL) {
        if (q == NULL) *T = NULL;
        else if (q->left == p) q->left = NULL;
        else q->right = NULL;
        free(p);
    }
    else {
        if (p->left != NULL) {
            if (q == NULL) *T = (*T)->left;
            else if (q->left == p) q->left = p->left;
            else q->right = p->left;
        }
        else {
            if (q == NULL) *T = (*T)->right;
            else if (q->left == p) q->left = p->right;
            else q->right = p->right;
        }
        free(p);
    }

    if (t >= 0) {
        q = stack[t--];
        q->height = 1 + ((height(q->left) > height(q->right)) ? height(q->left) : height(q->right));
        q->bf = height(q->left) - height(q->right);

        if (q->bf < -1 || 1 < q->bf) {
            x = q;
            f = stack[t];

            if (1 < x->bf) {
                if (0 < x->left->bf) rotateTree(T, 11, x, (t >= 0) ? f : NULL);
                else rotateTree(T, 12, x, (t >= 0) ? f : NULL);
            }
            else if (x->bf < -1) {
                if (x->right->bf < 0) rotateTree(T, 22, x, (t >= 0) ? f : NULL);
                else rotateTree(T, 21, x, (t >= 0) ? f : NULL);
            }
        }
    }


    return 1;
}
