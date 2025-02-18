#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msn {
    char message[140];
    char username[40];
    int len;
} msg;

void secret_backdoor(void)
{
    char buff[128];

    fgets(buff, 128, stdin);
    system(buff);
    return ;
}

void set_msg(struct msn *msg)
{
    char buff[1024];

    memset(buff, 0, 1024);
    puts(">: Msg @Unix-Dude");
    printf(">>: ");

    fgets(buff, 1024, stdin);
    strncpy(msg->message, buff, msg->len);
    return ;
}

void set_username(struct msn *msg)
{
    char buff[128];
    int i;

    memset(buff, 0, 128);
    puts(">: Enter your username");
    printf(">>: ");
    fgets(buff, 128, stdin);

    i = 0;
    while (i < 41 && buff[i]) {
        msg->username[i] = buff[i];
        i++;
    }

    printf(">: Welcome, %s", msg->username);
    return ;
}

void handle_msg(void)
{
    memset(msg.message, 0, 140);
    msg.len = 140;

    set_username(&msg);
    set_msg(&msg);

    puts(">: Msg sent!");
    return;
}

int main(void)
{
    puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |  \n--------------------------------------------");
    handle_msg();
    return 0;
}