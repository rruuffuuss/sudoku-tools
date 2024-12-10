#include <errno.h>
#include <stdio.h>

const int rootWidth = 3;
const int width = rootWidth * rootWidth;
const int area = width * width;

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

void printIntBoard(int* board){
    for(int i = 0; i < width; i++){
        for(int y = 0; y < width; y++){
            printf("%d", board[i * width + y]);
            printf(",");
        }
        printf("\n");
    }
}

int *solve(int* inputBoard){



    //int cellbytes = ((size / 8) + 1);

    //int area = width * width;

    int board[area];
    int res;
    int rem;
    int mask;

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

    //printIntBoard(board);

    

    int runThrough = 0;
    while(runThrough < 10){ //prevent infinite loop
        for(int i = 0; i < area; i++){
            if(board[i] % 2 == 1){ 

                res = 0;
                rem = 0;

                //loop through row
                for(int y = (i / width) * width; y < ((i / width) * width) + width; y++){
                    if(board[y] % 2 != 1) res = res | board[y];
                    else rem = rem | board[y];
                }

                //loop through column
                for(int y = i % width; y < area; y += width){
                    if(board[y] % 2 != 1) res = res | board[y];
                    else rem = rem | board[y];
                }

                
                //loop through region
                for(int y = (i / (width * rootWidth)) * width * rootWidth; y < i + width * rootWidth; y += width){
                    for(int o = ((i % width) / rootWidth) * rootWidth; o < rootWidth; o++){
                        if(board[y + o] % 2 != 1) res = res | board[y + o];
                        else rem = rem | board[y + o];
                    }

                }
                
                board[i] = board[i] & ~res & 1023; //set the cell to all numbers that do not conflict with known cell
                if(board[i] - 1 & board[i] - 2 == 0) board[i]--;//set last bit zero when final value reached
                else{
                    //if cell is the only one with possible value in cell/column/region then set cell to value
                    mask = board[i] & ~rem & 1022; //set the cell to all numbers that do not
                    if(mask & mask - 1 == 0) board[i] = mask;
                }
            }
        }
        runThrough++;
        printf("--------------------\n");
        printIntBoard(board);
    }

}

/*
set of cells x
within a set x each value must appear once
each cell has a set of possible values
if a subset of x of N cells have N unique possible values between them then all other values are impossible
*/
/*
to generate all permutations of array
pointers start on first N indices
permutation for each increment of front pointer until end
increment 2nd pointer from front and repeat until front 2 pointer in last 2 indices
repeat 


set[]
currentPerm[]
currentPermLength

permAnd 
notPermOr

for each cell in set
    if in perm permAnd = permAnd & cell
    else notPermOr = notPermOr & cell

permUnique = permAnd & ~notPermOr

if the number of set bits in permUnique = currentPermLength
    set all cells in current permutation to permUnique
*/
void retainUnique(int setSize, int* perm, int permLength, int depth, int start, int* set){
    if(depth == permLength){

        /*
        for(int i = 0; i < permLength; i++){
            printf("%d", perm[i]);
        }
        printf("\n");
        */

        int permAnd = 1022;
        int notPermOr = 0;
        int permUnique;

        for(int i = 0; i < setSize; i++){

            int inPerm = 0;
            for(int y = 0; y < permLength; y++){
                if(i == perm[y]) inPerm = 1;
            }

            if(inPerm) {
                permAnd = permAnd & set[i];
            }
            else notPermOr = notPermOr | set[i];
        }

        permUnique = permAnd & ~notPermOr;

        if(__builtin_popcount(permUnique) == permLength){
            for(int i = 0; i < permLength; i++){
                set[perm[i]] = permUnique;
            }
        }
    } else {
        for(int i = start; i < setSize - (permLength - depth) + 1; i++){
            perm[depth] = i;
            retainUnique(setSize, perm, permLength, depth + 1, i + 1, set);
        }
    }
}

void permGenerate(int setSize, int* perm, int permLength, int depth, int start){
    if(depth == permLength){
        for(int i = 0; i < permLength; i++){
            //printf("%d", perm[i]);
        }
        //printf("\n");
    } else {
        for(int i = start; i < setSize - (permLength - depth) + 1; i++){
            perm[depth] = i;
            permGenerate(setSize, perm, permLength, depth + 1, i + 1);
        }
    }
}



