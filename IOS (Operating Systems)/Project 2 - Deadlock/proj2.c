/*
File   : proj2.c

Author : Tomas Juhasz

Login  : xjuhas04
*/
#define _XOPEN_SOURCE   600
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    sem_t santa;
    sem_t getHitch;
    sem_t helpingElfs;
    sem_t semElf;
    sem_t hitched;
    sem_t finished;
    sem_t wrtToOut;
    int shmid;
}Semaphores;

typedef struct {
    int NE;
    int NR;
    int TE;
    int TR;
    int shmid;
}Arguments;

typedef struct {
    Semaphores * semaphores;
    Arguments * args;
    bool wrkClosed;
    bool santaNotHelping;
    bool error;
    int reindeers;
    int elfs;
    int shmid;
    int outputCount;
} SharedMem;

void wakeProcesses(SharedMem * sharMem) //wakes all processes in case of an error
{
    for (int i = sharMem->elfs;i > 0; i--)
        sem_post(&sharMem->semaphores->semElf);
    for (int i = sharMem->reindeers;i > 0; i--)
        sem_post(&sharMem->semaphores->getHitch);
    sem_post(&sharMem->semaphores->santa);
    sem_post(&sharMem->semaphores->hitched);
    sem_post(&sharMem->semaphores->finished);

}
void printTofile(SharedMem * sharMem,char * type, char * message,int order)
{
    sem_wait(&sharMem->semaphores->wrtToOut); //makes sure only one process writes at a time
    FILE  * f = fopen("proj2.out", "a"); //appends to a file
    if (!f){ //if file can't be opened 
        fprintf(stderr,"Process couldn't open file");
        sharMem->error = true;
        wakeProcesses(sharMem);
        exit(1);
    }
    if (order != 0) //santa process (no id)
        fprintf(f,"%d: %s %d: %s\n",++sharMem->outputCount,type,order,message);
    else
        fprintf(f,"%d: %s: %s\n",++sharMem->outputCount,type,message);

    fclose(f);
    sem_post(&sharMem->semaphores->wrtToOut); //allows next process to write

}
void elfPrc(SharedMem * sharMem, int i)
{
    if (sharMem->wrkClosed){ //if workshop closed end process
        printTofile(sharMem,"Elf","taking holidays",i);
        sharMem->args->NE--;
        if (sharMem->args->NE == 0){//if last elf wake parrent process end program
            sem_post(&sharMem->semaphores->finished);
        }
        exit(0);
    }

    sharMem->elfs++; //elf comes to the workshop
    if (sharMem->elfs>= 3 && sharMem->santaNotHelping){ //if santa isn't busy and 3+ are waiting
        sharMem->santaNotHelping= false; //santa is now busy
        sem_post(&sharMem->semaphores->santa); //wakes santa
    }
    printTofile(sharMem,"Elf","need help",i);

    sem_wait(&sharMem->semaphores->semElf); //waits to be helped by santa
    if (sharMem->error)
        exit(1);
    else if (!sharMem->wrkClosed){ //if workshop not closed process waits before calling function again
        printTofile(sharMem,"Elf","get help",i);
        sem_post(&sharMem->semaphores->helpingElfs);
    }else
        elfPrc(sharMem,i);
    if (sharMem->args->TE != 0)
        usleep(1000* (rand() % sharMem->args->TE)); 
    elfPrc(sharMem,i);
}

void reindeerPrc(SharedMem * sharMem,int i)
{
    printTofile(sharMem,"RD","return home",i);
    sharMem->reindeers++;
    if (sharMem->reindeers == sharMem->args->NR){ //when all reindeers returned wake santa
        sem_post(&sharMem->semaphores->santa);
    }
    sem_wait(&sharMem->semaphores->getHitch); //wait for santa to get hitched
    if (sharMem->error)
        exit(1);
    printTofile(sharMem,"RD", "get hitched",i);
    sharMem->reindeers--;
    
    if (sharMem->reindeers == 0)
        sem_post(&sharMem->semaphores->hitched); //last reindeer wakes santa to start christmas
        
    exit(0);
}

