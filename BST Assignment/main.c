#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <math.h>
#include "leak_detector_c.h"

#define MAXLEN 30
//insert structures 
typedef struct itemNode
{
  char name[MAXLEN];
  char OGRoot[MAXLEN];
  int count;
  struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode
{
  char treeName[MAXLEN];
  struct treeNameNode *left, *right;
  itemNode *theTree;
} treeNameNode; 

treeNameNode *createTreeNameNode(); 
treeNameNode *buildNameTree(treeNameNode *root, treeNameNode *node);
treeNameNode *searchNameNode(treeNameNode *root, char *treeName);
itemNode *searchItemNode(itemNode *root, char *ItemName); 
itemNode *createItemNode(treeNameNode *root); 
itemNode *connectItemNode(itemNode *root, itemNode *node);
void *treeMatch(itemNode *node, treeNameNode *root);
void InOrder(treeNameNode *root); //check if tree was printed out correctly
void traverse_in_traverse(treeNameNode *root); 
void traverse_item_tree(itemNode *root);
void find(treeNameNode *root, char *tree, char *item);
treeNameNode *findTree(treeNameNode *root, char *tree);
itemNode *findItem(itemNode *root, char *item, char *tree);
void count_before(treeNameNode *root, char *tree, char *item); 
int countB(itemNode *root, char *item, int total);
int CountTree(treeNameNode *root, char *tree, int total);
void check_balance(treeNameNode *root, char *tree);
int find_left_height(itemNode *root, int count, int check);
int find_right_height(itemNode *root, int count, int check);
void count(treeNameNode *root, char *tree);
int count_sub(itemNode *root, int count);
void Remove(treeNameNode *root, char *tree, char *item);
itemNode *isparent(itemNode *root, char *item);
itemNode *delete_parent(itemNode *root);
int isItemLeaf(itemNode *root);
itemNode *ItemMinVal(itemNode *root);
itemNode *ItemMaxVal(itemNode *root);
int OnlyLeftChild(itemNode *root);
int OnlyRightChild(itemNode *root);
void delete_name(treeNameNode *root, char *tree); 
itemNode *getToParent(itemNode *root, char *item); 
void freeItemTree(itemNode *root);
void dec(treeNameNode *root, char *tree, char *item, int num); 
itemNode *decrease_count(itemNode *root, int num);
void release_memory(treeNameNode *root); 
void freeItemNodeTwo(itemNode *root); 
treeNameNode *getToParentTree(treeNameNode *root, char *tree);



int main(void) 
{
  atexit(report_mem_leak);
  struct treeNameNode *TreeNode = NULL;
  struct treeNameNode *root = NULL;
  struct itemNode *ItemNode = NULL;
  int N, I, Q; 
  scanf("%d %d %d", &N, &I, &Q);
  //check that input was read printf("%d %d %d", N, Q, I);

  //create treeName tree 
  for (int i = 0; i < N; i++)
  {
      TreeNode = createTreeNameNode(); 
      root = buildNameTree(root, TreeNode); 
      
  }
  //create item tree
  for (int j = 0; j < I; j++)
  {
     ItemNode = createItemNode(root); 
     treeMatch(ItemNode, root);
  }
  InOrder(root);
  printf("\n");
  traverse_in_traverse(root);

  for (int p = 0; p < Q; p++)
  {
    char Quer[MAXLEN];
    char tree[MAXLEN];
    char item[MAXLEN];
    int num;
    scanf("%s", Quer);
    if (strcmp(Quer, "find") == 0)
    {
      scanf("%s %s", tree, item);
      find(root, tree, item); 
    }
    
    else if (strcmp(Quer, "check_balance") == 0)
    {
      scanf("%s", tree);
      check_balance(root, tree); 
    }
    
    else if (strcmp(Quer, "count_before") == 0)
    {
      scanf("%s %s", tree, item);
      count_before(root, tree, item);
    }

    else if (strcmp(Quer, "count") == 0)
    {
      scanf("%s", tree);
      count(root, tree);
    }
    
    else if (strcmp(Quer, "decrease") == 0)
    {
      scanf("%s %s %d", tree, item, &num);
      dec(root, tree, item, num); 
    }

    else if (strcmp(Quer, "remove") == 0)
    {
      scanf("%s %s", tree, item);
      Remove(root, tree, item);
      //traverse_in_traverse(root);
    }

    else if (strcmp(Quer, "delete_name") == 0)
    {
      scanf("%s", tree);
      delete_name(root, tree);
      //traverse_in_traverse(root);
    }
  }
  //traverse_in_traverse(root);
  release_memory(root); 
  //check OG is good: traverse_in_traverse(root);
}

treeNameNode *createTreeNameNode()
{
  char name[MAXLEN];   
  scanf("%s", name); 
  struct treeNameNode *node = malloc(sizeof(struct treeNameNode)); 
  strcpy(node->treeName, name); 
  node->left = NULL;
  node->right = NULL;
  node->theTree = NULL;
  //check that it registered: printf("%s\n", node->treeName); 
  return node;
}

treeNameNode *buildNameTree(treeNameNode *root, treeNameNode *node)
{
  if (root == NULL)
  {
    return node; 
  }
    
  else
  {
      if (strcmp(node->treeName, root->treeName) < 0)
      {
        if (root->left != NULL)
        {
          root->left = buildNameTree(root->left, node); 
        }
        else
        {
          root->left = node;
        }
      }
      else
      {
        if (root->right != NULL)
        {
          root->right = buildNameTree(root->right, node);
        }
        
        else
        {
        root->right = node;
        }
      }
  }
  return root;
}

void InOrder(treeNameNode *root)
{
  if (root != NULL)
  {
    InOrder(root->left);
    printf("%s ", root->treeName); 
    InOrder(root->right);
  }
}

itemNode *createItemNode(treeNameNode *root)
{
  struct itemNode *ItemNode = NULL;
  char tree[MAXLEN];  
  char type[MAXLEN]; 
  int value;
  scanf("%s %s %d", tree, type, &value);
  
  ItemNode = malloc(sizeof(struct itemNode));
  strcpy(ItemNode->name, type);
  ItemNode->count = value;
  strcpy(ItemNode->OGRoot, tree); 
  ItemNode->left = NULL;
  ItemNode->right = NULL;
  //check that items are good: printf("%s %s\n", ItemNode->OGRoot, ItemNode->name); 
  return ItemNode;
}


void *treeMatch(itemNode *node, treeNameNode *root)
{ 
  root = searchNameNode(root, node->OGRoot);
  //printf("%s\n", root->treeName);
  root->theTree = connectItemNode(root->theTree, node);
}

treeNameNode *searchNameNode(treeNameNode *root, char *treeName)
{
  treeNameNode *mroot;
  
  if (root == NULL) 
  {
    return NULL;
  }
  
  else
  {
    if (strcmp(treeName, root->treeName) == 0) 
    {
      return root;
    }
    else if (strcmp(root->treeName, treeName) > 0) 
    {
      mroot = searchNameNode(root->left, treeName);
    }
    else if (strcmp(root->treeName, treeName) < 0)
    {
      mroot = searchNameNode(root->right, treeName);
    }
  }
  return mroot;
  
}

itemNode *searchItemNode(itemNode *root, char *ItemName)
{
  itemNode *mroot;
  
  if (root == NULL) 
  {
    return NULL;
  }
  
  else
  {
    if (strcmp(root->name, ItemName) == 0) 
    {
      return root;
    }
    else if (strcmp(root->name, ItemName) > 0) 
    {
      mroot = searchItemNode(root->left, ItemName);
    }
    else 
    {
      mroot = searchItemNode(root->right, ItemName);
    }
  }
  return mroot;
}

itemNode *connectItemNode(itemNode *root, itemNode *node)
{
  if (root == NULL)
  {
    return node; 
  }
    
  else
  {
      if (strcmp(node->name, root->name) < 0)
      {
        if (root->left != NULL)
        {
          root->left = connectItemNode(root->left, node); 
        }
        else
        {
          root->left = node;
        }
      }
      else
      {
        if (root->right != NULL)
        {
          root->right = connectItemNode(root->right, node);
        }
      
        else
        {
        root->right = node;
        }
      }
  }
  return root;
}

void find(treeNameNode *root, char *tree, char *item)
{
  struct treeNameNode *rootcheck;
  struct itemNode *store;
  int check;
  rootcheck = findTree(root, tree);
  if (rootcheck == NULL)
  {
    return;
  }
  store = findItem(rootcheck->theTree, item, tree);
  if (store == NULL)
  {
    return;
  }
}

treeNameNode *findTree(treeNameNode *root, char *tree)
{
  treeNameNode *broot;
  if (root != NULL)
  {
    if (strcmp(root->treeName, tree) == 0)
    {
      return root;
    }
    else if (strcmp(root->treeName, tree) > 0)
    {
      broot = findTree(root->left, tree);  
    }
    else if (strcmp(root->treeName, tree) < 0)
    {
      broot = findTree(root->right, tree);
    }
  }
  else
  {
    printf("%s does not exist\n", tree);
    return NULL;
  }
  return broot;
}

itemNode *findItem(itemNode *root, char *item, char *tree)
{
  itemNode *broot;
  if (root != NULL)
  {
    
    if (strcmp(root->name, item) == 0 && strcmp(root->OGRoot, tree) == 0)
    {
      printf("%d %s found in %s\n", root->count, root->name, root->OGRoot);
    }
    else if (strcmp(root->name, item) > 0)
    {
      broot = findItem(root->left, item, tree); 
    }
    else if (strcmp(root->name, item) < 0)
    {
      broot = findItem(root->right, item, tree);
    }
  }
  
  else
  {
    printf("%s not found in %s\n", item, tree);
    return NULL;
  }
  return broot;
}

void count_before(treeNameNode *root, char *tree, char *item)
{
  int total = 0;
  treeNameNode *croot;
  int countRoot;
  int countItem;
  countRoot = CountTree(root, tree, total);
  croot = findTree(root, tree);
  countItem = countB(croot->theTree, item, total);
  int count = countRoot + countItem;
  printf("item before %s: %d\n", item, count);
}

int CountTree(treeNameNode *root, char *tree, int total)
{
  
  int mroot;
  if (root != NULL)
  {
    if (strcmp(root->treeName, tree) == 0)
    {
      total++;
      return total;
    }
    else if (strcmp(root->treeName, tree) > 0)
    {
      total++;
      mroot = CountTree(root->left, tree, total);  
    }
    else if (strcmp(root->treeName, tree) < 0)
    {
      total++;
      mroot = CountTree(root->right, tree, total);
    }
  }
  else
  {
    printf("tree does not exist.\n");
  }
  return mroot;
}


int countB(itemNode *root, char *item, int total)
{
  int mcount;
  if (root != NULL)
  {
    if (strcmp(root->name, item) == 0)
    {
      return total;
    }
    else if (strcmp(root->name, item) > 0)
    {
      total++;
      mcount = countB(root->left, item, total); 
    }
    else if (strcmp(root->name, item) < 0)
    {
      total++;
      mcount = countB(root->right, item, total);
    }
  }
  return mcount;
}

void check_balance(treeNameNode *root, char *tree)
{
  //height: nodes in between leaf and start root
  //check that input's good: printf("%s\n", tree);
  treeNameNode *Lroot;
  treeNameNode *Rroot;
  int LNode;
  int RNode;
  int count;
  int check = 0;
  int balance;
  Lroot = searchNameNode(root, tree);
  LNode = find_left_height(Lroot->theTree, count, check);
  Rroot = searchNameNode(root, tree);
  RNode = find_right_height(Rroot->theTree, count, check);
  balance = abs(RNode - LNode); 
  if (balance > 1)
  {
    printf("%s: left height %d, right height %d, difference %d, not balanced\n", tree, LNode, RNode, balance);
  }
  
  else
  {
    printf("%s: left height %d, right height %d, difference %d, balanced\n", tree, LNode, RNode, balance);
  }
  
}

int find_left_height(itemNode *root, int count, int check)
{
  int num = 0;
  if (root == NULL)
  {
    return -1;
  }
  else if (root != NULL)
  {
    if (root->left == NULL && root->right == NULL)
    {
      num--;
      return num;
    }
    
    if (root->left != NULL)
    {
      check = 1;
      num++;
      return find_left_height(root->left, num, check) + num;
    }
    
    else if (root->right != NULL)
    {
      
      if (check == 0)
      {
        return -1;
      }
      
      else
      {
      num++;
      return find_left_height(root->right, num, check) + num; 
      }
    }
  }
}

int find_right_height(itemNode *root, int count, int check)
{
  int num = 0;
  if (root == NULL)
  {
    return -1;
  }
  
  else if (root != NULL)
  {
    if (root->left == NULL && root->right == NULL)
    {
      num--;
      return num;
    }
    
    if (root->right != NULL)
    {
      check = 1;
      num++;
      return find_right_height(root->right, num, check) + num;
    }
    
    else if (root->left != NULL)
    {
      if (check == 0)
      {
        return -1;
      }
      
      else
      {
      num++;
      return find_right_height(root->left, num, check) + num;
      }
    }
  }
}

void count(treeNameNode *root, char *tree)
{
  int count = 0;
  struct treeNameNode *crootTree;
  int totalsub;
  crootTree = findTree(root, tree);
  totalsub = count_sub(crootTree->theTree, count); 
  printf("%s count %d\n", tree, totalsub);
}

int count_sub(itemNode *root, int count)
{
  int num = 0;
  itemNode *rightsub = root;
  if (root != NULL)
  {
    num = count_sub(root->left, num) + num;
    num+= root->count;
    num =  count_sub(root->right, num) + num;
  }
  return num;
}

void Remove(treeNameNode *root, char *tree, char *item)
{
  treeNameNode *broot;
  itemNode *P;
  itemNode *I;
  broot = findTree(root, tree); 

  I = searchItemNode(broot->theTree, item);
  
  if (isItemLeaf(I) == 1)
  {
    P = getToParent(broot->theTree, item); 
    if (P->left == I)
    {
      P->left = NULL;
      free(I);
    }
    else if (P->right == I)
    {
      P->right = NULL;
      free(I);
    }
    printf("%s deleted from %s\n", item, tree);
  }

  else if (OnlyLeftChild(I) == 1)
  {
    itemNode *temp;
    P = isparent(broot->theTree, item);
    if (P->left == I)
    {
      temp = I->left;
      P->left = temp;
      free(I);
    }
    else if (P->right == I)
    {
      temp = I->left;
      P->right = temp;
      free(I);
    }
    printf("%s deleted from %s\n", item, tree);
  }
    
  else if (OnlyRightChild(I) == 1)
  {
    itemNode *temp;
    P = isparent(broot->theTree, item);
    if (P->left == I)
    {
      temp = I->right;
      P->left = temp;
      free(I);
    }
    else if (P->right == I)
    {
      temp = I->right;
      P->right = temp;
      free(I);
    }
    printf("%s deleted from %s\n", item, tree);
  }
    
  else if (I->left != NULL && I->right != NULL)
  {
    itemNode *temp;
    itemNode *temp2;
    itemNode *temp3;
    temp3 = I;
    temp = delete_parent(I);
    temp2 = getToParent(I, temp->name);

    temp->right = broot->theTree->right;
    temp->left = broot->theTree->left;
    broot->theTree = temp;

    if (temp2->right == temp)
    {
      temp2->right = NULL;
    }
    
    if (temp2->left == temp)
    {
      temp2->left = NULL;
    }
    free(temp3);
    printf("%s deleted from %s\n", item, tree);
  }
  
}

itemNode *getToParent(itemNode *root, char *item)
{
  itemNode *broot;
  if (root != NULL)
  {
    if (strcmp(item, root->name) < 0)
    {
      broot = getToParent(root->left, item);
       if (strcmp(root->left->name, item) == 0)
      {
        return root;
      }
    }
    else if (strcmp(item, root->name) > 0)
    {
      broot = getToParent(root->right, item);
      if (strcmp(root->right->name, item) == 0)
      {
        return root;
      }
    }
  }
  return broot;
}

itemNode *isparent(itemNode *root, char *item)
{
  itemNode *broot;
  if (strcmp(root->name, item) == 0)
  {
    return NULL;
  }
  
  if (root != NULL)
  {
    if (strcmp(root->right->name,item) == 0 || strcmp(root->left->name,item) == 0)
    {
      return root;
    }
    else if (strcmp(item, root->name) < 0)
    {
      broot = isparent(root->left, item);
    }
    else if (strcmp(item, root->name) > 0)
    {
      broot = isparent(root->right, item);
    }
  }
  else
  {
    return broot;
  }
}

itemNode *delete_parent(itemNode *root)
{
  itemNode *HighLeft;
  itemNode *LowRight;
  if (root->left != NULL)
  {
    HighLeft = ItemMaxVal(root->left);
    if (isItemLeaf(HighLeft) == 1)
      return HighLeft;
    else
      if (root->right != NULL)
      {
        LowRight = ItemMinVal(root->right); 
        return LowRight;
      }
  }
}

itemNode *ItemMinVal(itemNode *root)
{
  if (root->left == NULL)
  {
    return root;
  }
  return ItemMinVal(root->left);
}
itemNode *ItemMaxVal(itemNode *root)
{
  if (root->right == NULL)
  {
    return root;
  }
  return ItemMaxVal(root->right);
}

int isItemLeaf(itemNode *root)
{
  return (root->left == NULL && root->right == NULL);
}

int OnlyLeftChild(itemNode *root)
{
  return (root->left != NULL && root->right == NULL);
}

int OnlyRightChild(itemNode *root)
{
  return (root->right != NULL && root->left == NULL);
}

void delete_name(treeNameNode *root, char *tree)
{
  treeNameNode *broot;
  treeNameNode *P;
  broot = findTree(root, tree); 
  freeItemTree(broot->theTree);
  P = getToParentTree(root, tree); 
  if (broot->right != NULL)
  {
    P->left = broot->right;
    free(broot);
  }
  printf("%s deleted\n", tree);
}

treeNameNode *getToParentTree(treeNameNode *root, char *tree)
{
  treeNameNode *croot;
  if (root != NULL)
  {
    if (strcmp(tree, root->treeName) < 0)
    {
      croot = getToParentTree(root->left, tree);
       if (strcmp(root->left->treeName, tree) == 0)
      {
        return root;
      }
    }
    else if (strcmp(tree, root->treeName) > 0)
    {
      croot = getToParentTree(root->right, tree);
      if (strcmp(root->right->treeName, tree) == 0)
      {
        return root;
      }
    }
  }
  return croot;
}

void freeItemTree(itemNode *root)
{
  if (root == NULL)
  {
    return;
  }
  freeItemTree(root->left);
  freeItemTree(root->right);
  free(root);
}

void dec(treeNameNode *root, char *tree, char *item, int num)
{
  treeNameNode *broot;
  itemNode *I; 
  broot = findTree(root, tree);
  I = searchItemNode(broot->theTree, item);
  if (I != NULL)
  {
    I = decrease_count(I, num);
    printf("%s reduced\n", item); 
  } 
   
  
}
itemNode *decrease_count(itemNode *root, int num)
{
  if ((root->count - num) <= 0)
  {
    free(root); 
    return root;
  }
  else
  {
    root->count = root->count - num;
  }
  return root;
}

void traverse_item_tree(itemNode *root)
{
  if (root != NULL)
  {
    traverse_item_tree(root->left);
    printf("%s ", root->name);
    traverse_item_tree(root->right);
  }
}

void traverse_in_traverse(treeNameNode *root) //left, mid, right
{
  if (root != NULL)
  {
    traverse_in_traverse(root->left);
    printf("###%s###\n", root->treeName);
    traverse_item_tree(root->theTree); 
    printf("\n");
    traverse_in_traverse(root->right); 
  }
}

void release_memory(treeNameNode *root)
{
  if (root != NULL)
  {    
    release_memory(root->left);
    freeItemNodeTwo(root->theTree);
    release_memory(root->right);
    free(root);
  }
}

void freeItemNodeTwo(itemNode *root)
{
  if (root != NULL)
  {
    freeItemNodeTwo(root->left);
    freeItemNodeTwo(root->right);
    free(root);
  }
}

