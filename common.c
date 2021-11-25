// Name: Farnaz Zinnah   Date: 29th September, 2020   Class: CSC 220 Algorithms //

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Function to count the number of common elements without the 
   multiplicity
*/
int common(int A[], int B[], int m, int n);

/* mergesort: sort integer array a[length] ; standard mergesort */
/* sorts in increasing order, requires auxiliary array */
void mergesort(int *a, int *aux, int length);

void main()
{
  int P[500000];
  int Q[200000];
  int i, t1, t2, result;
  time_t t;
  srand((unsigned)time(&t));   /*initialize random numbers */
  for (i = 0; i < 500000; i++) /* fill P with even numbers */
    P[i] = 2 * ((579 * i + 234987) % 500000);
  for (i = 0; i < 200000; i++) /* fill Q with odd numbers */
    Q[i] = 2 * ((997 * i + 98771) % 200000) + 1;
  t1 = (rand() % 10) + 1;
  t2 = (rand() % 10) + 1;
  for (i = 0; i < t1; i++) /* put some odd numbers in P */
  {
    P[rand() % 500000] = 2 * (51 * i + 237) + 1;
    P[rand() % 500000] = 2 * (51 * i + 237) + 1;
    P[rand() % 500000] = 2 * (51 * i + 237) + 1;
    Q[2 * i] = 2 * (51 * i + 237) + 1;
  }
  for (i = 0; i < t2; i++) /* put some even numbers in Q */
  {
    Q[rand() % 200000] = 2 * (53 * i + 732);
    Q[rand() % 200000] = 2 * (53 * i + 732);
    Q[rand() % 200000] = 2 * (53 * i + 732);
    P[5 * i] = 2 * (53 * i + 732);
  }
  printf(" finished preparing two arrays. they have %d elements in common\n",
         t1 + t2);
  result = common(P, Q, 500000, 200000);
  printf(" found %d elements in common.\n", result);
  if (result == t1 + t2)
    printf(" Passed test.\n");
  else
    printf(" Failed test.\n");
}

/* mergesort: sort integer array a[length] ; standard mergesort */
/* sorts in increasing order, requires auxiliary array */
void mergesort(int *a, int *aux, int length)
{
    int i, j1, j2, previous;
    if (length > 1)
    {
        mergesort(a, aux, (length / 2)); /* recursive calls */
        mergesort(a + (length / 2), aux, (length - (length / 2)));

        for (i = 0, j1 = 0, j2 = (length / 2); i < length;) /* merging */
        {
            if ((j1 < (length / 2) && a[j1] < a[j2]) || j2 == length)
                aux[i++] = a[j1++];
            else {
                aux[i++] = a[j2++];
            }
        }
        
        for (i = 0; i < length; i++) /* copying back */
            a[i] = aux[i];
    }
}

/* Function to count the number of common elements without the 
   multiplicity
*/
int common(int A[], int B[], int m, int n)
{
  int * aux1 = (int *) malloc(sizeof(int) * m);
  int * aux2 = (int *) malloc(sizeof(int) * n);

  // Sort them
  mergesort(A, aux1, m);
  mergesort(B, aux2, n);

  // Count common elements without multiplicity
  int i=0, j=0, count=0; 
  while(i<m && j<n)
  {
    if(A[i] < B[j]) {
      i++;
    } else if(B[j] < A[i]) {
      j++;
    } else {
      int x = A[i];
      count++;
      while(A[i] == x) i++;
      while(B[j] == x) j++;
    }
  }

  return count;
}
