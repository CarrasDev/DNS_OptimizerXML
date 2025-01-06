/***********************************/
/*      Daniel Carrasco Luque      */
/*           Producto 4            */
/*                                 */
/*   Generación de un documento	   */
/*               XML               */
/***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "procesos.h"


int main(void) {

	AdaptadorRed adaptRed;
	memset(&adaptRed, 0, sizeof(adaptRed)); // Inicializar a 0


	// Permite seleccionar el adaptador y retorna el apadtador y las DNS
	seleccionarAdaptadorRed(&adaptRed);


	// Obtenemos IP, Mascara de subred y puerta de enlace
	printf("\n\nObtener informacion del adaptador... ");
	obtenerIpPuertaMascara(&adaptRed);
	printf("Finalizado.\n");


	// Calculo tiempo medio ping
	printf("Obtener tiempo medio de respuesta... ");
	if (!strcmp(adaptRed.dnsPrimario, "ninguno")) {
		adaptRed.tiempoMedio = -1;
		printf("No calculado.\n");
	}
	else {
		adaptRed.tiempoMedio = tiempoMedioPing(adaptRed.dnsPrimario);
		if (adaptRed.tiempoMedio == -1) {
			printf("Error al calcular.\n");
		}
		else {
			printf("Finalizado.\n");
		}
	}
	

	// Calculo de saltos y obtencion de ip de los saltos
	printf("Obtener cantidad de saltos e IPs...  ");
	if (adaptRed.tiempoMedio == -1) {
		printf("No Calculado.\n");
		adaptRed.saltos = 0;
	}
	else {
		obtenerSaltos(&adaptRed);
		printf("Finalizado.\n");
	}
	

	// Crear fichero XML
	printf("Generando fichero XML...");
	crearXML(&adaptRed);
	printf("             Finalizado.");



	// Mostrar datos completos del adaptador
	printf("\n\nDatos obtenidos:");
	printf("\n----------------------");
	printf("\n> Adaptador: \"%s\"", adaptRed.adaptador);
	printf("\n> IP: %s", adaptRed.ip);
	printf("\n> Puerta de enlace: %s", adaptRed.puerta);
	printf("\n> Mascarad de red: %s", adaptRed.mascara);
	printf("\n> DNS primario: %s", adaptRed.dnsPrimario);
	if (adaptRed.tiempoMedio == -1) {
		printf("\n> Tiempo Medio respuesta: No Calculado");
	}
	else {
		printf("\n> Tiempo Medio respuesta: %d ms", adaptRed.tiempoMedio);
	}

	if (strcmp(adaptRed.dnsPrimario, "ninguno")) {
		printf("\n> Saltos hasta DNS %s: (%d)\n\n", adaptRed.dnsPrimario, adaptRed.saltos);

		for (int i = 0; i < adaptRed.saltos; i++) {
			printf("    %d)  %s\n", i + 1, adaptRed.listaSaltos[i]);
		}
	}
	else {
		printf("\n> Saltos no calculados\n\n");
	}



	printf("\n");
	system("pause");


	return 0;
}