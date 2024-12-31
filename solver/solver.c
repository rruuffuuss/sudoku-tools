#define _POSIX_C_SOURCE 199309L
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/*
TODO
possibly treat permutation certainty like width and increment the certainty used depending on the success/failure of previous runs
*/


const int rootWidth = 3;
const int width = rootWidth * rootWidth;
const int area = width * width;

int permutationWidth = 4; //max size of a permutation
int permutationCertainty = 6; //max number of potential values for any given cell for a permutation to be considered 

int num_of_puzzles = 500;

char puzzlePath[50] = "../examples/allUnsolveableTest.csv";
char solutionPath[50] = "../examples/allUnsolveableTestSolutions.csv";

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

void CheckSwordFish(int* board){

    //all row combinations
    for(int row1 = 0; row1 < area - 2 * width; row1 += width){
        for(int row2 = row1 + width; row2 < area - width; row2 += width){
            for(int row3 = row2 + width; row3 < area; row3 += width){
            
                //all column combinations
                for(int col1 = 0; col1 < width - 2; col1++){
                    for(int col2 = col1 + 1; col2 < width - 1; col2++){
                        for(int col3 = col2 + 1; col3 < width; col3++){

                            int row1Comp = 0, row2Comp = 0, row3Comp = 0, col1Comp = 0, col2Comp = 0, col3Comp = 0;
                            int intersection = 1022;

                            //loop through rows
                            for(int i = 0; i < width; i++){
                                if(i == col1 || i == col2 || i == col3) {
                                    intersection = intersection & board[row1 + i] & board[row2 + i] & board[row3 + i];
                                }
                                else {
                                    row1Comp = row1Comp | board[row1 + i];
                                    row2Comp = row2Comp | board[row2 + i];
                                    row3Comp = row3Comp | board[row3 + i];
                                }
                            }
                            
                            //loop through columns
                            for(int i = 0; i < area; i += width){
                                if(i != row1 && i != row2 && i != row3){
                                    col1Comp = col1Comp | board[i + col1];
                                    col2Comp = col2Comp | board[i + col2];
                                    col3Comp = col3Comp | board[i + col3];
                                }
                            }

                            int colUnique = intersection & ~(col1Comp | col2Comp | col3Comp);
                            int rowUnique = intersection & ~(row1Comp | row2Comp | row3Comp);

                            if(colUnique != 0){
                                //remove colUnique from rows
                                for(int i = 0; i < width; i++){
                                    if(i != col1 && i != col2 && i != col3) {
                                        board[row1 + i] = board[row1 + i] & ~colUnique;
                                        board[row2 + i] = board[row2 + i] & ~colUnique;
                                        board[row3 + i] = board[row3 + i] & ~colUnique;
                                    }
                                }
                            }
                            if(rowUnique != 0){
                                //remove rowUnique from cols
                                for(int i = 0; i < area; i += width){
                                    if(i != row1 && i != row2 && i != row3){
                                        board[i + col1] = board[i + col1] & ~rowUnique;
                                        board[i + col2] = board[i + col2] & ~rowUnique;
                                        board[i + col3] = board[i + col3] & ~rowUnique;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void CheckXwing(int* board){

    //all row combinations
    for(int row1 = 0; row1 < area - width; row1 += width){
        for(int row2 = row1 + width; row2 < area; row2 += width){
            
            //all column combinations
            for(int col1 = 0; col1 < width - 1; col1++){
                for(int col2 = col1 + 1; col2 < width; col2++){

                    int row1Comp = 0, row2Comp = 0, col1Comp = 0, col2Comp = 0;
                    int intersection = 1022;

                    //loop through rows
                    for(int i = 0; i < width; i++){
                        if(i == col1 || i == col2) {
                            intersection = intersection & board[row1 + i] & board[row2 + i];
                        }
                        else {
                            row1Comp = row1Comp | board[row1 + i];
                            row2Comp = row2Comp | board[row2 + i];
                        }
                    }
                    
                    //loop through columns
                    for(int i = 0; i < area; i += width){
                        if(i != row1 && i != row2){
                            col1Comp = col1Comp | board[i + col1];
                            col2Comp = col2Comp | board[i + col2];
                        }
                    }

                    int colUnique = intersection & ~(col1Comp | col2Comp);
                    int rowUnique = intersection & ~(row1Comp | row2Comp);

                    if(colUnique != 0){
                        //remove colUnique from rows
                        for(int i = 0; i < width; i++){
                            if(i != col1 && i != col2) {
                                board[row1 + i] = board[row1 + i] & ~colUnique;
                                board[row2 + i] = board[row2 + i] & ~colUnique;
                            }
                        }
                    }
                    if(rowUnique != 0){
                        //remove rowUnique from cols
                        for(int i = 0; i < area; i += width){
                            if(i != row1 && i != row2){
                                board[i + col1] = board[i + col1] & ~rowUnique;
                                board[i + col2] = board[i + col2] & ~rowUnique;
                            }
                        }
                    }

                }
            }
        }
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
    //dont do this. It is impossible for these cells to contain permUnique //set all cells outside the current permutation to value AND NOT permUnique
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

        /*
        If a subset of set x of N cells all share N possible values then these values are impossible outside the subset
        e.g. if 3 values are the only possible values in 3 cells, no other cell can contain these values
        */
        if(equalCheck && __builtin_popcount(first) <= permLength){
            for(int i = 0; i < setSize; i++){
                if(!permMask[i]) set[i] = set[i] & ~first;
            }
        }

        /*
        If a subset of set x of N cells have N unique possible values between them then all other values are impossible
        e.g. if 3 values only appear in 3 cells, then these cells cannot contain anything other than these 3 values         
        */
        if(__builtin_popcount(permUnique) == permLength){
            for(int i = 0; i < setSize; i++){
                if(permMask[i]) set[i] = permUnique;
            }
        }
    } else {
        for(int i = start; i < setSize - (permLength - depth) + 1; i++){
            perm[depth] = i;
            int x = set[i];
            if(__builtin_popcount(set[i]) <= permutationCertainty ){
                retainUnique(setSize, perm, permLength, depth + 1, i + 1, set);
            }
            
        }
    }
}

int solve(int* board){

    //int board[area]; 
    

    int set[width];
    int perm[width - 1];
    int currentWidth = 1;

    int rowSum = 0;
    int lastRowSum = 0;

    int runThrough = 0;
    while(rowSum != 9198){ //prevent infinite loop
        
        //printIntBoard(board);
        //printf("--------------------\n");
        //retain unique for columns

       
        for(int i = 0; i < width; i++){
            for(int y = 0; y < width; y++){
                set[y] = board[y * width + i];  
            }
            retainUnique(width, perm, currentWidth, 0, 0, set);
            for(int y = 0; y < width; y++){
                board[y * width + i] = set[y];  
            }
        }


        //retain unique in rows
        for(int i = 0; i < area; i += width){
            retainUnique(width, perm, currentWidth, 0, 0, &board[i]);
        }

        //retain unique for region
        for(int i = 0; i < area; i += rootWidth * width){//each vertical region
            for(int j = 0; j < width; j += rootWidth){//each horizontal region
                
                for(int y = 0, regionSetCounter = 0; y < rootWidth * width; y += width){//each row within a region
                    for(int o = 0; o < rootWidth; o++, regionSetCounter++){//each column within a region
                        set[regionSetCounter] = board[i + j + y + o];
                    }
                }

                retainUnique(width, perm, currentWidth, 0, 0, set);
                

                for(int y = 0, regionSetCounter = 0; y < rootWidth * width; y += width){//each row within a region
                    for(int o = 0; o < rootWidth; o++, regionSetCounter++){//each column within a region
                        board[i + j + y + o] = set[regionSetCounter];
                    }
                }
            }
        }
        

        rowSum = 0;
        //printIntBoard(board);
        for(int i = 0; i < area; i++) {
            if(board[i] == 0) return 0; //if there are no candidates for a cell, a solution does not exist
            rowSum += board[i];
        }
        if(lastRowSum == rowSum){
            //printf("Unable to solve, multiple solutions exist\n");
            //mapBoardToIntBoard(board,area);
            currentWidth++;
            if(currentWidth > permutationWidth ){

                int candidateNo = 2;
                int currentCell = 0;
                int currentBit = 0;
                int childSolve = 0;

                while (!childSolve)
                {
                    
                    if(__builtin_popcount(board[currentCell]) == candidateNo){
                        int childBoard[area * num_of_puzzles];
                        memcpy(childBoard, board, sizeof(int) * area);
                        
                        int v;
                        int v2 = childBoard[currentCell];

                        //remove the most significant bit current bit number of times
                        for(int i = 0; i < currentBit + 1; i++) {
                            v = v2;
                            v |= v >> 1; 
                            v |= v >> 2;
                            v |= v >> 4;
                            v |= v >> 8;
                            v2 = (v >> 1) & v2;
                        }
                        v++;
                        v >>= 1;
                        
                        //currentbit going stupid high for no reason

                        childBoard[currentCell] = v;

                        if(solve(childBoard)){
                            memcpy(board, childBoard, sizeof(int) * area);
                            return 1;
                            childSolve = 1;
                        } else {
                            currentBit++;
                            if(currentBit == candidateNo){
                                currentBit = 0;
                                currentCell++;
                            }
                        }
                    } else {
                        currentCell++;
                        if(currentCell >= area){
                            currentCell = 0;
                            candidateNo++;
                            if(candidateNo > 7) return 0;
                        }
                    }
                }
                                
            }
        } else {
            currentWidth = 1;
        }
        lastRowSum = rowSum;    
    }
    mapBoardToIntBoard(board, area);
    return 1;
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
        char puzzle[area + 4]; //+ 2 for null char and EOL
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
    struct timespec start, end;
    // Get the start time
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0) {
        perror("clock_gettime");
        return 1;
    }

    if(argc >= 3){
        //sscanf(*argv[1], "%d", &permutationWidth);
        //sscanf(*argv[2], "%d", &permutationCertainty);
        permutationWidth = *argv[1] - 48;
        permutationCertainty = *argv[2] - 48;

        if(permutationWidth < 0 || permutationWidth > 10 || permutationCertainty < 0 || permutationCertainty > 10){
            printf("permutation width and certainty must be between 1 and 9 inclusive\n");
            return 1;
        }
    }
    if(argc >= 5){
        strcpy(puzzlePath, argv[3]);
        strcpy(solutionPath, argv[4]);
    }
    if(argc >= 6){
        sscanf(argv[5], "%d", &num_of_puzzles);
    }

    //printf("Hello World\n"); 

    //permGenerate(10, perm, 3, 0, 0);

    int successNo = 0;
    int intBoard[area * num_of_puzzles];
    int solBoard[area * num_of_puzzles];
    //if(readSudokusFromFile("../examples/test1.csv", intBoard, 1, 1)){
    if(readSudokusFromFile(puzzlePath, intBoard, 0, num_of_puzzles)){
        /*for(int i = 0; i < num_of_puzzles; i++){
            printf("------------------------------\n");
            printf("Sudoku number %d:\n", i);
            printIntBoard(&intBoard[i * area]);
        }*/
       //printf("loading successful\n");

    } else {
        printf("loading unsuccessful\n");
        return 1;
    }

    for(int i = 0; i < num_of_puzzles; i++){
        intBoardToMapBoard(&intBoard[i * area], area);
        successNo += solve(&intBoard[i * area]);
    }

    // Get the end time
    if (clock_gettime(CLOCK_MONOTONIC, &end) != 0) {
        perror("clock_gettime");
        return 1;
    }

    // Calculate the elapsed time in nanoseconds
    long long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

    /*for(int i = 0; i < num_of_puzzles; i++){
            printf("------------------------------\n");
            printf("Sudoku number %d:\n", i);
            printIntBoard(&intBoard[i * area]);
        }
    printf("The function took %ld microseconds to execute.\n", elapsed_us);
    printf("This is an average of %d microseconds per puzzle\n", elapsed_us / num_of_puzzles);
    printf("a solution was found for %d puzzles\n", successNo);
    */

    successNo = 0;
    int allMatch;
    if(readSudokusFromFile(solutionPath, solBoard, 0, num_of_puzzles)){
        for(int i = 0; i < num_of_puzzles; i++){
            allMatch = 1;
            for(int y = 0; y < area; y++){
                if(solBoard[i * area + y] != intBoard[i * area + y]) allMatch = 0;
            }
            // if(!allMatch) {
            //     printIntBoard(&intBoard[i * area]);
            //     printf("--\n");
            //     printIntBoard(&solBoard[i * area]);
            //     printf("------------------------------\n");
            // }
            successNo += allMatch;
        }
        //printf("\n");
        printf("%d ", permutationWidth);
        printf("%d ", permutationCertainty);
        printf("%d ", successNo);
        printf("%d", elapsed_us);
        
       
    }
    return 0;
}

