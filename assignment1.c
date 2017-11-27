#include "assignment1.h"

/*************************************************************************
Name: Convert
Purpose: Takes the parsed information from the file and decides what needs to
         be done with it to convert to a c file. Essentially it is a switch
         statement that splits up into multiple if statements in order
         to find out what part of the .cc file you are in
**************************************************************************/
bool Convert(char ** parsedStorage, int parsedSize, char * outputFileString) {
  int innerCount, outerCount, globalSize, bracketCount, nomClasses, i, noClassSizes, funcArgsSize, TFHSize, nameBufferSize, findMain, mainSize, findVars, classFuncVarSize, CNASize, oCountStored, constructorSize, finalProgramSize, addCVarDefSize, NumConNames, tcSize, gAssSize, couterCount;
  int ** classSizes;
  char ** classesArray, ** classNameArray, **constructorNames;
  char funcArgs[30][30];
  char varChar;
  char tClassName[30], tClassVar[30];
  bool varChk, varChk2, clasChk, foundMain, haveFoundMain, structCheck, inClass, starCheck, squareBracketCheck, tnamefound, tvarfound, withinMain;
  struct Class * Classes, *current;
  char * globalString, * tempFunctionHolder, * classNameBuffer, * mainBuffer, * classFunctionVars, * tempConstructorHolder, * finalProgram, * addCVarDef2Constr;
  struct Association globalAssociated[50];
  FILE * outputFile;

  innerCount = outerCount = globalSize = classFuncVarSize = bracketCount = nomClasses = funcArgsSize = TFHSize = nameBufferSize = findMain = mainSize = findVars = CNASize = oCountStored = constructorSize = finalProgramSize = addCVarDefSize = NumConNames = tcSize = gAssSize = couterCount = 0;
  noClassSizes = 100;
  varChk = varChk2 = clasChk = foundMain = haveFoundMain = structCheck = inClass = starCheck = squareBracketCheck = tnamefound = tvarfound = withinMain = false;
  varChar = '\0';
  strcpy(tClassName, "\0");
  strcpy(tClassVar, "\0");

  outputFile = fopen(outputFileString, "w");

  classSizes = mallocIntArray(noClassSizes);
  /*The function mallocBuffer is essentially a malloc function that also initializes
    the string with "\0"*/
  addCVarDef2Constr = mallocBuffer(0);
  finalProgram = mallocBuffer(0);
  mainBuffer = mallocBuffer(0);
  classesArray = malloc(sizeof(char*));
  constructorNames = malloc(sizeof(char*));
  classNameArray = malloc(sizeof(char*));
  tempConstructorHolder = mallocBuffer(0);
  globalString = mallocBuffer(0);
  tempFunctionHolder = mallocBuffer(0);
  classNameBuffer = mallocBuffer(0);
  classFunctionVars = mallocBuffer(0);
  Classes = NULL;
  Classes = MallocAndInitializeClass(Classes);
  /*The Association structs will be used later to associate variables with the structs
    they represent, there are two of these structs, one for global and one for within functions*/
  for (i=0; i<50; i++) {
    globalAssociated[i] = InitializeAssociation(globalAssociated[i]);
  }
  do {
    /* If there is over 100 classes the integer array holding their sizes will have to be realloced*/
    if (nomClasses == noClassSizes-1) {
      noClassSizes = noClassSizes+100;
      classSizes = reallocIntArray(classSizes, noClassSizes-100, noClassSizes);
    }
    switch (parsedStorage[outerCount][innerCount]) {
      case '#':
        globalString = addInclude(globalString, parsedStorage, &globalSize, &outerCount);
        break;
      case '}':
      /* Case for when a class ends*/
        if (nomClasses > 0) {
          classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], parsedStorage[outerCount], classSizes[nomClasses-1]);
        }
        if (TFHSize != 0) {
          /*Adds functions from class onto end of class string from the temporary function buffer*/
          classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], ";\n\n", classSizes[nomClasses-1]);
          classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], tempFunctionHolder, classSizes[nomClasses-1]);
          classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], "\n", classSizes[nomClasses-1]);
          if (constructorSize != 0) {
            if (addCVarDefSize != 0) {
              tempConstructorHolder = extendString(tempConstructorHolder, addCVarDef2Constr, &constructorSize);
              tempConstructorHolder = extendString(tempConstructorHolder, "\n", &constructorSize);
            }
            tempConstructorHolder = extendString(tempConstructorHolder, "}\n", &constructorSize);
            classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], tempConstructorHolder, classSizes[nomClasses-1]);
          }
        } else {
          /*if variable constructor needs to be set but there are no functions*/
          if ((constructorSize == 0) && (addCVarDefSize != 0)) {
            tempConstructorHolder = CreateVariableOnlyConstructor(tempConstructorHolder, addCVarDef2Constr, &constructorSize, Classes);
            classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], ";\n\n", classSizes[nomClasses-1]);
            classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], tempConstructorHolder, classSizes[nomClasses-1]);
          } else {
            classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], ";\n\n", classSizes[nomClasses-1]);
          }
        }
        /*if a constructor exists it is added to an array*/
        if (strcmp(tempConstructorHolder, "\0") != 0) {
          current = Classes;
          if (current != NULL) {
            while(current->next != NULL) {
              current = current->next;
            }
            NumConNames++;
            constructorNames = reallocStringArray(constructorNames, NumConNames);
            tcSize = strlen("constructor ");
            tcSize = tcSize+strlen(current->classname);
            constructorNames[NumConNames-1] = mallocBuffer(tcSize);
            strcpy(constructorNames[NumConNames-1], "constructor");
            strcat(constructorNames[NumConNames-1], current->classname);
          }
        }
        /*Wipes name buffers*/
        classNameBuffer = WipeBuffer(classNameBuffer, &nameBufferSize);
        addCVarDef2Constr = WipeBuffer(addCVarDef2Constr, &addCVarDefSize);
        tempConstructorHolder = WipeBuffer(tempConstructorHolder, &constructorSize);
        tempFunctionHolder = WipeBuffer(tempFunctionHolder, &TFHSize);
        bracketCount--;
        while((strcmp(parsedStorage[outerCount], ";") != 0) && (outerCount < parsedSize)) {
          outerCount++;
        }
        break;
      case '/':
        if ((strcmp(parsedStorage[outerCount], "/") == 0) && (strcmp(parsedStorage[outerCount+1], "/") == 0)) {
          if (bracketCount == 0) {
            /*copies double slash comments to global string*/
            globalString = CopyDoubleSlashComments(globalString, &globalSize, parsedStorage, &outerCount);
            break;
          } else {
            /*copies double slash comments to current class string*/
            classesArray[nomClasses-1] = CopyDoubleSlashComments(classesArray[nomClasses-1], classSizes[nomClasses-1], parsedStorage, &outerCount);
            break;
          }
        } else if ((strcmp(parsedStorage[outerCount], "/") == 0) && (strcmp(parsedStorage[outerCount+1], "*") == 0)) {
            if (bracketCount == 0) {
              globalString = CopySlashStarComments(globalString, &globalSize, parsedStorage, &outerCount);
              break;
            } else {
              /*copies star slash comments to current class string*/
              if (nomClasses > 0) {
                classesArray[nomClasses-1] = CopySlashStarComments(classesArray[nomClasses-1], classSizes[nomClasses-1], parsedStorage, &outerCount);
              }
              break;
            }
        } else {
          break;
        }
      case 'c':
        clasChk = classCheck(parsedStorage, outerCount, parsedSize);
        if (clasChk == true) {
          bracketCount++;
          nomClasses++;
          CNASize++;
          /*Finds class name for later use on functions within class*/
          classNameBuffer = FindClassName(classNameBuffer, parsedStorage, outerCount, &nameBufferSize);

          if (nomClasses == 1) {
            current = Classes;
            strcpy(Classes->classname, classNameBuffer);
          } else {
            current = Classes;
            while (current->next != NULL) {
              current = current->next;
            }
            /*Sets up new class in the linked list*/
            current->next = MallocAndInitializeClass(current->next);
            current = current->next;
            strcpy(current->classname, classNameBuffer);
          }

          classNameArray = reallocStringArray(classNameArray, CNASize);
          classNameArray[CNASize-1] = mallocBuffer(strlen(classNameBuffer));
          strcpy(classNameArray[CNASize-1], classNameBuffer);

          classesArray = reallocStringArray(classesArray, nomClasses);
          classesArray[nomClasses-1] = mallocBuffer(0);
          classesArray[nomClasses-1] = convClassToStruct(classesArray[nomClasses-1], parsedStorage, &outerCount, classSizes[nomClasses-1]);
          clasChk = false;
          break;
        } else {
          /*Similar to default case, but wich 'c' as the first letter*/
            if (bracketCount == 0) {
              /*checks for variable using initial checks*/
              varChk = VarChecker(parsedStorage[outerCount]);
              if (varChk == true) {
                /*secondary variable checks to see if it is a function or a variable*/
                varChk = VariableOrFunctionChecker(parsedStorage, outerCount);
                if (varChk == true) {
                  if (strcmp(parsedStorage[outerCount], "class") == 0) {
                    couterCount = outerCount;
                    couterCount++;
                    while (strcmp(parsedStorage[couterCount], ";") != 0) {
                      if ((strcmp(parsedStorage[couterCount], " ") != 0) && (strcmp(parsedStorage[couterCount], "*") != 0) && (tnamefound == false) && (strcmp(parsedStorage[couterCount], "\f") != 0) && (strcmp(parsedStorage[couterCount], "\v") != 0) && (strcmp(parsedStorage[couterCount], "\r") != 0) && (strcmp(parsedStorage[couterCount], "\t") != 0) && (strcmp(parsedStorage[couterCount], "\n") != 0)) {
                        strcpy(tClassName, parsedStorage[couterCount]);
                        tnamefound = true;
                      } else if ((strcmp(parsedStorage[couterCount], " ") != 0) && (strcmp(parsedStorage[couterCount], "*") != 0) && (tnamefound == true) && (tvarfound == false) && (strcmp(parsedStorage[couterCount], ",") != 0) && (strcmp(parsedStorage[couterCount], "\f") != 0) && (strcmp(parsedStorage[couterCount], "\v") != 0) && (strcmp(parsedStorage[couterCount], "\r") != 0) && (strcmp(parsedStorage[couterCount], "\t") != 0) && (strcmp(parsedStorage[couterCount], "\n") != 0)) {
                        strcpy(tClassVar, parsedStorage[couterCount]);
                        tvarfound = true;
                      }
                      if ((tnamefound == true) && (tvarfound == true)) {
                        strcpy(globalAssociated[gAssSize].varname, tClassVar);
                        strcpy(globalAssociated[gAssSize].classname, tClassName);
                        gAssSize++;
                      }
                      couterCount++;
                    }
                    /*adds a global variable to the globalString*/
                    globalString = addVariableToProgram(globalString, parsedStorage, &outerCount, &globalSize, inClass);
                    varChk = false;

                    tnamefound = tvarfound = false;
                    strcpy(tClassName, "\0");
                    strcpy(tClassVar, "\0");
                    break;
                  } else {
                    /*adds a global variable to the globalString*/
                    globalString = addVariableToProgram(globalString, parsedStorage, &outerCount, &globalSize, inClass);
                    varChk = false;
                    break;
                  }
                } else {
                  /*this is the else statement for it is a global function*/
                  /*printf("\nglobal function to be added\n");*/
                  globalString = CopyGlobalFunction(globalString, parsedStorage, &outerCount, &globalSize, Classes, structCheck, constructorNames, NumConNames, globalAssociated, gAssSize, withinMain);
                  break;
                }
              } else {
                /*This is the else statement for it not being a variable and can just be added to the global string*/
                globalString = extendString(globalString, parsedStorage[outerCount], &globalSize);
                break;
              }
            } else {
                /*This is the else statement for if it is not global*/
                /*checks for variable using initial checks*/
                varChk = VarChecker(parsedStorage[outerCount]);
                if (varChk == true) {
                  /*secondary variable checks*/
                  varChk = VariableOrFunctionChecker(parsedStorage, outerCount);
                  if (varChk == true) {
                      /*Saving class variables to class struct*/
                        current = Classes;
                        while(current->next != NULL) {
                          current = current->next;
                        }
                        current->variablesExist = true;
                        addCVarDef2Constr = AddClassVarToStruct(Classes, parsedStorage, outerCount, addCVarDef2Constr, &addCVarDefSize);
                      /*adds a class variable to the current class string*/
                      inClass = true;
                      classesArray[nomClasses-1] = addVariableToProgram(classesArray[nomClasses-1], parsedStorage, &outerCount, classSizes[nomClasses-1], inClass);
                      inClass = false;
                      varChk = false;
                      break;
                  } else {
                    /*this is the else statement for it is a class function*/
                    findVars = outerCount+1;
                    /*This while loop finds the variable arguments of a function and adds them to a buffer*/
                    /*string for storing function arguments*/
                    for (i=0; i<30; i++) {
                      strcpy(funcArgs[i], "\0");
                    }
                    funcArgsSize = 0;

                    current = Classes;
                    while (current->next != NULL) {
                      current = current->next;
                    }
                    /* This block does a few things, including checking for pointers, [] arrays,
                       and finding the variable types being used as arguments */
                    while (strcmp(parsedStorage[findVars], ")") != 0) {
                      varChk2 = VarChecker(parsedStorage[findVars]);
                      if (varChk2 == true) {
                        current->Functions[current->numfuncs].numarguments++;
                        varChar = TypeOfVariable(parsedStorage[findVars]);
                        strcpy(funcArgs[funcArgsSize], parsedStorage[findVars]);
                        starCheck = CheckForStars(parsedStorage, findVars);
                        squareBracketCheck = CheckForSqareBracket(parsedStorage, findVars);
                        if (squareBracketCheck == true) {
                          strcat(funcArgs[funcArgsSize], "[]");
                          squareBracketCheck = false;
                        }
                        if (starCheck == true) {
                          strcat(funcArgs[funcArgsSize], "*");
                          starCheck = false;
                        }
                        funcArgsSize++;
                        classFuncVarSize = classFuncVarSize+1;
                        classFunctionVars = reallocBuffer(classFunctionVars, classFuncVarSize);
                        classFunctionVars[classFuncVarSize-1] = varChar;
                      }
                      findVars++;
                    }
                    /*copies the function pointer in place of he function, and then adds the function to a temporary buffer
                      also adds constructor line*/
                    classesArray[nomClasses-1] = copyFunctionPointer(classesArray[nomClasses-1], classSizes[nomClasses-1], parsedStorage, outerCount, classNameBuffer, classFunctionVars, &oCountStored, funcArgs, funcArgsSize, Classes);
                    tempFunctionHolder = CopyFunctionFromClass(tempFunctionHolder, parsedStorage, &outerCount, &TFHSize, classNameBuffer, classFunctionVars, Classes);

                    current->numfuncs++;
                    tempConstructorHolder = AddConstructor(tempConstructorHolder, classNameBuffer, oCountStored, parsedStorage, &constructorSize);
                    classFunctionVars = WipeBuffer(classFunctionVars, &classFuncVarSize);
                    /*classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], parsedStorage[outerCount], classSizes[nomClasses-1]);*/
                    break;
                  }
                } else {
                /*This is the else statement for if it is not global and can just be added to che class string*/
                if (nomClasses > 0) {
                  classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], parsedStorage[outerCount], classSizes[nomClasses-1]);
                }
              break;
            }
          }
        }
      default:
        if (bracketCount == 0) {
          /*checks for variable using initial checks*/
          varChk = VarChecker(parsedStorage[outerCount]);
          if (varChk == true) {
            /*secondary variable checks*/
            varChk = VariableOrFunctionChecker(parsedStorage, outerCount);
            if (varChk == true) {
              /*adds a global variable to the globalString*/
              globalString = addVariableToProgram(globalString, parsedStorage, &outerCount, &globalSize, inClass);
              varChk = false;
              break;
            } else {
              /*this is the else statement for it is a global function*/
              /*printf("\nglobal function to be added\n");*/
              findMain = outerCount;
              while (strcmp(parsedStorage[findMain], "(") != 0) {
                /* Checks to see if the global function is the main*/
                if (strcmp(parsedStorage[findMain], "main") == 0) {
                  withinMain = true;
                  mainBuffer = CopyGlobalFunction(mainBuffer, parsedStorage, &outerCount, &mainSize, Classes, structCheck, constructorNames, NumConNames, globalAssociated, gAssSize, withinMain);
                  findMain = 0;
                  foundMain = true;
                  haveFoundMain = true;
                  withinMain = false;
                  break;
                }
                findMain++;
              }
              structCheck = StructChecker(parsedStorage[outerCount]);
              if (foundMain == false) {
                globalString = CopyGlobalFunction(globalString, parsedStorage, &outerCount, &globalSize, Classes, structCheck, constructorNames, NumConNames, globalAssociated, gAssSize, withinMain);
              }
              if (structCheck == true) {
                structCheck = false;
              }
              break;
            }
          } else {
            /*This is the else statement for it not being a variable and can just be added to the global string*/
            globalString = extendString(globalString, parsedStorage[outerCount], &globalSize);
            break;
          }
        } else {
          /*This is the else statement for if it is not global*/
          /*checks for variable using initial checks*/
          varChk = VarChecker(parsedStorage[outerCount]);
          if (varChk == true) {
            /*secondary variable checks*/
            varChk = VariableOrFunctionChecker(parsedStorage, outerCount);
            if (varChk == true) {
              /*Saving class variables to class struct*/
                current = Classes;
                while(current->next != NULL) {
                  current = current->next;
                }
                current->variablesExist = true;
                addCVarDef2Constr = AddClassVarToStruct(Classes, parsedStorage, outerCount, addCVarDef2Constr, &addCVarDefSize);
                /*adds a class variable to the current class string*/
                inClass = true;
                classesArray[nomClasses-1] = addVariableToProgram(classesArray[nomClasses-1], parsedStorage, &outerCount, classSizes[nomClasses-1], inClass);
                inClass = false;
                varChk = false;
                break;
            } else {
              /*this is the else statement for it is a class function*/
              findVars = outerCount+1;
              /*string for storing function arguments*/
              for (i=0; i<30; i++) {
                strcpy(funcArgs[i], "\0");
              }
              funcArgsSize = 0;

              current = Classes;
              while (current->next != NULL) {
                current = current->next;
              }

              /*This while loop finds the variable arguments of a function and adds them to a buffer*/
              while (strcmp(parsedStorage[findVars], ")") != 0) {
                varChk2 = VarChecker(parsedStorage[findVars]);
                if (varChk2 == true) {
                  current->Functions[current->numfuncs].numarguments++;
                  varChar = TypeOfVariable(parsedStorage[findVars]);
                  strcpy(funcArgs[funcArgsSize], parsedStorage[findVars]);
                  starCheck = CheckForStars(parsedStorage, findVars);
                  squareBracketCheck = CheckForSqareBracket(parsedStorage, findVars);
                  if (squareBracketCheck == true) {
                    strcat(funcArgs[funcArgsSize], "[]");
                    squareBracketCheck = false;
                  }
                  if (starCheck == true) {
                    strcat(funcArgs[funcArgsSize], "*");
                    starCheck = false;
                  }
                  funcArgsSize++;
                  classFuncVarSize = classFuncVarSize+1;
                  classFunctionVars = reallocBuffer(classFunctionVars, classFuncVarSize);
                  classFunctionVars[classFuncVarSize-1] = varChar;
                }
                findVars++;
              }

              classesArray[nomClasses-1] = copyFunctionPointer(classesArray[nomClasses-1], classSizes[nomClasses-1], parsedStorage, outerCount, classNameBuffer, classFunctionVars, &oCountStored, funcArgs, funcArgsSize, Classes);
              tempFunctionHolder = CopyFunctionFromClass(tempFunctionHolder, parsedStorage, &outerCount, &TFHSize, classNameBuffer, classFunctionVars, Classes);

              current->numfuncs++;
              tempConstructorHolder = AddConstructor(tempConstructorHolder, classNameBuffer, oCountStored, parsedStorage, &constructorSize);
              classFunctionVars = WipeBuffer(classFunctionVars, &classFuncVarSize);

              break;
            }
          } else {
            /*This is the else statement for it not being a variable and can just be added to the class string*/
            if (nomClasses > 0) {
              classesArray[nomClasses-1] = extendString(classesArray[nomClasses-1], parsedStorage[outerCount], classSizes[nomClasses-1]);
            }
            break;
          }
        }
      }
    foundMain = false;
    outerCount++;
  } while(outerCount < parsedSize);


  /*The next section first copies the global variables and functions to the finalString,
    then the class strings one by one, then the main string*/
  finalProgramSize = globalSize+mainSize;
  for (i=0; i<nomClasses; i++) {
    finalProgramSize = finalProgramSize+ *classSizes[i];
  }
  finalProgramSize = finalProgramSize+100;
  finalProgram = reallocBuffer(finalProgram, finalProgramSize);

  finalProgram = strcpy(finalProgram, globalString);
  finalProgram = strcat(finalProgram, "\n");
  for (i=0; i<nomClasses; i++) {
    finalProgram = strcat(finalProgram, classesArray[i]);
    finalProgram = strcat(finalProgram, "\n");
  }
  if (haveFoundMain == true) {
    finalProgram = strcat(finalProgram, mainBuffer);
  }
  finalProgram = strcat(finalProgram, "\n");

  /*Finally outputs the .c program to the outputfile*/
  fprintf(outputFile, "%s\n", finalProgram);

  /*Various frees*/
  FreeClassStruct(Classes);
  for (i=0; i<nomClasses; i++) {
    free(classesArray[i]);
  }
  for (i=0; i<CNASize; i++) {
    free(classNameArray[i]);
  }
  for (i=0; i<NumConNames; i++) {
    free(constructorNames[i]);
  }
  free(constructorNames);
  free(addCVarDef2Constr);
  free(finalProgram);
  free(tempConstructorHolder);
  free(classNameArray);
  free(classFunctionVars);
  free(mainBuffer);
  free(classNameBuffer);
  free(tempFunctionHolder);
  free(classesArray);
  free(globalString);
  for (i=0; i<noClassSizes; i++) {
    free(classSizes[i]);
  }
  free(classSizes);
  fclose(outputFile);
  return true;
}

