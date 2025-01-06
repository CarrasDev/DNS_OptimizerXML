/***********************************/
/*  Autor: Daniel Carrasco Luque   */
/***********************************/

#include <stdio.h>
#include <string.h>
#include "fun_aux.h"


/*  Se utiliza para vaciar el buffer despúes de una llamada de teclado */

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/*  Se utiliza para quitar la \n de un string */

void limpiarBarraN(char* destino, char* origen) {
    int j = 0;
    for (int i = 0; i < strlen(origen); i++) {
        if (origen[i] != '\n') {
            destino[j++] = origen[i];
        }
    }
    destino[j] = '\0';
}


/*	Función generica para realizar preguntas tipo si o no.
    Se le pasa String con la pregunta.
    Retorna: 1 = Si, 0 = No; */

int preguntarSiNo(const char* pregunta) {
    char respuesta[3];

    while (1) {
        printf("%s", pregunta);
        scanf("%2s", respuesta);
        limpiarBuffer();

        if ((strcmp(respuesta, "s") == 0) || (strcmp(respuesta, "S") == 0)) {
            return 1;
        }
        else if ((strcmp(respuesta, "n") == 0) || (strcmp(respuesta, "N") == 0)) {
            return 0;
        }
    }
}


/*	Presentación de la palicacacion */

void presentacion(void) {

    printf("***********************************\n");
    printf("*      Daniel Carrasco Luque      *\n");
    printf("*           Producto 4            *\n");
    printf("***********************************\n\n");
}