#include <stdio.h>
#include <stdlib.h>

#define ALLOC_CHECK(p) if (!(p)) puts("Neuspesna alokacija"), exit(1)

int is_prime(int n)
{
    int i, prime = 1;
    if (n == 1) {
        return 0;
    }
    for (i = 2; i < n; ++i) {
        if (n % i == 0) {
            prime = 0;
            break;
        }
    }
    return prime;
}

void modify(int **mat, int n)
{
    int i;
    for (i = 1; i < n; ++i) {
        int j = n-i, *p = &mat[i][j];
        if (is_prime(*p)) {
            *p = mat[i-1][j-1];
        }
    }
}

int main(int argc, char *argv[])
{
    int n, **mat, i, j;

    if (argc != 2) {
        return 1;
    }
    n = atoi(argv[1]);

    mat = malloc(n * sizeof(*mat));
    ALLOC_CHECK(mat);

    for (i = 0; i < n; ++i) {
        mat[i] = malloc(n * sizeof(**mat));
        ALLOC_CHECK(mat[i]);
        
        printf("Uneti elemente %d. vrste:\n", i);
        for (j = 0; j < n; ++j) {
            scanf("%d", mat[i]+j);
        }
    }

    modify(mat, n);

    puts("Matrica nakon obrade:");
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            printf("%d\t", mat[i][j]);
        }
        putchar('\n');
        
        free(mat[i]);
    }

    free(mat);
    return 0;
}
