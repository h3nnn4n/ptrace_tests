// compile: cc filename.c -lbfd -liberty
// // Type `info bfd' on your GNU/Linux system to
// // read more about the BFD library.
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    int i, j;

    i = 0;
    j = 0;

    while( 1 ) {
        j--;

        if ( i < 100 ) {
            i++;
        }

        if ( j < -50 ){
            j = 50;
        }
    }

    return EXIT_SUCCESS;
}