/*************************************************************************
Name: CopyFunctionFromClass
Purpose: One of the larger functions. If a function within a class is found, the
         parsed tokens are sent here to be copied to a temporary function holder
         that will be printed at the end of the class
**************************************************************************/
char * CopyFunctionFromClass(char * tempFunctionHolder, char ** parsedStorage, int * outerCount, int * TFHSize, char * classNameBuffer, char * classFunctionVars, struct Class * Classes) {
  int oCount, toAdd, pSize, bufferSize, toAdd2, oCountCopy, i, methodNum;
  bool nameFound, varChk, varChk2, moreThanOne, timeToAppend, timeToLoop, doneConflicts, returnCheck, methodCheck, parameterBoolean;
  char * nameBuffer;
  struct Class * current;
  oCount = *outerCount;
  pSize = *TFHSize;
  nameFound = varChk = varChk2 = moreThanOne = timeToAppend = timeToLoop = doneConflicts = returnCheck = methodCheck = parameterBoolean = false;
  methodNum = 0;

  bufferSize = 0;
  nameBuffer = mallocBuffer(0);
  current = Classes;
  while (current->next != NULL) {
    current = current->next;
  }
  while (strcmp(parsedStorage[oCount], "}") != 0) {
    /*gets rid of comments if they exist*/
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount+1], "/") == 0)) {
      tempFunctionHolder = CopyDoubleSlashComments(tempFunctionHolder, &pSize, parsedStorage, &oCount);
    }
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount+1], "*") == 0)) {
      tempFunctionHolder = CopySlashStarComments(tempFunctionHolder, &pSize, parsedStorage, &oCount);
    }
    /* Finds function name and appends classname to it*/
    varChk = VarChecker(parsedStorage[oCount]);
    if ((strcmp(parsedStorage[oCount], "(") != 0) && (nameFound == false) && (varChk == false)) {
      if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "*") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0) &&(strcmp(parsedStorage[oCount], "\n") != 0)) {
        /*Store old function Name*/
        strcpy(current->Functions[current->numfuncs].oldname, parsedStorage[oCount]);

        toAdd2 = strlen(classNameBuffer);
        bufferSize = bufferSize+toAdd2;
        nameBuffer = reallocBuffer(nameBuffer, bufferSize);
        strcpy(nameBuffer, classNameBuffer);
        toAdd2 = strlen(parsedStorage[oCount]);
        bufferSize = bufferSize+toAdd2;
        nameBuffer = reallocBuffer(nameBuffer, bufferSize);
        strcat(nameBuffer, parsedStorage[oCount]);
        toAdd2 = strlen(classFunctionVars);
        bufferSize = bufferSize+toAdd2;
        nameBuffer = reallocBuffer(nameBuffer, bufferSize);
        strcat(nameBuffer, classFunctionVars);
        parsedStorage[oCount] = reallocBuffer(parsedStorage[oCount], bufferSize);
        strcpy(parsedStorage[oCount], nameBuffer);
        /*Store new function name*/
        strcpy(current->Functions[current->numfuncs].newname, parsedStorage[oCount]);

        nameBuffer = WipeBuffer(nameBuffer, &bufferSize);
        nameFound = true;
        timeToAppend = true;
      }
    }
    /*Finds any variable conflicts in the function and class*/
    if ((timeToAppend == true) && (doneConflicts == false)) {
      FindVariableConflicts(Classes, parsedStorage, oCount);
      doneConflicts = true;
    }
    /*adds struct parameter to function arguments*/
    if ((strcmp(parsedStorage[oCount], "(") == 0) && (nameFound == true) && (timeToAppend == true)) {
      toAdd = strlen(parsedStorage[oCount]);
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, parsedStorage[oCount]);
      oCount++;
      oCountCopy = oCount;
      /*Finds out if arguments already exist*/
      while(strcmp(parsedStorage[oCountCopy], ")") != 0) {
        varChk2 = VarChecker(parsedStorage[oCountCopy]);
        if (varChk2 == true) moreThanOne = true;
        if (moreThanOne == true) break;
        oCountCopy++;
      }
      /*adds function argument that will be used to connect class variables*/
      toAdd = strlen("struct _ * theStruct");
      toAdd = toAdd+strlen(current->classname);
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder,pSize);
      strcat(tempFunctionHolder, "struct ");
      strcat(tempFunctionHolder, current->classname);
      strcat(tempFunctionHolder, " * theStruct");
      if (moreThanOne == true) {
        pSize=pSize+2;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, ", ");
      }
      timeToAppend = false;
      timeToLoop = true;
    }
    /*sets struct variables to function static ones if there is a return statement, otherwise will be done at end of function*/
    if (strcmp(parsedStorage[oCount], "return") == 0) {
      returnCheck = true;
      for (i=0; i<current->numvars; i++) {
        if (current->Functions[current->numfuncs].varconflict[i] == 0) {
          toAdd = strlen("\n = theStruct->;");
          pSize = pSize+toAdd;
          toAdd = strlen(current->variables[i]);
          toAdd = toAdd*2;
          pSize = pSize+toAdd;
          tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
          strcat(tempFunctionHolder, "\n");
          strcat(tempFunctionHolder, "theStruct->");
          strcat(tempFunctionHolder, current->variables[i]);
          strcat(tempFunctionHolder, " = ");
          strcat(tempFunctionHolder, current->variables[i]);
          strcat(tempFunctionHolder, ";");
        }
      }
      toAdd = strlen("\n");
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, "\n");
    }
    /*adds all variables and sets them equal to the struct variables*/
    if ((strcmp(parsedStorage[oCount], "{") == 0) && (timeToLoop == true)) {
      toAdd = strlen(parsedStorage[oCount]);
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, parsedStorage[oCount]);
      oCount++;
      for (i=0; i<current->numvars; i++) {
        if (current->Functions[current->numfuncs].varconflict[i] == 0) {
          toAdd = strlen(current->vartypes[i]) + strlen(current->variables[i]);
          pSize = pSize+toAdd+3; /*3 for newline and semicolon and space*/
          tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
          strcat(tempFunctionHolder, "\n");
          strcat(tempFunctionHolder, current->vartypes[i]);
          strcat(tempFunctionHolder, " ");
          strcat(tempFunctionHolder, current->variables[i]);
          strcat(tempFunctionHolder, ";");
        }
      }
      /*sets all variables equal to the struct vars*/
      for (i=0; i<current->numvars; i++) {
        if (current->Functions[current->numfuncs].varconflict[i] == 0) {
          toAdd = strlen("\n = theStruct->;");
          pSize = pSize+toAdd;
          toAdd = strlen(current->variables[i]);
          toAdd = toAdd*2;
          pSize = pSize+toAdd;
          tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
          strcat(tempFunctionHolder, "\n");
          strcat(tempFunctionHolder, current->variables[i]);
          strcat(tempFunctionHolder, " = theStruct->");
          strcat(tempFunctionHolder, current->variables[i]);
          strcat(tempFunctionHolder, ";");
        }
      }
      timeToLoop = false;
    }
    /*Finds a method within the same class*/
    for (i=0; i<current->numfuncs; i++) {
      if (strcmp(parsedStorage[oCount], current->Functions[i].oldname) == 0) {
        methodCheck = true;
        methodNum = i;
      }
    }
    /*class variable inside a function inside a class*/
    if (strcmp(parsedStorage[oCount], "class") == 0) {
      toAdd = 6;
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, "struct");
      oCount++;
    } else if (methodCheck == true) {
      /*adds new function name and parameter for method from own class*/
      toAdd = strlen(current->Functions[methodNum].newname);
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, current->Functions[methodNum].newname);
      oCount++;
      while (strcmp(parsedStorage[oCount], "(") != 0) {
        oCount++;
      }
      parameterBoolean = FindParameters(parsedStorage, oCount);
      toAdd = strlen(parsedStorage[oCount]);
      toAdd = toAdd+strlen("theStruct");
      pSize = pSize+toAdd;
      tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
      strcat(tempFunctionHolder, parsedStorage[oCount]);
      strcat(tempFunctionHolder, "theStruct");
      if (parameterBoolean == true) {
        pSize = pSize+2;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, ", ");
        parameterBoolean = false;
      }
      oCount++;
      methodCheck = false;
    } else {
      /* Finds the rest of the function and copies it*/
      if ((strcmp(parsedStorage[oCount], "struct") == 0) && (nameFound == false)) {
        /*Struct returning function within a class*/
        toAdd = strlen(parsedStorage[oCount]);
        pSize = pSize+toAdd;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, parsedStorage[oCount]);
        oCount++;
        while (strcmp(parsedStorage[oCount], " ") == 0) {
          toAdd = strlen(parsedStorage[oCount]);
          pSize = pSize+toAdd;
          tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
          strcat(tempFunctionHolder, parsedStorage[oCount]);
          oCount++;
        }
        toAdd = strlen(parsedStorage[oCount]);
        pSize = pSize+toAdd;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, parsedStorage[oCount]);
        oCount++;
        varChk = false;
      } else {
        toAdd = strlen(parsedStorage[oCount]);
        pSize = pSize+toAdd;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, parsedStorage[oCount]);
        oCount++;
        varChk = false;
      }
    }
  }
  if (returnCheck == false) {
    for (i=0; i<current->numvars; i++) {
      if (current->Functions[current->numfuncs].varconflict[i] == 0) {
        toAdd = strlen("\n = theStruct->;");
        pSize = pSize+toAdd;
        toAdd = strlen(current->variables[i]);
        toAdd = toAdd*2;
        pSize = pSize+toAdd;
        tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
        strcat(tempFunctionHolder, "\n");
        strcat(tempFunctionHolder, "theStruct->");
        strcat(tempFunctionHolder, current->variables[i]);
        strcat(tempFunctionHolder, " = ");
        strcat(tempFunctionHolder, current->variables[i]);
        strcat(tempFunctionHolder, ";");
      }
    }
  }
  toAdd = 4;
  pSize = pSize+toAdd;
  tempFunctionHolder = reallocBuffer(tempFunctionHolder, pSize);
  strcat(tempFunctionHolder, "\n}\n\n");

  *outerCount = oCount;
  *TFHSize = pSize;
  free(nameBuffer);
  return tempFunctionHolder;
}

