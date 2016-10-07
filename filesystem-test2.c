
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <zconf.h>

#include "evaluate.h"




char * getRandomString(int len, unsigned seed){
    int flag, i;

    srand((unsigned) time(NULL) + seed);
    
   //printf("%llu\n",(unsigned long long) getTime());
    
    char *string;
    if ((string = (char *) malloc(sizeof(char) * len)) == NULL){

        printf("Malloc Failed \n");
        return NULL;
    }
    for (i = 0; i < len - 1; i++){
        flag = rand() % 3;

        switch (flag)
        {
            case 0:
                string[i] = 'A' + rand() % 26;
                break;
            case 1:
                string[i] = 'a' + rand() % 26;
                break;
            case 2:
                string[i] = '0' + rand() % 10;
                break;
            default:
                string[i] = 'x';
                break;

        }
    }

    string [len - 1] = '\0';
    return string;
}

int main(int argc, char *argv[]) {
    int len = 1024;
    
   // char *s = getRandomString(len);
    int totalfiles = 100;
    int i;
    totalfiles = atoi(argv[2]);    
    int file_fds[totalfiles];

    memset(file_fds, -1, sizeof(file_fds));

    char stri[10];

    //totalfiles = (int) atoi(argv[2]);
 

    printf("TEST BEGIN!\n");
    
    printf("TEST DIR:%s\n",argv[1]);
    
    printf("TOTAL Files:%d\n", totalfiles);

    
    double start, end;

    double total_time= 0.0;
    //char *content;
    for (i = 0; i < totalfiles; i++){
        char  file_path[80]; 
	strcpy(file_path,argv[1]);
	sprintf(stri,"%d", i);
        strcat(file_path, stri);

        int content_len = 1024 * 4;
        char *content = getRandomString(content_len, (unsigned) i);




        start = getUTtime();
        file_fds[i] = open(file_path, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
        write(file_fds[i], content, sizeof(char) * content_len);
        fsync(file_fds[i]);
        end = getUTtime();
        total_time +=  end - start;
        close(file_fds[i]);
        free(content);
    }
    
    
    printf("%lf s\n", total_time);
    printf("%lf files/s\n", totalfiles / total_time);

//    printf ("%s\n", s);
    return 0;
}
