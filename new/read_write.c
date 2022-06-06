#include <stdio.h>
#include <stdlib.h>


int main(){

	FILE *f_read = fopen("readme.txt" , "r");
	FILE *f_write = fopen("writeme.txt" , "w");

	if (f_read ==  NULL){

		printf("can't open file\n");
		return -1;
	} 
	char c ;

	while ( (c = fgetc(f_read)) != EOF ){

		printf("%c",c);
		fputc(c , f_write);
	}

	fclose(f_read);
	fclose(f_write);
	return 0;
}