/*************************************************************************
Name: copyFunctionPointer
Purpose: Copies the function pointer to the class string when a function is found within
         a class.
**************************************************************************/
char * copyFunctionPointer(char * classString, int * classSize, char ** parsedStorage, int oCount, char * classNameBuffer, char * classFunctionVars, int * oCountStored, char funcArgs[][30], int funcArgsSize, struct Class * Classes) {
  bool nameChk, varChk;
  char * pointerBuffer;
  struct Class * current;
  int pBufferSize, toAdd, cSize, i, findStar, numStars;

  cSize = *classSize;
  toAdd = numStars = 0;
  pBufferSize = 3;
  nameChk = varChk = false;
  pointerBuffer = mallocBuffer(3);

  current = Classes;
  while (current->next != NULL) {
    current = current->next;
  }
  /*Specific case if the type returned is 'struct'*/
  if (strcmp(parsedStorage[oCount], "struct") == 0) {
    toAdd = strlen(parsedStorage[oCount]);
    pBufferSize = pBufferSize+toAdd;
    pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
    strcpy(pointerBuffer, parsedStorage[oCount]);
    oCount++;
    while (strcmp(parsedStorage[oCount], " ") == 0) {
      toAdd = strlen(parsedStorage[oCount]);
      pBufferSize = pBufferSize+toAdd;
      pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
      strcat(pointerBuffer, parsedStorage[oCount]);
      oCount++;
    }
    toAdd = strlen(parsedStorage[oCount]);
    pBufferSize = pBufferSize+toAdd+1;
    pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
    strcat(pointerBuffer, parsedStorage[oCount]);
    strcat(pointerBuffer, " ");
    oCount++;
  } else {
    toAdd = strlen(parsedStorage[oCount]);
    pBufferSize = pBufferSize+toAdd;
    pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
    strcpy(pointerBuffer, parsedStorage[oCount]);
    strcat(pointerBuffer, " ");
  }
  findStar = oCount;
  /*Looks for pointer stars*/
  while (strcmp(parsedStorage[findStar], "(") != 0) {
    if (strcmp(parsedStorage[findStar], "*") == 0) {
      numStars++;
    }
    findStar++;
  }
  /*Appends the necessary amount of pointer stars*/
  for (i=0; i<numStars; i++) {
    toAdd = 1;
    pBufferSize=pBufferSize+toAdd;
    pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
    strcat(pointerBuffer, "*");
  }
  strcat(pointerBuffer, "(*");
  toAdd = strlen(classNameBuffer);
  pBufferSize = pBufferSize+toAdd;

  toAdd = strlen(classFunctionVars);
  pBufferSize = pBufferSize+toAdd;

  pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
  strcat(pointerBuffer, classNameBuffer);
  /*Finds the name of the class, and then appends the rest of the pointer*/
  while (nameChk == false) {
    varChk = VarChecker(parsedStorage[oCount]);
    if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "*") != 0) && (varChk == false) && (strcmp(parsedStorage[oCount], "\n") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0)) {
      toAdd = strlen(parsedStorage[oCount]);
      pBufferSize = pBufferSize+toAdd;
      pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
      strcat(pointerBuffer, parsedStorage[oCount]);
      strcat(pointerBuffer, classFunctionVars);
      *oCountStored = oCount;
      nameChk = true;
    }
    oCount++;
  }
  pBufferSize = pBufferSize+2;
  pBufferSize = pBufferSize+strlen("struct *");
  pBufferSize = pBufferSize+strlen(current->classname);
  pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
  strcat(pointerBuffer, ")(struct ");
  strcat(pointerBuffer, current->classname);
  strcat(pointerBuffer, "*");
  if (funcArgsSize != 0) {
    for (i=0; i<funcArgsSize; i++) {
      pBufferSize = pBufferSize+2;
      pBufferSize = pBufferSize+strlen(funcArgs[i]);
      pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
      strcat(pointerBuffer, ", ");
      strcat(pointerBuffer, funcArgs[i]);
    }
  }
  pBufferSize = pBufferSize+4;
  pointerBuffer = reallocBuffer(pointerBuffer, pBufferSize);
  strcat(pointerBuffer, ");\n");
  cSize = cSize+pBufferSize;
  classString = reallocBuffer(classString, cSize);
  strcat(classString, pointerBuffer);

  *classSize = cSize;
  free(pointerBuffer);
  return classString;
}

