#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "leak_detector_c.h"

//global variable for X and Y
int X;
int Y;

//create struct to hold coordinates 
typedef struct cord
{
  int x; 
  int y; 
  float dist; 
} cord;

struct cord *ReadData(struct cord *arr, int n); 
void insertionSort(struct cord *arr, int l, int r);
void merge(struct cord *arr, int l, int mid, int r); 
void mergeSort(struct cord *arr, int l, int r, int t); 
int compareTo(struct cord *ptrPt1, struct cord *ptrPt2); 
struct cord *ReadQuer(struct cord *quer, int s);
int binarySearch(struct cord *arr, struct cord* quer, int n); 

int main(void) 
{
  atexit(report_mem_leak);
  int n, s, X, Y, t; 
  scanf("%d %d %d %d %d", &n, &s, &X, &Y, &t); 
  
  if ((n >= 2) && (n <= 1000000) && (s >= 1) && (s <= 200000) && (t >= 1) && (t <= 30))
  {
    //check that inputs were scanned 
    //printf("%d %d %d %d %d", n, s, X, Y, t);   
    
    struct cord *arr = malloc(n * sizeof(struct cord)); 
    ReadData(arr, n); 

    //check if array is stored
    /*
    for (int i = 0; i < n; i++)
      {
        printf("%d %d %f\n", arr[i].x, arr[i].y, arr[i].dist);
      }
    */
    
    //apply merge and insertion sort
    /*
    conditions -
      size/subsect of arr <= t: insertion sort
      else: merge sort 
    */
    
    int l = 0;
    int r = n-1;  
    
    mergeSort(arr, l, r, t); 
    //check if merge worked

    
    for (int i = 0; i < n; i++)
    {
      printf("%d %d", arr[i].x, arr[i].y);
      printf("\n"); 
    }
    
    struct cord *quer = malloc(s * sizeof(struct cord));
    ReadQuer(quer, s);
    //check if queries are stored
    /*
    for (int i = 0; i < s; i++)
      {
        printf("%d %d %f\n", quer[i].x, quer[i].y, quer[i].dist);
      }
    */ 
    int R;
    for (int i = 0; i < s; i++)
      {
        R = binarySearch(arr, &quer[i], n); 
        if (R != -1)
        {
          printf("%d %d found at position %d", quer[i].x, quer[i].y, R+1); 
        }
        else
        {
        printf("%d %d not found", quer[i].x, quer[i].y);
        }
        printf("\n");
      }
    free(arr);
    free(quer); 
  }
  
  else
  {
    printf("inputs are invalid"); 
  }

  return 0; 
}

struct cord *ReadData(struct cord *arr, int n)
{
  int x, y;
  for (int i = 0; i < n; i++)
  {
    scanf("%d %d", &x, &y); 
    arr[i].x = x;
    arr[i].y = y; 
    arr[i].dist = (pow((x - X), 2) + pow((y - Y), 2)); 
  }
  return arr;
}

 void mergeSort(struct cord *arr, int l, int r, int t)
{
  //if out of bounds
  if (l >= r)
  {
    return; 
  }  
  if (((r-l)+1) <= t)
  {
   insertionSort(arr, l, r); 
    
    //check if insertion sort worked
    /*
    for (int m = l; m <= r; m++)
      {
        printf("%d,%d\t", arr[m].x, arr[m].y);
      }
    printf("\n");
    */ 
    return;
  }
    int mid = (l + r)/2; 
    mergeSort(arr, l, mid, t); 
    mergeSort(arr, mid+1, r, t);
    merge(arr, l, mid, r);
}

//fix insertion sort
void insertionSort(struct cord *arr, int l, int r)
{
  int i;
  int j;
  struct cord check; 
   
   for (i = l + 1; i <= r; i++) 
   {
    
    struct cord check = arr[i];
    
    for (j = i - 1; j >= l; j--)
    {
      if (compareTo(&arr[j], &check) == 1) 
      {
        arr[j + 1] = arr[j];
      }   
      else
      {
        break;
      }
    }
    arr[j + 1] = check;
  }
}

int binarySearch(struct cord *arr, struct cord* quer, int n)
{
  int l = 0, r = n - 1;
    int mid;
    int j;
    while (l <= r)
    {
        mid = (l + r) / 2;
        // Check if item is present at mid
        if (compareTo(&arr[mid], quer) == 0)
        {
          return mid;
        }
         // If item greater, ignore left half
        if (compareTo(&arr[mid], quer) == -1)
        {
          l = mid + 1;
        }
        // If item is smaller, ignore right half
        else
        {
          r = mid - 1;
        } 
    }
    // if we reach here, then element was
    // not present
    return -1;
}

int compareTo(struct cord *ptrPt1, struct cord *ptrPt2)
{
  if (ptrPt1->dist < ptrPt2->dist)
  {
    return -1;
  }
  else if (ptrPt1->dist > ptrPt2->dist)
  {
    return 1;
  }
  //if distances are equal
  if (ptrPt1->dist == ptrPt2->dist)
  {
    if (ptrPt1->x < ptrPt2->x)
    {
      return -1;
    }
    else if (ptrPt1->x > ptrPt2->x)
    {
      return 1;
    }
    //if x is equal
    if (ptrPt1->x == ptrPt2->x)
    {
      if (ptrPt1->y < ptrPt2->y)
      {
        return -1;
      }
      else if (ptrPt1->y > ptrPt2->y)
      {
        return 1;
      }
      //if y is equal, x is equal, distances are equal 
      if (ptrPt1->dist == ptrPt2->dist && ptrPt1->x == ptrPt2->x && ptrPt1->y == ptrPt2->y)
      {
        return 0;
      }
    }
  }
}

void merge(struct cord *arr, int l, int mid, int r)
{
  int i, j, k;
    int n1 = mid - l + 1;
    int n2 =  r - mid;

    /* create temp arrays */
    struct cord *L = malloc(n1*sizeof(struct cord));
    struct cord *R = malloc(n2*sizeof(struct cord));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
    {
      L[i] = arr[l + i]; 
    }
    for (j = 0; j < n2; j++)
    {
      R[j] = arr[mid + 1+ j];   
    }


    i = 0;
    j = 0; 
    k = l; 

  
    while (i < n1 && j < n2)
    {
        if (compareTo(&L[i], &R[j]) < 0)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
      k++;
    }
     /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

struct cord *ReadQuer(struct cord *quer, int s)
{
  int x, y;
  for (int i = 0; i < s; i++)
  {
    scanf("%d %d", &x, &y); 
    quer[i].x = x;
    quer[i].y = y; 
    quer[i].dist = (pow((x - X), 2) + pow((y - Y), 2)); 
  }
  return quer;
}
