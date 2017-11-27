#include "assignment1.h"

/*************************************************************************
Name: Main
Purpose: Main Function, parses all information from the file using a switch
         statement, and calls a converter function which does what is necessary
         to the information to make it compile as a .c program
**************************************************************************/
int main(int argc, char *argv[]) {
  char currentChar, prevChar;
  char ** parsedStorage;
  char * buffer, * outputFileString, *theToken;
  int bSize, storSize, parsedSize, OFSSize;
  int i;
  FILE * inputFile;

  inputFile = fopen(argv[1], "r");

  bSize = storSize = parsedSize = OFSSize = 0;
  currentChar = ' ';

  parsedStorage = malloc(sizeof(char*)*storSize+1);
  buffer = malloc(sizeof(char)*bSize+1);
  outputFileString = mallocBuffer(0);

  do {
    if (inputFile == NULL) {
      printf("File was not found...Exiting program\n");
      free(parsedStorage);
      free(buffer);
      free(outputFileString);
      return(-1);
    }
    currentChar = fgetc(inputFile);
    if (feof(inputFile)) {
      break;
    }
    /*Switch statement that parses information from the file*/
    switch (currentChar) {
      case ' ':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ' ');
        buffer = mallocBuffer(bSize);
        break;
      case '\n':
      /*Checks for empty buffer string, if it is not empty adds it on to the char ** array parsedStorage
        and begins a new one which it adds on as well*/
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\n');
        bSize = 0;
        buffer = mallocBuffer(bSize);
        break;
      case '\r':
      /*Almost every switch does the same thing as the above comment*/
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\r');
        buffer = mallocBuffer(bSize);
      break;
      case '\t':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\t');
        buffer = mallocBuffer(bSize);
      break;
      case '\v':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\v');
        buffer = mallocBuffer(bSize);
        break;
      case '\f':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '\f');
        buffer = mallocBuffer(bSize);
        break;
      case '{':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '{');
        buffer = mallocBuffer(bSize);
        break;
      case '}':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '}');
        buffer = mallocBuffer(bSize);
      break;
      case ';':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ';');
        buffer = mallocBuffer(bSize);
      break;
      case '.':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '.');
        buffer = mallocBuffer(bSize);
      break;
      case ':':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ':');
        buffer = mallocBuffer(bSize);
      break;
      case '(':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '(');
        buffer = mallocBuffer(bSize);
      break;
      case ')':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ')');
        buffer = mallocBuffer(bSize);
      break;
      case ',':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, ',');
        buffer = mallocBuffer(bSize);
      break;
      case '+':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '+');
        buffer = mallocBuffer(bSize);
        break;
      case '*':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '*');
        buffer = mallocBuffer(bSize);
        break;
      case '/':
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '/');
        buffer = mallocBuffer(bSize);
        break;
      case '"':
      /*This switch case is for comments, it will add entire comments on as one token*/
        if (strcmp(buffer, "\0") != 0) {
          parsedStorage = addBufferStringNoChar(parsedStorage, buffer, &bSize, &parsedSize);
          buffer = mallocBuffer(bSize);
        }
        bSize++;
        buffer = reallocBuffer(buffer, bSize);
        buffer[bSize-1] = currentChar;
        prevChar = currentChar;
        currentChar = fgetc(inputFile);
        if (feof(inputFile)) {
          break;
        }
        while ((currentChar != '"') || (prevChar == '\\')) {
          bSize++;
          buffer = reallocBuffer(buffer, bSize);
          buffer[bSize-1] = currentChar;
          prevChar = currentChar;
          currentChar = fgetc(inputFile);
          if (feof(inputFile)) {
            break;
          }
        }
        parsedStorage = addBufferStringYesChar(parsedStorage, buffer, &bSize, &parsedSize, '"');
        buffer = mallocBuffer(bSize);
        break;
      default:
        /* The default case is to just add on a character to the token*/
        bSize++;
        buffer = reallocBuffer(buffer, bSize);
        buffer[bSize-1] = currentChar;
        break;
    }
  } while (1);


  /*Creates new .c file name based on inputFile*/
  OFSSize = strlen(argv[1]);
  OFSSize = OFSSize-1;
  theToken = strtok(argv[1], ".");
  outputFileString = reallocBuffer(outputFileString, OFSSize);
  strcpy(outputFileString, theToken);
  strcat(outputFileString, ".c");

  /*Calls the main converter*/
  Convert(parsedStorage, parsedSize, outputFileString);

  /*Various Frees*/
  for(i=0; i<parsedSize; i++) {
    free(parsedStorage[i]);
  }
  free(outputFileString);
  free(parsedStorage);
  free(buffer);
  fclose(inputFile);
  return 0;
}