/*************************************************************************
Name: CopyGlobalFunction
Purpose: If a global function is found, the parsed information is sent here to be added
         onto the end of the globalString
**************************************************************************/
char * CopyGlobalFunction(char * globalString, char ** parsedStorage, int * outerCount, int * globalSize, struct Class * Classes, bool structCheck, char ** constructorNames, int NumConNames, struct Association * globalAssociated, int gAssSize, bool withinMain) {
  int bracketCount, oCount, toAdd, gSize, oCountTemp, tClassNameSize, tCVarSize, cSize, assCount, i, numFArgs, gaClassSize, gaVarSize, oCountTemp3, numCommas, assCountStored;
  bool firstLoop, foundClass, newfunc, addStructAsParam, parameterBoolean, structFuncCall, inClass, constExists, foundFuncStruct, alreadyGlobaled;
  char * tClassName, * tCVarName, * constructorLine;
  char structBuffer[75];
  char foundClassName[75];
  struct Association associated[75];
  struct Class * current;
  firstLoop = foundClass = newfunc = addStructAsParam = parameterBoolean = structFuncCall = inClass = constExists = foundFuncStruct = alreadyGlobaled = false;
  oCount = *outerCount;
  gSize = *globalSize;

  constructorLine = mallocBuffer(0);
  tClassName = mallocBuffer(0);
  tCVarName = mallocBuffer(0);
  bracketCount = oCountTemp = tClassNameSize = tCVarSize = cSize = assCount = numFArgs = gaClassSize = gaVarSize = oCountTemp3 = numCommas = assCountStored = 0;
  strcpy(foundClassName, "\0");
  for (i=0; i<75; i++) {
    associated[i] = InitializeAssociation(associated[i]);
  }
  /*Runs until the function is done, firstLoop allows it to enter the loop before the first { so
    the function name and arguments can be copied*/
  while((bracketCount>0) || (firstLoop == false)) {
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount+1], "/") == 0)) {
      globalString = CopyDoubleSlashComments(globalString, &gSize, parsedStorage, &oCount);
    }
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount+1], "*") == 0)) {
      globalString = CopySlashStarComments(globalString, &gSize, parsedStorage, &oCount);
    }
    if (strcmp(parsedStorage[oCount], "{") == 0) {
      bracketCount++;
      firstLoop = true;
      /*These if statements use a series of boolean logic to go through them one by one*/
    } else if ((firstLoop == true) && (alreadyGlobaled == false) && (withinMain == true)) {
      for (i=0; i<gAssSize; i++) {
        constExists = CheckIfConstructorExists(constructorNames, NumConNames, globalAssociated[i].classname);
        if (constExists == true) {
          gaClassSize = strlen(globalAssociated[i].classname);
          gaVarSize = strlen(globalAssociated[i].varname);
          constructorLine = CreateConstructorLine(constructorLine, &cSize, globalAssociated[i].classname, &gaClassSize, globalAssociated[i].varname, &gaVarSize);
          constExists = false;
        }
        globalString = extendString(globalString, constructorLine, &gSize);
        constructorLine = WipeBuffer(constructorLine, &cSize);
      }
      alreadyGlobaled = true;
    }
    if (strcmp(parsedStorage[oCount], "}") == 0) bracketCount--;
    if (strcmp(parsedStorage[oCount], "class") == 0) {

      oCountTemp = oCount;
      oCountTemp++;
      oCountTemp3 = oCountTemp;
      while (strcmp(parsedStorage[oCountTemp3], ";") != 0) {
        if (strcmp(parsedStorage[oCountTemp3], ",") == 0) {
          numCommas++;
        }
        oCountTemp3++;
      }
      /*This while loop will find class name and class variable name and add constructor call underneath it*/
      while (strcmp(parsedStorage[oCountTemp], ";") != 0) {
        /*checking for spaces or pointer stars*/
        if ((strcmp(parsedStorage[oCountTemp], " ") != 0) && (strcmp(parsedStorage[oCountTemp], "*") != 0) && (foundClass == false) && (strcmp(parsedStorage[oCountTemp], "\f") != 0) && (strcmp(parsedStorage[oCountTemp], "\v") != 0) && (strcmp(parsedStorage[oCountTemp], "\r") != 0) && (strcmp(parsedStorage[oCountTemp], "\t") != 0) && (strcmp(parsedStorage[oCountTemp], "\n") != 0)) {
          tClassNameSize = strlen(parsedStorage[oCountTemp]);
          tClassName = reallocBuffer(tClassName, tClassNameSize);
          strcpy(tClassName, parsedStorage[oCountTemp]);
          oCountTemp++;
          foundClass = true;
        }
        if ((strcmp(parsedStorage[oCountTemp], " ") != 0) && (strcmp(parsedStorage[oCountTemp], "*") != 0) && (foundClass == true) && (strcmp(parsedStorage[oCountTemp], "\f") != 0) && (strcmp(parsedStorage[oCountTemp], "\v") != 0) && (strcmp(parsedStorage[oCountTemp], "\r") != 0) && (strcmp(parsedStorage[oCountTemp], "\t") != 0) && (strcmp(parsedStorage[oCountTemp], "\n") != 0)) {
          tCVarSize = strlen(parsedStorage[oCountTemp]);
          tCVarName = reallocBuffer(tCVarName, tCVarSize);
          strcpy(tCVarName, parsedStorage[oCountTemp]);
          oCountTemp++;
          /*Associates struct variable with correct struct*/
          strcpy(associated[assCount].varname, tCVarName);
          strcpy(associated[assCount].classname, tClassName);
          assCountStored = assCount;
          assCount++;
          if (numCommas == 0) {
            foundClass = false;
            break;
          } else {
            /*This while loop associates all comma'd variables*/
            while (strcmp(parsedStorage[oCountTemp], ";") != 0) {
              if ((strcmp(parsedStorage[oCountTemp], " ") != 0) && (strcmp(parsedStorage[oCountTemp], "*") != 0) && (foundClass == true) && (strcmp(parsedStorage[oCountTemp], "\f") != 0) && (strcmp(parsedStorage[oCountTemp], "\v") != 0) && (strcmp(parsedStorage[oCountTemp], "\r") != 0) && (strcmp(parsedStorage[oCountTemp], "\t") != 0) && (strcmp(parsedStorage[oCountTemp], "\n") != 0) && (strcmp(parsedStorage[oCountTemp], ",") != 0)) {
                tCVarSize = strlen(parsedStorage[oCountTemp]);
                tCVarName = reallocBuffer(tCVarName, tCVarSize);
                strcpy(tCVarName, parsedStorage[oCountTemp]);

                /*Associates struct variable with correct struct*/
                strcpy(associated[assCount].varname, tCVarName);
                strcpy(associated[assCount].classname, tClassName);
                assCount++;
              }
              oCountTemp++;
            }
            foundClass = false;
            break;
          }
        }
        oCountTemp++;
      }

      oCountTemp = 0;
      /*Finally adds entire struct variable to program*/
      globalString = addVariableToProgram(globalString, parsedStorage, &oCount, &gSize, inClass);
      /*adds constructor line*/
      for (i=assCountStored; i<assCount; i++) {
        constExists = CheckIfConstructorExists(constructorNames, NumConNames, associated[i].classname);
        if (constExists == true) {
          tClassNameSize = strlen(associated[i].classname);
          tCVarSize = strlen(associated[i].varname);
          constructorLine = CreateConstructorLine(constructorLine, &cSize, associated[i].classname, &tClassNameSize, associated[i].varname, &tCVarSize);
          constExists = false;
        }
        globalString = extendString(globalString, constructorLine, &gSize);
      }

      tClassName = WipeBuffer(tClassName, &tClassNameSize);
      tCVarName = WipeBuffer(tCVarName, &tCVarSize);
      constructorLine = WipeBuffer(constructorLine, &cSize);
      oCount++;
      numCommas = 0;
    } else {
      /*finds a struct variable, then decides if it is just a variable or a function call*/
      if (strcmp(parsedStorage[oCount], ".") == 0) {
        structFuncCall = FindTypeOfStruct(parsedStorage, oCount);
        if (structFuncCall == true) {
          structFuncCall = false;
          strcpy(structBuffer, parsedStorage[oCount-1]);
          addStructAsParam = true;
          foundFuncStruct = false;
          for (i=0; i<assCount; i++) {
            if (strcmp(parsedStorage[oCount-1], associated[i].varname) == 0) {
              strcpy(foundClassName, associated[i].classname);
              foundFuncStruct = true;
              break;
            }
          }
          /*checks for global variable if not found in function*/
          if (foundFuncStruct == false) {
            for (i=0; i<gAssSize; i++) {
              if (strcmp(parsedStorage[oCount-1], globalAssociated[i].varname) == 0) {
                strcpy(foundClassName, globalAssociated[i].classname);
                foundFuncStruct = false;
                break;
              }
            }
          }
          current = Classes;
          while ((current != NULL) && (newfunc == false)) {
            if (strcmp(foundClassName, current->classname) == 0) {
              for (i=0; i<current->numfuncs; i++) {
                /*This section added in order to deal with overloaded functions*/
                numFArgs = findNumArguments(parsedStorage, oCount);
                if((strcmp(parsedStorage[oCount+1], current->Functions[i].oldname) == 0) && (numFArgs == current->Functions[i].numarguments)) {
                  toAdd = strlen(parsedStorage[oCount]);
                  gSize = gSize+toAdd;
                  globalString = reallocBuffer(globalString, gSize);
                  strcat(globalString, parsedStorage[oCount]);
                  oCount++;
                  /*add new function name in exchange of old one*/
                  toAdd = strlen(current->Functions[i].newname);
                  gSize = gSize+toAdd;
                  globalString = reallocBuffer(globalString, gSize);
                  strcat(globalString, current->Functions[i].newname);
                  newfunc = true;
                  break;
                }
              }
            }
            if (newfunc == true) break;
            current = current->next;
          }
          newfunc = false;
          strcpy(foundClassName, "\0");
          oCount++;
        } else {
          toAdd = strlen(parsedStorage[oCount]);
          gSize = gSize+toAdd;
          globalString = reallocBuffer(globalString, gSize);
          strcat(globalString, parsedStorage[oCount]);
          oCount++;
        }
        /*Adds a pointer to the struct itself as a parameter to deal with class variables*/
      } else if (addStructAsParam == true) {
        toAdd = strlen(parsedStorage[oCount]);
        gSize = gSize+toAdd;
        globalString = reallocBuffer(globalString, gSize);
        strcat(globalString, parsedStorage[oCount]);
        parameterBoolean = FindParameters(parsedStorage, oCount);
        oCount++;
        toAdd = strlen(structBuffer);
        toAdd++;/*Make room for &*/
        gSize = gSize+toAdd;
        globalString = reallocBuffer(globalString, gSize);
        strcat(globalString, "&");
        strcat(globalString, structBuffer);
        /*decide if you need a comma or not (more than one argument)*/
        if (parameterBoolean == true) {
          toAdd = strlen(", ");
          gSize = gSize+toAdd;
          globalString = reallocBuffer(globalString, gSize);
          strcat(globalString, ", ");
        }
        strcpy(structBuffer, "\0");
        addStructAsParam = false;
      } else {
        toAdd = strlen(parsedStorage[oCount]);
        gSize = gSize+toAdd;
        globalString = reallocBuffer(globalString, gSize);
        strcat(globalString, parsedStorage[oCount]);
        oCount++;
      }
    }
  }
  if (structCheck == true) {
    toAdd = 1;
    gSize = gSize+toAdd;
    globalString = reallocBuffer(globalString, gSize);
    strcat(globalString, ";");
  }
  toAdd = 1;
  gSize = gSize+toAdd;
  globalString = reallocBuffer(globalString, gSize);
  strcat(globalString, "\n");

  *outerCount = oCount;
  *globalSize = gSize;

  free(constructorLine);
  free(tCVarName);
  free(tClassName);
  return globalString;
}

