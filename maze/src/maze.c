/*  Software Lab. I
    Aim: Finding a possible path in the maze

    Compile: gcc [-o maze] maze.c -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *readfile(char *);
char **create_maze(char *, int *);
void find_path(char **, int);
void writefile(int *, int);


int main(int argc, char *argv[])
{
    if(argc < 2)
		exit(0);

    /*give file name*/
	char *file_name = argv[1];
	/* give maze element in buffer*/
	char *buffer = readfile(file_name);

    /* find maze array length and fill maze array with buffer */
    int *lenght;
    char **maze = create_maze(buffer, lenght);

    /* find possible path*/
    find_path(maze, *lenght);

    free(buffer);
    free(maze);

	return 0;
}


void find_path(char **maze, int array_lenght)
{
    char **maze_path = maze;
    int max_lenght = array_lenght;
    int *direction = (int *)malloc(max_lenght*sizeof(int));
    int re_path[max_lenght];
    int maze_border = sqrt(max_lenght);
    char path[4]={'-'}; /*store each direction*/

    /*stack start point*/
    int top_repath=-1;
    int top_direction=-1;

    /* give finding key in array  */
    char path_key[20];
    int key_no = 0; /* given key number*/


    /* The number of steps that need to be returned.
        It is used to return to intersecting paths and
        to take back possible paths to the stack.
    */
    int step_number = 0;


    int j; /*find Start point in Array */
    for(j=0; j< maze_border;j++)
        if(maze_path[0][j]=='S') break;
    maze_path[0][j]='0';

    /* start travel in maze */
    int i=0; char door='0';
    while(1)
    {
        /* border control in array */
        if(j+1 < maze_border)
            path[0] = maze_path[i][j+1]; /*east*/
        if(i+1 < maze_border)
            path[1] = maze_path[i+1][j]; /*south*/
        if(j-1 >= 0)
            path[2] = maze_path[i][j-1]; /*west*/
        if(i-1 >= 0)
            path[3] = maze_path[i-1][j];  /*north*/


        /* Possible options to check */
        int d, sum=0;
        for(d=0; d < 4; d++){
            if(path[d] == '0') /* sum for counter number of ways to go */
                sum++; /* sum for counter number of ways to go */
            if(path[d] >= 'A' && path[d]<='Z') /* door control */
                if(path[d] != 'E'){ /* exit control */
                    int a; /* if there is a key open door and go */
                    for(a = 0; a < key_no; a++){
                        if(path_key[a] == (path[d]+32)){
                               /* find direction to door */
                            if(d == 0){
                                maze_path[i][j+1] = '0';
                                door = path[d];
                                path[d] = '0';
                            }
                            else if(d == 1){
                                maze_path[i+1][j] = '0';
                                door = path[d];
                                path[d] = '0';
                            }
                            else if(d == 2){
                                maze_path[i][j-1] = '0';
                                door = path[d];
                                path[d] = '0';
                            }
                            else if(d == 3){
                                maze_path[i-1][j] = '0';
                                door = path[d];
                                path[d] = '0';
                            }

                            sum++;
                        }
                    }
            }
            if(path[d] == 'E')
                break; /* if exit then finish program */
            if(path[d] >= 'a' && path[d] <= 'z')
                break; /* if key then give them */
        }

        if(path[d] == 'E'){ /* exit direction push stack end exit program */
            push(direction, (d+1), &top_direction, max_lenght);
            push(direction, 5, &top_direction, max_lenght);
            break;
        }


        /* if given a key */
        if(path[d] >= 'a' && path[d] <= 'z')
        {
            path_key[key_no] = path[d]; /* given which key */
            /*if there is no exit when going on a road */
            push(re_path, ((i*maze_border)+j), &top_repath, max_lenght);
            /* on which path the received key is */
            push(direction, (d+1), &top_direction, max_lenght);
            key_no++; /* increase key counter */

            /* the step number is 1 on each intersection and
                on the path that is the key. So we can see
                how many wrong path are on the way back.
            */
            step_number=1;

            int a,b;
            for(a = 0; a < maze_border; a++)
                for(b=0; b < maze_border; b++)
                    if(maze_path[a][b] == '2')
                        maze_path[a][b] = '0';


            /* store key direction in stack */
            if(d == 0){
                maze_path[i][++j] = '2';
                push(direction, path[d], &top_direction, max_lenght); /*east*/
            }
            else if(d == 1){
                maze_path[++i][j] = '2';
                push(direction, path[d], &top_direction, max_lenght); /*south*/
            }
            else if(d == 2){
                maze_path[i][--j] = '2';
                push(direction, path[d], &top_direction, max_lenght); /*west*/
            }
            else if(d == 3){
                maze_path[--i][j] = '2';
                push(direction, path[d], &top_direction, max_lenght); /*north*/
            }

        }
        else if(sum >= 1){ /* open path. possible direction number except key */
            if(sum > 1){/* store the path back */
                push(re_path, ((i*maze_border)+j), &top_repath, max_lenght);

                /* the step number is 1 on each intersection and
                on the path that is the key. So we can see
                how many wrong path are on the way back.
                */
                step_number=1;
            }

            /* the wrong way is the number
                                of steps that need to be returned */
            if(sum == 1)
                step_number++;

            /* keep the direction of the path going */
            if(path[0] == '0'){
                maze_path[i][++j] = '2';
                push(direction, 1, &top_direction, max_lenght); /*east*/
            }
            else if(path[1] == '0'){
                maze_path[++i][j] = '2';
                push(direction, 2, &top_direction, max_lenght); /*south*/
            }
            else if(path[2] == '0'){
                maze_path[i][--j] = '2';
                push(direction, 3, &top_direction, max_lenght); /*west*/
            }
            else if(path[3] == '0'){
                maze_path[--i][j] = '2';
                push(direction, 4, &top_direction, max_lenght); /*north*/
            }

            /* If the path going is the door, take the door */
            if(door != '0')
                push(direction, door, &top_direction, max_lenght);

        }
        else if(sum==0){ /* if no way turn back previous intersection */

            int return_path = pop(re_path, &top_repath);

            j = return_path % maze_border;
            i = (int)(return_path - j)/maze_border;

            int k;
            for(k=0; k < step_number; k++)
                pop(direction, &top_direction);

        }

        door='0';/* clear door */

        int k; /* clear direction path */
        for(k=0; k< 4; k++)
            path[k] = '-';



    }/*end while*/

   /* int asd;
    for(asd=0; asd <= top_direction+150; asd++)
        printf("%d", pop(direction, &top_direction)); */

    writefile(direction, top_direction);
    /*return direction; */
}

