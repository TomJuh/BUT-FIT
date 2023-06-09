/*
File: sps.c

Author: Tomas Juhasz

Login: xjuhas04

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <float.h>

#define SUCCES_CODE 0;
#define FAIL_CODE 1;
#define IGNORE_CODE 2;

char * inputString();
int delimSetter();
char ** readLine();
void freeRow();
void failFree();
int printColumn();
int inc(); bool startsWith();
int addRow();int drow(); int sum(); int def();
int addCol(); int dcol(); int swap(); int avg();int use();
int cset(); void selectionInit(); int len();
int addCell(); void initVar();
int clear(); void addRows(); void addCells();

typedef struct{
 char **cell;
}rows_t;

typedef struct{
 rows_t *rows;
 int rowCount;
 int cellCount;
}table_t;

typedef struct {
int R[2];
int C[2]; 
bool wholeCol;
bool wholeRow;
}selection;

typedef struct {
table_t *tab;
char * variable[10];
selection * tempValSel;
int repeat;
int firstArg;
int secondArg;
char * str;
}arguments;

typedef struct {
int funcIndex;
int (* function)(arguments*,selection *);
}commands;
bool startsWith (char * str, char * startStr){ //checks if string starts with substring
    for (int i = 0; startStr[i];i++){
        if (str[i]!=startStr[i]) return false;
    }
    return true;
}
int setCmd(commands * cmd,arguments * args,char *argv){ //sets pointer function and sets up args
    char * ch; 
    args->repeat = 0; //specifies recursive repetition
    if (strcmp (argv,"irow")==0){
        args->firstArg= 1;
        cmd->function = &addRow;
    }else if (strcmp (argv,"arow")==0){
        args->firstArg= 0;
        cmd->function = &addRow;
    }else if (strcmp (argv,"icol")==0){
        args->firstArg= 0;
        cmd->function  = &addCol;
    }else if (strcmp (argv,"acol")==0){
        args->firstArg= 1;
        cmd->function  = &addCol;
    }else if (strcmp (argv,"dcol")==0){
        cmd->function = &dcol;
    }else if (strstr(argv,"set ")!=NULL){
        args->str = strchr(argv,' ')+1;
        if (args->str == NULL)  return FAIL_CODE; 
        cmd->function  = &cset;
    }else if (strcmp(argv,"drow")==0){
        cmd->function = &drow;
    }else if (strcmp(argv,"clear")==0){
        cmd->function = &clear;
    }else if (startsWith(argv,"swap [")){
        args->firstArg = strtol(strchr(argv,'[')+1,&ch,10)-1;
        args->secondArg = strtol(strchr(argv,',')+1,&ch,10)-1;
        if (args->firstArg < 0 || args->secondArg < 0) return FAIL_CODE;
        cmd->function = &swap;
    }else if(startsWith(argv,"avg [")){
        args->firstArg = strtol(strchr(argv,'[')+1,&ch,10);
        args->secondArg = strtol(strchr(argv,',')+1,&ch,10);
        if (args->firstArg < 0 || args->secondArg < 0) return FAIL_CODE;
        cmd->function = &avg;
    }else if(startsWith(argv,"sum [")){
        args->firstArg = strtol(strchr(argv,'[')+1,&ch,10);
        args->secondArg = strtol(strchr(argv,',')+1,&ch,10);
        if (args->firstArg < 0 || args->secondArg < 0) return FAIL_CODE;
        cmd->function = &sum;
    }else if(startsWith(argv,"len [")){
        args->firstArg = strtol(strchr(argv,'[')+1,&ch,10);
        args->secondArg = strtol(strchr(argv,',')+1,&ch,10);
        if (args->firstArg < 0 || args->secondArg < 0) return FAIL_CODE;
        cmd->function = &len;
    }else if (startsWith(argv,"def _")){
        args->firstArg = strtol(strchr(argv,'_')+1,&ch,10);
        if (args->firstArg < 0 || args->firstArg > 9) return FAIL_CODE;
        cmd->function = &def;
    }else if (startsWith(argv,"use _")){
        args->firstArg = strtol(strchr(argv,'_')+1,&ch,10);
        if (args->firstArg < 0 || args->firstArg > 9) return FAIL_CODE;
        cmd->function = &use;
    }else if (startsWith(argv,"inc _")){
        args->firstArg = strtol(strchr(argv,'_')+1,&ch,10);
        if (args->firstArg < 0 || args->firstArg > 9) return FAIL_CODE;
        cmd->function = &inc;
    }
   return SUCCES_CODE; 
}

int inc (arguments * args){ //increments temp value by 1 or sets it to 1 
    if (args->variable[args->firstArg]!=NULL){
        if (isdigit(args->variable[args->firstArg][0])){ //if temp value is a num increment
            double tmp; char * ch;
            char * buffer = malloc(sizeof(char) *strlen(args->variable[args->firstArg])+1); 
            tmp = strtod(args->variable[args->firstArg],&ch);
            tmp++;
            sprintf(buffer,"%.50g",tmp); 

            buffer[strlen(buffer)]='\0';
            free(args->variable[args->firstArg]);
            args->variable[args->firstArg] = realloc(buffer,sizeof(char)*strlen(buffer)+1);
            return SUCCES_CODE;
        } 
    }

    args->variable[args->firstArg] = realloc(args->variable[args->firstArg],sizeof(char )*2); //if temp value is null sets it to 1
    args->variable[args->firstArg][0] = '1';
    args->variable[args->firstArg][1] = '\0';
    return SUCCES_CODE;
}
int def (arguments * args,selection * sel){ //defines temp value and allocates memmory for it
    int length = strlen(args->tab->rows[sel->R[0]].cell[sel->C[0]]);
    args->variable[args->firstArg]=realloc (args->variable[args->firstArg],sizeof(char)*(length+1)); 
    memcpy(args->variable[args->firstArg],args->tab->rows[sel->R[0]].cell[sel->C[0]],length);
    args->variable[args->firstArg][length]= '\0';
    return SUCCES_CODE
}
int use (arguments * args,selection * sel){ //applies temp value to selected cell
    args->str = args->variable[args->firstArg];
    cset(args,sel);

    return SUCCES_CODE;
}
    
int calcLenOfBuffer (int value){ //calculates the length of buffer for num to char * transission
    int result = 0;
    while (value != 0){
       value /=10; 
        result++;
    }
    return result;
}
int len (arguments * args,selection * sel){ //counts the char length in select range
double sum=0;
int row = sel->R[0];
  do{
    int cell = sel->C[0];
    do{
        sum += strlen(args->tab->rows[row].cell[cell]);
        cell++;
    }while (cell <= sel->C[1]); //repeats for each cell in range
    row++;
  }while (row <= sel->R[1]); //repeats for each row in range
    int size = calcLenOfBuffer(sum);//calculates buffer size
    char buffer[size+1];
    sprintf(buffer,"%.50g",sum);
    args->str = buffer;
    selection csetSel;
    selectionInit(&csetSel);// creates custom selection for cset based on args
    csetSel.R[0] = args->firstArg-1;
    csetSel.C[0] = args->secondArg-1;
    cset(args,&csetSel);
return SUCCES_CODE;
}
int avg (arguments * args,selection * sel){ //calculates arithmetic average for selected cells
 char * ch;
 double  sum = 0;
 int count = 0;   
 int row = sel->R[0];
  do{
    int cell = sel->C[0];
    do{
        sum += strtod (args->tab->rows[row].cell[cell],&ch);
        count++;
        cell++;
    }while (cell <= sel->C[1]); //repeats for each cell in range
    row++;
  }while (row <= sel->R[1]); //repeats for each row in range

    sum = (sum)/count;
    int size = calcLenOfBuffer(sum); //calculates buffer size
    char buffer[size+1];    
    sprintf(buffer,"%.50g",sum);
    args->str = buffer;
    selection csetSel;
    selectionInit(&csetSel);// creates custom selection for cset based on args
    csetSel.R[0] = args->firstArg-1;
    csetSel.C[0] = args->secondArg-1;

    cset(args,&csetSel);
return SUCCES_CODE;

}
int sum(arguments * args,selection * sel){ //sums the value of selected cells
 char * ch;
 double  sum = 0;

 int row = sel->R[0];
  do{
    int cell = sel->C[0];
    do{
        sum += strtod (args->tab->rows[row].cell[cell],&ch);
        cell++;
    }while (cell <= sel->C[1]); //repeats for each cell in range
    row++;
  }while (row <= sel->R[1]); //repeats for each row in range

  int size = calcLenOfBuffer(sum); //calculates buffer size
  char buffer[size+1];
  sprintf(buffer,"%.50g",sum);
  args->str = buffer;
  selection csetSel; // creates custom selection for cset based on args
  selectionInit(&csetSel);
  csetSel.R[0] = args->firstArg-1;
  csetSel.C[0] = args->secondArg-1;

  cset(args,&csetSel);
return SUCCES_CODE;
}
int clear (arguments * args,selection * sel){ //frees the memmory on selected cell and replaces it with an empty one
    int row = sel->R[0];
    do {
        int cell = sel->C[0];
        do {
            free(args->tab->rows[row].cell[cell]);
            if (addCell(args->tab,row,cell)==1) return FAIL_CODE;
            cell++;
        }while(cell <= sel->C[1]); //repeats for each cell in range
        row++;
    }while (row <= sel->R[1]); //repeats for each row in range
    return SUCCES_CODE;
}

int swap (arguments * args,selection * sel){
    int targetRow = sel->R[0];
    int  targetCell=sel->C[0];
    do{ //for each cell in range switches selected cell with the one specified with swap args
    char * temp = args->tab->rows[targetRow].cell[targetCell]; 
    args->tab->rows[targetRow].cell[targetCell] = args->tab->rows[args->firstArg].cell[args->secondArg]; 
    args->tab->rows[args->firstArg].cell[args->secondArg] = temp; 
    targetCell++;
    }while (targetCell <= sel->C[1]);

    if (sel->R[0] < sel->R[1]){ //recursively repeats for each row
        sel->R[0]++;
        return swap(args,sel);
    }
    return SUCCES_CODE;
}
int cset(arguments * args,selection * sel){
 int targetCell = sel->C[0];
 int targetRow = sel->R[0]+ args->repeat;//select R[0] + the number of recursive repeats
 do {

    char *str;
    int size = strlen(args->str)+1;//allocs memmory based on the length of args (+1 for null char)
    str = realloc (NULL,(sizeof(char)*size));
    if (!str) {
        free(str);
        return FAIL_CODE;
    }
    int k =0;
    for(;args->str[k];k++){  //writes arg to the new str
        str[k]=args->str[k];
    }
    str[k]='\0';

    free(args->tab->rows[targetRow].cell[targetCell]); //frees the cell
    args->tab->rows[targetRow].cell[targetCell] = str; //writes new str to it
    
    targetCell++;
    }while(targetCell<= sel->C[1]); //repeats for each cell in range

    if (sel->R[0]+args->repeat < sel->R[1]){ //recursively repeats for each row in range
        args->repeat++;
        return cset(args,sel);
    } 

    return SUCCES_CODE;

}
int dcol(arguments * args,selection * sel){
    int cellsInRow =  args->tab->cellCount /args->tab->rowCount; 
    int targetCell = sel->C[0] + args->repeat;//select C[0] + the number of recursive repeats

    for (int i=0;i<args->tab->rowCount;i++){ //frees the column and moves everything back
        for (int l = targetCell;l<cellsInRow;l++){
           if(l==targetCell)free(args->tab->rows[i].cell[targetCell]);
           args->tab->rows[i].cell[l] =  args->tab->rows[i].cell[l+1]; 
        } 
        args->tab->cellCount--;
    }
     if (sel->C[0] + args->repeat< sel->C[1]){ //repeats recursively for each column in select range
        args->repeat++;
        return dcol(args,sel);
    }    
    return 0;
}
int addCol(arguments * args,selection * sel){
    int cellsInRow =  args->tab->cellCount /args->tab->rowCount; 
    int targetCell = sel->C[0] + args->repeat; //select C[0] + the number of recursive repeats
    if (args->repeat != 0) targetCell +=args->repeat; //on repetition target cell needs to moved by the number of already created ones
    if (args->firstArg!= 1) targetCell-- ;//argument used to differentiated between icol and acol (icol=1 acol=0 )

       for (int i=0;i<args->tab->rowCount;i++){ //adds a cell for each row
            args->tab->cellCount++;
            args->tab->rows[i].cell= realloc (args->tab->rows[i].cell,sizeof(char*)*(args->tab->cellCount+1));
            for (int l = cellsInRow;l > targetCell;l--){ //makes space for the new cell
                args->tab->rows[i].cell[l+1] =  args->tab->rows[i].cell[l]; 
            } 

        if(addCell(args->tab,i,targetCell+1)) return FAIL_CODE;
    }
    if (sel->C[0] + args->repeat< sel->C[1]){ //recursive repetition for each column in select range
        args->repeat++;
       return  addCol(args,sel);
    }
    if (args->repeat != 0 && sel->wholeCol) sel->C[1]+=args->repeat +1;
      return SUCCES_CODE;
}


int addRow (arguments * args,selection * sel){
    int cellsInRow = args->tab->cellCount /args->tab->rowCount; 
    int targetRow = sel->R[0]+args->repeat;//select R[0] + the number of recursive repeats
    args->tab->rows[args->tab->rowCount].cell= malloc (sizeof(char*)*(args->tab->cellCount+1)); //allocs the new row

    if (args->repeat != 0) targetRow+=args->repeat; //on repetition target row needs to moved by the number of already created ones
    if (args->firstArg!= 1) targetRow++; //argument used to differentiated between arow and irow (arow=0 irow=0)
    for (int i=args->tab->rowCount;i>=targetRow;i--){
         for (int l = 0; l<cellsInRow;l++){
            if (i==targetRow){
                if (addCell(args->tab,(targetRow),l) == 1) return FAIL_CODE;
            }else{
             args->tab->rows[i].cell[l] = args->tab->rows[i-1].cell[l]; 
            }
        }
    } 

    args->tab->rowCount++;
    args->tab->rows = realloc(args->tab->rows,sizeof*args->tab->rows*(args->tab->rowCount+1));
    args->tab->cellCount+= cellsInRow;

    if (sel->R[0]+args->repeat< sel->R[1]){
        args->repeat++;
        return addRow(args,sel);
    }
    if (args->repeat != 0 && sel->wholeRow) sel->R[1]+=args->repeat +1;

    return SUCCES_CODE;
}
int drow (arguments * args,selection * sel){ //deletes rows
   int cellsInRow = args->tab->cellCount/args->tab->rowCount; 
   int targetRow = sel->R[0]+args->repeat; //select R[0] + the number of recursive repeats
   for (int i=targetRow;i<args->tab->rowCount-1;i++){//frees every cell on a row
        for (int l = 0; l<cellsInRow;l++){
            if(i==targetRow)free(args->tab->rows[targetRow].cell[l]);
            args->tab->rows[i].cell[l] = args->tab->rows[i+1].cell[l];//otherwise move everything down
        }
   } 
   args->tab->cellCount-= cellsInRow; //calculates new cell and row count
   args->tab->rowCount--;
   
   free(args->tab->rows[args->tab->rowCount].cell);
    if (sel->R[0]+args->repeat< sel->R[1]){ //repeats recursively for all rows in select
        args->repeat++; 
        return drow(args,sel);
    } 
    return SUCCES_CODE;
}

int addCell(table_t * tab,int row,int column){ //adds a single empty cell to a row
    char * str = malloc(sizeof(char));
    if (!str){
        free(str);
        return FAIL_CODE;
    }

    str[0]='\0';
    tab->rows[row].cell[column] = str;
    return SUCCES_CODE;
}

void evenOutRow (table_t * tab,int newCellCount,int row){ //evens out the row
    int diff =abs((tab->rowCount*newCellCount) - tab->cellCount); 
    if(newCellCount<=tab->cellCount/tab->rowCount){//if this row has more cells than the previous one
        for (int l = row;l>0;l--){ //go backwards and add cells to the other rows
             for (int k = 0;k<diff;k++){
                addCell(tab,l-1,newCellCount+(k));
                tab->cellCount++;
                tab->rows[l-1].cell= realloc (tab->rows[l-1].cell,sizeof(char*)*(tab->cellCount+1));
             }  
        }
    }else { //if it has fewer cells than previous ones
         for (int l = 0;l<diff;l++){ //add that number of cells
            addCell(tab,row,newCellCount-(diff-l));
            tab->cellCount++;
            }
    }
}

int setTable(table_t *tab,FILE * fp,char *delim){
  int ch;
  int currCellInRow=0;
  tab->rowCount = 0; //init of tab num values
  tab->cellCount = 0;
  tab->rows = malloc (sizeof*tab->rows);

  for (int i=0;EOF!=(ch=fgetc(fp));i++){ //adds rows to a table until EOF

    ungetc(ch,fp);
    tab->rows[i].cell= readLine(&tab->rows[i],&tab->cellCount,fp,delim);
    if (!tab->rows[i].cell){
        failFree(tab);
        tab->rowCount = 0;
        tab->cellCount = 0;
        return FAIL_CODE;
    }

    tab->rowCount++;
    tab->rows = realloc(tab->rows,sizeof*tab->rows*(tab->rowCount+1));
    if (currCellInRow != 0 && currCellInRow*(i+1) != tab->cellCount){//if new row has diffrent cell count than previous ones 
        evenOutRow(tab,currCellInRow,i);   //evens out the table 
    }

    currCellInRow = tab->cellCount/tab->rowCount; //sets the current cells in one row
  }
  return SUCCES_CODE;

}
char ** readLine(rows_t*rows,int * rowC,FILE *fp,char *delim){
    int ch;
    rows->cell= malloc (sizeof*rows->cell);
    for(int i=0;'\n'!=(ch=fgetc(fp));i++){//adds cells to a row until new line char
    if (ch == EOF){ //or EOF
        ungetc(ch,fp);
        break;
    }
    
    ungetc(ch,fp); //returns tested char 
    rows->cell[i] = inputString (fp,delim);//adds cell to a row
    
    *rowC+=1;    
    rows->cell= realloc(rows->cell,sizeof(char*)*(*rowC+1));
    if (!rows->cell[i]){
        for (int l = 0;l<=i;l++){
            free(rows->cell[l]);
        } 
        free(rows->cell);   
        return NULL;
    }
    
  }

    return realloc(rows->cell,sizeof(char*)*(*rowC+1));
}
char *inputString(FILE* fp,char * delim){
    char *str;
    int ch;
    size_t size = 16;
    size_t len = 0;
    bool inQuotes=false;
    str = malloc(sizeof(char)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp))&& (!(strchr(delim,ch) != NULL) || inQuotes)){ //collects chars from file until EOF or delim
                                                                            //that isn't in quotes
        if (ch == '\\'){ // if \is found skip the condition in while
             str[len++]=ch;
             str = realloc(str, sizeof(char)*(size+=16));
             ch = fgetc(fp);
        }else if (ch == '"'){
            inQuotes= !inQuotes;
        }
        if (ch == '\n'||ch ==EOF ) {
            ungetc(ch,fp);
            break;
        } 
        str[len++]=ch;
        
         if(len==size){ //reallocates more memmory after every 16 chars
            str = realloc(str, sizeof(char)*(size+=16));
        }
        if (!str){
            free(str);
            return NULL;
         }
    }
    str[len++]='\0'; //makes sure char * ends with null
    return realloc(str, sizeof(char)*len);
}
int printColumn(table_t * tab,int cellCount){ //finds out if trailling columns should be printed
    int lastC=0;
    for (int i = 0;i<tab->rowCount;i++){
        for (int l = lastC;l<cellCount;l++){
            if (*tab->rows[i].cell[l]!='\0') lastC=l;
        }
    }

    return (cellCount == lastC+1) ? cellCount+1 : lastC;
}
void printLine (table_t *tab,char * file,char delim){ //writes date to a file and frees memory
    FILE * fp = fopen(file,"w");
    int cellC = tab->cellCount/tab->rowCount; //cellCount on one row
    int dontPrintC = printColumn(tab,cellC); //finds out if there are empty trailling col
    for (int i=0;i<tab->rowCount;i++){
        for (int l =0;l<cellC;l++ ){
            if (dontPrintC >= l){//if l is bigger than first the first Col that shouldn't be printed don't print
            if (l!=0) fprintf(fp,"%c",delim);//skips printing del at the beginning
            fprintf(fp,"%s",tab->rows[i].cell[l]);}
            free(tab->rows[i].cell[l]);
        }
        free(tab->rows[i].cell);
        fprintf(fp,"\n");
    }
    free(tab->rows);
    fclose(fp);
}

void failFree(table_t *tab){ //frees memory without writing anything
    for (int i=0;i<tab->rowCount;i++){
        for (int l =0;l<(tab->cellCount/tab->rowCount);l++ ){
            if (!tab->rows[i].cell[l]) continue;
            free(tab->rows[i].cell[l]);
        }
        free(tab->rows[i].cell);
    }
}
void addCells(arguments * args,int newCellC){ //adds cells based on selection extending the table
    selection sel;
    sel.C[0] = args->tab->cellCount/args->tab->rowCount -1 ;
    sel.C[1] = 0;
    args->repeat = 0;
    args->firstArg = 1;
    for ( ;sel.C[0]<newCellC;sel.C[0]++){
        addCol(args,&sel);
    }
}
void addRows(arguments * args,int newRowsC) { //adds rows based on selection extending the table
    selection sel;
    sel.R[0] = args->tab->rowCount-1;
    sel.R[1] = 0;
    args->repeat = 0;
    args->firstArg = 0;
    for ( ;sel.R[0]<newRowsC;sel.R[0]++){
        addRow(args,&sel);
    }
}
int setSelect (arguments * args,selection * sel,char * selection,int index){ //sets numeric selectionx1
    if (strchr(selection,',') == NULL) {
        return IGNORE_CODE;
    }
    if (index != 1) selectionInit(sel); //restarts the value of sel
    char * tmp;
    int num = strtol (++selection,&tmp,10);
    if(*tmp==',') { //if first nom-num char is , sets numeric R sel
        if (num < 1) return FAIL_CODE;
        if (num > args->tab->rowCount)addRows(args,num-1); //adds rows if sel extends the current table
        sel->R[index] = num -1; //-1 to adjust to array indexes         
    } else if (*tmp=='_'||*tmp=='-'){ //if first non-num char is _ select the whole row
        sel->wholeRow= true;
        if (index==0)sel->R[0]=0;
        sel->R[1] = args->tab->rowCount -1;
    }else {return IGNORE_CODE;}

    selection = strchr(selection,','); //the same thing just for cells 
    num = strtol(++selection,&tmp,10);
    int cellsInRow = args->tab->cellCount/args->tab->rowCount;
    if(*tmp==',' || *tmp ==']') {
        if (num < 1) return FAIL_CODE;
        if (num > cellsInRow) addCells(args,num); 
        sel->C[index] = num-1;
    } else if (*tmp=='_'||*tmp=='-'){
        sel->wholeCol = true;
        if (index==0)sel->C[0]=0;
        sel->C[1] = cellsInRow -1;
    } else {return IGNORE_CODE;}  
    if (index != 1 && strchr(selection,',')!=NULL){ // char * select still contains , run again for second select
        selection = strchr(selection,','); 
        return setSelect(args,sel,selection,++index);
    }

    return SUCCES_CODE;
}
char *parseCMD (char * cmd,int * currentCh){ //parses up cmd string
    char * buffer = malloc(sizeof(char) * strlen(cmd)+1); 
    if (!buffer) return buffer;
    int i = 0;
    for (;cmd[i];i++){
        if (cmd[i]==';'){
            if (cmd[i-1]!='\\') { //if ; wasn't prefaced with \ cmd is parsed
                break;
            }
        }
        buffer[i] = cmd[i];
    }
            
    buffer[i] = '\0';  //puts null char at the end 
    *currentCh += i+1; //moves the index by the amount of chars parsed (+1 for ;)
    return buffer;
}
void selectionInit (selection * sel){
    sel->R[0] = 0;
    sel->R[1] = 0;
    sel->C[0] = 0;
    sel->C[1] = 0;
    sel->wholeRow = false;
    sel->wholeCol = false;
}
int findStr (selection * sel,table_t * tab,char *strToFind){//finds the string containing substring
   strtok(++strToFind,"]");  //gets rid of []
   int row = sel->R[0];
   do{
    int cell = sel->C[0];
    do{
            if (strstr(tab->rows[row].cell[cell],strToFind)!=NULL){  
                sel->C[0] = cell;
                sel->R[0] = row;
                sel->C[1] = 0;
                sel->R[1] = 0;
                return SUCCES_CODE;
            }
            cell++;
        }while(cell <= sel->C[1]);
       row++;
   }while(row <= sel->R[1]);
   return FAIL_CODE;
}
int findMin (selection * sel,table_t * tab) { //finds the cell with the lowest num value
   double min=DBL_MAX;
   double tmp=DBL_MAX;
   char * ch;
   int row=sel->R[0];
     do {
         int cell=sel->C[0];
         do{
           tmp = strtod(tab->rows[row].cell[cell],&ch);
           if (*ch == '"' && isdigit(ch[1])) tmp = strtod(++ch,&ch); //for cells inside ""
           if (tmp == 0 && *ch == *tab->rows[row].cell[cell]) tmp = DBL_MAX; //makes sure that if tmp is 0 it's not because num wasn't found
           if ( tmp < min){
                 min = tmp; 
                 sel->R[0] = row;
                 sel->C[0] = cell;
            }
           cell++;
         }while(cell<=sel->C[1]);
        row++;
    }while(row<=sel->R[1]);
   sel->R[1] = 0; 
   sel->C[1] = 0;
   if (min == DBL_MAX) return FAIL_CODE;
   return SUCCES_CODE;
}
int findMax (selection * sel,table_t * tab){ //finds the cell with the highest num value 
   double max=DBL_MIN;                       
   double tmp;
   char * ch;
     int row = sel->R[0]; 
     do{
         int cell = sel->C[0];
         do{
           tmp = strtod(tab->rows[row].cell[cell],&ch);
           if (*ch == '"' && isdigit(ch[1])) tmp = strtod(++ch,&ch);//for cells inside "" 
           if ( tmp > max){                                          
                 max = tmp; 
                 sel->R[0] = row;
                 sel->C[0] = cell;
            }
            cell++;
         }while(cell<=sel->C[1]);
         row++;
    }while(row<=sel->R[1]);
   sel->R[1] = 0; 
   sel->C[1] = 0;
   if (max == DBL_MIN) return FAIL_CODE;
   return SUCCES_CODE;

}
int setTempSel(arguments * args,selection * sel){ //sets temp variable select
    for (int i=0;i<=1;i++){
        args->tempValSel->R[i] = sel->R[i];
        args->tempValSel->C[i] = sel->C[i];

    }
    return SUCCES_CODE;
}
int useTempSel(arguments * args,selection * sel){ //writes values stored 
    for (int i = 0;i<=1;i++){                   //in temp select to global select
        sel->R[i] = args->tempValSel->R[i]; 
        sel->C[i] = args->tempValSel->C[i]; 
    }
    return SUCCES_CODE;

}
bool isNumSelect (char * cmd){ //checks if select starts with [num,
        if (cmd[0]=='['&& strlen(cmd) >= 1){
            if (isdigit(cmd[1])) return true;
        }
    
    return false;
}
int SelectionSpec (arguments * args,selection * sel,char * cmd){ //decides the type of 
    if (strcmp(cmd,"[max]")==0){                                //select
       return (findMax(sel,args->tab));
    }else if (strcmp(cmd,"[min]")==0){
       return (findMin(sel,args->tab));
    }else if (startsWith(cmd,"[find ")){
       return findStr(sel,args->tab,strchr(cmd,' ')); 
    }else if (strcmp(cmd,"[set]")==0){
        return setTempSel(args,sel);
    }else if (strcmp(cmd,"[_]")==0){
        return useTempSel(args,sel);
    }else if (isNumSelect(cmd) || startsWith(cmd,"[_")){ //for select starting with
        return setSelect(args,sel,cmd,0);                        //[x, or [_,
    }
      
    return IGNORE_CODE; //ignore specifies that no selection cmd was found and that parsed cmd is a function cmd

} 
void freeTempVariables(arguments * args){ //frees temp val
    for (int i=0;i<=9;i++){
        free(args->variable[i]); 
    }
}
void initVar(char *  var[]){ //initializes temp var values
    for (int i=0;i<=9;i++){
        var[i] = realloc (NULL,sizeof(char));
        var[i][0]='\0';
    }
}
int readCmd(table_t * tab,char *argv){
    if (argv[strlen(argv)-1]!=';') argv[strlen(argv)]=';'; //if argv doesnt end with ; adds one to the end
    commands cmd;
    cmd.function = NULL;
    arguments args;
    args.tab = tab;
    selection sel,tempValSel;
    args.tempValSel = &tempValSel;
    initVar(args.variable); //init temp values
    selectionInit(&sel); // init selection and temp value selection
    selectionInit(args.tempValSel);
    int charInd=0; //index of character from which to create a pointer
    char * cmdParsed = NULL; 
    int errorC=0;

  for (int i=0;strchr(&argv[charInd],';')!=NULL;i++){
        if (cmdParsed) free(cmdParsed); //if tmp is not void free
        cmdParsed = parseCMD(&argv[charInd],&charInd); //parses arg
        if (!cmdParsed){
            errorC = 1;
            break;
        }
        while (errorC != 2){ //runs until selection section finishes (with Ignore code)
        errorC=SelectionSpec(&args,&sel,cmdParsed);
            if (errorC!=2)  { //if selection still isnt finished
                free(cmdParsed); 
                if (errorC==1) break; //breaks if selection specification was wrong or malloc fails
                cmdParsed= parseCMD(&argv[charInd],&charInd);//parses the next part
             }
        }
        
        errorC = setCmd (&cmd,&args,cmdParsed); //sets function pointer and req args
        
        if (errorC == 1 || cmd.function == NULL){ //if arg doesn't contain func
            break;
        }
         if (cmd.function(&args,&sel)==1){
            break;
         }
         
        cmd.function=NULL; //restarts pointer
    }
     
    free (cmdParsed); 
    freeTempVariables(&args);
    return errorC;
}

int main (int argvc, char *argv[]){
  table_t tab;
  int fileIndex = argvc-1; 
  int exitCode = 0;
  char * delim = " ";
  FILE *fp; 
  if (strcmp(argv[1],"-d")==0){ //if d flag is included 
    delim = argv[2];            //delim should be specified at 2 argv index
    fileIndex = 4;              //and file at 4
  } 

  if ((fp = fopen(argv[fileIndex],"r"))!=NULL){
    if (setTable(&tab,fp,delim)==1){ //sets up the table
        fprintf(stderr,"Malloc error");
        exitCode = -1;
    }
    fclose(fp);
  }else{
        fprintf(stderr,"File not found");
        return FAIL_CODE;
  }

  if (tab.rowCount == 0 || tab.cellCount == 0) { 
    free(tab.rows); 
    return exitCode;
  }
    exitCode = readCmd(&tab,argv[fileIndex-1]);
    if (exitCode == 1) failFree(&tab); //if malloc failed at any point free everything
    else printLine(&tab,argv[fileIndex],*delim);
    return exitCode;
  
  }