/*************************************************************************
Name: AddConstructor
Purpose: The main function in creating the constructor function
**************************************************************************/
char * AddConstructor(char * constructorString, char * className, int oCountStored, char ** parsedStorage, int * constructorSize) {
  int cSize, cNameSize, mainStringSize, funcNameSize;
  cSize = *constructorSize;

  /*Adding the necessary strings to create function*/
  if (cSize == 0) {
    mainStringSize = strlen("void constructor_(struct _ * sPointer) {\n sPointer->__ = &__;\n}");
    cNameSize = strlen(className);
    cNameSize = cNameSize*2;
    funcNameSize = strlen(parsedStorage[oCountStored]);
    funcNameSize = funcNameSize*2;
    cSize = cSize+cNameSize+mainStringSize+funcNameSize;

    constructorString = reallocBuffer(constructorString, cSize);
    strcat(constructorString, "void constructor");
    strcat(constructorString, className);
    strcat(constructorString, "(struct ");
    strcat(constructorString, className);
    strcat(constructorString, " * sPointer) {\n sPointer->");
    strcat(constructorString, parsedStorage[oCountStored]);
    strcat(constructorString, " = &");
    strcat(constructorString, parsedStorage[oCountStored]);
    strcat(constructorString, ";\n");
  } else {
    /*adds a line to point to a function*/
    mainStringSize = strlen(" sPointer->_ = &__;\n");
    funcNameSize = strlen(parsedStorage[oCountStored]);
    funcNameSize = funcNameSize*2;
    cSize = cSize+funcNameSize+mainStringSize;
    constructorString = realloc(constructorString, cSize);
    strcat(constructorString, " sPointer->");
    strcat(constructorString, parsedStorage[oCountStored]);
    strcat(constructorString, " = &");
    strcat(constructorString, parsedStorage[oCountStored]);
    strcat(constructorString, ";\n");
  }

  *constructorSize = cSize;
  return constructorString;
}

