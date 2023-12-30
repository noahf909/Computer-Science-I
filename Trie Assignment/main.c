#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

typedef struct trie {
    int freq;
    struct trie* children[26];
    int sum_freq;
    int cur_max_freq;
} trie;

struct trie* init();
void insert(struct trie* tree, char word[], int k, int numOfWord);
void check_input(struct trie* tree, char word[], int k);
void query(struct trie *tree, char prefix[], int k); 
void free_memory(struct trie *tree);

int main(void) 
{
    atexit(report_mem_leak);
    struct trie* dictionary = init(); //root

    //scan num of words/prefixes
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) 
    {
        //scan commands 
        int q;
        scanf("%d", &q);

        if (q == 1) 
        {
            //scan word, process into dictionary
            char word[100];
            scanf("%s", word);
            int numOfWord;
            scanf("%d", &numOfWord);
            insert(dictionary, word, 0, numOfWord);

            //check that input was read correctly (root/child freq, sum freq, max freq)
            //check_input(dictionary, word, 0);
        }

        if (q == 2) 
        {
          //scan prefix, process into query (print out next likely letter if in dictionary, unkowwn otherwise)
          char prefix[100];
          scanf("%s", prefix);
          query(dictionary, prefix, 0);
        }
    }
  free_memory(dictionary); 
}

struct trie* init() {
    struct trie* tree = malloc(sizeof(struct trie));
    tree->freq = 0;
    tree->sum_freq = 0;
    tree->cur_max_freq = 0;
    for (int i = 0; i < 26; i++) {
        tree->children[i] = NULL;
    }
    return tree;
}

void insert(struct trie* tree, char word[], int k, int numOfWord) 
{
    if (k == strlen(word))
    {
        tree->freq = numOfWord;
        tree->cur_max_freq = 0;
        tree->sum_freq = numOfWord;
        return;
    }

    int nextIndex = word[k] - 'a';

    if (tree->children[nextIndex] == NULL) 
    {
      tree->children[nextIndex] = init();
    }

    insert(tree->children[nextIndex], word, k + 1, numOfWord);

    tree->sum_freq += numOfWord;

    if (tree->children[nextIndex]->sum_freq > tree->cur_max_freq) 
    {
      tree->cur_max_freq = tree->children[nextIndex]->sum_freq;
    }
}

/*
void check_input(struct trie* tree, char word[], int k) 
{
    if (tree == NULL) 
    {
      return;
    }

    int nextIndex = word[k] - 'a';

    printf("node: %d | freq: %d | cur: %d | sum: %d\n", k, tree->freq, tree->cur_max_freq, tree->sum_freq);

    if (tree->children[nextIndex] != NULL) 
    {
      check_input(tree->children[nextIndex], word, k + 1);
    }
}
*/

void query(struct trie *tree, char prefix[], int k)
{
   if (prefix[k] == '\0') //same as strlen but for whatever reason it wouldn't work 
  {
      int found = 0;
      //iterate through child nodes
      for (int i = 0; i < 26; i++) 
      {
        //if child node exists and if sum (total) frequency of child node(s) is the same as the highest (cur max) freq of all child nodes, following letter is valid
          if (tree->children[i] != NULL && tree->children[i]->sum_freq == tree->cur_max_freq) 
          {
              //print next letter(s)
              printf("%c", 'a' + i);
              found = 1;
          }
      }

      //otherwise print unknown word
      if (found == 0)
      {
          printf("unknown word");
      }

      printf("\n");
      return;
  }

  int nextIndex = prefix[k] - 'a';

  //also if child is node, print unknown word
  if (tree->children[nextIndex] == NULL) 
  {
      printf("unknown word\n");
      return;
  }
  //iterate through nodes of prefix
  query(tree->children[nextIndex], prefix, k + 1);
}

void free_memory(struct trie *tree)
{
  if (tree == NULL)
  {
    return;
  }
  //iterate through child nodes
  for (int i = 0; i < 26; i++)
  {
    free_memory(tree->children[i]);
  }
  //free child nodes, parent node at the end
  free(tree);
}