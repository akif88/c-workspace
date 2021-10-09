#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

int main(int argc, char *argv[])
{
    char *local_buff, *buffer;
    int total_char, local_char;
    int comm_size; //total process
    int my_rank;    // rank for each process
    MPI_Comm comm;


    //to start MPI
    MPI_Init(NULL, NULL);

    comm = MPI_COMM_WORLD;
    //send process total process size
    MPI_Comm_size(comm, &comm_size);
    //send process self number
    MPI_Comm_rank(comm, &my_rank);

    read_file(my_rank, comm_size, buffer, &local_char, &total_char);
    local_buff = (char *)malloc(local_char*sizeof(char));

    send_thing(my_rank, comm_size, local_buff, buffer, &local_char, comm);

    write_thing(my_rank, comm_size, local_buff, buffer, &local_char, comm);

    MPI_Reduce()

    if(my_rank == 0)
        printf("%c", buffer[0]);


    //to terminate MPI
    MPI_Finalize();
    return 0;
}


void read_file(int my_rank, int comm_size, char *buffer, int *local_ch, int *total_ch)
{
    if(my_rank == 0)
    {
        int file_char;
        buffer = (char*) malloc(10000);
        FILE *file;

        if((file = fopen("encodedfile.txt", "r")) != NULL)
        {
            int i=0;
            while((file_char=fgetc(file))!=EOF)
                    buffer[i++] = (char) file_char;
            *total_ch = i;
            buffer = realloc(buffer, i);
            fclose(file);
        }// end if. read file in main process

    }//process 0

    *local_ch = *total_ch/comm_size;
}


void send_thing(int my_rank, int comm_size, char *local_buff, char *buffer, int local_ch, MPI_Comm comm)
{
    if(my_rank == 0)
    {
        if(buffer != NULL)
            MPI_Scatter(buffer, local_ch, MPI_CHAR, local_buff, local_ch, MPI_CHAR, 0, comm);
    }


}

void write_thing(int my_rank, int comm_size, char *local_buff, char *buffer, int local_ch, MPI_Comm comm)
{
    if(my_rank == 0)
    {
        if(buffer != NULL)
            MPI_Gather(local_buff, local_ch, MPI_CHAR, buffer, local_ch, MPI_CHAR, 0, comm);
    }

}



