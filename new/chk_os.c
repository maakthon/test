#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <sys/stat.h>



#if defined(_WIN32)
	#define PLATFORM_NAME "windows_both" // Windows

#elif defined(_WIN64)
	#define PLATFORM_NAME "windows64" // Windows

#elif defined(__linux__)
	#define PLATFORM_NAME "linux maak comp" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other

#else
	#define PLATFORM_NAME NULL
#endif


const char *get_platform_name() {
    return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}


int main(int argc, char *argv[]) {
    puts(get_platform_name());
    return 0;
}

    #define PLATFORM_NAME NULL