void initElfs(SharedMem * sharMem)
{
    for (int i = 1; i <= sharMem->args->NE;i++) { //creates NE child processes
        if (fork() == 0) {
            srand(time(NULL)*getpid()); //generates unique rand seed
            printTofile(sharMem,"Elf","started",i); 
            if (sharMem->args->TE != 0)
                usleep (1000 * (rand() % sharMem->args->TE)); // sleeps for rand value in range <0,TE>
            elfPrc(sharMem,i);
        }
    }
}

void initRaindeers(SharedMem * sharMem)
{
    for (int i = 1; i <= sharMem->args->NR;i++) { //creates NR child processes
        if (fork() == 0) {

            srand(time(NULL)*getpid()); //generates unique rand seed
            printTofile(sharMem,"RD","rstarted",i); 
            if (sharMem->args->TR != 0)
                usleep (1000* (sharMem->args->TR/2 + //sleeps for rand value in range  <TR/2,TR>
                                ( rand() % (sharMem->args->TR + 1 - sharMem->args->TR/2)))); 
            reindeerPrc(sharMem,i);
        }
    }
}

void helpElfs(SharedMem * sharMem)
{
    if (!sharMem->wrkClosed){
        printTofile(sharMem,"Santa","helping elves",0);
        for (int i=0;i<3;i++)
            sem_post(&sharMem->semaphores->semElf); //activates the other two elfs

        for (int i=0;i<3;i++)
            sem_wait(&sharMem->semaphores->helpingElfs);

        sharMem->santaNotHelping = true;
        sharMem->elfs-= 3;
    }
}

void santaPrc(SharedMem * sharMem)
{
    while(!sharMem->wrkClosed) { //santas process loops until workshop is closed
        printTofile(sharMem,"Santa","going to sleep",0);
        sem_wait(&sharMem->semaphores->santa);
        if (sharMem->error)
            exit(1);
        else if (sharMem->reindeers == sharMem->args->NR) { //all reindeers returned close workshop
            printTofile(sharMem,"Santa","closing workshop",0);
            sharMem->wrkClosed = true;
            for(int i = sharMem->reindeers;i>0;i--){ //wake reindeers get them hitched
                sem_post(&sharMem->semaphores->getHitch);
            }
        }else if (sharMem->elfs>= 3) {
            helpElfs(sharMem);
        }
    }
    if (sharMem->elfs> 0) {
    for (int i = sharMem->elfs;i > 0;i--)
         sem_post(&sharMem->semaphores->semElf); //wakes all elfs that are waiting for santa
    }
    sem_wait(&sharMem->semaphores->hitched); //waits untill all reindeers are hitched
    printTofile(sharMem,"Santa","Christmas started",0);
    exit(0);
}

