#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

MYSQL *conn;

int i;
int sockets[100];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int DameGanador(char fecha[20])
{

	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	conn = mysql_init(NULL);
	if (conn == NULL){
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	conn = mysql_real_connect (conn,"localhost", "root", "mysql", "juego",0,NULL,0);
	if (conn == NULL){
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	char consulta[80];
	strcpy (consulta, "SELECT jugador.username FROM jugador,partida,informacion WHERE partida.fecha = '");
	strcat (consulta,fecha);
	strcat (consulta, "'AND partida.id = informacion.id_p AND informacion.posicion = 1 AND informacion.id_j = jugador.id");
	err = mysql_query (conn, consulta);
	if (err != 0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL){
		printf ("No se ha jugado ninguna partida ese dia.\n");
	}
	else{
		while(row != NULL){
			printf("%s\n", row[0]);
			row = mysql_fetch_row (resultado);
		}
		
	}

	return 0;
}

int DameFecha(char nombre[20])
{

	int err;
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	conn = mysql_init(NULL);
	if (conn == NULL){
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	conn = mysql_real_connect (conn,"localhost", "root", "mysql", "juego",0,NULL,0);
	if (conn == NULL){
		printf ("Error al crear la conexion: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	char consulta[80];
	strcpy (consulta, "SELECT partida.fecha FROM jugador,partida,informacion WHERE jugador.username = '");
	strcat (consulta,nombre);
	strcat (consulta, "'AND partida.id = informacion.id_p  AND informacion.id_j = jugador.id");
	err = mysql_query (conn, consulta);
	if (err != 0) {
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
	}
	else{
		while(row!= NULL){
			printf("%s\n", row[0]);
			row = mysql_fetch_row (resultado);
		};
		
	}

	return 0;
}

int PartidasEntreEllos(char jugador1[20], char jugador2[20]) 
	
{ 
	

	int err1; 
	int err2; 
	
	// Estructura especial para almacenar resultados de consultas  
	
	MYSQL_RES *resultado1; 
	MYSQL_ROW row1; 
	MYSQL_RES *resultado2; 
	MYSQL_ROW row2; 
	int contador=0;
	
	//Creamos una conexion al servidor MYSQL  
	conn = mysql_init(NULL); 
	if (conn==NULL) { 
		
		printf ("Error al crear la conexi?n: %u %s\n",  
				
				mysql_errno(conn), mysql_error(conn)); 
		
		exit (1); 
		
	} 
	
	//inicializar la conexin 
	
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "juego",0, NULL, 0);
	
	if (conn==NULL) { 
		
		printf ("Error al inicializar la conexion: %u %s\n",  
				
				mysql_errno(conn), mysql_error(conn)); 
		
		exit (1); 
		
	} 
	
	char consulta1[80];
	char consulta2[80];
	
	sprintf (consulta1,"SELECT partida.id FROM jugador,partida,informacion WHERE jugador.username = %s AND partida.id = informacion.id_p AND informacion.id_j = jugador.id", jugador1);
	err1=mysql_query (conn, consulta1);
	if (err1!=0) { 
		
		printf ("Error al consultar datos de la base 1 %u %s\n", 
				
				mysql_errno(conn), mysql_error(conn)); 
		
		exit (1); 
		
	}
	resultado1 = mysql_store_result (conn);
	
	sprintf(consulta2,"SELECT partida.id FROM jugador,partida,informacion WHERE jugador.username = %s AND partida.id = informacion.id_p AND informacion.id_j = jugador.id", jugador2);
	
	err2=mysql_query (conn, consulta2); 
	
	if (err2!=0) { 
		
		printf ("Error al consultar datos de la base 2 %u %s\n", 
				
				mysql_errno(conn), mysql_error(conn)); 
		
		exit (1); 
		
	} 
	resultado2 = mysql_store_result (conn);
	row1=mysql_fetch_row (resultado1);
	row2=mysql_fetch_row (resultado2);
	
	while (row1 != NULL){
		while (row2 != NULL){
			if (strcmp(row2[0], row1[0]) == 0){
				contador++;
			}
			row2 = mysql_fetch_row (resultado2);
		}
		mysql_free_result(resultado2);
		resultado2 = mysql_store_result(conn);
		row1 = mysql_fetch_row(resultado1);
		
	}
	
	if (contador == 0){
		printf ("No han jugado ninguna partida juntos\n");
	}
	else {
		printf ("Han jugado entre ellos alguna partida");
	} 
	
	// Pregunto los nombre de los dos jugadores 
	
	
	
	mysql_close (conn); 
	
	return 0; 
	
}

int Registrarse(char username[20], char password[20])
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	
	char consulta[100];
	strcpy(consulta, "SELECT username FROM jugador WHERE username='");
	strcat(consulta,username);
	strcat(consulta, "'");
	
	err = mysql_query(conn, consulta);
	if(err!=0)
		return -1;
	else
	{
		resultado = mysql_store_result(conn);
		row = mysql_fetch_row(resultado);
		
		if(row==NULL)
		{
			err = mysql_query(conn, "SELECT FROM jugador WHERE id= (SELECT max(id) FROM jugador);");
			if(err!=0)
			{
				return -1;
			}
			resultado = mysql_store_result(conn);
			row = mysql_fetch_row(resultado);
			printf("%s\n", row[0]);
			int id = atoi(row[0])+ 1;
			sprintf(consulta,"INSERT INTO jugador VALUES (%d, '%s', '%s');", id,username,password);
			printf("%s\n",consulta);
			err = mysql_query(conn,consulta);
			if(err!=0)
			{
				return -1;
			}
			else
			   return 0;
		}
		else
		   return 1;
	}
}

int IniciarSesion (char username[20], char password[20])
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	int err;
	
	char consulta[100];
	strcpy(consulta, "SELECT contraseña FROM jugador WHERE username='");
	strcat(consulta,username);
	strcat(consulta, "'");
	err = mysql_query(conn,consulta);
	if(err!=0)
	{
		printf("Error al consultar la informacion en la base de datos %u %s", mysql_errno(conn),mysql_error(conn));
		return -1;
	}
	else
	{
		resultado = mysql_store_result(conn);
		row = mysql_fetch_row(resultado);
		if(row==NULL)
			return 1;
		else
		{
			printf("%s\n", row[0]);
			if(strcmp(password,row[0])==0)
				return 0;
			else
				return 2;
		}
	}
}

int *AtencionClientes(void *socket){
	
	char peticion[512];
	char respuesta[512];
	
	int ret;
	int sock_conn;
	int *s;
	s = (int *) socket;
	sock_conn = *s;
	
	int terminar = 0;
	while(terminar == 0)
	{
		ret = read(sock_conn,peticion,sizeof(peticion));
		printf("Recibida una peticion\n");
		peticion[ret]='\0';
		
		printf("La peticion es: %s\n", peticion);
		char *p = strtok(peticion, "/");
		int codigo = atoi(p);
		char nombre[20];
		char password[20];
		char fecha[20];
		char nombre2[20];
		
		if(codigo==0)
		{
			terminar = 1;
		}
		else if(codigo==1)
		{
			p = strtok(NULL, "/");
			strcpy(nombre, p);
			p = strtok(NULL, "/");
			strcpy(password, p);
			
			int res = Registrarse(nombre,password);
			sprintf(respuesta, "1/%d", res);
			
		}
		else if(codigo==2)
		{
			p = strtok(NULL, "/");
			strcpy(nombre,p);
			p = strtok(NULL, "/");
			strcpy(password,p);
			
			int res = IniciarSesion(nombre,password);
			sprintf(respuesta, "2/%d", res);
		}
		else if(codigo==3)
		{
			p = strtok(NULL, "/");
			strcpy(nombre,p);
			
			int res = DameFecha(nombre);
			sprintf(respuesta, "3/%d", res);
		}
		else if(codigo==4)
		{
			p = strtok(NULL, "/");
			strcpy(fecha,p);
			
			int res = DameGanador(fecha);
			sprintf(respuesta, "4/%d", res);
		}
		else if(codigo==5)
		{
			p = strtok(NULL,"/");
			strcpy(nombre,p);
			p = strtok(NULL,"/");
			strcpy(nombre2,p);
			
			int res = PartidasEntreEllos(nombre, nombre2);
			sprintf(respuesta, "5/%d", res);
		}
		
	}
	close(sock_conn);
}

int main(int argc, char *argv[]) {
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) <0)
		printf("Error creando socket");
	
	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(9060);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) <0)
		printf("Error en el bind");
	
	if (listen(sock_listen, 2) <0)
		printf("Error en el listen");
	
	conn = mysql_init(NULL);
	if(conn==NULL)
	{
		printf("Error al crear la conexion: %u %s\n", mysql_errno(conn),mysql_error(conn));
		exit(1);
	}

	pthread_t thread[100];
	pthread_mutex_init(&mutex,NULL);
	
	for(;;)
	{
		printf("Escuchando\n");
		sock_conn = accept(sock_listen,NULL,NULL);
		printf("He recibido conexion\n");
		sockets[i] = sock_conn;
		pthread_create (&thread[i], NULL, AtencionClientes, &sockets[i]);
	}
	pthread_mutex_destroy(&mutex);
			
	
	exit(0);
}

