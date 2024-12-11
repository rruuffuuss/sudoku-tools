#include <errno.h>
#include <stdio.h>

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

void intBoardToMapBoard(int* inputBoard, int* board, int len){
    for(int i = 0; i < len; i++){
        if(inputBoard[i] == 0) board[i] = 1022;
        else intToMap(inputBoard[i], &board[i]);
    }
}

void mapBoardToIntBoard(int* inputBoard, int* board, int len){
    for(int i = 0; i < len; i++){
        board[i] = mapToInt(inputBoard[i]);
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

        int permMask[setSize];
        for(int i = 0; i < setSize; i++) permMask[i] = 0;
        for(int i = 0; i < permLength; i++) permMask[perm[i]] = 1;


        /*
        for(int i = 0; i < permLength; i++){
            printf("%d", perm[i]);
        }
        printf("\n");
        */

        int permAnd = 1022;
        int notPermOr = 0;
        int permUnique;

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

        if(equalCheck && __builtin_popcount(first) <= permLength){
            for(int i = 0; i < setSize; i++){
                if(!permMask[i]) set[i] = set[i] & ~first;
            }
        }

        if(__builtin_popcount(permUnique) == permLength){
            for(int i = 0; i < setSize; i++){
                if(permMask[i]) set[i] = permUnique;
                else set[i] = set[i] & ~permUnique;
            }
        }
    } else {
        for(int i = start; i < setSize - (permLength - depth) + 1; i++){
            perm[depth] = i;
            retainUnique(setSize, perm, permLength, depth + 1, i + 1, set);
        }
    }
}

void solve3(int* inputBoard, int* board){

    //int board[area]; 
    intBoardToMapBoard(inputBoard, board, area);

    int set[width];
    int perm[width - 1];


    int runThrough = 0;
    while(runThrough < 10){ //prevent infinite loop
        
        
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
 
        runThrough++;    
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


int main(int argc, char **argv) {

    printf("Hello World\n"); 

    //permGenerate(10, perm, 3, 0, 0);

    
    int intBoard[area];
    int outBoard[area];
    if(readSudokuFromFile("../examples/test1.csv", intBoard)){
        printIntBoard(intBoard);
    }

    solve3(intBoard, outBoard);
    mapBoardToIntBoard(outBoard, intBoard, area);

    printf("------------------------------\n");

    printIntBoard(intBoard);
    
    return 0;
}

