#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_not_prime[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);
/*
  for (int i = 3; i <= n; i += 2)
    is_prime[i] = true;
   
  for (int i = 3; (unsigned long long)i * i <= n; i += 2) {
    if (is_prime[i]) {
      for (int j = i * i; j <= n; j += i) {
        is_prime[j] = false;
      }
    }
  }

  int *p = primes;
  *p++ = 2;
  for (int i = 3; i <= n; i += 2)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
*/
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
}
