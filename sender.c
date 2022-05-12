#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#define reglonTotal 1000
#define LineasTotales 100
#define NOMBRE_ARCHIVO "texto.txt"
void* Llegada(void *arg)//usando la segunda cola , el  sender resivira el mensaje del receiver , utilizando el segundo hilo 
{
    mqd_t mq1 = mq_open("/mq1",O_RDONLY);

    char buff[64];

    while (1)
    {
        mq_receive(mq1, buff, 64, NULL);
        if (strncmp(buff, "Mandar", strlen("Mandar")) == 0) 
        {
          
        }
        else{
             printf("Mensaje del otro Usuario: %s\n", buff);
        
        }
        if (strncmp(buff, "exit", strlen("exit")) == 0) {
            break;
        }

    }

    mq_close(mq1);
    mq_unlink("/mq0");
    exit(EXIT_SUCCESS);

}

int main(int argc, char* argv[])//el primer hilo se encarga de  enviar el mensaje del sender al receiver 
{
    mqd_t mq = mq_open("/mq0", O_WRONLY);
    char str[64];
    char direccion[60];
    char palabras[LineasTotales][reglonTotal];
    char buferArchivo[reglonTotal];

    pthread_t threadID1;
    pthread_create(&threadID1,NULL,&Llegada,NULL);

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = 0;
        mq_send(mq, str, strlen(str) + 1, 0);
        if (strncmp(str, "exit", strlen("exit")) == 0)
        {
            break;
        }
        if (strncmp(str, "Mandar", strlen("Mandar")) == 0)
        {
             while (1)
            {
                printf("esperando archivo\n");
                if (fgets(direccion, 60, stdin) != NULL)
                {
                    direccion[strlen(direccion) - 1] = 0;
                
                }
                FILE *archivo = fopen(direccion, "r");
                if (archivo == NULL)
                {
                    printf("No se puede abrir el archivo \n");
                }
                else
                {

                    int contador = 0;

                    while (fgets(buferArchivo, reglonTotal, archivo))
                    {

                        strtok(buferArchivo, "\n");

                        memcpy(palabras[contador], buferArchivo, reglonTotal);


                        contador++;
                    }
                    fclose(archivo);
                    printf("archivo montado , como quieres el orden ,normal o invertido ?\n");
                    if (fgets(direccion, 1000, stdin) != NULL)
                    {
                        direccion[strlen(direccion) - 1] = 0;

                        if (strncmp(direccion, "normal", strlen("normal")) == 0)
                        {
                             printf("se enviara de forma normal");
                            fgets(str, sizeof(str), stdin);
                            for (int i = 0; i < contador-1; i++)
                            {
                                mq_send(mq, palabras[i], 64, 0);
                            }
                        }
                        else if (strncmp(direccion, "invertido", strlen("invertido")) == 0)
                        {
                            printf("se invertido");
                            fgets(str, sizeof(str), stdin);
                            for (int i = contador-1; i >=0; i--)
                            {
                                mq_send(mq, palabras[i], 64, 0);
                            }
                        }else
                        {
                             printf("se enviara de forma normal predeterminada mente");
                            fgets(str, sizeof(str), stdin);
                            for (int i = 0; i < contador-1; i++)
                            {
                                mq_send(mq, palabras[i], 64, 0);
                            }

                        }
                    }

                  
                }
            break;
        }
    }
    }
    mq_close(mq);
    exit(EXIT_FAILURE);
}