#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct linked_list{
    char *word;
    int count;
    char **word_list;

    struct linked_list *node;
}linked_list;


char *readfile(char*);
void word_list(linked_list*,char*);
int is_word_available(linked_list*, char*);
void sort_list(linked_list*);
void append_word(linked_list*, char,int);
void delete_word(linked_list*, char*);
void similarity(linked_list*, linked_list*);

int main()
{
    linked_list *linked_word;
    linked_list *linked_word_2;
    char cmd[2], cmd_1[20], cmd_2[20];
    int count=0;
    while(1){
        scanf("%s", cmd);

         if(strcmp(cmd, "-r")==0){
            scanf("%s", cmd_1);
            readfile(cmd_1);
            if(count==0){
                word_list(linked_word, cmd_1);
                count++;
            }
            else
                word_list(linked_word_2, cmd_1);
        }else  if(strcmp(cmd, "-a")==0){
            scanf("%s %s", cmd_1, cmd_2);
            append_word(linked_word, cmd_1, cmd_2);
        }else  if(strcmp(cmd, "-d")==0){
            scanf("%s", cmd_1);
            delete_word(linked_word, cmd_1);
        }else  if(strcmp(cmd, "-s")==0){
            similarity(linked_word, linked_word_2);
        }else  if(strcmp(cmd, "-q")==0){
            exit(0);
        }
    }

    return 0;
}

void word_list(linked_list *linked_word,char *words)
{
    linked_word = (linked_list*)malloc(sizeof(linked_list));
    char *word = strtok(words, " ");
    linked_word->word = word;
    linked_word->count = 1;

    linked_list *head = linked_word;

    word = strtok(NULL, " ");
    while(word != NULL)
    {
        if(!is_word_available(head, word)){
            while(linked_word->node !=NULL)
                linked_word = linked_word->node;

            linked_word->node = (linked_list*)malloc(sizeof(linked_list));
            linked_word->node->word = word;
            linked_word->node->count = 1;
            linked_word->node->node = NULL;
        }

        word = strtok(NULL, " ");
    }

    sort_list(head);
   /* display_list(head); */

}

int is_word_available(linked_list *linked_word, char *word)
{
    while(linked_word->node != NULL)
    {
        if(strcmp(linked_word->word, word)==0)
        {
            linked_word->count++;
            return 1;
        }

        linked_word = linked_word->node;
    }

    return 0;
}

void sort_list(linked_list *linked_word)
{
    int temp_i;
    char *temp_c;
    int swap = 0;

    do{
            swap=0;
        while(linked_word->node != NULL)
        {
            if(linked_word->count > linked_word->node->count)
            {
                temp_i = linked_word->count;
                temp_c = linked_word->word;
                linked_word->count = linked_word->node->count;
                linked_word->word = linked_word->node->word;
                linked_word->node->count = temp_i;
                linked_word->node->word = temp_c;
                swap =1;
            }

            linked_word = linked_word->node;
        }
    }
    while(swap);
}

void append_word(linked_list *linked_word, char word,int count)
{
    while(linked_word->node !=NULL)
                linked_word = linked_word->node;

            linked_word->node = (linked_list*)malloc(sizeof(linked_list));
            linked_word->node->word = word;
            linked_word->node->count = 1;
            linked_word->node->node = NULL;
}

void delete_word(linked_list *linked_word, char *word)
{
    while(linked_word->node != NULL)
    {
        if(strcmp(linked_word->word, word)==0)
        {
            linked_word = linked_word->node->node;
            break;
        }

        linked_word = linked_word->node;
    }
}

void similarity(linked_list *linked_word, linked_list *linked_word_2)
{
    double count = 0.0;
    double pow_count_A = 0.0;
    double pow_count_B = 0.0;
    double total;
    while(linked_word->node !=NULL){
        count += linked_word->count*linked_word_2->count;

        pow_count_A += pow(linked_word->count, 2.0);
        pow_count_B += pow(linked_word_2->count, 2.0);

        linked_word = linked_word->node;
        linked_word_2 = linked_word_2->node;
    }

    total = sqrt(pow_count_A) * sqrt(pow_count_B);
    double cos_similarity = count/total;
    printf("similarity=%d\n", cos_similarity);

}

/* read given text */
char *readfile(char *file_name)
{
	int file_char;
	char *buffer = (char*) malloc(100000);
	FILE *file;

	if((file = fopen(file_name, "r")) != NULL)
    {
		int i=0;
		while((file_char=fgetc(file))!=EOF){
           if(file_char != '\n' && !ispunct(file_char)){
                if(isupper(file_char))
                    buffer[i++] = tolower(file_char);
                else
                    buffer[i++] = file_char;
            }else
                buffer[i++] = ' ';

		}

		buffer = realloc(buffer, i);
		fclose(file);
		return buffer;
	}

	return NULL;
}

