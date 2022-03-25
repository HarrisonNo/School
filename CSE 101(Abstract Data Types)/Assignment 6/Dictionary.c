/***********************************************************************
 * Harrison Quiring, hquiring
 * 2021 Spring CSE 101 pa6
 * Dictionary.c
 * Functions comprising the C Dictionary ADT
 * ***********************************************************************/
#include "Dictionary.h"
typedef struct nodeobject
    {
        KEY_TYPE key;
        VAL_TYPE value;
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
    //printf("GOING TO READJUST PARENT CHILDREN\n");
    if(fakecursor->parent!=NULL)
    {
        if(fakecursor->parent->left!=NULL)
        {
            if(KEY_CMP(fakecursor->key,fakecursor->parent->left->key)==0)//if current fakenode is on the left of it's parent
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
            if(KEY_CMP(fakecursor->key,fakecursor->parent->right->key)==0)//if current fakenode is on the right of it's parent
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
    node temp=fakecursor;
    D->size--;
    if(D->cursor!=NULL)
    {
        if(KEY_CMP(k,D->cursor->key)==0)
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
            freeNode(&temp);
        }
        else//Left node exists
        {
            fakecursor->left->parent=fakecursor->parent;
            freeNode(&fakecursor);
        }
    }
    else if(fakecursor->right!=NULL)//Right node exists
    {
        fakecursor->right->parent=fakecursor->parent;
        freeNode(&fakecursor);
    }
    else//Neither node exists
    {
        freeNode(&fakecursor);
    }
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
void printDictionary(FILE* out, Dictionary D)
{
    node fakecursor=D->lowest;
    KEY_TYPE keytobeat;
    for(int i=0;i<D->size;i++)
    {
        fprintf(out,"%s %i\n",fakecursor->key,fakecursor->value);
        //basically just a carbon copy of next()
        if(fakecursor==NULL || fakecursor==D->highest)//no iteration has started or has reached the end
        {
            break;
        }
        keytobeat=fakecursor->key;
        if(fakecursor->right!=NULL)
        {
            fakecursor=fakecursor->right;
            while(fakecursor->left!=NULL)
            {
                fakecursor=fakecursor->left;
            }
        }
        else
        {
            fakecursor=fakecursor->parent;
            while(KEY_CMP(keytobeat,fakecursor->key)>=0)//goes until it hits a new branch it hasn't been to
            {
                fakecursor=fakecursor->parent;
            }
        }
    }
}