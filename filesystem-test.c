
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <zconf.h>
#include <pthread.h>
#include <assert.h>
#include "evaluate.h"





inline int min(int a, int b){
    
    return a < b ? a:b;
}



struct creat_file_param_t {

    
    int filesnum;

    char  **file_path;
    char **content;
   
    int content_len;


};


typedef struct creat_file_param_t creat_file_param;



void *create_file_single(void *param)
{
    creat_file_param *p = (creat_file_param *) param;

    int i;
    int fd = -1;
 
    for (i = 0; i < p->filesnum; i++){
        fd = open(*(p->file_path + i), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
 	//printf("content:%s %d\n",*(p->content + i), p->content_len);	
        write(fd, *(p->content + i), sizeof(char) * p->content_len);
        //printf("content_len: %d \n", p->content_len);
	fsync(fd);
	 
	close(fd);	
	
//	printf("success !\n");
    }
}

void parallel_creat_file(int filesnum, char **path, char **content, int content_len, int threads_num){
	
    assert (path != NULL);
    assert (content != NULL);

    long long offset = 0;

    pthread_t threads[threads_num];

    creat_file_param params[threads_num];

    int i;
    
    for (i = 0; i < threads_num; i++){
	creat_file_param p;
        p.file_path = path + offset;
        p.content  = content + offset;
        p.content_len = content_len;
        p.filesnum = min( (filesnum+ threads_num - 1)  / threads_num,  filesnum -  offset);
        params[i] = p;
	offset += (filesnum + threads_num - 1) / threads_num;
	
	pthread_create(threads + i, NULL, create_file_single, (void *)(params + i));
    }

    for(i = 0; i < threads_num; i++){
	
	pthread_join(threads[i], NULL);
    
    }
}


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


char **getFilesContent(int filesnum, int content_len){

    int i;
    char **content;

    content = (char **)malloc(sizeof(char*) * filesnum);
    

    for(i = 0; i < filesnum; i++){
	
	*(content + i) = getRandomString(content_len, (unsigned) i);
	
    }

    return content;
    
}




char **getFilesPath(int filesnum, char *base_dir){

    int i;
    char **filepath;

    filepath = (char **)malloc(sizeof(char *) * filesnum);

    
    
    char str[25];

    for (i = 0; i < filesnum; i++){
	//char *filename = base_dir;
	char filename[100];
	*(filepath + i) = (char *)malloc(sizeof(char) * 125);

	strcpy(filename, base_dir);
	
	sprintf(str, "%d", i);

	strcat(filename, str);
	
	strcpy(*(filepath + i), filename);
//	*(filepath + i) = filename;	
			
    }

    return filepath;

    
}








int main(int argc, char *argv[]) {
    int len = 1024;
// Usage: /gfapi-filesystem-test  dir filesnum threadsnum;    
   // char *s = getRandomString(len);
    int totalfiles = 100;
    int i;
    totalfiles = atoi(argv[2]);
    
    printf("TEST BEGIN!\n");
    
    printf("TEST DIR:%s\n",argv[1]);
    
    printf("TOTAL Files:%d\n", totalfiles);
  

 
         
    double start, end;

    double total_time= 0.0;
   
    int content_len = 1024 * 4;  
   
    int threads_num = 4;

    if(argc >= 4){
	threads_num = atoi(argv[3]);
    }
    
    printf("Threads num: %d\n", threads_num);   
    char **content = getFilesContent(totalfiles, content_len);

    char **filepath = getFilesPath(totalfiles, argv[1]);
    

    start = getUTtime();
    parallel_creat_file(totalfiles, filepath, content, content_len, threads_num);
    end = getUTtime();
	    
        
/*
    //char *content;
    for (i = 0; i < totalfiles; i++){
       // glfs_fd_t *fd = NULL;
//        char  file_path[80]; 
//	strcpy(file_path,argv[1]);
        //itoa(i, stri, 10);
//        sprintf(stri, "%d", i);
//        strcat(file_path, stri);

//        int content_len = 1024 * 4;
       // printf("%c\n", *getRandomString(content_len));
//	printf("here:%u\n", getTime());
 //       char *content = getRandomString(content_len, (unsigned) i);
	
//	printf("%s\n", *(content + i));
	//printf("%s\n",content);

//	printf("%s\n", *(filepath + i));
    //    printf("%s\n", file_path);

        start = getUTtime();
        fd = glfs_creat(fs2, *(filepath + i), O_RDWR, 0644);
        glfs_write(fd, *(content + i), sizeof(char) * content_len, 0);
        glfs_fsync(fd);
        end = getUTtime();
        total_time +=  end - start;
        glfs_close(fd);
 //       free(content);
    }
    
 */ 
    total_time = end - start;  
    printf("%lf s\n", total_time);
    printf("%lf  4k files/s\n", totalfiles / total_time);
    
    for (i = 0; i < totalfiles; i++){
	free(*(content + i));
	
	free(*(filepath + i));
    }

    free(content);
    free(filepath);
//    printf ("%s\n", s);
    return 0;
}
