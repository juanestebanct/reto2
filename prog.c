#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>


int main(int argc, char *argv[])
{

    // char character;
    // int counter;

    // char mensaje[20];
    // //char *mensajeDir[] = malloc(sizeof(mensaje)); ???

    // while (1)
    // {
    //     fgets(str, sizeof(str), stdin);
    //     if(str[strlen(str) - 1 ] == '\n') str[strlen(str) - 1 ] = 0;
    //     mq_send(mq, str, strlen(str) + 1, 0);
    //     if (strncmp(str, "exit", strlen("exit")) == 0)
    //     {
    //         break;
    //     }
    // }

    //declaramos los Hilos del proceso 1
    pthread_t receiver1;

    //creamos la cola sobre la cual se va escribir el mensaje enviado por el proceso 1

     mqd_t mq1;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;


    mq1 = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);
    char buff[32];
     while (1)
    {
        fgets(buff, sizeof(buff), stdin);
        if(buff[strlen(buff) - 1 ] == '\n') buff[strlen(buff) - 1 ] = 0;
        mq_send(mq1, buff, strlen(buff) + 1, 0);
        if (strncmp(buff, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }

    

}