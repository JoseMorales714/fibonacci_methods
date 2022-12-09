#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FIB_VAL 100

int c_n_fib(int n, int arr[]);
int i_n_fib(int n, int size);
int r_n_fib(int n);
int l_n_fib(int n, int size, int arr[]);

typedef struct {
  int pid;
  int fib_n;
  unsigned long long fib_val;
} LRUEntry;

LRUEntry array[];

int call_index = 0, found_result = 0;
float chr;

int main(int argc, char *argv[]) {

  // the below block is for ./main -h
  // helper command

  char *first = argv[1], help[] = "-h";

  if (strcmp(first, help) == 0) {
    printf("Sytnax: ./main <n> <max value for fib> <cache size> <r or i or c "
           "or l>\n");
    printf("Example: ./main 15 10 15 l\n");
    printf("Example: ./main 15 100 100 c\n");
    return -1;
  }

  int num_of_tests = atoi(argv[1]), max_fib = atoi(argv[2]),
      CACHE_SIZE = atoi(argv[3]); // size of cache, smaller caches has more
                                  // replacements in output

  char *buff = argv[4]; // this to cmp r i c or l for method

  // for iterative method

  if (!strcmp(buff, "i")) {

    for (int ix = 0; ix < num_of_tests; ix++) {
      int fib_to_get = rand() % max_fib; // from 0 to max fib
      printf("fib %d = %d\n", ix, i_n_fib(fib_to_get, CACHE_SIZE));
    }

    // for recursive method

  } else if (!strcmp(buff, "r")) {

    for (int ix = 0; ix < num_of_tests; ix++) {
      int fib_to_get = rand() % max_fib;
      printf("fibb %d = %d\n", ix, r_n_fib(fib_to_get));
    }

    // for cache method

  } else if (!strcmp(buff, "c")) {

    for (int ix = 0; ix < num_of_tests; ix++) {
      int fib_to_get = rand() % max_fib;
      int result = c_n_fib(fib_to_get, array);
      printf("fib %d = %d\n", ix, result);
    }

    // for LRU cache method

  } else if (!strcmp(buff, "l")) {

    for (int ix = 0; ix < num_of_tests; ix++) {
      int fib_to_get = rand() % max_fib;
      printf("fib %d = %d\n", ix, l_n_fib(fib_to_get, CACHE_SIZE, array));
    }

    chr = (float)found_result / call_index * 100;

    printf("CHR: %.2f%%\n", chr);
    printf("found: %d\n", found_result);
    printf("call: %d\n", call_index);

  } else {
    printf("invalid choice\n");
    printf("run './main -h' to see how to run program\n");
  }
  return 0;
}

// the below function is a recursive way to calculate the nth fib number

int r_n_fib(int n) {

  if (n <= 1) {
    return n;
  }
  return r_n_fib(n - 1) + r_n_fib(n - 2);
}

// the below function is an interative way to calculate the nth fib number

int i_n_fib(int n, int size) {

  n++;
  int first = 0, second = 1, result = 0;

  for (int i = 0; i < n; i++) {
    if (i <= 1) {
      result = i;
    } else {
      result = first + second;
      first = second;
      second = result;
    }
  }

  return result;
}

// the below function is a cache way to calcualte the nth fib number for
// optimziation

int c_n_fib(int n, int ca[]) {

  if (n <= 1) {
    return n;
  }
  if (ca[n] != 0) {
    return ca[n];
  }

  ca[n] = c_n_fib(n - 1, ca) + c_n_fib(n - 2, ca);

  return ca[n];
}

// the below function is a LRU cache way to calculate the nth fib number for
// it is supposed to be faster since the values are stored in the cache
// and it checks to see if that value has already been stored or not

int l_n_fib(int n, int size, int arr[]) {

  call_index++;

  if (n <= 1) { // base cases
    return n;
  }

  for (int i = 0; i < size; i++) {
    if (array[i].fib_n == n) { // if value is in cache then rtn it
      call_index++;
      found_result++;
      array[i].pid = call_index;
      printf("Fib = %d already in cache: %d\n", n, array[i].fib_val);
      return array[i].fib_val;
    }
  }
  // need to calculate since was not found in cache
  int result = l_n_fib(n - 1, size, arr) + l_n_fib(n - 2, size, arr);
  // printf("Fib = %d, result: %d\n", n, result);

  int lowest_pid = 0;

  for (int i = 0; i < size; i++) {
    if (array[i].fib_n == 0) { // if open space in cache then store
      call_index++;
      array[i].fib_n = n;
      array[i].fib_val = result;
      array[i].pid = call_index;
      printf("Added to open space: %d, Result is: %d\n", array[i].fib_n,
             result);
      return result;
    } else {
      for (int j = 1; j < size; j++) {
        if (array[i].pid <
            array[lowest_pid].pid) { // finds lowest last called index
          call_index++;
          array[lowest_pid].pid = array[i].pid;
          printf("Alert: Replacing with %d\n", array[lowest_pid].pid);
        }
      }
    }
  }

  return result;
}