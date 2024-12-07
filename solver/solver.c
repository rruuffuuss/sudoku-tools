#include <errno.h>
#include <stdio.h>

const int rootWidth = 3;
const int width = 9;
const int area = 81;

void intToMap(int n, int *c){
    *c = 1 << (n);
}


int mapToInt(int c){
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
    int res = 0;

    /*
    Convert the board from an array of integers to an array of maps of potential numbers
    in the inputBoard 0 represents an empty cell and 1 through 9 represent starting numbers
    in the working Board, a positive bit at an index represents potential that the index could be the answer for the cell
    smallest bit set to 0 when final value
    */
    for(int i = 0; i < area; i++){
        if(inputBoard[i] == 0) board[i] = 1023;
        else intToMap(inputBoard[i], &board[i]);
    }
    
    printf("--------------------\n");

    printIntBoard(board);

    

    int runThrough = 0;
    while(runThrough < 10){ //prevent infinite loop
        for(int i = 0; i < area; i++){
            if(board[i] % 2 == 1){ 

                //loop through row
                for(int y = (i / width) * width; y < ((i / width) * width) + width; y++){
                    if(board[y] % 2 != 1) res = res | board[y];
                }

                //loop through column
                for(int y = i % width; y < area; y += width){
                    if(board[y] % 2 != 1) res = res | board[y];
                }

                /*
                //loop through region
                for(int y = (i / (width * rootWidth)) * width * rootWidth; y < i + width * rootWidth; y += width){
                    for(int o = ((i % width) / rootWidth) * rootWidth; o < rootWidth; o++){
                        if(board[y + o] % 2 != 1) res = res | board[y + o];
                        printf("%d", y + o);
                    }

                }*/
                
                board[i] = board[i] & ~res & 1023;
                if(board[i] - 1 & board[i] - 2 == 0) board[i]--;//set last bit zero when final value reached
            }
        }
        runThrough++;
        printf("--------------------\n");
        printIntBoard(board);
    }

}



int readSudokuFromFile(char *fileName, int* intBoard){
    FILE *f;
    f = fopen(fileName, "r");

    if (f == NULL) {
        printf("Can't open %s \n", fileName);
        perror("Failed to open file.\n");
      
        return 0;
   }

    char stringBoardRow[2 * width + 1];

    int boardPosition = 0;

    for(int i = 0; i < width; i++){
        //printf("test\n");
        //printf("%d", sizeof(stringBoardRow));
        fgets(stringBoardRow, sizeof(stringBoardRow), f);
        for(int y = 0; y < 2 * width - 1; y++){
            if(stringBoardRow[y] > 47 && stringBoardRow[y] < 58){
                intBoard[boardPosition++] = stringBoardRow[y] - 48; //subtract 48 from ascii code for int value & increment board pointer
            }
        }

    }

    fclose(f);
    return 1;
}

void printIntBoard(int* board){
    for(int i = 0; i < width; i++){
        for(int y = 0; y < width; y++){
            printf("%d", board[i * width + y]);
            printf(",");
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {

    printf("Hello World\n"); 

    int intBoard[area];
    if(readSudokuFromFile("../examples/test1.csv", intBoard)){
        printIntBoard(intBoard);
    }

    solve(intBoard);


    /*
    //store each cell as a bits where numbers possible are 1 and numbers impossible are 0
    //OR cells together to find impossible numbers in a set
    //


    //oroight. Probably just use a 2 byte data type to begin with to prevent array fuckery whilst learning

    int c;

    
    intToMap(1, &c);
    printf("%d\n",c);
    printf("%d\n",mapToInt(c));
    intToMap(2, &c);
    printf("%d\n",c);
    printf("%d\n",mapToInt(c));
    intToMap(3, &c);
    printf("%d\n",c);
    printf("%d\n",mapToInt(c));
    intToMap(4, &c);
    printf("%d\n",c);
    printf("%d\n",mapToInt(c));
    

    intToMap(10, &c);
    printf("%d\n",c);
    */
    
    return 0;
}

