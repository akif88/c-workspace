#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct machine{
    /*use only server other otherwise -1*/
    int server_no;

     /*to enqueue and dequeue item client and server*/
    char *process_queue;
    int queue_max_length;
    int head_queue;
    int tail_queue;

    /*to push and pop item client and server */
    char *interrupt_stack;
    int stack_max_length;
    int top_stack;

    /* history log to client and server */
    char log[200];
    int log_no;

}machine;


char *readfile(char*);
machine *create_client(char*, machine*, int);
machine create_server(char*, machine, int);
void execute_command(char *, int, machine*, machine*, int);
int machine_command_size(char*);
void process(machine*, char, char, int);
void interrupt(machine*, machine*, char, char, int);
void send_data(machine*, machine*, char, char, int);
void operate_data(machine*);
void writefile(char*, machine*, machine*);

/* Stack Data Structure */
int push(char*, int*, char, int);
char pop(char *, int *);

/* Queue Data Structure */
int enqueue(char*, int*, int*, char, int);
char dequeue(char*, int*, int*, int);


int main(int argc, char *argv[])
{
    if(argc != 4)
        exit(0);

    /*read input1 file to read machine number, queue and stack size*/
    char *input_file_one = argv[1];
    char *buffer_machine_info = readfile(input_file_one);

    /*read input2 file to read machine command for client and server*/
    char *input_file_two = argv[2];
    char *buffer_machine_command = readfile(input_file_two);

    /* get machine number */
    int machine_size = machine_command_size(input_file_one);
    /* get command number */
    int command_size = machine_command_size(input_file_two);

    /* create client and server. Also allocated all machine queue and stack */
    machine *client = create_client(buffer_machine_info, client, machine_size);
    machine server = create_server(buffer_machine_info, server, machine_size); /* just one */

    /* execute all command from file */
    execute_command(buffer_machine_command, command_size, client, &server, machine_size);

    char *output_file_name = argv[3];
    writefile(output_file_name, client, &server);


    free(buffer_machine_info);
   /* free(buffer_machine_command); */
    return 0;
}

void execute_command(char *machine_command, int size, machine *client, machine *server, int max_machine_size)
{
    char command, value, machine_no;

    /* execute command */
    int i=0; if(size>=10) i=1; if(size>=100) i=2;
    int loop_number = strlen(machine_command);
    while(i<loop_number)
    {
        command = machine_command[++i];
        machine_no = machine_command[++i];
        value = machine_command[++i];

        switch (command){
        /* new item is added to the client queue with the A command */
        case 'A': process(client, machine_no, value, max_machine_size);
            break;

        case 'I': /* if server, then enter and add item into stack */
            if(max_machine_size == (machine_no-'0'))
                interrupt(client, server, machine_no, value, max_machine_size);
            else /* if client, then enter and add item into stack */
                interrupt(client, server, machine_no, value, max_machine_size);
            break;
        /* send data from client to server */
        case 'S': send_data(client, server, machine_no, value, max_machine_size);
            break;
        /* pop stack and dequeue queue data from server  */
        case 'O': operate_data(server);
            break;

        default: break;

        }// end switch
    }// end while loop

}

/* new item is added to the client queue and
 the queue is checked by the process method */
void process(machine *client, char machine_no, char value, int total_machine)
{
    int client_no = machine_no-'0';
    if(client_no >= total_machine || client_no <= 0){
        fprintf(stderr, "ERROR: number is not in the proper range\n");
        return -1;
    }

    int array_no = client_no - 1;
    /* add process queue new item */
    int is_queue_overflow = enqueue(client[array_no].process_queue, &client[array_no].head_queue,
            &client[array_no].tail_queue, value, client[array_no].queue_max_length);

    /* control overflow if queue overflow, then client log add '1' error no */
    if(is_queue_overflow){
        int no = client[array_no].log_no++;
        client[array_no].log[no]= '1';
    }
}

/* new interrupt item is added to the server stack and client stack,
 the stack is checked by the interrupt method */
