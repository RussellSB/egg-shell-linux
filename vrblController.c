#include "eggshell.h"

//struct for storing shellVariable with it's string name and string values
typedef struct shellVariable {
    char * name;
    char * value;
} Var;


//struct for a dynamic array called VariableArray that points to ShellVariables
typedef struct variableArray {
    Var ** varArr; //double pointer to act as an array of structs
    int amount; //stores amount of variables in array, increases array dynamically
} VarArr;


/* Global dynamic array "variables" */
VarArr * variables;


//validates variable name for containing (a-z, A-Z), (0-9) or (_), returns 0 when succeeded, -1 when failed
int validateVarName(char * name) {

    char tempName[strlen(name)]; //temporarily stores name for character scanning
    strcpy(tempName, name); //copies String pointer name into String array tempName

    //scan every character in string name of variable
    for (int i = 0; i < strlen(name); i++) {

        //if character is not a digit or letter and is not an underscore
        if (!isalnum(tempName[i]) && tempName[i] != '_') {

            return -1; //return -1, indicating the name failed validation

        }

    }

    return 0; //return 0, indicating it checked every character, proved valid

}


//returns value content from variable name if exists, if doesn't returns NULL
char * getVarValue(const char * varName){

    varName+=1; //points to string after '$', so it can compare the name

    //traverses through variable array searching for a variable name match
    for(int i = 0; i < variables->amount; i++ ){

        //if a match is found
        if(strcmp(variables->varArr[i]->name,varName) == 0){

            return variables->varArr[i]->value; //return value when match is found

        }

    }

    printf("Error: Requested Shell Variable doesn't exist\n");
    return NULL; //return NULL when no match is found (variable doesn't exist)

}


//gets the index of a variable in the dynamic array by name
int getVarIndex(const char * varName){

    //traverses through variable array searching for a variable name match
    for(int i = 0; i < variables->amount; i++ ){

        //if a match is found
        if(strcmp(variables->varArr[i]->name,varName) == 0){
            return i; //return index when match is found
        }

    }

    return -1; //return -1 when no match is found (variable doesn't exist)

}


//method used for adding a shell variable and storing to a dynamic array of structs if doesn't already exist
void addVar(char * name, char * value) {

    if (validateVarName(name) != 0) { //used to catch just in case the variable name is not valid

        printf("Error: Variable \"%s\" should contain digits, letters and underscores only. Please try again.\n", name);
        return; //returns, to cancel adding variable

    }

    int v = variables->amount; //sets variable index v to the amount of variables stores

    //if at variable after the first entry
    if(v >= 1){

        int i = getVarIndex(name); //i becomes the index of varName if exists, -1 if doesn't exist

        //check if variable name is already stored in the variable storage
        if(i != -1){

            variables->varArr[i]->value = value; //since already stored just overwrites previous value with new one
            return; //returns to avoid unnecessary memory allocating

        }else{

            //dynamically increases the varArr by one as adding a new variable
            variables->varArr = (Var **) realloc(variables->varArr, sizeof(Var) * v);

        }
    }

    variables->varArr[v] = malloc(sizeof(Var)); //allocates memory for new variable in varArr[v]

    variables->varArr[v]->name = name; //sets name at index v
    variables->varArr[v]->value = value; //sets value at index v

    variables->amount++; //increases amount by 1

}


//method that sets the variable CWD for current working directory
void setCWD(void){

    char buf[MAX_CHAR];//sets buffer with max character length for string
    char * cwd; //pointer to point to this String array buffer

    getcwd(buf, sizeof(buf)); //gets new current working directory

    cwd = strdup(buf); //allocates memory for cwd, by using buffer as a source
    addVar("CWD", cwd); //current working directory, file operations relative to this

}


//initializes storage platform "variables" and adds initial shell variables to storage
void initShellVariables(void) {

    //allocates memory for Variables Array which is just one item
    variables = malloc(sizeof(VarArr));

    //initializes variable amount with starting size 0, dynamically grows when adding variables
    variables->amount = 0;

    //allocating memory
    variables->varArr = calloc(1, sizeof(Var)); //initially allocates memory for 1 var in varArr

    /* Adding initial Shell Variables */

    //getenv() variables
    addVar("PATH", getenv("PATH")); //path to external commands
    addVar("USER", getenv("USER")); //name of current user
    addVar("HOME", getenv("HOME")); //home directory of user

    //cwd variable
    setCWD();

    //other variables
    addVar("PROMPT", "eggShell-lineInput~$> "); //command prompt variable, to be used in cmdController.c
    addVar("TERMINAL", ttyname(STDIN_FILENO)); //current terminal name

    //addVariable("SHELL", ---, variables); //absolute path of the eggshell binary
    //addVariable("EXITCODE", ---, variables); //exit code returned by the last program in the shell

}


//prints all the variables stored in the shell
void printAllVar(void){

    //traverses through storage of variables, printing out each one
    for(int i = 0; i < variables->amount; i++ ){

        printf("%s=%s\n", variables->varArr[i]->name, variables->varArr[i]->value);

    }

}


//frees all the variables stored in the shell by freeing dynamic array
void freeAllVar(void){

    free(variables); //frees all variables and dynamic array that stores them

}