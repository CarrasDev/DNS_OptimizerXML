/***********************************/
/*  Autor: Daniel Carrasco Luque   */
/***********************************/

#include <stdio.h>
#include <string.h> 

#include "fun_aux.h"
#include "procesos.h"



/*
*	Realiza una consulta con Netsh para obtener la IP, máscara de subred y puerta de enlace
*	En caso de no tetectar IP se entiende que el adaptador no está operativo
*	
*	Retorno:	0 = OK;	1 = FAIL (sin IP asignada) 
*/

int obtenerIpPuertaMascara(AdaptadorRed* adapt) {

	char* NETSH_OUT = "netsh_temp.txt";
	char instruccion[150] = { 0 };
	char* token;


	// Construir instruccion para system
	strcat(instruccion, "netsh interface ip show config name=\"");
	strcat(instruccion, adapt->adaptador);
	strcat(instruccion, "\" > ");
	strcat(instruccion, NETSH_OUT);

	system(instruccion);

	// Abrir fichero resultado de netsh
	FILE* fileNetsh = fopen(NETSH_OUT, "r");
	if (fileNetsh == NULL) {
		printf("Error al abrir archivo temporal");
		return 1;
	}

	// Procesar fichero en busca de ip, máscara de subred y puerta de enlace
	char linea[300] = {0};
	int control = 0;
	while (fgets(linea, sizeof(linea), fileNetsh) && control == 0) {
		
		if (strstr(linea, "IP: ")) {				// Obtener IP
			token = strtok(linea, ":");
			token = strtok(NULL, " ");
			limpiarBarraN(adapt->ip, token);		// Almacenar IP sin \n

			fgets(linea, sizeof(linea), fileNetsh); // Abanzar línea
			token = strtok(linea, "(");				// Obtener máscara de subred
			token = strtok(NULL, ")");
			token += 8;
			strcpy(adapt->mascara, token);			// Almacenar máscara de subred
		
			fgets(linea, sizeof(linea), fileNetsh); // Abanzar línea
			if (strstr(linea, "Puerta de enlace")) {
				token = strtok(linea, ":");				// Obtener puerta de enlace
				token = strtok(NULL, " ");
				limpiarBarraN(adapt->puerta, token);	// Almacenar puerta de enlace
			}
			else {
				strcpy(adapt->puerta, "ninguno");
			}
			control = 1;
		}
	}

	fclose(fileNetsh);
	remove(NETSH_OUT);

	// Si no existe IP damos valores por defecto y finalizamos con return = 1;
	if (control == 0) {

		strcpy(adapt->ip, "ninguno");
		strcpy(adapt->mascara, "ninguno");
		strcpy(adapt->puerta, "ninguno");
		return 1;	
	}

	return 0;
}



/*	Dado un adaptador de red obtiene la DNS Primaria y la DNS Secundaria
*	retornandolas dentro de la estructura AdaptadorRed
*/

int obtenerDNSAdaptador(AdaptadorRed* adapt) {

	// Instruccion para obtener las DNS del adaptador
	char instruccion[200] = { 0 };

	strcat(instruccion, "netsh interface ip show dnsservers \"");
	strcat(instruccion, adapt->adaptador);
	strcat(instruccion, "\" > netsh.txt");

	system(instruccion);

	FILE* file = fopen("netsh.txt", "r");
	if (file == NULL) {
		printf("Error al abrir fichero netsh.");
		return 1;
	}


	int control = 0;
	char* encontrada;
	char linea[100] = { 0 };

	while (fgets(linea, sizeof(linea), file) && control == 0) {
		encontrada = strstr(linea, ": ");

		// Hay DNS Primario?
		if (encontrada) {
			encontrada++;
			while (*encontrada == ' ') {
				encontrada++;
			}
			limpiarBarraN(adapt->dnsPrimario, encontrada); // Guardamos DNS Primario
			control = 1;

			// Hay DNS Secundario?
			fgets(linea, sizeof(linea), file);
			encontrada = strstr(linea, "Registrar");

			if (encontrada) {		// No hay DNS Secundario
				strcpy(adapt->dnsSecundario, "ninguno");
			}
			else {					// Hay DNS Secundario
				encontrada = linea; // Encontrada es puntero a linea
				while (*encontrada == ' ') {
					encontrada++;
				}
				limpiarBarraN(adapt->dnsSecundario, encontrada); // Guardamos DNS Secundario
			}
		}
	}

	fclose(file);
	remove("netsh.txt");
	return 0;
}





