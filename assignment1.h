#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*Struct that stores parts of a function within a class,
  it is found within a Class struct*/
struct Function {
  char oldname[50];
  char newname[50];
  int varconflict[50];
  int numconflicts;
  int numarguments;
};

/*Struct that stores parts of a class, in this program it is used as
  a linked list*/
struct Class {
  char classname[75];
  struct Function Functions[75];
  int numfuncs;
  int numvars;
  char vartypes[50][30];
  char variables[50][50];
  bool variablesExist;
  struct Class * next;
};

/* A struct that is used to associate variable names with the struct type
  they are*/
struct Association {
  char varname[75];
  char classname[75];
};

/*************************************************************************
Name: addBufferStringYesChar
Purpose: Used in the initial parsing of information from a file.
         Adds a buffer string to the double pointer string array storing each token
         from the file in it's own string. The entire parsedStorage array is dynamically
         allocated so it is necessary
**************************************************************************/
char ** addBufferStringYesChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize, char toAdd);
/*************************************************************************
Name: addBufferStringNoChar
Purpose: Used for the same purpose as above function, except it does not append
         a character to the end before appending buffer to double string array
**************************************************************************/
char ** addBufferStringNoChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize);
/*************************************************************************
Name: mallocBuffer
Purpose: allocates a string *, while initializing the string with "\0"
**************************************************************************/
char * mallocBuffer(int bSize);
/*************************************************************************
Name: reallocBuffer
Purpose: Used to realloc a string *, and sets final char to '\0'
**************************************************************************/
char * reallocBuffer(char * buffer, int bSize);
/*************************************************************************
Name: Convert
Purpose: Takes the parsed information from the file and decides what needs
         to be done with it to convert to a c file
**************************************************************************/
bool Convert(char ** parsedStorage, int parsedSize, char * outputFileString);
/*************************************************************************
Name: addInclude
Purpose: When an include statement is found, it is added to the globalString
**************************************************************************/
char * addInclude(char * string, char ** parsedStorage, int * programSize, int * outerCount);
/*************************************************************************
Name: extendString
Purpose: extends a string *, reallocing the string within the function. Essentially a helper
         function
**************************************************************************/
char * extendString(char * finalProgram, char * string, int * programSize);
/*************************************************************************
Name: classCheck
Purpose: Checks if the next line is a class
**************************************************************************/
bool classCheck(char ** parsed, int position, int parsedSize);
/*************************************************************************
Name: convClassToStruct
Purpose: Converts a class header to a struct header
**************************************************************************/
char * convClassToStruct(char * finalProgram, char ** parsedStorage, int * outerCount, int * programSize);
/*************************************************************************
Name: VarChecker
Purpose: Checks if the parsed token is a type of variable
**************************************************************************/
bool VarChecker(char * string);
/*************************************************************************
Name: StructChecker
Purpose: Checks if the parsed token is a struct
**************************************************************************/
bool StructChecker(char * string);
/*************************************************************************
Name: VariableOrFunctionChecker
Purpose: Checks if the parsed line is a variable declaration, or if it is the
         beginning of a function
**************************************************************************/
bool VariableOrFunctionChecker(char ** parsedStorage, int outerCount);
/*************************************************************************
Name: addVariableToProgram
Purpose: If it is a variable, adds the variable to the string
**************************************************************************/
char * addVariableToProgram(char * finalProgram, char ** parsedStorage, int * outerCount, int * programSize, bool inClass);
/*************************************************************************
Name: reallocStringArray
Purpose: Helper function to allocate and initialize a char ** array
**************************************************************************/
char ** reallocStringArray(char ** classesArray, int size);
/*************************************************************************
Name: mallocIntArray
Purpose: Helper function to allocate enough space for a double integer array
**************************************************************************/
int ** mallocIntArray(int size);
/*************************************************************************
Name: reallocIntArray
Purpose: helper function to allocate enough space for a double integer array
**************************************************************************/
int ** reallocIntArray(int ** array, int oldSize, int newSize);
/*************************************************************************
Name: CopyFunctionFromClass
Purpose: One of the larger functions. If a function within a class is found, the
         parsed tokens are sent here to be copied to a temporary function holder
         that will be printed at the end of the class
**************************************************************************/
char * CopyFunctionFromClass(char * tempFunctionHolder, char ** parsedStorage, int * outerCount, int * TFHSize, char * classNameBuffer, char * classFunctionVars, struct Class * Classes);
/*************************************************************************
Name: WipeBuffer
Purpose: Reinitializes a string *
**************************************************************************/
char * WipeBuffer(char * buffer, int * size);
/*************************************************************************
Name: FindClassName
Purpose: Finds the class name when a class is found in the parsed information
**************************************************************************/
char * FindClassName(char * nameBuffer, char ** parsedStorage, int oCount, int * nameBufferSize);
/*************************************************************************
Name: CopyGlobalFunction
Purpose: If a global function is found, the parsed information is sent here to be added
         onto the end of the globalString
**************************************************************************/
char * CopyGlobalFunction(char * globalString, char ** parsedStorage, int * outerCount, int * globalSize, struct Class * Classes, bool structCheck, char ** constructorNames, int NumConNames, struct Association * globalAssociated, int gAssSize, bool withinMain);
/*************************************************************************
Name: TypeOfVariable
Purpose: returns the type of variable
**************************************************************************/
char TypeOfVariable(char * string);
/*************************************************************************
Name: copyFunctionPointer
Purpose: copies the function pointer to the class string when a function is found within
         a class
**************************************************************************/
char * copyFunctionPointer(char * classString, int * classSize, char ** parsedStorage, int oCount, char * classNameBuffer, char * classFunctionVars, int * oCountStored, char funcArgs[][30], int funcArgsSize, struct Class * Classes);
/*************************************************************************
Name: CopyDoubleSlashComments
Purpose: Copies entire comments to the string, as they should pass through
         the program untouched. For double slash type comments
**************************************************************************/
char * CopyDoubleSlashComments(char * string, int * stringSize, char ** parsedStorage, int * outerCount);
/*************************************************************************
Name: CopySlashStarComments
Purpose: Copies entire comments to the string, as they should pass through
         the program untouched. For slash star type comments
**************************************************************************/
char * CopySlashStarComments(char * string, int * stringSize, char ** parsedStorage, int * outerCount);
/*************************************************************************
Name: AddConstructor
Purpose: The main function in creating the constructor function
**************************************************************************/
char * AddConstructor(char * constructorString, char * className, int oCountStored, char ** parsedStorage, int * constructorSize);
/*************************************************************************
Name: CreateConstructorLine
Purpose: When a class variable is called in a function, this prints the call
         to the constructor
**************************************************************************/
char * CreateConstructorLine(char * constructorLine, int * constructSize, char * tClassName, int * tClassNameSize, char * tCVarName, int * tCVarSize);
/*************************************************************************
Name: MallocAndInitializeClass
Purpose: This function allocates enough space for a struct Class, as well as
         initializes everything inside it
**************************************************************************/
struct Class * MallocAndInitializeClass(struct Class * Classes);
/*************************************************************************
Name: FreeClassStruct
Purpose: Properly frees a struct Class
**************************************************************************/
void FreeClassStruct(struct Class * Classes);
/*************************************************************************
Name: PrintClass
Purpose: A function used for printing the contents of the class information
         array. Mainly used for debugging
**************************************************************************/
void PrintClass(struct Class * Classes);
/*************************************************************************
Name: InitializeAssociation
Purpose: Initializes a struct Association
**************************************************************************/
struct Association InitializeAssociation(struct Association associated);
/*************************************************************************
Name: AddClassVarToStruct
Purpose: Adds a class variable to the Class struct
**************************************************************************/
char * AddClassVarToStruct(struct Class * Classes, char ** parsedStorage, int oCount, char * addCVarDef2Constr, int * addCVarDefSize);
/*************************************************************************
Name: FindVariableConflicts
Purpose: If there is a variable with the same name as a class variable in the
         function within a class, or if an argument is the same, then conflicts
         are marked
**************************************************************************/
void FindVariableConflicts(struct Class * Classes, char ** parsedStorage, int oCount);
/*************************************************************************
Name: FindParameters
Purpose: If there are parameters in the function true is returned, if not,
         false is returned
**************************************************************************/
bool FindParameters(char ** parsedStorage, int oCount);
/*************************************************************************
Name: FindTypeOfStruct
Purpose: If in a function a struct variable is sent, false is returned
         If a struct function call is sent, true is returned
**************************************************************************/
bool FindTypeOfStruct(char ** parsedStorage, int oCount);
/*************************************************************************
Name: CreateVariableOnlyConstructor
Purpose: If no functions are present in the class and a constructor has not
         been initialized, but there are class variables that need to be set,
         this will create a constructor for them
**************************************************************************/
char * CreateVariableOnlyConstructor(char * tempConstructorHolder, char * addCVarDef2Constr, int * constructorSize, struct Class * Classes);
/*************************************************************************
Name: CheckForStars
Purpose: checks for pointer stars, returns true if found
**************************************************************************/
bool CheckForStars(char ** parsedStorage, int oCount);
/*************************************************************************
Name: CheckForSqareBracket
Purpose: Checks for a square bracket, returns true if found.
**************************************************************************/
bool CheckForSqareBracket(char ** parsedStorage, int oCount);
/*************************************************************************
Name: CheckIfConstructorExists
Purpose: Checks if the constructor exists for a specific class type
         If none exists, then the call to the constructor will not be created
**************************************************************************/
bool CheckIfConstructorExists(char ** constructorNames, int NumConNames, char * tClassName);
/*************************************************************************
Name: findNumArguments
Purpose: returns the number of arguments between brackets
**************************************************************************/
int findNumArguments(char ** parsedStorage, int oCount);
