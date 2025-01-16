#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, const char **argv, const char **envp) {
    int stat_loc;
    char s[128];
    int v6, v7, v8;
    pid_t v9;

    memset(s, 0, sizeof(s));
    v8 = 0;
    stat_loc = 0;
    v9 = fork();
    if (v9) {
        do {
            wait(&stat_loc);
            v6 = stat_loc;
            if ((stat_loc & 0x7F) == 0 || ((v7 = stat_loc), ((char)((stat_loc & 0x7F) + 1) >> 1) > 0)) {
                puts("child is exiting...");
                return 0;
            }
            v8 = ptrace(PTRACE_PEEKUSER, v9, sizeof(long) * 44, 0);
        } while (v8 != 11);
        puts("no exec() for you");
        kill(v9, SIGKILL);
    } else {
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(s);
    }

    return 0;
}
