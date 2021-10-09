#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

#define FILENAME "/var/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);
void fatal(char *);

int main(int argc, char *argv[]){
	int userid, printing=1, fd;
	char search_str[100];

	if(argc > 1)
		strcpy(search_str, argv[1]);
	else
		search_str[0] = 0;

	userid = getuid();
	fd = open(FILENAME, O_RDONLY);
	if(fd == -1)
		fatal("in main() while opening fle for reading");

	while(printing)
		printing = print_notes(fd, userid, search_str);

	printf("------[end of note data]------\n");
	close(fd);
}

//print note given uid
//return 0 end of file,return 1 still more notes
int print_notes(int fd, int uid, char *search_str){
	int note_lenght;
	char byte=0, note_buffer[100];

	note_lenght = find_user_note(fd, uid);
	if(note_lenght == -1)
		return 0;

	read(fd, note_buffer, note_lenght);
	note_buffer[note_lenght] = 0;

	if(search_note(note_buffer, search_str))
		printf(note_buffer);
	return 1;
}

//find the next note for given uid
//return -1 if the end of the file is reached
//otherwise, return the lenght of the found note.
int find_user_note(int fd, int user_uid){
	int note_uid=-1;
	unsigned char byte;
	int length;

	while(note_uid != user_uid){
		if(read(fd, &note_uid, 4) != 4)
			return -1;
		if(read(fd, &byte, 1) != 1)
			return -1;

		byte = length = 0;
		while(byte != '\n'){
			if(read(fd, &byte, 1) != 1)
				return -1;
			length++;
		}
	}
	lseek(fd, length*-1, SEEK_CUR);

	printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
	return length;
}

//search a note for a given keyword
//return 1 if match, return 0 if no match
int search_note(char *note, char *keyword){
	int i, keyword_lenght, match=0;

	keyword_lenght = strlen(keyword);
	if(keyword_lenght == 0)
		return 1;

	for(i=0; i < strlen(note); i++){
		if(note[i] == keyword[match])
			match++;
		else{
			if(note[i] == keyword[0])
				match = 1;
			else
				match = 0;
		}
		if(match == keyword_lenght)
			return 1;
	}//end for
	return 0; //not match
}






