/*************************************************************************
Name: CreateConstructorLine
Purpose: When a class variable is called in a function, this prints the call
         to the constructor
**************************************************************************/
char * CreateConstructorLine(char * constructorLine, int * constructSize, char * tClassName, int * tClassNameSize, char * tCVarName, int * tCVarSize) {
  int cSize, TCSize, TCVSize;

  TCSize = *tClassNameSize;
  TCVSize = *tCVarSize;
  cSize = *constructSize;

  cSize = strlen("\nconstructor_(&_);");
  cSize = cSize+TCVSize+TCSize;
  constructorLine = reallocBuffer(constructorLine, cSize);
  strcpy(constructorLine, "\nconstructor");
  strcat(constructorLine, tClassName);
  strcat(constructorLine, "(&");
  strcat(constructorLine, tCVarName);
  strcat(constructorLine, ");");

  *tClassNameSize = TCSize;
  *tCVarSize = TCVSize;
  *constructSize = cSize;

  return constructorLine;
}

/*************************************************************************
Name: AddClassVarToStruct
Purpose: Adds a class variable to the Class struct
**************************************************************************/
char * AddClassVarToStruct(struct Class * Classes, char ** parsedStorage, int oCount, char * addCVarDef2Constr, int * addCVarDefSize) {
  bool commaDeclaration, foundType, varChk, done;
  char typebuffer[30];
  char varbuffer[30];
  int findComma, commaCount, goThru1, ACVDSize, toAdd, goThru2;
  struct Class * current;
  findComma = oCount;
  goThru1 = oCount;
  commaCount = 0;
  ACVDSize = *addCVarDefSize;

  current = Classes;
  while (current->next != NULL) {
    current = current->next;
  }
  /*Initialize buffers*/
  strcpy(typebuffer, "\0");
  strcpy(varbuffer, "\0");

  commaDeclaration = foundType = varChk = done = false;
  /*Finds if there are multiple variables on one line*/
  while (strcmp(parsedStorage[findComma], ";") != 0) {
    if (strcmp(parsedStorage[findComma], ",") == 0) {
      commaDeclaration = true;
      commaCount++;
    }
    findComma++;
  }
  if (commaDeclaration != true) {
    /*if there is only one variable before the semicolon*/
    while (strcmp(parsedStorage[goThru1], ";") != 0) {
      if ((strcmp(parsedStorage[goThru1], " ") != 0) && (strcmp(parsedStorage[goThru1], "*") != 0) && (strcmp(parsedStorage[goThru1], "\n") != 0) && (strcmp(parsedStorage[goThru1], "\f") != 0) && (strcmp(parsedStorage[goThru1], "\v") != 0) && (strcmp(parsedStorage[goThru1], "\r") != 0) && (strcmp(parsedStorage[goThru1], "\t") != 0)) {
        varChk = VarChecker(parsedStorage[goThru1]);
        if (varChk == true) {
          strcpy(current->vartypes[current->numvars], parsedStorage[goThru1]);
          current->numvars++;

          varChk = false;
        } else {
          strcpy(current->variables[current->numvars-1], parsedStorage[goThru1]);
          strcpy(varbuffer, parsedStorage[goThru1]);
          while (strcmp(parsedStorage[goThru1], ";") != 0) {
            if (strcmp(parsedStorage[goThru1], "=") == 0) {
              /*Copying function definition to string to be added onto constructor later*/
              toAdd = strlen("\n  sPointer->_ = _;");
              toAdd = toAdd+strlen(varbuffer);
              ACVDSize = ACVDSize+toAdd;
              addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
              strcat(addCVarDef2Constr, "\n sPointer->");
              strcat(addCVarDef2Constr, varbuffer);
              strcat(addCVarDef2Constr, " ");
              while(strcmp(parsedStorage[goThru1], ";") != 0) {
                toAdd = strlen(parsedStorage[goThru1]);
                ACVDSize = ACVDSize+toAdd;
                addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
                strcat(addCVarDef2Constr, parsedStorage[goThru1]);
                goThru1++;
              }
              toAdd = strlen(parsedStorage[goThru1]);
              ACVDSize = ACVDSize+toAdd;
              addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
              strcat(addCVarDef2Constr, parsedStorage[goThru1]);
              goThru1--;
              break;
            }
            goThru1++;
          }
          break;
        }
      }
      goThru1++;
    }
  } else {
    /*if there are multiple variables separated by commas*/
    while (strcmp(parsedStorage[goThru1], ";") != 0) {
      if ((strcmp(parsedStorage[goThru1], " ") != 0) && (strcmp(parsedStorage[goThru1], "*") != 0) && (strcmp(parsedStorage[goThru1], ",") != 0) && (strcmp(parsedStorage[goThru1], "\n") != 0) && (strcmp(parsedStorage[goThru1], "\f") != 0) && (strcmp(parsedStorage[goThru1], "\v") != 0) && (strcmp(parsedStorage[goThru1], "\r") != 0) && (strcmp(parsedStorage[goThru1], "\t") != 0)) {
        varChk = VarChecker(parsedStorage[goThru1]);
        if (varChk == true) {
          strcpy(typebuffer, parsedStorage[goThru1]);

          varChk = false;
        } else {
          /*add new variable to struct and check for = signs*/
          strcpy(current->vartypes[current->numvars], typebuffer);
          strcpy(current->variables[current->numvars], parsedStorage[goThru1]);
          strcpy(varbuffer, parsedStorage[goThru1]);
          current->numvars++;
          goThru2 = goThru1;
          while((strcmp(parsedStorage[goThru2], ";") != 0) && (strcmp(parsedStorage[goThru2], ",") != 0)) {
            if (strcmp(parsedStorage[goThru2], "=") == 0) {
              /*Copying function definition to string to be added onto constructor later*/
              toAdd = strlen("\n  sPointer->_ = _;");
              toAdd = toAdd+strlen(varbuffer);
              ACVDSize = ACVDSize+toAdd;
              addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
              strcat(addCVarDef2Constr, "\n sPointer->");
              strcat(addCVarDef2Constr, varbuffer);
              strcat(addCVarDef2Constr, " ");
              while((strcmp(parsedStorage[goThru2], ";") != 0) && (strcmp(parsedStorage[goThru2], ",") != 0)) {
                toAdd = strlen(parsedStorage[goThru2]);
                ACVDSize = ACVDSize+toAdd;
                addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
                strcat(addCVarDef2Constr, parsedStorage[goThru2]);
                goThru2++;
              }
              toAdd = 1;
              ACVDSize = ACVDSize+toAdd;
              addCVarDef2Constr = reallocBuffer(addCVarDef2Constr, ACVDSize);
              strcat(addCVarDef2Constr, ";");
              goThru2--;
            }
            goThru1 = goThru2;
            goThru2++;
          }
        }
      }
      goThru1++;
    }
  }
  *addCVarDefSize = ACVDSize;
  return addCVarDef2Constr;
}

/*************************************************************************
Name: CreateVariableOnlyConstructor
Purpose: If no functions are present in the class and a constructor has not
         been initialized, but there are class variables that need to be set,
         this will create a constructor for them
**************************************************************************/
char * CreateVariableOnlyConstructor(char * tempConstructorHolder, char * addCVarDef2Constr, int * constructorSize, struct Class * Classes) {
  int cSize, toAdd;
  struct Class * current;

  current = Classes;
  while (current->next != NULL) {
    current = current->next;
  }

  cSize = *constructorSize;
  toAdd = 0;

  toAdd = strlen("void constructor_(struct _ * sPointer) {\n");
  cSize = toAdd;
  toAdd = strlen(current->classname);
  toAdd = toAdd*2;
  cSize = cSize+toAdd;
  tempConstructorHolder = reallocBuffer(tempConstructorHolder, cSize);
  strcat(tempConstructorHolder, "void constructor");
  strcat(tempConstructorHolder, current->classname);
  strcat(tempConstructorHolder, "(struct ");
  strcat(tempConstructorHolder, current->classname);
  strcat(tempConstructorHolder, " * sPointer) {");
  toAdd = strlen(addCVarDef2Constr);
  cSize = cSize+toAdd;
  tempConstructorHolder = reallocBuffer(tempConstructorHolder, cSize);
  strcat(tempConstructorHolder, addCVarDef2Constr);
  toAdd = strlen("\n}\n");
  cSize = cSize+toAdd;
  tempConstructorHolder = reallocBuffer(tempConstructorHolder, cSize);
  strcat(tempConstructorHolder, "\n}\n");

  *constructorSize = cSize;
  return tempConstructorHolder;
}

/*************************************************************************
Name: addBufferStringYesChar
Purpose: Used in the initial parsing of information from a file.
         Adds a buffer string to the double pointer string array storing each token
         from the file in it's own string. The entire parsedStorage array is dynamically
         allocated so it is necessary
**************************************************************************/
char ** addBufferStringYesChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize, char toAdd) {
  int pSize, bSize;
  pSize = *parsedSize;
  bSize = *bufferSize;
  pSize++;
  bSize++;
  buffer = reallocBuffer(buffer, bSize);
  parsedStorage = realloc(parsedStorage, sizeof(char*)*(pSize)+1);
  buffer[bSize-1] = toAdd;
  parsedStorage[pSize-1] = buffer;

  bSize = 0;
  *parsedSize = pSize;
  *bufferSize = bSize;
  return parsedStorage;
}

/*************************************************************************
Name: addBufferStringNoChar
Purpose: Used for the same purpose as above function, except it does not append
         a character to the end before appending buffer to double string array
**************************************************************************/
char ** addBufferStringNoChar(char ** parsedStorage, char * buffer, int * bufferSize, int * parsedSize) {
  int pSize, bSize;
  pSize = *parsedSize;
  bSize = *bufferSize;
  pSize++;

  parsedStorage = realloc(parsedStorage, sizeof(char*)*(pSize)+1);
  parsedStorage[pSize-1] = buffer;

  bSize = 0;
  *parsedSize = pSize;
  *bufferSize = bSize;
  return parsedStorage;
}

/*************************************************************************
Name: mallocBuffer
Purpose: allocates a string *, while initializing the string with "\0"
**************************************************************************/
char * mallocBuffer(int bSize) {
  char * buffer;
  buffer = malloc(sizeof(char)*bSize+1);
  strcpy(buffer, "\0");
  return buffer;
}