void interrupt(machine *client, machine *server, char machine_no, char value, int total_machine)
{
    int client_no = machine_no-'0';
    if(client_no > total_machine || client_no <= 0){
        fprintf(stderr, "ERROR: number is not in the proper range\n");
        return -1;
    }

    if(client_no == total_machine){ /* check if it is a server */
        int is_stack_overflow = push(server->interrupt_stack, &server->top_stack,
              value, server->stack_max_length);

        /* control overflow if stack overflow, then client log add '2' error no */
        if(is_stack_overflow){
            int no = server->log_no++;
            server->log[no]= '2';
        }
    }
    else{
        int array_no = client_no - 1;
        /* check if it is a client */
        int is_stack_overflow = push(client[array_no].interrupt_stack, &client[array_no].top_stack,
              value, client[array_no].stack_max_length);

        /* control overflow if stack overflow, then client log add '2' error no */
        if(is_stack_overflow){
            int no = client[array_no].log_no++;
            client[array_no].log[no]= '2';
        }

    }
}

/* Send data from client to server, first get the interrupts from the stack,
 and if the stack is empty, get the processes from the queue.*/
void send_data(machine *client, machine *server, char machine_no, char value, int total_machine)
{
    int client_no = machine_no-'0';
    if(client_no >= total_machine || client_no <= 0){
        fprintf(stderr, "ERROR: number is not in the proper range\n");
        return -1;
    }

    int array_no = client_no - 1;
    // first, pop item from client
    char item_stack = pop(client[array_no].interrupt_stack, &client[array_no].top_stack);

    if(item_stack != '-'){ // If the client stack has data, get it and push it server stack
        int is_queue_overflow = enqueue(server->process_queue, &server->head_queue, &server->tail_queue,
                            item_stack, server->queue_max_length);
        int no = client[array_no].log_no++;
        client[array_no].log[no]= item_stack; /* add item history log */

         /* control overflow if queue overflow, then server log add '1' error no */
        if(is_queue_overflow){
            int no_s = server->log_no++;
            server->log[no_s]= '1';
        }
    }
    else{ // If the client stack has no data, go to the client's queue and push it server stack
            char item_queue = dequeue(client[array_no].process_queue, &client[array_no].head_queue,
                            &client[array_no].tail_queue, client[array_no].queue_max_length);

            if(item_queue != '-'){ // If the client queue has data, get it and push it server stack
                int is_queue_overflow = enqueue(server->process_queue, &server->head_queue, &server->tail_queue,
                                    item_queue, server->queue_max_length);
                int no = client[array_no].log_no++;
                client[array_no].log[no]= item_queue;

                /* control overflow if queue overflow, then server log add '1' error no */
                if(is_queue_overflow){
                    int no_s = server->log_no++;
                    server->log[no_s]= '1';
                }
            }
            else{ // If the client queue has no data, add '3' error into log and push it server stack
                     int no_c = client[array_no].log_no++;
                     client[array_no].log[no_c]= '3';

            }// end if item_queue

    }// end if item_stack
}

/* get all data from server queue and stack */
void operate_data(machine *server)
{
    char stack_data = pop(server->interrupt_stack, &server->top_stack);

    if(stack_data != '-'){
        int no = server->log_no++;
        server->log[no]= stack_data;
    }
    else{
        char queue_data = dequeue(server->process_queue, &server->head_queue, &server->tail_queue,
                        server->queue_max_length);

        if(queue_data != '-'){
            int no = server->log_no++;
            server->log[no]= queue_data;
        }
        else{
            int no = server->log_no++;
            server->log[no]= '3';
        }// end if queue_data

    }// end if stack_data
}


