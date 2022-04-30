//
// Created by ball on 4/28/22.
//

#include "kernel/types.h"
#include "user/user.h"
void pipeline(int);
int main(int argc,char const *argv[]) {
    int p[2];
    pipe(p);

    if (fork() == 0) { // start pipeline
        close(p[1]);  //close write side
        pipeline(p[0]);
        exit(0);
    }else {
        close(p[0]);
        int i;
        for (i = 2;i<=35;i++) {
            write(p[1],&i, sizeof(int)); // not calling func pipeline but write into pipe p
        }
        close(p[1]);
        wait((int*)0); //wait until child precess ends
        exit(0);
    }
}

// fd is read side of pipe
void pipeline(int fd) {
    int p,n;
    if (0 == read(fd,&p,sizeof (p))){
        close(fd);
        exit(0);
    }
    printf("prime %d\n",p);

    //start a new pipe
    int newPipe[2];
    pipe(newPipe);

    if (0 == fork()) {
        close(newPipe[1]);
        pipeline(newPipe[0]);
    }else {
        close(newPipe[0]);
        for (;;){
            if (0 == read(fd,&n, sizeof(n))) {
                close(fd);
                break;
            }
            if (n % p != 0) {
                write(newPipe[1],&n, sizeof(n));
            }
        }
        close(newPipe[1]);
        wait((int*)0);//wait until child precess ends
        exit(0);
    }
}