/*************************************************************************
Name: reallocBuffer
Purpose: Used to realloc a string *, and sets final char to '\0'
**************************************************************************/
char * reallocBuffer(char * buffer, int bSize) {
  buffer = realloc(buffer, sizeof(char)*bSize+1);
  buffer[bSize] = '\0';
  return buffer;
}

/*************************************************************************
Name: addInclude
Purpose: When an include statement is found, it is added to the globalString
**************************************************************************/
char * addInclude(char * string, char ** parsedStorage, int * programSize, int * outerCount) {
  int newSize, oCount, pSize;
  newSize = 0;
  oCount = *outerCount;
  pSize = *programSize;
  do {
    newSize = strlen(parsedStorage[oCount]);
    pSize = pSize+newSize;
    string = reallocBuffer(string, pSize);
    if (pSize-newSize == 0) {
      strcpy(string, parsedStorage[oCount]);
    } else {
      strcat(string, parsedStorage[oCount]);
    }
    oCount++;
  } while (((strcmp(parsedStorage[oCount], "\n"))!= 0));
  *outerCount = oCount-1;
  *programSize = pSize;

  return string;
}

/*************************************************************************
Name: addVariableToProgram
Purpose: If it is a variable, adds the variable to the string
**************************************************************************/
char * addVariableToProgram(char * string, char ** parsedStorage, int * outerCount, int * programSize, bool inClass) {
  int oCount, pSize, toAdd;
  char commaOrSemi;
  oCount = *outerCount;
  pSize = *programSize;

  commaOrSemi = '\0';

  if (strcmp(parsedStorage[oCount], "class") == 0) {
    toAdd = 6;
    pSize = pSize+toAdd;
    string = reallocBuffer(string, pSize);
    strcat(string, "struct");
    oCount++;
  }
  while (strcmp(parsedStorage[oCount], ";") != 0) {
    /*if it is equal to something it moves to the end of the variable as this was dealt with in another function*/
    if ((strcmp(parsedStorage[oCount], "=") == 0) && (inClass == true)) {
      while((strcmp(parsedStorage[oCount], ";") != 0) && (strcmp(parsedStorage[oCount], ",") != 0)) {
        oCount++;
        commaOrSemi = parsedStorage[oCount][0];
      }
      if (commaOrSemi == ';') break;
    }
    toAdd = strlen(parsedStorage[oCount]);
    pSize = pSize+toAdd;
    string = reallocBuffer(string, pSize);
    strcat(string, parsedStorage[oCount]);
    oCount++;
  }
  toAdd = strlen(parsedStorage[oCount]);
  pSize = pSize+toAdd;
  string = reallocBuffer(string, pSize);
  strcat(string, parsedStorage[oCount]);

  *outerCount = oCount;
  *programSize = pSize;
  return string;
}

/*************************************************************************
Name: VariableOrFunctionChecker
Purpose: Checks if the parsed line is a variable declaration, or if it is the
         beginning of a function
**************************************************************************/
bool VariableOrFunctionChecker(char ** parsedStorage, int outerCount) {
  int count;
  count = outerCount;

    while (strcmp(parsedStorage[count], ";") != 0) {
        if ((strcmp(parsedStorage[count], "{") == 0)) {
          return false;
        } else count++;
    }
    return true;
}

/*************************************************************************
Name: convClassToStruct
Purpose: Converts a class header to a struct header
**************************************************************************/
char * convClassToStruct(char * string, char ** parsedStorage, int * outerCount, int * programSize) {
  int toAdd, pSize, oCount;
  pSize = *programSize;
  oCount = *outerCount;

  pSize = pSize+7;
  string = reallocBuffer(string, pSize);
  strcat(string, "struct ");
  oCount++;
  while ((strcmp(parsedStorage[oCount], "{") != 0)) {
    toAdd = strlen(parsedStorage[oCount]);
    pSize = pSize+toAdd;
    string = reallocBuffer(string, pSize);
    strcat(string, parsedStorage[oCount]);
    oCount++;
  }
  toAdd = strlen(parsedStorage[oCount]);
  pSize = pSize+toAdd;
  string = reallocBuffer(string, pSize);
  strcat(string, parsedStorage[oCount]);

  *outerCount = oCount;
  *programSize = pSize;

  return string;
}

/*************************************************************************
Name: VarChecker
Purpose: Checks if the parsed token is a type of variable
**************************************************************************/

bool VarChecker(char * string) {
  if ((strcmp(string, "class") == 0) || (strcmp(string, "int") == 0) || (strcmp(string, "char") == 0) || (strcmp(string, "bool") == 0) || (strcmp(string, "short") == 0) || (strcmp(string, "double") == 0) || (strcmp(string, "long") == 0) || (strcmp(string, "signed") == 0) || (strcmp(string, "unsigned") == 0) || (strcmp(string, "float") == 0) || (strcmp(string, "void") == 0) || (strcmp(string, "struct") == 0)) {
    return true;
  } else return false;
}
/*************************************************************************
Name: StructChecker
Purpose: Checks if the parsed token is a struct
**************************************************************************/

bool StructChecker(char * string) {
  if ((strcmp(string, "struct") == 0)) {
    return true;
  } else return false;
}

/*************************************************************************
Name: TypeOfVariable
Purpose: returns the type of variable
**************************************************************************/
char TypeOfVariable(char * string) {
  return string[0];
}

/*************************************************************************
Name: classCheck
Purpose: Checks if the next line is a class
**************************************************************************/
bool classCheck(char ** parsed, int position, int parsedSize) {
  if ((parsed[position][0] == 'c') && (parsed[position][1] == 'l') && (parsed[position][2] == 'a') && (parsed[position][3] == 's') && (parsed[position][4] == 's')) {
    while ((strcmp(parsed[position], "{") != 0) && (position < parsedSize)) {
        if (strcmp(parsed[position], ";") == 0) {
          return false;
        }
        position++;
    }
  } else return false;
  return true;
}

/*************************************************************************
Name: extendString
Purpose: extends a string *, reallocing the string within the function. Essentially a helper
         function
**************************************************************************/
char * extendString(char * finalProgram, char * string, int * programSize) {
  int pSize, toAdd;
  pSize = *programSize;

  toAdd = strlen(string);
  pSize = pSize+toAdd;
  finalProgram = reallocBuffer(finalProgram, pSize);
  strcat(finalProgram, string);

  *programSize = pSize;
  return finalProgram;

}

/*************************************************************************
Name: reallocStringArray
Purpose: Helper function to allocate and initialize a char ** array
**************************************************************************/
char ** reallocStringArray(char ** classesArray, int size) {
  classesArray = realloc(classesArray, sizeof(char*)*size+1);
  return classesArray;
}

/*************************************************************************
Name: mallocIntArray
Purpose: Helper function to allocate enough space for a double integer array
**************************************************************************/
int ** mallocIntArray(int size) {
  int ** array;
  int i;

  array = malloc(sizeof(int*)*size);
  for (i=0; i<size; i++) {
    array[i] = malloc(sizeof(int));
    *array[i] = 0;
  }
  return array;
}

/*************************************************************************
Name: reallocIntArray
Purpose: helper function to allocate enough space for a double integer array
**************************************************************************/
int ** reallocIntArray(int ** array, int oldSize, int newSize) {
  int i;
  array = realloc(array, sizeof(int*)*newSize);
  for (i=oldSize; i<newSize; i++) {
    array[i] = malloc(sizeof(int));
    *array[i] = 0;
  }
  return array;

}

/*************************************************************************
Name: WipeBuffer
Purpose: Reinitializes a string *
**************************************************************************/
char * WipeBuffer(char * buffer, int * size) {
  free(buffer);
  buffer = mallocBuffer(0);
  *size = 0;

  return buffer;
}

/*************************************************************************
Name: FindClassName
Purpose: Finds the class name when a class is found in the parsed information
**************************************************************************/
char * FindClassName(char * nameBuffer, char ** parsedStorage, int oCount, int * nameBufferSize) {
  int size;
  oCount++;
  while (strcmp(parsedStorage[oCount], "{") != 0) {
    if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "\n") != 0) && (strcmp(parsedStorage[oCount], "*") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0)) {
      size = strlen(parsedStorage[oCount]);
      nameBuffer = reallocBuffer(nameBuffer, size);
      strcpy(nameBuffer, parsedStorage[oCount]);
      *nameBufferSize = size;
      return nameBuffer;
    }
    oCount++;
  }
  return nameBuffer;
}

/*************************************************************************
Name: CopyDoubleSlashComments
Purpose: Copies entire comments to the string, as they should pass through
         the program untouched. For double slash type comments
**************************************************************************/
char * CopyDoubleSlashComments(char * string, int * stringSize, char ** parsedStorage, int * outerCount) {
  int oCount, toAdd, sSize;

  toAdd = 0;
  oCount = *outerCount;
  sSize = *stringSize;

  while (strcmp(parsedStorage[oCount], "\n") != 0) {
    toAdd = strlen(parsedStorage[oCount]);
    sSize=sSize+toAdd;
    string = reallocBuffer(string, sSize);
    strcat(string, parsedStorage[oCount]);
    oCount++;
  }
  toAdd = strlen(parsedStorage[oCount]);
  sSize=sSize+toAdd;
  string = reallocBuffer(string, sSize);
  strcat(string, parsedStorage[oCount]);

  *outerCount = oCount;
  *stringSize = sSize;

  return string;
}

