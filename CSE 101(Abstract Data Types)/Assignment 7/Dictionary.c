/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa7
 * Dictionary.c
 * Functions comprising the C Red Black Dictionary ADT
 * ***********************************************************************/
#include "Dictionary.h"
typedef struct nodeobject
    {
        KEY_TYPE key;
        VAL_TYPE value;
        char color;
        struct nodeobject* parent;
        struct nodeobject* left;
        struct nodeobject* right;
    }nodeobject;
    typedef nodeobject* node;
typedef struct DictionaryObj
{
    int size,unique;
    node root;
    node cursor;
    node lowest;//lowest and highest exist to make it easier to find when next/prev have reached their ends
    node highest;
}DictionaryObj;

void RB_DeleteFixUp(Dictionary D, node x);
void RB_InsertFixUp(Dictionary D, node z);
void RB_Transplant(Dictionary D,node u, node v);
void InOrderTreeWalk(FILE* out,node x);
void PreOrderTreeWalk(FILE* out,node x);
void PostOrderTreeWalk(FILE* out,node x);
KEY_TYPE KeyOfNode(node n);

// Constructors-Destructors ---------------------------------------------------

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique)
{
    Dictionary D;
    D=malloc(sizeof(DictionaryObj));
    D->unique=unique;
    D->size=0;
    D->root=NULL;
    D->cursor=NULL;
    D->lowest=NULL;
    D->highest=NULL;
    return D;
}

// freeDictionary()
// Frees heap memory associated with *pD, sets *pD to NULL.
void freeDictionary(Dictionary* pD)
{
    if(pD!=NULL && *pD!=NULL)
    {
        makeEmpty(*pD);
        free(*pD);
        *pD=NULL;
    }
}

node newNode(KEY_TYPE key,VAL_TYPE value)
{
    node N;
    N=malloc(sizeof(nodeobject));
    N->color='r';//currently initializes to red, but could change
    N->parent=NULL;
    N->left=NULL;
    N->right=NULL;
    N->key=key;
    N->value=value;
    return N;
}
void freeNode(node* pN)
{
    if(pN!=NULL && *pN!=NULL)
    {
        free(*pN);
        *pN=NULL;
    }
}
// Access functions -----------------------------------------------------------

