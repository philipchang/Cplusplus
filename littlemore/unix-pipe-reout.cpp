#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void printmain()
{
   printf("it is in main process\n");
}

int main(void)
{
    int n = 0;
    int fd[2];
    pid_t pid;
    char line[256]={0};

    if(NULL == freopen("des.log","w",stdout))
       printf("reopen error\n");

    if(pipe(fd) < 0)
        printf("pipe error\n");
    printf("%d %d\n", fd[0],fd[1]);
    
    if( (pid=fork() ) < 0)
        printf("fork error\n");
    else if(pid > 0){
        sleep(2);
        close(fd[0]);
        write(fd[1],"pipe come\n", 10);
    } else{
        close(fd[1]);
        printmain();

        printf("sub fork waitting\n");
        n = read(fd[0],line,256);
        write(STDOUT_FILENO, line, n);
        fflush(stdout);

        printf("exec ret %d\n", execv("exceshell.sh",NULL));
        printmain();
    }
    

    fclose(stdout);
    return 0;
}