/* create and allocated all client with their queue and stack */
machine *create_client(char *machine_info, machine *client, int size)
{
    /* last number server size - 1 */
    client = (machine *) malloc((size-1)*sizeof(machine));

    int i=0,j=0; if(size >=10) i=1;
    while(i < strlen(machine_info)-2)
    {    /*allocated all client queue and stack */
        client[j].process_queue = (char*)malloc(machine_info[++i]-'0');
        client[j].queue_max_length = machine_info[i]-'0'; /* '0' convert char to int */
        client[j].head_queue = -1;
        client[j].tail_queue = -1;

        client[j].interrupt_stack = (char*)malloc(machine_info[++i]-'0');
        client[j].stack_max_length = machine_info[i]-'0';
        client[j].top_stack = -1;
        client[j].server_no = -1; /* no server */
        client[j].log_no = 0;

        j++;
    }//end while

    return client;
}

/* create and allocated server with its queue and stack */
machine create_server(char *machine_info, machine server, int size)
{
    /*allocated server queue */
    server.process_queue = (char*)malloc(machine_info[strlen(machine_info)-2]-'0');
    server.queue_max_length = machine_info[strlen(machine_info)-2]-'0';
    server.head_queue = -1; /* queue array start address */
    server.tail_queue = -1; /* queue array end address */
    /*allocated server stack */
    server.interrupt_stack = (char*)malloc(machine_info[strlen(machine_info)-1]-'0');
    server.stack_max_length = machine_info[strlen(machine_info)-1]-'0';
    server.top_stack = -1; /* stack array start address */
    server.server_no = size; /*assign Server Number */
    server.log_no = 0;

    return server;
}

/* write log all clients and server*/
void writefile(char *file_name, machine *client, machine *server)
{
    FILE *file;

    if((file = fopen(file_name, "w"))==NULL)
            exit(0);
    else
    {
        int i,j, total = server->server_no;
        for(i=0; i < total-1; i++)
        {
            int log_number = client[i].log_no;
            for(j=0; j < log_number; j++)
                fprintf(file, "%c ", client[i].log[j]);
            fprintf(file, "\n");
        }

        for(i=0; i < server->log_no; i++)
            fprintf(file, "%c ", server->log[i]);


        fclose(file);
    }
}

/* read given text */
char *readfile(char *file_name)
{
	int file_char;
	char *buffer = (char*) malloc(10000);
	FILE *file;

	if((file = fopen(file_name, "r")) == NULL)
		exit(0);
	else
    {
		int i=0;
		while((file_char=fgetc(file))!=EOF){
           if(file_char != '\n' && file_char != ' ')
                buffer[i++] = (char) file_char;
		}

		buffer = realloc(buffer, i);

		fclose(file);
		return buffer;
	}

	return NULL;
}

/* to get machine size and command size */
int machine_command_size(char *file_name)
{
	char buffer[5];
	FILE *file;
	int size;

	if((file = fopen(file_name, "r")) != NULL)
    {   /* read just first row and get size */
		if(fgets(buffer, 5, file) != NULL)
                size = atoi(buffer);

		fclose(file);
		return size;
	}

	return NULL;
}

/* push and pop methods for stack data structure
used to receive data and send data. */
int push(char *inter_stack, int *top, char item, int max_size)
{
    if(*top<max_size-1)
    {
        ++(*top);
        inter_stack[*top]=item;
        return 0;
    }
    return 1;
}

char pop(char *inter_stack, int *top)
{
    char item;

    if(*top>=0)
    {
        item=inter_stack[*top];
        --(*top);

        return item;
    }

    return '-';
}

/* enqueue and dequeue methods for queue data structure
used to receive data and send data. */
int enqueue(char *proc_queue, int *head, int *tail, char item, int max_size)
{
    if( (*head == (*tail + 1)) || (*head == 0 && *tail == max_size-1))
        return 1;

    if(*head == -1) *head = 0;
    *tail = (*tail + 1) % max_size;
    proc_queue[*tail] = item;
    return 0;
}

char dequeue(char *proc_queue, int *head, int *tail, int max_size)
{
    char item;
    if(*head == -1) return '-';

    item = proc_queue[*head];
    if (*head == *tail){
        *head = -1;
         *tail = -1;
    }
    else {
        *head = (*head + 1) % max_size;
    }
    return item;
}