/*	Dada una direccion IP retorna el tiempo medio de respuesta usando ping
*	Retorno: tiempo en ms
*/

int tiempoMedioPing(char* ip) {

	char instruccion[100] = { 0 };
	char* encontrada;
	char* token;
	int tiempoMedio;

	// Construir instruccion
	strcat(instruccion, "ping -n 2 -w 100 ");	// 2 saltos 100ms Max
	strcat(instruccion, ip);
	strcat(instruccion, " > ping_dns.txt");

	system(instruccion);

	FILE* pingOut = fopen("ping_dns.txt", "r");
	if (pingOut == NULL) {
		printf("Error al abrir resultado de ping");
		return -1;
	}

	char linea[200];
	while (fgets(linea, sizeof(linea), pingOut)) {
		encontrada = strstr(linea, "Media = ");

		if (encontrada) {
			token = strtok(encontrada, "Media = ");		// Reduzco string al minimo
			sscanf(token, "%d", &tiempoMedio);			// Extraigo tiempo de tipo entero

			fclose(pingOut);
			remove("ping_dns.txt");
			return tiempoMedio;
		}
	}

	fclose(pingOut);
	remove("ping_dns.txt");
	return -1;
}




/*
*	Realiza un tracert de la DNS primaria y obtiene el numero de saltos y sus IP
*/

int obtenerSaltos(AdaptadorRed* adapt) {

	char* TRACERT_OUT = "tracert.txt";
	char instruccion[100] = { 0 };
	char* token;
	char* ultimaPosicion;

	// Construir instruccion
	strcat(instruccion, "tracert -d -w 1 ");
	strcat(instruccion, adapt->dnsPrimario);
	strcat(instruccion, " > ");
	strcat(instruccion, TRACERT_OUT);

	system(instruccion);

	FILE* tracertOut = fopen(TRACERT_OUT, "r");
	if (tracertOut == NULL) {
		printf("Error al abrir fichero temporal tracert");
		return 1;
	}

	// Saltar las 3 primeras líneas del fichero
	char linea[300] = { 0 };
	for (int i = 0; i < 3; i++) {
		fgets(linea, sizeof(linea), tracertOut);
	}

	adapt->saltos = 0; // Contador de saltos a 0
	while (fgets(linea, sizeof(linea), tracertOut) && (adapt->saltos < 30)) { // 30 saltos max por defecto en tracert

		if (strstr(linea, "Tiempo")) {
			strcpy(adapt->listaSaltos[adapt->saltos], "Desconocida"); // En caso de ip desconocida
			adapt->saltos++;
		}
		else {
			// Filtrar IP
			token = linea;
			ultimaPosicion = token;
			while ((token = strstr(ultimaPosicion, "ms")) != NULL) {
				ultimaPosicion = token + 2;
			}
			while (*ultimaPosicion == ' ' || *ultimaPosicion == '*') { // Se filtra por ' ' y '*' ya que se pueden llegar a dar los 2 casos
				ultimaPosicion++;
			}

			// Quitar " \n" final
			size_t leng = strlen(ultimaPosicion);
			ultimaPosicion[leng - 2] = '\0';

			// Almacenar IP
			strcpy(adapt->listaSaltos[adapt->saltos], ultimaPosicion);
			adapt->saltos++;

			if (!strcmp(adapt->dnsPrimario, adapt->listaSaltos[adapt->saltos - 1])) { // Finalizar busqueda de saltos

				fclose(tracertOut);
				remove(TRACERT_OUT);
				return 0;
			}
		}

	}

	fclose(tracertOut);
	remove(TRACERT_OUT);

	return 1;
}