int CheckIfBlack(node n)//returns 1 if node is black, 0 if red
{
    if(n==NULL)
    {
        return 1;
    }
    else if(n->color=='b')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void SetBlack(node n)
{
    if(n!=NULL)
    {
        n->color='b';
    }
}


// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D)
{
    return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D)
{
    if(D->unique==0)
    {
        return 0;
    }
    return 1;
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k)
{
    if(D->size==0)
    {
        return VAL_UNDEF;
    }
    node fakecursor=D->root;
    while(1)
    {
        if(KEY_CMP(k,fakecursor->key)<0 && fakecursor->left!=NULL)//key is to the left and there is a node to the left
        {
            //printf("Moved to the left of %s\n",fakecursor->key);
            fakecursor=fakecursor->left;
        }
        else if(KEY_CMP(k,fakecursor->key)>0 && fakecursor->right!=NULL)//key is to the right and there is a node to the right
        {
            //printf("Moved to the right of %s\n",fakecursor->key);
            fakecursor=fakecursor->right;
        }
        else if(KEY_CMP(k,fakecursor->key)==0)//found the key
        {
            //printf("Found the key of %s\n",fakecursor->key);
            return fakecursor->value;
        }
        else//did not find the key/key does not exist
        {
            //printf("Did not find the key of %s THE LAST CHECKED NODE WAS %s\n",k,fakecursor->key);
            return VAL_UNDEF;
        }
    }
}


// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v)
{
    if(D->unique!=0)
    {
        if(lookup(D,k)!=VAL_UNDEF)
        {
            printf("Dictionary error: calling insert() with an identical node already in place\n");
            exit(EXIT_FAILURE);
        }
    }
    node fakecursor=D->root;
    node insertednode=newNode(k,v);
    int inserted=0;
    D->size++;
    if(fakecursor==NULL)
    {
        insertednode->parent=fakecursor;
        inserted=1;
        D->lowest=insertednode;
        D->highest=insertednode;
        D->root=insertednode;
    }
    while(inserted==0)
    {
        if(KEY_CMP(k,fakecursor->key)<0)//if inserted key is less than or equal to current cursor node key
        {
            if(fakecursor->left==NULL)
            {
                insertednode->parent=fakecursor;
                fakecursor->left=insertednode;
                inserted=1;
                if(KEY_CMP(insertednode->key,D->lowest->key)<=0)
                {
                    D->lowest=insertednode;
                }
                break;
            }
            else
            {
                fakecursor=fakecursor->left;
                continue;//continues the while loop if the inserted node's key is even less than the current fake cursor's left key
            }
        }
        else if(KEY_CMP(k,fakecursor->key)>=0)//if inserted key is greater than current cursor node key
        {
            if(fakecursor->right==NULL)
            {
                insertednode->parent=fakecursor;
                fakecursor->right=insertednode;
                inserted=1;
                if(KEY_CMP(insertednode->key,D->highest->key)>=0)
                {
                    D->highest=insertednode;
                }
                break;
            }
            else
            {
                fakecursor=fakecursor->right;
                continue;//continues the while loop if the inserted node's key is even less than the current fake cursor's right key
            }
        }
    }
    RB_InsertFixUp(D,insertednode);
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k)
{
    if(lookup(D,k)==VAL_UNDEF)
    {
        printf("Dictionary error: calling delete() on key %s when it does not exist\n",k);
        exit(EXIT_FAILURE);
    }
    node fakecursor=D->root;
    node x;//used to pass the node position onto RB_DeleteFixUp
    //printf("GOING TO FIND REQUIRED KEY\n");
    while(k!=fakecursor->key)
    {
        if(KEY_CMP(k,fakecursor->key)<0)//key is to the left of the current cursor key
        {
            fakecursor=fakecursor->left;
        }
        else if(KEY_CMP(k,fakecursor->key)>0)//key is to the right of the current cursor key
        {
            fakecursor=fakecursor->right;
        }
        else//the key at the cursor is the requested key
        {
            break;
        }
    }//has placed the cursor on the requested node to be deleted after the while loop
    char originalcolor=fakecursor->color;
    //printf("GOING TO READJUST PARENT CHILDREN\n");
    if(fakecursor->parent!=NULL)
    {
        if(fakecursor->parent->left!=NULL)
        {
            if(fakecursor==fakecursor->parent->left)//if current fakenode is on the left of it's parent
            {
                if(fakecursor->right!=NULL)
                    fakecursor->parent->left=fakecursor->right;
                else if(fakecursor->left!=NULL)
                    fakecursor->parent->left=fakecursor->left;
                else
                    fakecursor->parent->left=NULL;
            }
        }
        if(fakecursor->parent->right!=NULL)
        {
            if(fakecursor==fakecursor->parent->right)//if current fakenode is on the right of it's parent
            {
                if(fakecursor->right!=NULL)
                    fakecursor->parent->right=fakecursor->right;
                else if(fakecursor->left!=NULL)
                    fakecursor->parent->right=fakecursor->left;
                else
                    fakecursor->parent->right=NULL;
            }
        }
    }
    //printf("GOING TO READJUST HIGHEST IF NECESSARY\n");
    if(fakecursor==D->highest)//changing what the highest is since it will be deleted
    {
        if(fakecursor->left!=NULL)
        {
            D->highest=fakecursor->left;
        }
        else if(fakecursor->parent!=NULL)
        {
            D->highest=fakecursor->parent;
        }
    }
    //printf("GOING TO READJUST LOWEST IF NECESSARY\n");
    if(fakecursor==D->lowest)//changing the lowest as it will be deleted
    {
        if(fakecursor->right!=NULL)
        {
            D->lowest=fakecursor->right;
        }
        else if(fakecursor->parent!=NULL)
        {
            D->lowest=fakecursor->parent;
        }
    }
    //printf("GOING TO READJUST ROOT IF NECESSARY\n");
    if(fakecursor==D->root)//readjusting root if necessary
    {
        if(fakecursor->right!=NULL)
            D->root=fakecursor->right;
        else if(fakecursor->left!=NULL)
            D->root=fakecursor->left;
        else
            D->root=NULL;
    }
    node temp=fakecursor;//stores original position of the fakecursor
    D->size--;
    if(D->cursor!=NULL)
    {
        if(fakecursor==D->cursor)
        {
            D->cursor=NULL;
        }
    }
    //printf("BEGAN REAL LOGIC OF DELETE\n");

    if(fakecursor->left!=NULL)
    {
        if(fakecursor->right!=NULL)//Both nodes exist
        {
            fakecursor->right->parent=fakecursor->parent;
            fakecursor=fakecursor->right;//moves cursor to right
            while(fakecursor->left!=NULL)//finds the furthest left node that is to the right of the to be deleted node
            {
                fakecursor=fakecursor->left;
            }
            temp->left->parent=fakecursor;
            fakecursor->left=temp->left;
            x=fakecursor->right;//RB TREE
            originalcolor=fakecursor->color;
            fakecursor->color=temp->color;
            fakecursor=temp;
        }
        else//Left node exists
        {
            fakecursor->left->parent=fakecursor->parent;
            x=fakecursor->left;//RB TREE
        }
    }
    else if(fakecursor->right!=NULL)//Right node exists
    {
        fakecursor->right->parent=fakecursor->parent;
        x=fakecursor->right;
    }
    else//Neither node exists
    {
        x=fakecursor->right;
    }
    if(originalcolor=='b')//RB TREE
    {
        RB_DeleteFixUp(D,x);
    }
    freeNode(&fakecursor);
}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D)
{
    while(D->size!=0)
    {
        delete(D,D->root->key);
    }
    D->root=NULL;
    D->cursor=NULL;
    D->lowest=NULL;
    D->highest=NULL;
}

// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D)
{
    if(D->size==0)
    {
        return VAL_UNDEF;
    }
    D->cursor=D->lowest;
    return D->cursor->value;
}

// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D)
{
    if(D->size==0)
    {
        return VAL_UNDEF;
    }
    D->cursor=D->highest;
    return D->cursor->value;
}

// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D)
{
    if(D->cursor==NULL)//no iteration has started
    {
        return KEY_UNDEF;
    }
    return D->cursor->key;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D)
{
    if(D->cursor==NULL)//no iteration has started
    {
        return VAL_UNDEF;
    }
    return D->cursor->value;
}

// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D)
{
    if(D->cursor==NULL || D->cursor==D->highest)//no iteration has started or has reached the end
    {
        D->cursor=NULL;
        return VAL_UNDEF;
    }
    KEY_TYPE keytobeat=D->cursor->key;
    if(D->cursor->right!=NULL)
    {
        D->cursor=D->cursor->right;
        while(D->cursor->left!=NULL)
        {
            D->cursor=D->cursor->left;
        }
        return D->cursor->value;
    }
    else
    {
        D->cursor=D->cursor->parent;
        while(KEY_CMP(keytobeat,D->cursor->key)>=0)//goes until it hits a new branch it hasn't been to
        {
            D->cursor=D->cursor->parent;
        }
        return D->cursor->value;
    }
}


// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D)
{
    if(D->cursor==NULL || D->cursor==D->lowest)//no iteration has started or has reached the end
    {
        D->cursor=NULL;
        return VAL_UNDEF;
    }
    KEY_TYPE keytobeat=D->cursor->key;
    if(D->cursor->left!=NULL)
    {
        D->cursor=D->cursor->left;
        while(D->cursor->right!=NULL)
        {
            D->cursor=D->cursor->right;
        }
        return D->cursor->value;
    }
    else
    {
        D->cursor=D->cursor->parent;
        while(KEY_CMP(keytobeat,D->cursor->key)<0)//goes until it hits a new branch it hasn't been to
        {
            D->cursor=D->cursor->parent;
        }
        return D->cursor->value;
    }
}


// Other operations -----------------------------------------------------------


// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space.  The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D,const char* ord)
{
   if(strcmp(ord,"pre")==0)
   {
       PreOrderTreeWalk(out,D->root);
   }
   if(strcmp(ord,"in")==0)
   {
       InOrderTreeWalk(out,D->root);
   }
   if(strcmp(ord,"post")==0)
   {
        PostOrderTreeWalk(out,D->root);
   }
}

//RBT HELPER FUNCTIONS
//Copied from Tantalo pseudocode
void LeftRotate(Dictionary D,node X)
{
    node Y;
    Y=X->right;

    //turn y's left subtree into x's right subtree
    X->right=Y->left;
    if(Y->left!=NULL)
    {
        Y->left->parent=X;
    }

    //link y's parent to x
    Y->parent=X->parent;
    if(X->parent==NULL)
    {
        D->root=Y;
    }
    else if(X==X->parent->left)
    {
        X->parent->left=Y;
    }
    else
    {
        X->parent->right=Y;
    }

    //put x on y's left
    Y->left=X;
    X->parent=Y;
}

void RotateRight(Dictionary D,node X)
{
    node Y;
    //set Y
    Y=X->left;
    
    //turn y's right subtree into x's left subtree
    X->left=Y->right;
    if(Y->right!=NULL)
    {
        Y->right->parent=X;
    }

    //link y's parent to x
    Y->parent=X->parent;
    if(X->parent==NULL)
    {
        D->root=Y;
    }
    else if(X==X->parent->right)
    {
        X->parent->right=Y;
    }
    else
    {
        X->parent->left=Y;
    }

    //put x on y's right
    Y->right=X;
    X->parent=Y;
}

void RB_InsertFixUp(Dictionary D, node z)
{
    node y;
    while(CheckIfBlack(z->parent)==0)
    {
        if(z->parent==z->parent->parent->left)
        {
            y=z->parent->parent->right;
            if(CheckIfBlack(y)==0)//CASE 1
            {
                z->parent->color='b';
                y->color='b';
                z->parent->parent->color='r';
                z=z->parent->parent;
            }
            else//CASE 2/3
            {
                if(z==z->parent->right)
                {
                    z=z->parent;
                    LeftRotate(D,z);
                }
                z->parent->color='b';
                z->parent->parent->color='r';
                RotateRight(D,z->parent->parent);
            }
        }
        else
        {
            y=z->parent->parent->left;
            if(CheckIfBlack(y)==0)//CASE 4
            {
                z->parent->color='b';
                y->color='b';
                z->parent->parent->color='r';
                z=z->parent->parent;
            }
            else//CASE 5/6
            {
                if(z==z->parent->left)
                {
                    z=z->parent;
                    RotateRight(D,z);
                }
                z->parent->color='b';
                z->parent->parent->color='r';
                LeftRotate(D,z->parent->parent);
            }
        }
    }
    D->root->color='b';//may be in the wrong spot
}

void RB_Transplant(Dictionary D,node u, node v)
{
    if(u->parent==NULL)
    {
        D->root=v;
    }
    else if(u==u->parent->left)
    {
        u->parent->left=v;
    }
    else
    {
        u->parent->right=v;
    }
    v->parent=u->parent;
}

