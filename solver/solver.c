#define _POSIX_C_SOURCE 199309L
#include <errno.h>
#include <stdio.h>
#include <time.h>

const int rootWidth = 3;
const int width = rootWidth * rootWidth;
const int area = width * width;

void intToMap(int n, int *c){
    *c = 1 << (n);
}

int mapToInt(int c){
    int r = 0;
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

void intBoardToMapBoard(int* inputBoard, int len){
    for(int i = 0; i < len; i++){
        if(inputBoard[i] == 0) inputBoard[i] = 1022;
        else intToMap(inputBoard[i], &inputBoard[i]);
    }
}

void mapBoardToIntBoard(int* inputBoard, int len){
    for(int i = 0; i < len; i++){
        inputBoard[i] = mapToInt(inputBoard[i]);
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
    set all cells outside the current permutation to value AND NOT permUnique
*/


void retainUnique(int setSize, int* perm, int permLength, int depth, int start, int* set){
    if(depth == permLength){

        int permMask[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        //for(int i = 0; i < setSize; i++) permMask[i] = 0;
        for(int i = 0; i < permLength; i++) permMask[perm[i]] = 1;


        /*
        for(int i = 0; i < permLength; i++){
            printf("%d", perm[i]);
        }
        printf("\n");
        */

        int permAnd = 1022; //Values possible for all cells in a subset
        int notPermOr = 0; //Values possbile for any cell not in a subset
        int permUnique; //values unique to the subset

        int first = set[perm[0]];
        int equalCheck = 1;

        for(int i = 0; i < setSize; i++){
            if(permMask[i]) {
                permAnd = permAnd & set[i];
                if(equalCheck && first != set[i]) equalCheck = 0;
            }
            else notPermOr = notPermOr | set[i];
        }

        permUnique = permAnd & ~notPermOr;

        //If a subset of set x of N cells all share N possible values then these values are impossible outside the subset
        if(equalCheck && __builtin_popcount(first) <= permLength){
            for(int i = 0; i < setSize; i++){
                if(!permMask[i]) set[i] = set[i] & ~first;
            }
        }

        //If a subset of set x of N cells have N unique possible values between them then all other values are impossible
        if(__builtin_popcount(permUnique) == permLength){
            for(int i = 0; i < setSize; i++){
                if(permMask[i]) set[i] = permUnique;
                else set[i] = set[i] & ~permUnique;
            }
        }
    } else {
        for(int i = start; i < setSize - (permLength - depth) + 1; i++){
            perm[depth] = i;
            if(__builtin_popcount(set[i]) != 1){
                retainUnique(setSize, perm, permLength, depth + 1, i + 1, set);
            }
        }
    }
}

void solve(int* board){

    //int board[area]; 
    intBoardToMapBoard(board, area);

    int set[width];
    int perm[width - 1];


    int runThrough = 0;
    while(runThrough < 100){ //prevent infinite loop
        
        //printIntBoard(board);
        //printf("--------------------\n");
        //retain unique for columns
        
        for(int i = 0; i < width; i++){
            for(int y = 0; y < width; y++){
                set[y] = board[y * width + i];  
            }
            for(int y = 1; y < width; y++){ //retain unique for every permutation size  
                retainUnique(width, perm, y, 0, 0, set);
            }
            for(int y = 0; y < width; y++){
                board[y * width + i] = set[y];  
            }
        }


        //retain unique in rows
        for(int i = 0; i < area; i += width){
            for(int y = 1; y < width; y++){ //retain unique for every permutation size
                retainUnique(width, perm, y, 0, 0, &board[i]);
            }
        }

        //retain unique for region
        for(int i = 0; i < area; i += rootWidth * width){//each vertical region
            for(int j = 0; j < width; j += rootWidth){//each horizontal region
                
                for(int y = 0, regionSetCounter = 0; y < rootWidth * width; y += width){//each row within a region
                    for(int o = 0; o < rootWidth; o++, regionSetCounter++){//each column within a region
                        set[regionSetCounter] = board[i + j + y + o];
                    }
                }

                for(int y = 1; y < width; y++){ //retain unique for every permutation size  
                    retainUnique(width, perm, y, 0, 0, set);
                }

                for(int y = 0, regionSetCounter = 0; y < rootWidth * width; y += width){//each row within a region
                    for(int o = 0; o < rootWidth; o++, regionSetCounter++){//each column within a region
                        board[i + j + y + o] = set[regionSetCounter];
                    }
                }
            }
        }
        //printIntBoard(board);
        runThrough++;    
    }
    mapBoardToIntBoard(board, area);
}


int readSudokusFromFile(char *fileName, int* intBoards, int newLineDelimeted, int number){
    FILE *f;
    f = fopen(fileName, "r");

    if (f == NULL) {
        printf("Can't open %s \n", fileName);
        perror("Failed to open file.\n");
      
        return 0;
   }

    
    if(newLineDelimeted && number == 1){

        char stringBoardRow[2 * width + 1];

        int boardPosition = 0;

        for(int i = 0; i < width; i++){
            fgets(stringBoardRow, sizeof(stringBoardRow), f);
            for(int y = 0; y < 2 * width - 1; y++){
                if(stringBoardRow[y] > 47 && stringBoardRow[y] < 58){
                    intBoards[boardPosition++] = stringBoardRow[y] - 48; //subtract 48 from ascii code for int value & increment board pointer
                }
            }
        }
        fclose(f);
        return 1;
    } else if (!newLineDelimeted){
        char puzzle[area + 1];
        for(int i = 0; i < number * area; i += area){
            fgets(puzzle, sizeof(puzzle), f);
            for(int y = 0; y < area; y++){
                if(puzzle[y] > 47 && puzzle[y] < 58){
                    intBoards[i + y] = puzzle[y] - 48; //subtract 48 from ascii code for int value & increment board pointer
                }
            }
        }
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}


int main(int argc, char **argv) {

    printf("Hello World\n"); 

    //permGenerate(10, perm, 3, 0, 0);

    
    int intBoard[area];
    int outBoard[area];
    //if(readSudokusFromFile("../examples/test1.csv", intBoard, 1, 1)){
    if(readSudokusFromFile("../examples/test2.csv", intBoard, 0, 1)){
        printIntBoard(intBoard);
    }

    struct timespec start, end;

    // Get the start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Call the function to measure
    solve(intBoard);

    // Get the end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Calculate the elapsed time in nanoseconds
    long elapsed_ns = (end.tv_nsec - start.tv_nsec);
    

    printf("------------------------------\n");

    printIntBoard(intBoard);
    printf("The function took %ld nanoseconds to execute.\n", elapsed_ns);
    return 0;
}