/*************************************************************************
Name: CopySlashStarComments
Purpose: Copies entire comments to the string, as they should pass through
         the program untouched. For slash star type comments
**************************************************************************/
char * CopySlashStarComments(char * string, int * stringSize, char ** parsedStorage, int * outerCount) {
  int oCount, toAdd, sSize;
  bool endCommentCheck;

  endCommentCheck = false;
  toAdd = 0;
  oCount = *outerCount;
  sSize = *stringSize;

  toAdd = 2;
  sSize = sSize+toAdd;
  string = reallocBuffer(string, sSize);
  strcat(string, "/*");
  oCount = oCount+2;
  while (endCommentCheck == false) {
    if ((strcmp(parsedStorage[oCount], "*") == 0) && (strcmp(parsedStorage[oCount+1], "/") == 0)) {
      toAdd = 2;
      sSize = sSize+toAdd;
      string = reallocBuffer(string, sSize);
      strcat(string, "*/");
      oCount = oCount+2;
      endCommentCheck = true;
      break;
    }
    toAdd = strlen(parsedStorage[oCount]);
    sSize = sSize+toAdd;
    string = reallocBuffer(string, sSize);
    strcat(string, parsedStorage[oCount]);
    oCount++;
  }
  toAdd = toAdd+strlen("\n");
  sSize = sSize+toAdd;
  string = reallocBuffer(string,sSize);
  strcat(string, "\n");

  *outerCount = oCount;
  *stringSize = sSize;

  return string;
}

/*************************************************************************
Name: MallocAndInitializeClass
Purpose: This function allocates enough space for a struct Class, as well as
         initializes everything inside it
**************************************************************************/
struct Class * MallocAndInitializeClass(struct Class * Classes) {
  int i,k;
  Classes = malloc(sizeof(struct Class)+1);
  strcpy(Classes->classname, "\0");
  for (i=0; i<75; i++) {
    strcpy(Classes->Functions[i].oldname, "\0");
    strcpy(Classes->Functions[i].newname, "\0");
    Classes->Functions[i].numconflicts = 0;
    Classes->Functions[i].numarguments = 0;
  }
  for (i=0; i<50; i++) {
    strcpy(Classes->variables[i], "\0");
    strcpy(Classes->vartypes[i], "\0");
    for (k=0; k<50; k++) {
      Classes->Functions[i].varconflict[k] = 0;
    }
  }
  Classes->variablesExist = false;
  Classes->numfuncs = 0;
  Classes->numvars = 0;
  Classes->next = NULL;
  return Classes;
}

/*************************************************************************
Name: FreeClassStruct
Purpose: Properly frees a struct Class
**************************************************************************/
void FreeClassStruct(struct Class * Classes) {
  struct Class *current, *prev;
  current = Classes;
  while (current->next != NULL) {
    prev = current;
    current = current->next;
    free(prev);
  }
  free(current);
}

/*************************************************************************
Name: PrintClass
Purpose: A function used for printing the contents of the class information
         array. Mainly used for debugging
**************************************************************************/
void PrintClass(struct Class * Classes) {
  int i,k;
  struct Class * current;
  current = Classes;
  while (current != NULL) {
    printf("Class Name: %s\n", current->classname);
    printf("\n");
    printf("Number of Class Variables: %d\n", current->numvars);
    for (i=0; i<current->numvars; i++) {
      printf("Type %d: %s\n", i+1, current->vartypes[i]);
      printf("Name %d: %s\n", i+1, current->variables[i]);
    }
    printf("\n");
    printf("Number of Functions: %d\n", current->numfuncs);
    printf("\n");
    for (i=0; i<current->numfuncs; i++) {
      printf("Old function name %d: %s\n", i+1, current->Functions[i].oldname);
      printf("New function name %d: %s\n", i+1, current->Functions[i].newname);
      printf("Number of arguments: %d\n", current->Functions[i].numarguments);
      printf("Number of ClassVariable Conflicts: %d\n", current->Functions[i].numconflicts);
      for (k=0; k<current->numvars; k++) {
        printf("Class Variable %d confict?: %d\n", k+1, current->Functions[i].varconflict[k]);
      }
      printf("\n");
    }
    current = current->next;
  }
}
/*************************************************************************
Name: InitializeAssociation
Purpose: Initializes a struct Association
**************************************************************************/

struct Association InitializeAssociation(struct Association associated) {
  strcpy(associated.varname, "\0");
  strcpy(associated.classname, "\0");
  return associated;
}

/*************************************************************************
Name: FindVariableConflicts
Purpose: If there is a variable with the same name as a class variable in the
         function within a class, or if an argument is the same, then conflicts
         are marked
**************************************************************************/
void FindVariableConflicts(struct Class * Classes, char ** parsedStorage, int oCount) {
  int i, cbSize;
  char * commentBuffer;
  struct Class * current;
  bool varChk;
  varChk = false;
  oCount++;
  current = Classes;
  cbSize = 0;
  commentBuffer = mallocBuffer(0);
  while (current->next != NULL) {
    current = current->next;
  }
  while (strcmp(parsedStorage[oCount], "{") != 0) {
    varChk = VarChecker(parsedStorage[oCount]);
    /*Checks for comments*/
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount], "/") == 0)) {
      commentBuffer = CopyDoubleSlashComments(commentBuffer, &cbSize, parsedStorage, &oCount);
    }
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount], "*") == 0)) {
      commentBuffer = CopySlashStarComments(commentBuffer, &cbSize, parsedStorage, &oCount);
    }
    if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "(") != 0) && (strcmp(parsedStorage[oCount], ")") != 0) && (strcmp(parsedStorage[oCount], "*") != 0) && (strcmp(parsedStorage[oCount], ",") != 0) && (strcmp(parsedStorage[oCount], "\n") != 0) && (varChk == false)) {
      for (i=0; i<current->numvars; i++) {
        /*if a conflict is found it is added to the class struct*/
        if (strcmp(parsedStorage[oCount], current->variables[i]) == 0) {
          current->Functions[current->numfuncs].varconflict[i] = 1;
          current->Functions[current->numfuncs].numconflicts++;
        }
      }
    }
    oCount++;
  }
  while (strcmp(parsedStorage[oCount], "}") != 0) {
    varChk = VarChecker(parsedStorage[oCount]);
    /*checks for comments*/
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount], "/") == 0)) {
      commentBuffer = CopyDoubleSlashComments(commentBuffer, &cbSize, parsedStorage, &oCount);
    }
    if ((strcmp(parsedStorage[oCount], "/") == 0) && (strcmp(parsedStorage[oCount], "*") == 0)) {
      commentBuffer = CopySlashStarComments(commentBuffer, &cbSize, parsedStorage, &oCount);
    }
    if (varChk == true) {
      oCount++;
      while ((strcmp(parsedStorage[oCount], ";") != 0) && (strcmp(parsedStorage[oCount], "=") != 0)) {
        if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "*") != 0) && (strcmp(parsedStorage[oCount], "\n") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0)) {
          for (i=0; i<current->numvars; i++) {
            if (strcmp(parsedStorage[oCount], current->variables[i]) == 0) {
              current->Functions[current->numfuncs].varconflict[i] = 1;
              current->Functions[current->numfuncs].numconflicts++;
            }
          }
        }
        oCount++;
      }
    }
    oCount++;
  }
  free(commentBuffer);
}

/*************************************************************************
Name: FindParameters
Purpose: If there are parameters in the function true is returned, if not,
         false is returned
**************************************************************************/
bool FindParameters(char ** parsedStorage, int oCount) {
  if (strcmp(parsedStorage[oCount], "(") != 0) {
    return false;
  }
  while (strcmp(parsedStorage[oCount], ")") != 0) {
    if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], "\n") != 0) && (strcmp(parsedStorage[oCount], "(") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0)) {
      return true;
    }
    oCount++;
  }
  return false;
}

/*************************************************************************
Name: FindTypeOfStruct
Purpose: If in a function a struct variable is sent, false is returned
         If a struct function call is sent, true is returned
**************************************************************************/
bool FindTypeOfStruct(char ** parsedStorage, int oCount) {
  while (strcmp(parsedStorage[oCount], ";") != 0) {
    if (strcmp(parsedStorage[oCount], "(") == 0) {
      return true;
    }
    oCount++;
  }
  return false;
}

/*************************************************************************
Name: CheckForStars
Purpose: checks for pointer stars, returns true if found
**************************************************************************/
bool CheckForStars(char ** parsedStorage, int oCount) {
  while((strcmp(parsedStorage[oCount], ")") != 0) && (strcmp(parsedStorage[oCount], ",") != 0)) {
    if (strcmp(parsedStorage[oCount], "*") == 0) {
      return true;
    }
    oCount++;
  }
  return false;
}

/*************************************************************************
Name: CheckForSqareBracket
Purpose: Checks for a square bracket, returns true if found.
**************************************************************************/
bool CheckForSqareBracket(char ** parsedStorage, int oCount) {
  int sSize, i;
  while((strcmp(parsedStorage[oCount], ")") != 0) && (strcmp(parsedStorage[oCount], ",") != 0)) {
    sSize = strlen(parsedStorage[oCount]);
    for (i=0; i<sSize; i++) {
      if (parsedStorage[oCount][i] == '[') {
          return true;
      }
    }
    oCount++;
  }
  return false;
}

/*************************************************************************
Name: CheckIfConstructorExists
Purpose: Checks if the constructor exists for a specific class type
         If none exists, then the call to the constructor will not be created
**************************************************************************/
bool CheckIfConstructorExists(char ** constructorNames, int NumConNames, char * tClassName) {
  char temporary[70];
  int i;

  strcpy(temporary, "constructor");
  strcat(temporary, tClassName);
  for (i=0; i<NumConNames; i++) {
    if (strcmp(temporary, constructorNames[i]) == 0) {
      return true;
    }
  }
  return false;
}

/*************************************************************************
Name: findNumArguments
Purpose: returns the number of arguments between brackets
**************************************************************************/
int findNumArguments(char ** parsedStorage, int oCount) {
  int numArgs;
  bool startcounting;
  startcounting = false;
  numArgs = 0;
  while (strcmp(parsedStorage[oCount], ";") != 0) {
    if (strcmp(parsedStorage[oCount], "(") == 0) {
      startcounting = true;
    } else if (startcounting == true) {
      if (strcmp(parsedStorage[oCount], ")") == 0) {
        startcounting = false;
        break;
      } else if ((strcmp(parsedStorage[oCount], " ") != 0) && (strcmp(parsedStorage[oCount], ",") != 0) && (strcmp(parsedStorage[oCount], "\f") != 0) && (strcmp(parsedStorage[oCount], "\v") != 0) && (strcmp(parsedStorage[oCount], "\r") != 0) && (strcmp(parsedStorage[oCount], "\t") != 0)) {
        numArgs++;
      }
    }
    oCount++;
  }
  return numArgs;
}