int *solve2(int* inputBoard){



    //int cellbytes = ((size / 8) + 1);

    //int area = width * width;

    int board[area];
    int set[width];
    int res;

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

                res = 0;

                //loop through row
                for(int y = (i / width) * width; y < ((i / width) * width) + width; y++){
                    if(board[y] % 2 != 1) res = res | board[y];                   
                }

                //loop through column
                for(int y = i % width; y < area; y += width){
                    if(board[y] % 2 != 1) res = res | board[y];                 
                }

                
                //loop through region
                for(int y = (i / (width * rootWidth)) * width * rootWidth; y < i + width * rootWidth; y += width){
                    for(int o = ((i % width) / rootWidth) * rootWidth; o < rootWidth; o++){
                        if(board[y + o] % 2 != 1) res = res | board[y + o];
                    }

                }
                
                board[i] = board[i] & ~res & 1023; //set the cell to all numbers that do not conflict with known cell
                if(board[i] - 1 & board[i] - 2 == 0) board[i]--;//set last bit zero when final value reached
            }
        }
        runThrough++;
        printf("--------------------\n");
        printIntBoard(board);
    }

}

int *solve3(int* inputBoard){




    //int cellbytes = ((size / 8) + 1);

    //int area = width * width;

    int board[area];
    int set[width];
    int perm[width - 1];
    for(int i = 0; i < width - 1; i++){
        perm[i] = 0;
    }

    for(int i = 0; i < area; i++){
        if(inputBoard[i] == 0) board[i] = 1022;
        else intToMap(inputBoard[i], &board[i]);
    }
    
    printf("--------------------\n");

    printIntBoard(board);

    

    int runThrough = 0;
    while(runThrough < 10){ //prevent infinite loop
        

        //retain unique for columns
        for(int i = 0; i < area; i++){
            for(int y = 0; y < width; y++){
                set[y] = board[y * width + i];  
            }
            /*printf("\n--------------------\n");
            printIntBoard(board);
            printf("\n--------------------\n");
            for(int y = 0; y < width; y++){
                printf("%d", set[y]);
                printf(",");
            }
            fflush(stdout);*/
            for(int y = 1; y < width - 1; y++){ //retain unique for every permutation size  
                retainUnique(width, perm, y, 0, 0, set);
            }
            for(int y = 0; y < width; y++){
                board[y * width + i] = set[y];  
            }
        }


        //retain unique in rows
        for(int i = 0; i < area; i += width){
            for(int y = 1; y < width - 1; y++){ //retain unique for every permutation size
                retainUnique(width, perm, y, 0, 0, &board[i]);
            }
        }

        int ballsack2[width - 1];

        //retain unique for region
        for(int i = 0; i < area; i += rootWidth){
            for(int y = 0; y < rootWidth * width; y += width){
                for(int o = 0; o < rootWidth; o++){
                    set[o + (y / width) * rootWidth] = board[i + y + o];
                }
            }
            printf("\n--------------------\n");
            printIntBoard(board);
            printf("\n--------------------\n");
            for(int y = 0; y < width; y++){
                printf("%d", set[y]);
                printf(",");
            }
            fflush(stdout);
            for(int y = 1; y < width - 1; y++){ //retain unique for every permutation size  
                retainUnique(width, perm, y, 0, 0, set);
            }
            for(int y = 0; y < rootWidth * width; y += width){
                for(int o = 0; o < rootWidth; o++){
                    board[i + y + o] = set[o + (y / width) * rootWidth];
                }
            }
        }



        //retain unique in colums
        /*
        for(int i = 0; i < width; i++){
            for(int y = 0; y < width; y ++){
                set[y] = board[i + y * width]; 
            }
            for(int y = 1; y < width - 1; y++){ //retain unique for every permutation size
                retainUnique(width, perm, y, 0, 0, set);
            }
        }*/
 
        runThrough++;
        
    }
    printf("--------------------\n");
    printIntBoard(board);
    fflush(stdout);
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


int main(int argc, char **argv) {

    printf("Hello World\n"); 

    //permGenerate(10, perm, 3, 0, 0);

    
    int intBoard[area];
    if(readSudokuFromFile("../examples/test1.csv", intBoard)){
        printIntBoard(intBoard);
    }

    solve3(intBoard);

    int setsize = 9;
    int perm[8] = {3, 4, 5, 7, 8, 0, 0, 0};
    int permLength = 5;
    int depth = 5;
    int start = 0;
    int set[9] = {512,2,8,1022,1022,1022,32,1022,1022};
    
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

