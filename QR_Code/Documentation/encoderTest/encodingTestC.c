# include <stdio.h>
# include "shoco.h"
# include <string.h>
#include <stdlib.h>


char BUFFER[256];


char* replaceSpacesWithUnderscore(const char* string) {
    size_t length = strlen(string);
    char* buffer = malloc((length + 2) * sizeof(char)); // Add space for underscore and exclamation mark
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    size_t i;
    for (i = 0; i < length; i++) {
        if (string[i] == ' ') {
            buffer[i] = '_';
        } else {
            buffer[i] = string[i];
        }
    }
    buffer[length] = '_';
    buffer[length + 1] = '!'; // Add exclamation mark
    buffer[length + 2] = '\0';

    return buffer;
}



char* replaceUnderscoresWithSpaces(const char* input) {
    size_t length = strlen(input);
    char* buffer = malloc((length + 1) * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    size_t i;
    for (i = 0; i < length; i++) {
        if (input[i] == '_') {
            buffer[i] = ' ';
        } else {
            buffer[i] = input[i];
        }
    }
    buffer[length] = '\0';

    return buffer;
}

void extractSubstring(const char* input, char* output) {
    int length = strlen(input);
    int endIndex = -1;

    for (int i = 0; i < length-1; i++) {
        if (input[i] == ' ' && input[i+1] == '!') {
            endIndex = i;
            break;
        }
    }

    if (endIndex != -1) {
        strncpy(output, input, endIndex + 1);
        output[endIndex + 1] = '\0';
    } else {
        strcpy(output, input);
    }
}



//encode/compress function
void encodeData(int storageCabinet, int floor, const char* type){
  char typeBuffer[64];
  const char* typeBufferIn = replaceSpacesWithUnderscore(type);
  shoco_compress(test, strlen(test), typeBuffer, 64);
  sprintf(BUFFER, "%d %d %s", storageCabinet, floor, typeBuffer);
  //free(typeBufferIn);

}

void decodeData(int* storageCabinetAndFloor, char* type){
    char typeBuffer[32];
    int storageCabinet;
    int floor;
    sscanf(BUFFER, "%d %d %s", &storageCabinet, &floor, typeBuffer);
    shoco_decompress(typeBuffer, 64, type, 64);
    storageCabinetAndFloor[0] = storageCabinet;
    storageCabinetAndFloor[1] = floor;
    const char* typeBufferOut = replaceUnderscoresWithSpaces(type);
    strcpy(type, typeBufferOut);
    //free(typeBufferOut);
}


  int main(){
    int storageCabinet = 1;
    int floor = 2;
    const char type[] = "Rock From The Sun";
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
    extractSubstring(type2, type2);
    printf("type before: %s\n", type2);



   return 0;
}