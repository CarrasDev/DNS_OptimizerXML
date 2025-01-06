/***********************************/
/*  Autor: Daniel Carrasco Luque   */
/***********************************/

#ifndef PROCESOS_H
#define PROCESOS_H


/*	Estructura para almacenar el adaptdador escogido por el usuario y las DNS del adaptador */

typedef struct {
	char adaptador[300];
	char dnsPrimario[20];
	char dnsSecundario[20];
	char ip[20];
	char puerta[20];
	char mascara[20];
	int tiempoMedio;
	int saltos;
	char listaSaltos[30][20];
} AdaptadorRed;


/*	Muestra el listado de adaptadores de red del PC mediante "netsh"
*	Obtiene las DNS primaria y secundaria del adaptador seleccionado si las tiene.
*
*	Retorna estructura AdaptadorRed.
*/

// AdaptadorRed seleccionarAdaptadorRed(void);
int seleccionarAdaptadorRed(AdaptadorRed* adapt);


/*	Dado un adaptador de red obtiene la DNS Primaria y la DNS Secundaria
*	retornandolas dentro de la estructura AdaptadorRed
*/

int obtenerDNSAdaptador(AdaptadorRed* adapt);


/*	Dada una direccion IP retorna el tiempo medio de respuesta usando ping
*	Retorno: tiempo en ms
*/

int tiempoMedioPing(char* ip);


/*
*	Realiza una consulta con Netsh para obtener la IP, máscara de subred y puerta de enlace
*	En caso de no tetectar IP se entiende que el adaptador no está operativo
*
*	Retorno:	0 = OK;	1 = FAIL (sin IP asignada)
*/

int obtenerIpPuertaMascara(AdaptadorRed* adapt);


/*
*	Realiza un tracert de la DNS primaria y obtiene el numero de saltos y sus IP
*/

int obtenerSaltos(AdaptadorRed* adapt);


/*	
*	Crea un domumento XML con los datos del adaptador y estadisticas del DNS principal
* 
*	Nombre del documento de salida:		"nombre_del_adaptador.xml"
*/

int crearXML(AdaptadorRed* adapt);



#endif