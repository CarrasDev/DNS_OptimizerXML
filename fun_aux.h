/***********************************/
/*  Autor: Daniel Carrasco Luque   */
/***********************************/

#ifndef FUN_AUX_H
#define FUN_AUX_H





/*	Se utiliza para vaciar el buffer despúes de una llamada de teclado */

void limpiarBuffer();


/*	Se utiliza para quitar la \n de un string */

void limpiarBarraN(char* destino, char* origen);


/*	Función generica para realizar preguntas tipo si o no.
*	Se le pasa String con la pregunta.
*   Retorna: Si = 1, No = 0;  */

int preguntarSiNo(const char* pregunta);


/*	Presentación de la palicacacion */

void presentacion(void);

#endif