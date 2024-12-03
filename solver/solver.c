#include <stdio.h>

int CELLSIZE;

void IntToCharMap(int n, char *c){
    c[n/8] = 1 << (n - 1);
}


int CharMapToInt(char c){
    int r = 1;
    while(c >>= 1){
        r++;
    }
    return r;
}


int *solve(int* inputBoard, int size){

    CELLSIZE = size;

    int cellbytes = ((size / 8) + 1);

    char board [size * size * cellbytes];

}

int main(int argc, char **argv) {

    printf("Hello World\n"); 

    //store each cell as a bytes where numbers possible are 1 and numbers impossible are 0
    //OR cells together to find impossible numbers in a set
    //


    //oroight. Probably just use a 2 byte data type to begin with to prevent array fuckery whilst learning

    char c[2];

    /*
    IntToCharMap(1, &c);
    printf("%d\n",c);
    printf("%d\n",CharMapToInt(c));
    IntToCharMap(2, &c);
    printf("%d\n",c);
    printf("%d\n",CharMapToInt(c));
    IntToCharMap(3, &c);
    printf("%d\n",c);
    printf("%d\n",CharMapToInt(c));
    IntToCharMap(4, &c);
    printf("%d\n",c);
    printf("%d\n",CharMapToInt(c));
    */

   IntToCharMap(10, &c);
   printf("%d\n",c);

    return 0;
}

