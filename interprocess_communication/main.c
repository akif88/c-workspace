#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void child_process(int **, int *, int);
void file_writer(int **, int, int, int);
int **file_process(char *, int *);

int main(int argc, char *argv[])
{
    char *file_name = "matrix.txt";

    /* create nxn matrix with matrix.txt */
    int matrix_size;
    int **matrix = file_process(file_name, &matrix_size);


    int N = matrix_size;
    int matrix_read[N][N]; /* matrix to be read via child process */
    int fd[2]; /* file descriptors for pipe */
    pipe(fd); /* for communication between processors  */

    /* to fork child process with pid and wait process with wpid and status variable  */
    int pid[N], wpid,child_status;
    int i,j,k,n; /* for loop variables */

    /* give a argument user */
    int process_piece;
    if(argc == 2)
        process_piece = atoi(argv[1]);
    else
        process_piece = 3; /* If you take too much argument or do not take an argument */

    /* for loop until Argument value */
     for(k = 0; k < process_piece; k++){

        if((pid[k]=fork()) == 0){ /* create child process */
            child_process(matrix, fd, N); /* child process method */
            close(fd[0]);close(fd[1]); /* close pipe file descriptors */

            /* Because the matrix multiplication takes place in the first cycle
                and no data is received with the pipe. */
            if(k != 0)
                file_writer(matrix , N, k, getpid());
            exit(k);
        }
        else /* previous child processor read data */
            if((n=read(fd[0],  matrix_read, sizeof(int)*N*N))>=0){
                for(i = 0; i < N; i++)
                    for(j= 0; j < N; j++)
                        matrix[i][j] = matrix_read[i][j]; /* to get value in reading data */
        }

    }



    i = 0; /* Wait for the child processors in front */
    while ((wpid = waitpid(pid[i++], &child_status, 0)) > 0);


    free(matrix);

    return 0;
}

/* for matrix multiplication in child process */
void child_process(int **matrix, int *fd, int matrix_size)
{
    int i,j,p, N = matrix_size;
    int result[N][N];

    /* to get result to multiplication */
    for(i = 0; i < N; i++)
       for(j= 0; j < N; j++)
        result[i][j]=0;

    /* Matrix Multiplication */
    for(i = 0; i < N; i++)
       for(j= 0; j < N; j++)
          for(p = 0; p < N; p++)
             result[i][j] += matrix[i][p]*matrix[p][j];

    /* write data file descriptors for the next child to read */
    write(fd[1], result, sizeof(int)*N*N);

}

/* write text matrix multiplication results */
void file_writer(int **matrix, int N, int child_no, int process_id)
{
    int i, j;
    FILE *matrix_file_write;
    char text[10];

    sprintf(text, "%d", child_no); /* child process no */
    strcat(text, ".txt"); /* concatenate process no */

    /* write data for matrix multiplication */
    if((matrix_file_write=fopen(text, "w")) != NULL){

         fprintf(matrix_file_write, "Process-%d %d", child_no, process_id);
         for(i = 0; i < N; i++){
                fprintf(matrix_file_write, "\n");
                for(j= 0; j < N; j++)
                      fprintf(matrix_file_write, "%d\t", matrix[i][j]);}

        fclose(matrix_file_write);
    }

}


/* read matrix.txt file and matrix array memory allocation  */
int** file_process(char *file_name, int *size)
{
    char matrix_numbers; /* to get file matrix.txt row/column number */
    FILE *matrix_file;

    int **matrix;         /* nxn matrix */
    int  matrix_n;       /* for matrix size  */

    int i,j;


    if((matrix_file = fopen(file_name, "r")) != NULL){

        char n[1];
        n[0] = fgetc(matrix_file); /* matrix.txt first row matrix size */
        matrix_n = atoi(n);

        /* matrix dynamic memory allocation with matrix_n variable matrix size */
        matrix = (int **)malloc(matrix_n*sizeof(int*));
        for(i = 0; i < matrix_n; i++)
            matrix[i] = (int *)malloc(matrix_n*sizeof(int));

        /* to tokenize characters */
        char number_tokenize[matrix_n*matrix_n]; i =0 ;
        while((matrix_numbers = fgetc(matrix_file)) != EOF){ /* read char by char*/
                if(matrix_numbers != ',' && matrix_numbers != '\n') /* just take the number */
                    number_tokenize[i++] = matrix_numbers;
        }

        /* to assign numbers to rows and columns */
        int count = 0;
        for(i = 0; i < matrix_n; i++)
            for(j = 0; j < matrix_n; j++)
                matrix[i][j] = number_tokenize[count++] - '0';

        fclose(matrix_file);

    }


    *size = matrix_n; /* to give matrix size*/
    return matrix;

}



