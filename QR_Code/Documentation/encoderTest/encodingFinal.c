# include <stdio.h>
# include <string.h>
#include <stdlib.h>


char BUFFER[256];


//encode/compress function
void encodeData(int storageCabinet, int floor, const char* type){
  sprintf(BUFFER, "%d %d %s", storageCabinet, floor, type);
}

void decodeData(int* storageCabinetAndFloor, char* type){
    char typeBuffer[32];
    int storageCabinet;
    int floor;
    sscanf(BUFFER, "%d %d %s", &storageCabinet, &floor, typeBuffer);
    storageCabinetAndFloor[0] = storageCabinet;
    storageCabinetAndFloor[1] = floor;
    strcpy(type, typeBuffer);
    //free(typeBufferOut);
}


  int main(){
    int storageCabinet = 1;
    int floor = 2;
    const char type[] = "Rock";

    encodeData(storageCabinet, floor, type);
    printf("%s\n", BUFFER);

    int storageCabinetAndFloor[2];
    char type2[64];
    decodeData(storageCabinetAndFloor, type2);
    int storageCabinet2 = storageCabinetAndFloor[0];
    int floor2 = storageCabinetAndFloor[1];

    printf("storage : %d\n", storageCabinet2);
    printf("floor : %d\n", floor2);
    printf("type before : %s\n", type2);



   return 0;
}