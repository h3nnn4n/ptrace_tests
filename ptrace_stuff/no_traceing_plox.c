#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i = 0;

    while ( i < 10 ) {
        i+=2;
    }

    printf("%d ", i);

    if ( i == 10 ) {
        printf("Failed\n");
    } else if ( i == 11 ) {
        printf("Success\n");
    } else {
        printf("WAT?\n");
    }

    return EXIT_SUCCESS;
}
