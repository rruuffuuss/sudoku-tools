#include <stdio.h>

int main(int argc, char **argv) {

    printf("Hello World\n"); 

    int root_size = 4;
    int size = root_size * root_size;
    int cellbytes = ((size / 8) + 1);

    __uint8_t board [size * size * cellbytes];

    

    //store each cell as a bytes where numbers possible are 1 and numbers impossible are 0
    //OR cells together to find impossible numbers in a set
    //

    __uint8_t x = 4;
    printf("%u\n",x);

    return 0;
}