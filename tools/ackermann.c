// synacor challenge (@waffle87)
#include <stdio.h>
#include <stdlib.h>

int ackermann(int m, int n, int k, int **memo) {
  if (memo[m][n] != -1)
    return memo[m][n];
  int res;
  if (!m)
    res = (n + 1) % 32768;
  else if (!n)
    res = ackermann(m - 1, k, k, memo);
  else
    res = ackermann(m - 1, ackermann(m, n - 1, k, memo), k, memo);
  memo[m][n] = res;
  return memo[m][n];
}

int main() {
  int **memo = malloc(sizeof(int *) * 5);
  for (int i = 0; i < 5; i++)
    memo[i] = malloc(sizeof(int) * 32768);
  for (int i = 1; i < 32768; i++) {
    for (int j = 0; j < 5; j++)
      for (int k = 0; k < 32768; k++)
        memo[j][k] = -1;
    int res = ackermann(4, 1, i, memo);
    printf("checking %d -> %d\n", i, res);
    if (res == 6) {
      printf("\nregister value: %d\n", i);
      break;
    }
  }
  for (int i = 0; i < 5; i++)
    free(memo[i]);
  free(memo);
  return 0;
}
