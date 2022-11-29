#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);

  const int MAXN = (n >> 1) + 1;
  for (int i = 1; i <= MAXN; i++)
    is_prime[i] = true;
   
  for (int i = 3; i * i <= n; i += 2) {
    if (is_prime[i >> 1]) {
      for (int j = i * i; j <= n; j += (i * 2)) {
        is_prime[j >> 1] = false;
      }
    }
  }

  int *p = primes;
  *p++ = 2;
  for (int i = 3; i <= n; i += 2)
    if (is_prime[i]) {
      *p++ = i * 2 + 1;
    }
  *p = 0;
  return primes;

  
/*
  int *p = primes, *tmp = primes;
  for (int i = 2; i <= n; i++) {
    if (!is_not_prime[i]) {
      *p++ = i;
    }
    tmp = primes;
    while (tmp != p) {
      if (i * (*tmp) > n) {
        break;
      }
      is_not_prime[i * (*tmp)] = true;
      if (!(i % (*tmp))) {
        break;
      }
      tmp ++;
    }
  }
  return primes;
*/
}
