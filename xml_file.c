/***********************************/
/*  Autor: Daniel Carrasco Luque   */
/***********************************/


#include <stdio.h>
#include <string.h>

#include "procesos.h"


/*	Crea un domumento XML con los datos del adaptador y estadisticas del DNS principal
*
*	Nombre del documento de salida:		"nombre_del_adaptador.xml"
*/


int crearXML(AdaptadorRed* adapt) {

	char nombreFichero[254] = { 0 };
	strcat(nombreFichero, adapt->adaptador);
	strcat(nombreFichero, ".xml");

	//Creamos fichero
	FILE* fileXML = fopen(nombreFichero, "w");
	if (fileXML == NULL) {
		printf("Error al crear el fichero XML");
		return 1;
	}

	// Contenido del fichero XML
	fprintf(fileXML, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileXML, "<adaptador>\n");
	fprintf(fileXML, "    <nombre>%s</nombre>\n", adapt->adaptador);
	fprintf(fileXML, "    <ip>%s</ip>\n", adapt->ip);
	fprintf(fileXML, "    <mascara>%s</mascara>\n", adapt->mascara);
	fprintf(fileXML, "    <puerta>%s</puerta>\n", adapt->puerta);
	fprintf(fileXML, "    <DNS>\n");
	fprintf(fileXML, "        <tipo>primaria</tipo>\n");
	fprintf(fileXML, "        <ip>%s</ip>\n", adapt->dnsPrimario);

	if (adapt->tiempoMedio == -1) {
		fprintf(fileXML, "        <tiempo>No Calculado</tiempo>\n");
	}
	else {
		fprintf(fileXML, "        <tiempo>%d</tiempo>\n", adapt->tiempoMedio);
	}

	fprintf(fileXML, "        <saltos>%d</saltos>\n", adapt->saltos);
	
	if (!adapt->saltos == 0) {
		fprintf(fileXML, "        <saltar>\n");
		for (int i = 0; i < adapt->saltos; i++) {
			fprintf(fileXML, "            <ip>%s</ip>\n", adapt->listaSaltos[i]);
		}
		fprintf(fileXML, "        </saltar>\n");
	}
	
	fprintf(fileXML, "    </DNS>\n");
	fprintf(fileXML, "</adaptador>\n");

	fclose(fileXML);

	return 0;
}