/* create maze array, fill array with buffer and find array length*/
char **create_maze(char *buffer, int *lenght)
{
    char **maze;
    int maze_lenght = sqrt(strlen(buffer));
    *lenght = strlen(buffer);


    int i,j,k=0;
    maze = (char **) malloc(maze_lenght*sizeof(char *));
    for(i=0; i < maze_lenght; i++)
        maze[i] = (char *)malloc(maze_lenght*sizeof(char));


    for(i=0; i < maze_lenght; i++)
        for(j=0; j < maze_lenght; j++)
            maze[i][j] = buffer[k++];


    return maze;
}

/* write finding a maze path*/
void writefile(int *direction, int max_dir)
{
    FILE *file;

    if((file = fopen("path.txt", "w"))==NULL)
            exit(0);
    else
    {
        fprintf(file,"%s ", "Start");

        int i;
        for(i=0; i< max_dir; i++){
            if(direction[i] > 4)
                fprintf(file,"%c ",(char) direction[i]);
            else
            {
                /*convert each direction number to char*/
                if(direction[i] == 1)
                    fprintf(file,"%c ", 'E');
                else if(direction[i] == 2)
                    fprintf(file,"%c ", 'S');
                else if(direction[i] == 3)
                    fprintf(file,"%c ", 'W');
                else if(direction[i] == 4)
                    fprintf(file,"%c ", 'N');

            }
        }

        fprintf(file,"%s", "Exit");
        fclose(file);
    }
}

/* read given text */
char *readfile(char *file_name)
{
	int file_char;
	char *buffer = (char*) malloc(900);
	FILE *file;

	if((file = fopen(file_name, "r")) == NULL)
		exit(0);
	else
    {
		int i=0;
		while((file_char=fgetc(file))!=EOF){

            if(file_char != '\n'){
                buffer[i] = file_char;
                i++;
            }
		}

		fclose(file);
		return buffer;
	}

	return NULL;
}

/* use stack store possible path and intersecting paths */
void push (int stack[],int item,int *top,int max_size)
{
    if(*top<max_size-1)
    {
        ++(*top);
        stack[*top]=item;
    }

}
 /* pop intersection paths and wrong path */
int pop(int stack[],int *top)
{
    int item;

    if(*top>=0)
    {
        item=stack[*top];
        --(*top);
    }

    return (item);
}

