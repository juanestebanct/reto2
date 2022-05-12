#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#define MAXIMA_LONGITUD_CADENA 1000
#define CANTIDAD_LINEAS 10
#define NOMBRE_ARCHIVO "texto.txt"

void *salida(void *arg) // hilo que se encarga de crear la cola para que el pueda enviar los mensajes
{
    mqd_t mq1;

    struct mq_attr attr1;
    attr1.mq_flags = 0;
    attr1.mq_maxmsg = 10;
    attr1.mq_msgsize = 64;
    attr1.mq_curmsgs = 0;

    mq1 = mq_open("/mq1", O_WRONLY | O_CREAT, 0644, &attr1);
    char str[64];
    char direccion[60];
    // Arreglo de cadenas: aquí almacenamos todas las palabras
    char palabras[CANTIDAD_LINEAS][MAXIMA_LONGITUD_CADENA];
    // Útil para leer el archivo
    char buferArchivo[MAXIMA_LONGITUD_CADENA];

    while (1)
    {
        fgets(str, sizeof(str), stdin);
        if (str[strlen(str) - 1] == '\n')
            str[strlen(str) - 1] = 0;
        mq_send(mq1, str, strlen(str) + 1, 0);
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
                    printf("%s la direccion del correo es \n", direccion);
                }
                FILE *archivo = fopen(direccion, "r");
                if (archivo == NULL)
                {
                    printf("No se puede abrir el archivo \n");
                }
                else
                {
                    // Necesitamos este ayudante para saber en qué línea vamos
                    int indice = 0;
                    // Mientras podamos leer una línea del archivo
                    while (fgets(buferArchivo, MAXIMA_LONGITUD_CADENA, archivo))
                    {
                        // Remover salto de línea
                        strtok(buferArchivo, "\n");

                        memcpy(palabras[indice], buferArchivo, MAXIMA_LONGITUD_CADENA);

                        // Aumentarlo en cada iteración
                        indice++;
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
                            for (int i = 0; i < CANTIDAD_LINEAS; i++)
                            {
                                mq_send(mq1, palabras[i], 64, 0);
                            }
                        }
                        else if (strncmp(direccion, "invertido", strlen("invertido")) == 0)
                        {
                            printf("se invertido");
                            fgets(str, sizeof(str), stdin);
                            for (int i = CANTIDAD_LINEAS-1; i >=0; i--)
                            {
                                mq_send(mq1, palabras[i], 64, 0);
                            }
                        }else
                        {
                             printf("se enviara de forma normal predeterminada mente");
                            fgets(str, sizeof(str), stdin);
                            for (int i = 0; i < CANTIDAD_LINEAS; i++)
                            {
                                mq_send(mq1, palabras[i], 64, 0);
                            }

                        }
                    }

                  
                }

                break;
            }
        }
    }

    mq_close(mq1);
    mq_unlink("/mq1");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) // se encarga de resivir todos los mensajes y aparte de crear la cola
{
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 64;
    attr.mq_curmsgs = 0;

    mq = mq_open("/mq0", O_RDONLY | O_CREAT, 0644, &attr);

    char buff[32];

    pthread_t threadID1;
    pthread_create(&threadID1, NULL, &salida, NULL);

    while (1)
    {
        mq_receive(mq, buff, 64, NULL);
        printf("Mensaje del otro Usuario: %s\n", buff);
        if (strncmp(buff, "exit", strlen("exit")) == 0)
        {
            break;
        }
    }

    mq_close(mq);
    mq_unlink("/mq0");
    exit(EXIT_SUCCESS);
}