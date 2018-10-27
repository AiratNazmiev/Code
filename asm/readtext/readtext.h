#include <stdio.h>

#ifndef ASSEMBLER_READTEXT_H
#define ASSEMBLER_REEDTEXT_H

/*!
 * @brief structure includes string array (char *buffer),
 * array of char pointers (char **str_ptr) to strings from buffer,
 * number of symbols (numsym) and strings (numstr) in buffer
 */

struct LinkedBuffer {
    char *buffer;
    char **str_ptr;
    int numSym;
    int numStr;
};

/*!
 * @brief structure includes number of symbols (numSym), number of strings (numStr)
 */

struct TextParameters {
    int numSym;
    int numStr;
};

/*!
 * @brief Creates buffer and linked buffer
 * @param input input file
 * @return structure LinkedBuffer, includes string array (char *buffer), array of char pointers (char **str_ptr) to strings from buffer,
 * number of symbols (numsym) and strings (numstr) in buffer
 */

struct LinkedBuffer bufferLinking(FILE *input);

/*!
 * @brief read input file from input to char *buffer
 * @param input input file
 * @param buffer char *buffer
 * @return structure TextParameters, includes number of symbols (numSym), number of strings (numStr)
 */

struct TextParameters readText(FILE *input, char *buffer, int len);

/*!
 * @brief linking the beginnings of strings in buffer to str_ptr
 * @param buffer the source buffer of chars
 * @param str_ptr array of char pointers
 * @param numSym number of elements in buffer
 */

void linking(char buffer[], char *str_ptr[], int numSym);

#endif //ASSEMBLER_READTEXT_H