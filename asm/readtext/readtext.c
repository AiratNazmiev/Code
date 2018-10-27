#include <sys/stat.h>
#include <malloc.h>
#include <assert.h>
#include "readtext.h"

struct LinkedBuffer bufferLinking(FILE *input) {
    if (input == NULL) {
        fprintf(stderr, "bufferLinking: Cannot open file exception");
        struct LinkedBuffer err = {0,0,0,0};
        return err;
    }

    struct stat fileStat = {0, 0, 0, 0};
    fstat(fileno(input), &fileStat);

    char *buffer = (char *) calloc(fileStat.st_size, sizeof(*buffer));

    struct TextParameters txtPar = readText(input, buffer, fileStat.st_size);

    char **str_ptr = (char **) calloc(txtPar.numStr, sizeof(*str_ptr));
    struct LinkedBuffer linkedBuff = {buffer, str_ptr, txtPar.numSym, txtPar.numStr};
    linkedBuff.buffer[txtPar.numSym] = '\0';

    linking(linkedBuff.buffer, linkedBuff.str_ptr, linkedBuff.numSym);

    return linkedBuff;
}

struct TextParameters readText(FILE *input, char *buffer, int len) {
    if (input == NULL) {
        fprintf(stderr, "readText: Cannot open file exception");
        struct TextParameters err = {0, 0};
        return err;
    }

    fread(buffer, sizeof(char), len, input);

    int numStr = 0;
    int numSym = 0;

    while (buffer[numSym] != '\0') {
        if (buffer[numSym] == '\n') {
            buffer[numSym++] = '\0';
            numStr++;
        }
        numSym++;
    }

    if (buffer[numStr] != '\n')
        numStr++;

    struct TextParameters txtPar = {numSym, numStr};

    return txtPar;
}

void linking(char buffer[], char *str_ptr[], int numSym) {
    assert(numSym >= 0);
    assert(buffer != NULL);
    assert(str_ptr != NULL);

    int num = 0;
    str_ptr[num++] = &buffer[0];

    for (int i = 0; i < numSym - 1; i++) {
        if (buffer[i] == '\0') {
            str_ptr[num++] = &buffer[i + 1];
        }
    }
}