void RB_DeleteFixUp(Dictionary D, node x)
{
    node w;
    while(x!=D->root && CheckIfBlack(x)==1)//while x != T.root and x.color == BLACK
    {
        if(x!=NULL)
        {
        if(x==x->parent->left)//if x == x.parent.left
        {
            w=x->parent->right;//w = x.parent.right
            if(CheckIfBlack(w)==0)//CASE 1 //if w.color == RED
            {
                SetBlack(w);//w.color = BLACK
                x->parent->color='r';//x.parent.color = RED
                LeftRotate(D,x->parent);//LeftRotate(T, x.parent)
                w=x->parent->right;//w = x.parent.right
            }
            if(CheckIfBlack(w->left)==1 && CheckIfBlack(w->right)==1)//CASE 2//if w.left.color == BLACK and w.right.color == BLACK
            {
                w->color='r';//w.color = RED
                x=x->parent;//x = x.parent
            }
            else//CASE 3/4
            {
                if(CheckIfBlack(w->right)==1)//if w.right.color == BLACK
                {
                    SetBlack(w->left);//w.left.color = BLACK
                    w->color='r';//w.color = RED
                    RotateRight(D,w);//RightRotate(T, w)
                    w=x->parent->right;//w = x.parent.right
                }
                w->color=x->parent->color;//w.color = x.parent.color
                SetBlack(x->parent);//x.parent.color = BLACK
                SetBlack(w->right);//w.right.color = BLACK
                LeftRotate(D,x->parent);//LeftRotate(T, x.parent)
                x=D->root;//x = T.root
            }
        }
        else
        {
            w=x->parent->left;//w = x.parent.left
            if(CheckIfBlack(w)==0)//CASE 5//if w.color == RED
            {
                SetBlack(w);//w.color = BLACK
                x->parent->color='r';//x.parent.color = RED
                RotateRight(D,x->parent);//RightRotate(T, x.parent)
                w=x->parent->left;//w = x.parent.left
            }
            if(CheckIfBlack(w->right)==1 && CheckIfBlack(w->left)==1)//CASE 6//if w.right.color == BLACK and w.left.color == BLACK
            {
                w->color='r';//w.color = RED
                x=x->parent;//x = x.parent
            }
            else//CASE 7/8
            {
                if(CheckIfBlack(w->left)==1)//if w.left.color == BLACK
                {
                    SetBlack(w->right);//w.right.color = BLACK
                    w->color='r';//w.color = RED
                    LeftRotate(D,w);//LeftRotate(T, w)
                    w=x->parent->left;//w = x.parent.left
                }
                w->color=x->parent->color;//w.color = x.parent.color
                x->parent->color='b';//x.parent.color = BLACK
                SetBlack(w->left);//w.left.color = BLACK
                RotateRight(D,x->parent);//RightRotate(T, x.parent)
                x=D->root;//x = T.root
            }
        }
        }
        else
        {
            break;
        }
    }
    SetBlack(x);//x.color = BLACK
}

void InOrderTreeWalk(FILE* out,node x)
{
    if(x!=NULL)
    {
        InOrderTreeWalk(out,x->left);
        fprintf(out,"%s\n",x->key);
        InOrderTreeWalk(out,x->right);
    }
}

void PreOrderTreeWalk(FILE* out,node x)
{
    if(x!=NULL)
    {
        fprintf(out,"%s\n",x->key);
        PreOrderTreeWalk(out,x->left);
        PreOrderTreeWalk(out,x->right);
    }
}

void PostOrderTreeWalk(FILE* out,node x)
{
    if(x!=NULL)
    {
        PostOrderTreeWalk(out,x->left);
        PostOrderTreeWalk(out,x->right);
        fprintf(out,"%s\n",x->key);
    }
}

KEY_TYPE KeyOfNode(node n)
{
    if(n!=NULL)
    {
        return n->key;
    }
    return "FAILURENOKEYSHOULDMATCHTHISsojustforsafekeepingthiswillbenearimpossibletomatchaccidentally1239427123142743";
}