void initProcesses (SharedMem * sharMem)
{
    pid_t ppid = getpid(); //pid of the parrent process
    for (int i = 0;i < 3;i++) { //creates 3 child processes
        if (fork() == 0) {
            switch(i) {
                case 0:
                    santaPrc(sharMem);
                    break;
                case 1:
                    initElfs(sharMem);
                    break;
                case 2:
                    initRaindeers(sharMem);
                    break;
            }
        }
        if (getpid() != ppid) //all processes besides parrent are killed
            exit(0);
    }
}
void initSem(SharedMem * sharMem) 
{
    sem_init(&sharMem->semaphores->wrtToOut,1,1);
    sem_init(&sharMem->semaphores->helpingElfs,1,1);
    sem_init(&sharMem->semaphores->semElf,1,0);
    sem_init(&sharMem->semaphores->santa,1,0);
    sem_init(&sharMem->semaphores->hitched,1,0);
    sem_init(&sharMem->semaphores->getHitch,1,0);
    sem_init(&sharMem->semaphores->finished,1,0);
}
 SharedMem * initMem() //initializes sharedMemory and it's default variables
{
    FILE * f = fopen("proj2.out","w"); //creates/rewrittes file
    if (!f){
        fprintf(stderr,"Process couldn't open file");
        exit(1);
    }
    fclose(f);
    //creates shared memmory for all used structures
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMem), IPC_CREAT | 0666);
    int shmidSem = shmget(IPC_PRIVATE, sizeof(Semaphores), IPC_CREAT | 0666);
    int shmidArgs =shmget(IPC_PRIVATE, sizeof(Arguments), IPC_CREAT | 0666);

    if(shmid < 0 || shmidSem < 0 || shmidArgs < 0) {
        fprintf(stderr,"Shared memory could not be created for reason 1 shmid: %d.\n",shmid);
        exit(1);
    }
    SharedMem * sharMem= shmat(shmid, NULL, 0);
    sharMem->semaphores = shmat(shmidSem,NULL,0); 
    sharMem->args = shmat(shmidArgs,NULL,0);

    if((sharMem == (SharedMem *) -1) || (sharMem->args == (Arguments*) -1) ||
            (sharMem->semaphores == (Semaphores*)-1)) {
        fprintf(stderr,"Shared memory could not be created.\n");
        exit(1);
    }
    sharMem->shmid = shmid;
    sharMem->semaphores->shmid = shmidSem;
    sharMem->args->shmid = shmidArgs;

    sharMem->elfs= 0;
    sharMem->reindeers = 0;
    sharMem->santaNotHelping = true;
    sharMem->outputCount = 0;
    return sharMem;
}
void destroyMem(SharedMem * sharMem) //destroys semaphores and clears shared memory
{
    sem_destroy(&sharMem->semaphores->wrtToOut); 
    sem_destroy(&sharMem->semaphores->semElf);
    sem_destroy(&sharMem->semaphores->hitched);
    sem_destroy(&sharMem->semaphores->santa);
    sem_destroy(&sharMem->semaphores->helpingElfs);
    sem_destroy(&sharMem->semaphores->getHitch);
    sem_destroy(&sharMem->semaphores->finished);

    int shmidSem = sharMem->semaphores->shmid;;
    int shmidArgs =sharMem->args->shmid;
    int shmidStru = sharMem->shmid;

    if (shmdt(sharMem->semaphores)||shmdt(sharMem->args)||shmdt(sharMem)) {
        fprintf(stderr,"Error releasing memory\n");
        exit(1);
    }
    shmctl(shmidSem, IPC_RMID, NULL);
    shmctl(shmidArgs, IPC_RMID, NULL);
    shmctl(shmidStru, IPC_RMID, NULL);
}

int setNumArg(char * arg){
    char * ch;
    if (arg){
        for (size_t  i = 0; i < strlen(arg);i++){ //makes sure arguments are digits
           if(!isdigit(arg[i])){
                fprintf(stderr,"Error: input contains non-numeric characters\n");
                exit(1);
           }
        }
    }else{
        fprintf(stderr,"Error NULL argument");
        exit(1);
    }
    return strtol (arg,&ch,10);
}
void  setArgs(char *  inputArgs[],int * argC,Arguments * args)
{
    if (*argC > 4){
        args->NE = setNumArg(inputArgs[1]);
        args->NR = setNumArg(inputArgs[2]);
        args->TE = setNumArg(inputArgs[3]);
        args->TR = setNumArg(inputArgs[4]);
        if (args->NE <= 0 || args->NE >= 1000 || args->NR <= 0 || args->NR >= 20 || args->TE < 0 
                || args->TE > 1000 || args->TR < 0 || args->TR > 1000){
            fprintf(stderr,"Input Error incorrect range selected\n"
                    "Acceptable ranges are:\n" 
                        "NE : <1,999>\nNR : <1,19>\nTE : <0,1000>\nTR : <0,1000>\n");
            exit(1);
        }
    }else {
        fprintf(stderr,"Incorrect amount of arguments \nEntered: %d, should be 4\n",*argC);
        exit(1);
    }
}
int main (int argc, char * argv[])
{
    SharedMem * sharMem = initMem();
    setArgs(argv,&argc,sharMem->args);

    initSem(sharMem);
    initProcesses(sharMem);
    
    sem_wait(&sharMem->semaphores->finished); //waits until all elfs are finished
    destroyMem(sharMem);
    return 0;
}
