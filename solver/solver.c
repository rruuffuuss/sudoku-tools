#include <stdio.h>

int CELLSIZE;
const int width = 9;
const int area = 81;

void IntToCharMap(int n, int *c){
    *c = 1 << (n);
}


int CharMapToInt(int c){
    int r = 1;
    while(c >>= 1){
        r++;
    }
    return r;
}


int *solve(int* inputBoard){



    //int cellbytes = ((size / 8) + 1);

    //int area = width * width;

    int board[area];

    /*
    Convert the board from an array of integers to an array of maps of potential numbers
    in the inputBoard 0 represents an empty cell and 1 through 9 represent starting numbers
    in the working Board, a positive bit at an index represents potential that the index could be the answer for the cell
    */
    for(int i = 0; i < area; i++){
        if(inputBoard[i] == 0) board[i] = 1022;
        else IntToCharMap(inputBoard[i], board + i * sizeof(int));
    }



}

void readSudokuFromFile(char *fileName, int* intBoard){
    FILE *f = fopen(*fileName, "r");

    char stringBoardRow[2 * width - 1];
    int boardPosition = 0;

    for(int i = 0; i < width; i++){
        printf("test\n");
        fgets(stringBoardRow, 2 * width - 1, f);
        for(int y = 0; y < 2 * width - 1; y++){
            if(stringBoardRow[y] > 47 && stringBoardRow[y] < 58){
                intBoard[boardPosition++] = stringBoardRow[y] - 48; //subtract 48 from ascii code for int value & increment board pointer
            }
        }

    }

    fclose(f);
}

void printIntBoard(int* board){
    for(int i = 0; i < width; i++){
        for(int y = 0; y < width; y++){
            printf("%d", board[i * width + y] + ",");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    printf("Hello World\n"); 

    int intBoard[area];
    readSudokuFromFile("./../examples/test1.csv", intBoard);
    printIntBoard(intBoard);

    //store each cell as a bytes where numbers possible are 1 and numbers impossible are 0
    //OR cells together to find impossible numbers in a set
    //


    //oroight. Probably just use a 2 byte data type to begin with to prevent array fuckery whilst learning

    /*int c;

    
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
    

    IntToCharMap(10, &c);
    printf("%d\n",c);
    */
    return 0;
}

