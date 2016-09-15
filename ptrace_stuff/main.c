#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/types.h>

void read_maps(int pid);
void stuferino(int pid);
void forkerino();
void help();

void bin(unsigned n){
    /* step 1 */
    if (n > 1)
        bin(n/2);
    /* step 2 */
    printf("%d", n % 2);
}

int main(int argc, char *argv[]) {
    int option = 0;

    while ((option = getopt(argc, argv, "p:n:")) != -1) {
        switch (option) {
            case 'n' :
                /*printf("name: %s\n", optarg);*/
                forkerino(optarg);
                break;
            case 'p' :
                /*printf("pid: %d\n", atoi(optarg));*/
                break;
            default: help();
                     exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}


void forkerino(char *name){
    int pid = fork();

    if ( !pid ) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        kill(getpid(), SIGSTOP);
        execl(name, name, NULL);
        printf("Uname to execute %s\n");
    } else {
        stuferino(pid);
    } }

void stuferino(int pid){
    int data;
    int status;
    int i;
    int j;
    int normal;
    int revers;
    int flag;
    unsigned long dat;

    struct user* user_space = (struct user*)0;
    struct user_regs_struct regs;

    flag = 1;

    printf("Going haxxor mode on %d\n", pid);
    waitpid(pid, NULL, 0);

    int ret = ptrace(PTRACE_POKETEXT, pid, 0x00400556, 0x458B9090);

    printf("ret = %d\n", ret);
    if ( ret ){
        printf("code injection failed\n");
        exit(1);
    }

    ret = ptrace(PTRACE_PEEKTEXT, pid, 0x400556, NULL );
    printf("%08X\n", ret);

    /*while ( 1 ) {*/
        /*ptrace(PTRACE_SINGLESTEP, pid, 0, 0);*/
        /*wait(&status);*/
    /*}*/

    /*i = 0;*/

    /*ptrace(PTRACE_PEEKTEXT, pid, addr + i * 4, NULL);*/

    /*while ( 0 ) {*/
        /*dat = ptrace(PTRACE_PEEKTEXT, pid, addr + i * 4, NULL);*/
        /*normal = dat;*/

        /*[>printf("%02X\n", normal & 0xFF);<]*/
        /*[>printf("%02X", addr + i * 1, normal & 0xFF);<]*/
        /*printf("<%X> %02X\n", addr + i * 4, normal);*/

        /*if ( addr + i * 4 > 0x4004cc ) {*/
            /*exit(0);*/
        /*}*/

        /*i++;*/
    /*}*/

    while ( 1 ){
        ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
        wait(&status);

        if(WIFEXITED(status)){
            printf("child died? \n");
            break;
        }

        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
        if ( regs.rip > 0x400536 && regs.rip < 0x4005ab ) {
            if ( flag-- ) {//regs.rip == 0x400556 )
                ptrace(PTRACE_POKETEXT, pid, 0x00400556, 0x458B9090);
            }

            /*printf("%d %d %d %d %X %X %X %X \n", regs.rax, regs.rbx, regs.rcx, regs.rdx, regs.rip, regs.rsi, regs.rdi, regs.rsp);*/

            dat = ptrace(PTRACE_PEEKTEXT, pid, regs.rip, NULL);
            printf(" <%08X>  %08X\n", regs.rip, dat);
        }
    }

    waitpid(pid, NULL, 0);
}

void help(){
    printf("Ass tit fuck crap twwat\n");

    return;
}

void read_maps(int pid){
    char mapsFilename[1024];
    sprintf(mapsFilename, "/proc/%d/maps", pid);
    FILE* pMapsFile = fopen(mapsFilename, "r");
    char memFilename[1024];
    sprintf(memFilename, "/proc/%d/mem", pid);
    FILE* pMemFile = fopen(memFilename, "r");
    int serverSocket = -1;

    printf("%s %s\n", mapsFilename, memFilename);

    char line[256];
    while (fgets(line, 256, pMapsFile) != NULL)
    {
        unsigned long start_address;
        unsigned long end_address;
        sscanf(line, "%08lx-%08lx\n", &start_address, &end_address);
        /*printf("%s  \n", line);*/
        printf("%p %p\n", start_address, end_address - start_address);
        /*dump_memory_region(pMemFile, start_address, end_address - start_address, serverSocket);*/
        break;
    }

    fclose(pMapsFile);
    fclose(pMemFile);

}
