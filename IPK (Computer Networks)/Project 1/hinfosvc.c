/* IPK Projekt 1
 *
 * Autor : xjuhas04  
 *  
 *  */


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define OK_REQUEST "HTTP/1.1 200 OK\r\nContent-Type: text/plain;\r\n\r\n"
#define BAD_REQUEST "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain;\r\n\r\nBad Request"
#define INTERNAL_ERR_REQ "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain;\r\n\r\nInternal Server Error\n"

struct cpu {
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
};

//Allocates memory and combines strings 
char * construct_message (char * respond, char * req) 
{
    char * server_message = (char *) malloc(sizeof(char ) * (strlen(req) + strlen(respond)+1));
    if (server_message == NULL)
    {
        return INTERNAL_ERR_REQ;
    }
    strcpy(server_message, OK_REQUEST);
    strcat(server_message, respond);
    return server_message;
}
//reads hostname file and creates respond message
char * hostname() 
{
    char hostname[1024];
    char * server_message;
    FILE * file = fopen("/etc/hostname", "r");
    if (file != NULL)
    {
        fgets(hostname,sizeof(hostname),file);
        fclose(file);
        server_message = construct_message(hostname,OK_REQUEST); 
    }
    else
    {
       server_message = "Failed to open hostname file"; 
    }
    return server_message;
}
///reads cpuinfo file, finds line with cpu-name 
char * cpu_name()
{
    FILE * file = fopen("/proc/cpuinfo","r");
    char * lineF;
    char * server_message = NULL;
    if ( file != NULL )
    {
        char line[256]; 
        for (int line_count = 0;fgets(line, sizeof line, file) != NULL; line_count++) 
        {
            if (line_count == 4)
            {
                lineF = strtok(line,":");
                lineF = strtok(NULL,":");
                server_message = construct_message(++lineF,OK_REQUEST); 
                break;
            }
        }
        fclose(file);
    }
    else
    {
        return construct_message("Failed to read file /proc/cpuinfo\n", INTERNAL_ERR_REQ);
    }
    return server_message;
}
//calculates overall cpu usage based on stats
double calculate_load(struct cpu *prev, struct cpu *cur)
{
    int prev_idle = prev->idle + prev->iowait;
    int idle = cur->idle + cur->iowait;

    int prev_non_idle = prev->user + prev->nice + prev->system + prev->irq + prev->softirq + prev->steal;
    int non_idle = cur->user + cur->nice + cur->system + cur->irq + cur->softirq + cur->steal;

    int prev_total = prev_idle + prev_non_idle;
    int total = idle + non_idle;

    double totald = (double) total - (double) prev_total;
    double idled = (double) idle - (double) prev_idle;

    double cpu_perc = (1000 * (totald - idled) / totald + 1) / 10;

    return cpu_perc;
}
//reads cpu stats from /proc/stat and assigns them to cur cpu struct
int  read_cpustat(struct cpu * cpu_st)
{
    FILE * file = fopen("/proc/stat", "r");
    if (file == NULL) {
        return -1;
    }
    char buffer[256];
    fscanf(file, "%s %lu %lu %lu %lu %lu %lu %lu %lu", buffer, &(cpu_st->user), &(cpu_st->nice), 
        &(cpu_st->system), &(cpu_st->idle), &(cpu_st->iowait), &(cpu_st->irq),
        &(cpu_st->softirq), &(cpu_st->steal));
    fclose(file);
    return 0;
}
//creates 2 cpu structs,reads data for both with 1 second lag in between
//then calculates load and constructs respond
char * load() 
{
    struct cpu cpu_st_prev, cpu_st_cur;
    char * server_message;
    if (read_cpustat(&cpu_st_prev) != 0)
    {
        return construct_message("Failed to read file /proc/cpuinfo\n", INTERNAL_ERR_REQ);
    }
    sleep(1);
    if (read_cpustat(&cpu_st_cur) != 0)
    {
        return construct_message("Failed to read file /proc/cpuinfo\n", INTERNAL_ERR_REQ);
    }
    char buff[1024];
    double perc = calculate_load(&cpu_st_prev, &cpu_st_cur); 
    int rounded_perc  = (int)(perc < 0 ? (perc - 0.5) : (perc + 0.5));
    sprintf(buff,"%d",rounded_perc);
    strcat(buff, "%\n");
    server_message = construct_message(buff,OK_REQUEST); 
    return server_message;
}

//frees memory if it was allocated
void free_memory(char * server_message)
{
    if (strcmp(server_message, BAD_REQUEST) != 0 || strcmp(server_message, INTERNAL_ERR_REQ) != 0)
    {
        free(server_message);
    }

}

//chooses correct respond function based on client input
char * select_function (char * client_message)
{
    char * str_func= strtok(client_message, " ");
    str_func= strtok(NULL," ");
    str_func++;
    if (str_func != NULL)
        {
        if (strcmp(str_func,"hostname") == 0)
        {
             return  hostname();
        }
        else if (strcmp(str_func,"cpu-name") == 0)
        {
            return  cpu_name();
        }
        else if (strcmp(str_func, "load") == 0)
        {
            return  load(); 
        }
        return BAD_REQUEST;
    }
    return NULL;
}
//handles socket communication through always true while loop
int socket_comms(int * server_socket, int * client_socket)
{
    char client_message[256];
    char * server_message = NULL;
    while (1)
    {
        *client_socket = accept(*server_socket, NULL, NULL);
        if (recv(*client_socket, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            if (server_message != NULL)
                free_memory(server_message);
            return -1;
        }
        server_message = select_function(client_message);
        if (server_message == NULL) //no function was selected
        {
            continue;
        }
        //reads all the rest of clients data
        do {
            if (recv(*client_socket, client_message, sizeof client_message, MSG_DONTWAIT) < 0)
            {
                break;
            }
        } while (1);
        
     
        send(*client_socket, server_message, strlen(server_message), 0);
        free_memory(server_message);
        server_message = BAD_REQUEST;
        close(*client_socket);
    }
    return 0;
}
//sets up socket
int construct_socket (int * server_socket, int port_num) 
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_num);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if ( bind(*server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) <0)
    {
        printf("Couldn't bind socket.\n");
        return -1;
    }

    if (listen(*server_socket,0) < 0)
    {
        printf("Couldn't listen.\n");
        return -1;
    }
    return 0;
}
int main (int argc, char * argv[])
{
    int server_socket,client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (isdigit(*argv[1])) //checks if arg is truly number
    {
        if (construct_socket(&server_socket,atoi(argv[1])) != 0)
        {
            return -1;
        }
        if (socket_comms(&server_socket,&client_socket) != 0)
        {
            return -1;
        }
        return 0;
    }
    else
    {
        printf("Incorrect port number");
    }
}

