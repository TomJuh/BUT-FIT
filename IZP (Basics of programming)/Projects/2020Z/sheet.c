/*
File: sheet.c

Author: Tomas Juhasz

Login: xjuhas04

*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_LENGTH 10242
#define NULL_DELIM -1
// return values codes
#define SUCCESS_CODE 0
#define IGNORE_CODE 1
#define DELETE_LINE 5
//error codes
#define FAIL_CODE -1
#define FILE_EMPTY -2 
#define NO_ARGS -3
#define INCORRECT_SELECT_SYNTAX -4
#define COLUMN_NOT_FOUND_CODE -5
#define NO_NUM_IN_CELL_CODE -6

typedef struct{
    char * line;
    char * strArg;
    int firstArg;
    int secondArg;
    int rowIndex;
    char delim;
    bool lastLine;
}cmdArg;

 typedef struct {
int funcIndex;
int (* function)(cmdArg*);
}commands;
typedef struct {
bool usingProcDataCmd;
bool usingSelectRow;
int selectRowIndex;
}errorCheckValues;

//edit table functions
int drow();int icol(); int acol(); int dcol(); int  arow();
//process data functions
int toLower(); int toUpper(); int cset();int copy(); int swap();int irow();int move();int returnAsInt();int roundValueOfCell(); int findCell(); int setCmd();
//select row functions 
int contains();int beginswith();int rows();
//help functions
char * delimSetter(); int delimCount(); int getStrLength(); int setArguments();void parser();void initCheckValues();
void printError();void argInit();void runFunction();int setNumArg(); char *setStrArg(); void cmdInit();

int main (int argc, char * argv[]){

char line [LINE_LENGTH]; 
int exitCode=FAIL_CODE; //Fails by default
errorCheckValues checkValues;
initCheckValues(&checkValues);



commands cmd[argc];
cmdArg args[argc];

int argN=2;
int foundFunc=0;
bool runCmd=true; 
char *delim = " ";

if (argv[1]!=NULL&&strcmp(argv[1],"-d")==0){ //checks if delim is defined
    if (argv[2]!=NULL){
        delim = argv[2];
        argN = 4; //if -d flag is used skips the first 3 indexes of argv
    }else{
        runCmd = false;
        exitCode= NO_ARGS;
        printError(exitCode);
    }
}else{
    argN = 2;
}
for (int i=1;i<argc;i++){
    exitCode=setCmd(&foundFunc,&checkValues,&cmd[foundFunc],argv[i]); //sets cmd function pointer and cmd index
    if(exitCode==SUCCESS_CODE){
         exitCode = setArguments(&args[foundFunc],&argN,cmd[foundFunc].funcIndex,argv,delim); //sets args
         foundFunc++;}
        
    if(exitCode<SUCCESS_CODE){ //checks for errors
        runCmd = false;
        exitCode = NO_ARGS;
        printError (exitCode);
        break;}
}
if (foundFunc==0){ //if no arguments are used
    exitCode = NO_ARGS;
    runCmd = false ;
    printError(exitCode);
}else if (checkValues.usingSelectRow){ //selectRow must be followed by dataProcess command
    if (!(cmd[checkValues.selectRowIndex+1].funcIndex >=9 && cmd[checkValues.selectRowIndex+1].funcIndex <= 16)){
        exitCode = INCORRECT_SELECT_SYNTAX;
        runCmd = false;
        printError (exitCode);
    }
}

    bool printL; //flag to set if line should be print
    for (int l=1;fgets(line,LINE_LENGTH,stdin);l++){
        printL=true; 
        strtok(line,"\n"); //removes all /n characters
        delimSetter(line,delim); //edits line based on delim string
        for (int j=0;j<foundFunc;j++){
                if (runCmd){ //runs cmd only if set args part runned without errors
                args[j].line = line;
                if ((cmd->funcIndex >= 1 && cmd->funcIndex <= 4) || cmd->funcIndex == 17){ //assigns row index if needed
                    args[j].rowIndex = l;
                }
                runFunction(&j,&exitCode,&printL,&runCmd,&args[j],&cmd[j]);
                }
          }
        if(printL){  printf ("%s\n",line);} //Prints line if it wasn't "deleted"
    }
    (exitCode>0)?exitCode=0:exitCode; //if exit code is larger than 0 (non-error) changes to 0 
    return exitCode;
}

void runFunction(int * j,int * errorID,bool *printL,bool * runCmd,cmdArg*args,commands* cmd){
                     
                char ch = getchar();  
                if (ch == EOF){ //if next character is EOF current line is the last one
                if (cmd->funcIndex == 17 || cmd->funcIndex == 2){
                    args->lastLine = true;
                }
                }
                ungetc(ch,stdin);
                
                if(cmd->funcIndex >= 17 && cmd->funcIndex <=19){ //if selectRow function is runned
                  if (cmd->function(args)==1){                  //checks if line should be edited
                       *j+=1;                                   //if not skips the next command (process data cmd)
                   }
                }else {
                    *errorID = cmd->function(args);             //runs function and checks the exit code for errors,
                    if(*errorID < 0){                           //if found it will finish printing without running commands
                        printError(*errorID);
                        *runCmd = false;
                    }else if(*errorID == DELETE_LINE){
                        *printL = false;
                    }
        
            }
}

void printError(int errorID){
    fprintf(stderr,"Error - ");
    switch (errorID){ // prints specific error warning for some of the possible input errors
        case FAIL_CODE:
            fprintf(stderr,"Error incorrect input\n");
            break;
        case FILE_EMPTY:
            fprintf(stderr,"incorrect input: file is empty\n");
            break;
        case NO_ARGS:
            fprintf(stderr,"incorrect input: no arguments entered\n");
            break;
        case INCORRECT_SELECT_SYNTAX:
            fprintf(stderr,"incorrect input: select row command without process data command\n");
            break;
        case COLUMN_NOT_FOUND_CODE:
            fprintf(stderr,"incorrect input: selected column doesn't belong to table\n");
            break;
        case NO_NUM_IN_CELL_CODE:
            fprintf(stderr,"incorrect input: specified cell doesn't contain number\n");
            break;


    }
    

}


int setCmd(int * selectRowIndex,errorCheckValues* flags,commands * cmd,char *argv){
    cmdInit(cmd); //initializes cmd values
    if (argv==NULL){
        return FAIL_CODE;
    }
    if (strcmp(argv,"irow")==0){ //assigns each command their numeric representation and pointer to their function
        cmd->funcIndex = 1;
        cmd->function = &irow;
    }else if (strcmp(argv,"arow")==0){
        cmd->funcIndex = 2;
        cmd->function = &arow;
    }else if (strcmp(argv,"drow")==0){
        cmd->funcIndex = 3;
        cmd->function = &drow;
    }else if (strcmp(argv,"drows")==0){
        cmd->funcIndex = 4;
        cmd->function = &drow;
    }else if (strcmp(argv,"icol")==0){
        cmd->function = &icol;
        cmd->funcIndex = 5;
    }else if (strcmp(argv,"acol")==0){
        cmd->funcIndex = 6;
        cmd->function = &acol;
    }else if (strcmp(argv,"dcol")==0){
        cmd->funcIndex = 7;
        cmd->function = &dcol;
    }else if(strcmp(argv,"dcols")==0){
        cmd->funcIndex = 8;
        cmd->function = &dcol;
    }else if (strcmp(argv,"cset")==0){
        cmd->funcIndex = 9;
        cmd->function = &cset;
    }else if (strcmp(argv,"toupper")==0){
        cmd->function = &toUpper;
        cmd->funcIndex = 10;
    }else if (strcmp(argv,"tolower")==0){
        cmd->function = &toLower;
        cmd->funcIndex = 11;
    }else if (strcmp(argv,"round")==0){
        cmd->function = &roundValueOfCell;
        cmd->funcIndex = 12;
    }else if (strcmp(argv,"int")==0){
        cmd->function = &returnAsInt;
        cmd->funcIndex = 13;
    }else if (strcmp(argv,"copy")==0){
        cmd->function = &copy;
        cmd->funcIndex = 14;
    }else if (strcmp(argv,"swap")==0){
        cmd->function = &swap;
        cmd->funcIndex = 15;
    }else if (strcmp(argv,"move")==0){
        cmd->function = &move;
        cmd->funcIndex = 16;
    }else if (strcmp(argv,"rows")==0){
        cmd->function = &rows;
        cmd->funcIndex = 17;
    }else if (strcmp(argv,"beginswith")==0){
        cmd->function = &beginswith;
        cmd->funcIndex = 18;
    }else if (strcmp(argv,"contains")==0){
        cmd->function = &contains;
        cmd->funcIndex = 19;
    }else{return IGNORE_CODE;} //Ignores if nothing found

    if (cmd -> funcIndex >= 9 && cmd ->funcIndex <= 16){ //makes sure only one process data command is used
        if (flags->usingProcDataCmd==false){
            flags->usingProcDataCmd = true;
            
        }else{ 
            return FAIL_CODE;}
    }else if (cmd -> funcIndex >= 17 && cmd ->funcIndex <= 19){ //makes sure only one selectRow command is used
        flags->selectRowIndex = *selectRowIndex;
        if (flags->usingSelectRow==false){
            flags->usingSelectRow= true;
            
        }else{ return FAIL_CODE;}
    } 



    return SUCCESS_CODE;
}
void cmdInit(commands * cmd){
    cmd->funcIndex = 0;
    cmd->function = NULL;
}

cmdArg customArgs(cmdArg * oArgs,int firstArg,int secondArg, char * stringArg){
    cmdArg nArgs;
    nArgs.line = oArgs->line;
    nArgs.delim = oArgs->delim;
    nArgs.firstArg = firstArg;
    nArgs.secondArg = secondArg;
    nArgs.strArg= stringArg;
    return nArgs;

}
void initCheckValues(errorCheckValues* checkValues){
    checkValues->usingSelectRow = false;
    checkValues->selectRowIndex = false;
    checkValues->selectRowIndex = 0;

}
int setArguments(cmdArg * args,int * argN,int funcIndex,char *argv[],char * delim){
       argInit(args,delim); //initializes args  
       switch (funcIndex){
           case 2: //range for commands that don't require any argument
           case 6:
               return 0;
           case 1:  
           case 3:
           case 5:
           case 7:
           case 10: 
           case 11:
           case 12:
           case 13: //range for commands that require only one numeric arg
                args->firstArg = setNumArg(argv[*argN]); // runs function for save string to int conversion
                *argN +=2; //moves index of argv[] by the number of searched arguments
                return (args->firstArg<1)?FAIL_CODE:SUCCESS_CODE; //checks if any of the args are smaller than 1 
           case 14:                                               //fails program if they are
           case 15:
           case 16: //range for commands that require two numeric args
                args->firstArg = setNumArg(argv[*argN]);
                args->secondArg = setNumArg(argv[*argN+1]);

                *argN+=3;
                return (args->firstArg<1||args->secondArg<1)?FAIL_CODE:SUCCESS_CODE;
            case 4:
            case 8: //range for commands that require two numeric args, but where the first arg needs to be smaller than second
                args->firstArg = setNumArg(argv[*argN]);
                args->secondArg = setNumArg(argv[*argN+1]);

                *argN+=3;
                if (args->firstArg > args->secondArg) {
                    return FAIL_CODE;
                }
                return (args->firstArg<1||args->secondArg<1)?FAIL_CODE:SUCCESS_CODE;

            case 9:
            case 18:
            case 19: //range for commands that require one numeric and one string arg
                args->firstArg = setNumArg(argv[*argN]);
                args->strArg= setStrArg(argv[*argN+1]); //runs function that checks if string is useable
                *argN +=3;
                return (args->firstArg<1)||(args->strArg==NULL)?FAIL_CODE:SUCCESS_CODE;  
            case 17: 
                if (argv[*argN] == NULL || argv[*argN+1] == NULL){
                    return FAIL_CODE;
                }
                if ((strcmp(argv[*argN],"-")==0)&&(strcmp(argv[*argN+1],"-")==0)){ //if first and second argument is - leaves num args at their def value
                }else{
                    args->firstArg = setNumArg(argv[*argN]);
                    if (strcmp(argv[*argN+1],"-")==0){ //if second arg is - then only sets first one
                    }else {
                    args->secondArg = setNumArg(argv[*argN+1]); // otherwise sets both of them
                    }
                }
                *argN+=3;
                return (args->firstArg<0 || args->secondArg <0)?FAIL_CODE:SUCCESS_CODE;
       } 
    return FAIL_CODE;
}
void argInit(cmdArg *args,char *delim){ //sets default values to arg
    args->firstArg = 0;
    args->secondArg = 0;
    args->strArg=NULL;
    args->delim = *delim;
    args->lastLine = false;
}

int setNumArg(char * arg){
    char * ch;
    int tempNum;
    if (arg!=NULL){
           if(isdigit(arg[0])){
               tempNum = strtol (arg,&ch,10);
                if (strcmp(ch,"")==0&&tempNum>=1){
                    return tempNum;
                }
             }
        }
    
    return FAIL_CODE;
}
char * setStrArg (char *arg){
    if (strlen(arg)>100 || arg == NULL){
        return NULL;
    }
    return arg;
}

int irow(cmdArg* args){
    if (args->rowIndex==args->firstArg){ //if function is called on the correct row
        int delimC = delimCount(args->line,&args->delim);
        char nLine[delimC+1];  
        int l=0;
        for (; l<delimC;l++){
             nLine[l]=args->delim; //adds delim based on the value from previos line
        }
        nLine[l]='\0'; // ends the line with null
        printf("%s\n",nLine); 
    }
    return SUCCESS_CODE;
}
int arow(cmdArg* args){
    if (args->lastLine){
        printf("%s\n",args->line); //prints lastLine 
        int delimC = delimCount(args->line,&args->delim);
        char nLine [delimC];
        int l=0;
        for (;l<delimC;l++) {
            nLine[l]=args->delim; //adds delim based on the value from previos line
        } 
        nLine[l]='\0';
        printf("%s\n",nLine); //prints new line and then deletes the old one
        return DELETE_LINE;
        }
    return SUCCESS_CODE;
}
int drow (cmdArg* args){
      if( args->firstArg <=  args->rowIndex &&  args->rowIndex<= args->secondArg ) { // if row is in the range doesn't print line
        return DELETE_LINE;
      }else if (args->firstArg == args->rowIndex && args->secondArg == 0){ //if only one arg is specified and it eq to row index 
        return DELETE_LINE;                                               //doesn't print line
      }
    return SUCCESS_CODE;
}
int icol(cmdArg*args){
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    for (int i = strlen(args->line);i>columnStartIndex;i--){ //moves everything by one
            args->line[i] = args->line[i-1]; 
    }
        if (columnStartIndex==NULL_DELIM){ //if null is the start delim of the first line then it moves to the first character -1 to 0
            columnStartIndex++;
        }
        args->line[columnStartIndex]=args->delim; // adds delim to the newly freed space

        return SUCCESS_CODE;
}
int acol (cmdArg* args){
    args->line[strlen(args->line)]=args->delim; // ads delim to the last array index
    return SUCCESS_CODE;
}
int  dcol (cmdArg*args){
    int l=0;
    do{
        int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
        if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
            printError(COLUMN_NOT_FOUND_CODE);
            return SUCCESS_CODE;
        }
        int lengthOfToken=0; //length of token that is going to be removed
        for (int k = 1;args->line[columnStartIndex+k]!=args->delim&&args->line[columnStartIndex+k]!='\0';k++){ 
            lengthOfToken++; //count number of character until it reaches delim or null
        } 
        if (columnStartIndex==NULL_DELIM){//if null is the start delim of the first line then it moves to the first character -1 to 0
            columnStartIndex++;
        } 
        for (int i = columnStartIndex;args->line[i];i++){  
            args->line[i] = args->line[i+lengthOfToken+1]; //moves everything after the token back by the length of token + 1 
        }
         l++;
    }while((args->firstArg+l)<=args->secondArg); //if second arg was specified loops for the length of the difference

    return SUCCESS_CODE;
}

int cset (cmdArg* args){
    int delimC = delimCount(args->line,&args->delim);
    dcol(args); //deletes cell that is going to be rewritten 
    if (args->firstArg == delimC+1){ //adds new delim for the string to be attached to
        acol(args); //If new delims needs to be added at the end of line acol is used
    }else {icol(args);
    }

    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg); //ignores execution if column doesn't belong to the table
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
        int strLength = strlen(args->strArg);
            for (int i = strlen(args->line);i>columnStartIndex;i--){ // i is defined based on the length of the already scaned array - lengthofthe 
                args->line[i+strLength] = args->line[i]; //moves everything after the token back by the length of token + 1 
            }
                 for (int k = 0;args->strArg[k];k++){
                    args->line[k+(columnStartIndex)+1] = args->strArg[k];
            }

    return SUCCESS_CODE;
}
int toLower(cmdArg* args){
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    for (int i=columnStartIndex+1;args->line[i]!=args->delim&&args->line[i]!='\0';i++){ //iteretes until it finds end of cell
              if (args->line[i]>='A'&&args->line[i]<='Z'){ 
              args->line[i] += 32;} 
   }
    return SUCCESS_CODE;
}

int toUpper(cmdArg* args){
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    for (int i=columnStartIndex+1;args->line[i]!=args->delim&&args->line[i]!='\0';i++){
             if (args->line[i]>='a'&&args->line[i]<='z'){ 
                 args->line[i] -= 32;}
        }
    return SUCCESS_CODE;
}
int roundValueOfCell(cmdArg* args){
    char cell[100];
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array
    char *ch;
    double n = strtod(cell,&ch);
    if (strcmp(cell,"")==0||strcmp(ch,"")!=0){
        printError(NO_NUM_IN_CELL_CODE);
        return SUCCESS_CODE;
    }
    if (n<0.0){ //rounds number
        n -= 0.5;
    }else {
        n += 0.5;}

    char buffer [100]; //puts number to char array and writes it to the line
    sprintf(buffer,"%d",(int)n );
    args->strArg= buffer;
    cset(args);

  return SUCCESS_CODE;  
}
int returnAsInt(cmdArg* args){
    char cell[100];
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg); 
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if column doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parses value of cell into array
    char *ch;
    double n = strtod(cell,&ch);
    if (strcmp(cell,"")==0||strcmp(ch,"")!=0){ //if cell doesn't contain number ignores execution
        printError(NO_NUM_IN_CELL_CODE);
        return SUCCESS_CODE;
    }
   
    char buffer[100];
    sprintf(buffer,"%d",(int)n/1); //puts number to char array and writes it to the line
    args->strArg= buffer;
    cset(args);

    return SUCCESS_CODE;
}
int copy (cmdArg* args){
    char cell[100]; 
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array

    //creates a customm set of argumens for cset this allows usage of required function without the need to rewrite the value of arguments
    cmdArg csetArg = customArgs(args,args->secondArg,0,cell); 
    cset(&csetArg); //writes selected cell to second arg position                                         
    return SUCCESS_CODE;
}
int swap (cmdArg * args){
    char cell [100];
    char cell2 [100];
      
    if (args->firstArg>args->secondArg){ //swaps order of arguments 
        int temp = args->firstArg;
        args->firstArg = args->secondArg;
        args->secondArg = temp;
    } 
      
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    int secColumnStartIndex = findCell(args->line,&args->delim,args->secondArg);
  
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE || secColumnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array
    parser(args->line,cell2,&args->delim,secColumnStartIndex);

    //creates a customm set of argumens for cset this allows usage of required function without the need to rewrite the value of arguments
    cmdArg csetArg = customArgs(args,args->secondArg,0,cell);     
    cset(&csetArg); // writes first cell to the second arg position.                          
    csetArg.strArg= cell2;
    csetArg.firstArg = args->firstArg;  
    cset(&csetArg); //writes second cell to the first arg position


    return SUCCESS_CODE;
}
int move (cmdArg * args){
    char cell [100];
    int delimC = delimCount(args->line,&args->delim);
    if (args->firstArg == args->secondArg){ //if equal should do nothing
        return SUCCESS_CODE;
    } 
    
    if (args->firstArg > delimC+1 || args->secondArg>delimC+1){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return SUCCESS_CODE;
    }
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);

    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array

    //creates a customm set of argumens for cset this allows usage of required function without the need to rewrite the value of arguments
    cmdArg csetArg = customArgs(args,args->firstArg,0,cell);
    int temp= args->firstArg;
    csetArg.firstArg = args->secondArg;
    icol(&csetArg); //adds delim before second arg position
    cset(&csetArg); //writes cell to the newly created column 
    if (args->firstArg>args->secondArg){ //if first arg is larger than second arg the column that needs to be deleted 
        temp++ ;}                       //was moved forword by one (by the creation with icol) and therefore the arg value needs to be adjusted 
        
    csetArg.firstArg = temp;
    dcol(&csetArg); //deletes the original cell
    return SUCCESS_CODE;
}
int rows (cmdArg* args){
     if (args->firstArg == 0 && args->secondArg==0  && args->lastLine) { //if neither of the numeric args was specified and is on last line 
        return SUCCESS_CODE;                                            //equal to - -
    }else if (args->secondArg == 0 && args->firstArg <=args->rowIndex && args->firstArg >= 1){ //if second arg isn't specified and row index is larger than 
        return SUCCESS_CODE;                                                                   //first argument eq:N -
    }else if(args->firstArg <= args->rowIndex && args->secondArg >= args->rowIndex){ // if first arg is smaller than row index and second larger
        return SUCCESS_CODE;
    }
    return IGNORE_CODE;
}

int beginswith (cmdArg* args){
    char cell[100];
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return IGNORE_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array
    for (int i=0;args->strArg[i];i++){ //if substring character don't equal to start string characters ignores line
        if (cell[i]!=args->strArg[i]){
        return IGNORE_CODE;
        }
    }

    return SUCCESS_CODE;
}
int contains (cmdArg* args){
    char cell[100];
    int columnStartIndex = findCell(args->line,&args->delim,args->firstArg);
    if (columnStartIndex==COLUMN_NOT_FOUND_CODE){ //ignores execution if colomn doesn't belong to table
        printError(COLUMN_NOT_FOUND_CODE);
        return IGNORE_CODE;
    }
    parser(args->line,cell,&args->delim,columnStartIndex); //parser value of cell into array
    char * ch = strstr(cell,args->strArg);
    if (ch!=NULL){ //if substring was found in string 
        return SUCCESS_CODE;
    }
    return IGNORE_CODE;
}
int  findCell(char *line,char *delim,int colum){
    int foundDelim=1; //index of the last found delim
    if (colum==1){
        return NULL_DELIM; //symbolic for the start delimeter 
    }
    for (int l=0;line[l];l++){  //loops through the line
        if(line[l]==delim[0])   foundDelim++; //if delim found increment 
        if(foundDelim==colum) return l; // return index of the closing delim of a cell 
        
    }

    return COLUMN_NOT_FOUND_CODE;

}
void parser(char * line,char * cell,char *delim,int columnStartIndex){
    int l=0;
    for (;line[columnStartIndex+l+1]!=*delim&&line[columnStartIndex+l+1]!='\0';l++){ //transfers the value of cell into char array in a function

        cell[l]=line[columnStartIndex+l+1];
    }     
    cell[l]='\0'; // makes sure array ends with null
}
int delimCount(char *line,char * delim){
    int count=0; 
    for (int i=0; line[i]; i++)   count += (line[i] == delim[0]); // loops through line and if line equals delim adds one
    
    return count;
}
int getStrLength(char* line,int startIndex,char* delim){
    // loops through strings and returns it's length
    int length=0;
    for (int k = 1;line[startIndex+k]!=delim[0]&&line[startIndex+(k)]!='\0';k++){ 
        length++;}
    return length;
}
char * delimSetter(char *line,char *delim){
    char * ch = strpbrk (line,delim);  // splits line based on delim array
    for (int l=0;ch != NULL;l++){    // loops until pointer becomes null
        if (line[l]==*ch){            // if char equals to one of the delims
            line[l]=*delim;            //changes all delims to the same one at index 0 
            ch=strpbrk(ch+1,delim);}        // splits string again at the next point
        }
    return delim;
}