/*	Muestra el listado de adaptadores de red del PC mediante "netsh"
*	Obtiene las DNS primaria y secundaria del adaptador seleccionado si las tiene.
*
*	Retorna estructura AdaptadorRed.
*/

int seleccionarAdaptadorRed(AdaptadorRed* adapt) {

	// Archivos
	const char* NETSH_OUT = "netsh_out.txt";
	const char* ADAPTADORES = "adaptors.txt";

	char instruccion[150] = { 0 };
	char* token;

	// Construir sentencia para system
	strcat(instruccion, "netsh interface show interface > ");
	strcat(instruccion, NETSH_OUT);

	system(instruccion);

	// Abrir fichero de consulta a Netsh
	FILE* fileNetsh = fopen(NETSH_OUT, "r");
	if (fileNetsh == NULL) {
		printf("Error al abrir archivo.");
		return 1;
	}

	// Abrir fichero para almacenar adaptadores
	FILE* fileAdapt = fopen(ADAPTADORES, "w");
	if (fileAdapt == NULL) {
		printf("Error al abrir fichero temporal.");
		return 1;
	}


	// Obtener los nombres de adaptador de red	
	// Saltar las 3 primeras lineas de fileNetsh
	char linea[300];
	for (int i = 0; i < 3; i++) {
		fgets(linea, sizeof(linea), fileNetsh);
	}

	while (fgets(linea, sizeof(linea), fileNetsh) != NULL) {

		// Avanzamos por los tokens hasta encontrar el nombre de interfaz
		token = strtok(linea, " ");	// Estado admin.
		token = strtok(NULL, " ");	// Estado
		token = strtok(NULL, " ");	// Tipo
		token = strtok(NULL, "\n");	// Nombre interfaz

		// Eliminamos espacios y alamcenamos en fichero
		if (token != NULL) {
			while (*token == ' ') {
				token++;
			}
			fprintf(fileAdapt, "%s\n", token);
		}
	}
	fclose(fileNetsh);
	remove(NETSH_OUT);
	fclose(fileAdapt);


	// -----------------------------------------
	// Creamos menú de seleccion de adaptadores
	// -----------------------------------------
	int adaptCount = 0;
	char adaptadores[20][300];
	memset(adaptadores, 0, sizeof(adaptadores)); // Inicializar

	// Abrir fichero de adaptadores
	fileAdapt = fopen(ADAPTADORES, "r");
	if (fileAdapt == NULL) {
		printf("Error al abrir fichero de adaptadores");
		return 1;
	}

	// Obtener adaptadores y prepararlo para ser seleccionados
	memset(linea, 0, sizeof(linea));
	char lineaDestino[300] = { 0 };
	while (fgets(linea, sizeof(linea), fileAdapt) && adaptCount < 20) {
		limpiarBarraN(lineaDestino, linea);
		strcpy(adaptadores[adaptCount], lineaDestino);
		adaptCount++;
	}

	fclose(fileAdapt);
	remove(ADAPTADORES);

	// Mostrar adaptadores y registrar adaptador solicitado
	int opcion;
	do {
		system("cls");
		presentacion();
		// printf("\n");
		printf("Seleccionar adaptador:\n");
		printf("----------------------\n");
		for (int i = 0; i < adaptCount; i++) {
			printf("> %d. %s\n", i + 1, adaptadores[i]);
		}

		printf("\nEscoja opcion: ");
		scanf("%d", &opcion);
		limpiarBuffer();

	} while (opcion < 1 || opcion > adaptCount);

	// Adaptador escogido
	char* adaptador = adaptadores[opcion - 1];
	limpiarBarraN(adapt->adaptador, adaptador); // Guardamos adaptador


	// Obtener DNS a partir del adaptador
	obtenerDNSAdaptador(adapt);


	return 0;
}