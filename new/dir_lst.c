#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


char * current_dir(){

	char *files = malloc(50);

	DIR *d;
	struct dirent *entry ; // d_name  , d_type d_ino < serial number

	d = opendir(".");

	if (d == NULL){
		printf("Error to open dir stream!\n");

	}
	int num = 0;
	while( (entry = readdir(d)) != NULL ){
		if (entry->d_type == DT_REG){
			printf("File : %s \n" , entry->d_name);
			files[num] = entry->d_name;
			num = num + 1;
		}
		else{
		printf("Directory : %s \n" , entry->d_name);
		}
		
	}

	if (closedir(d) == -1){
		printf("Faield to close dir \n");

	}
	return files;
}

int main(){

	char *filess = current_dir();
	int aaa = sizeof(filess);
	for(int a = 0; a <= aaa ;a=a+1){
		printf("%s \n" , &filess[a]);
	}

	printf("%d \n" , sizeof(filess));

	return 0;
}
