


#include <iostream>
#include <string>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <ctype.h>
#include <wchar.h>  
#include <locale.h>
#include "sqlite/sqlite3.h"
//#include "sqlite3.h"

using namespace std;

#define LimpiarPantalla() system("cls")
#define NOMBRE "ePay"
#define CREAR_BASE_DATOS_RESTAURANTE(x) x "/" x ".db"
#define CREAR_USUARIO_ROOT(u, c)  "INSERT Usuario(Nombre, Contrasena) \
											      SELECT '" u "', '" c "', 1 \
											      WHERE NOT EXISTS(SELECT 1 FROM Usuario WHERE usuario = '" u "');"

#define CREAR_TABLA_REGISTRO_ORDENES "CREATE TABLE IF NOT EXISTS Registro_Ordenes(id_Orden INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																				Fecha TEXT, \
																				Hora TEXT, \
																				Identidad_Emisor TEXT, \
																				Monto FLOAT, \
																				Identidad_Receptor TEXT, \
																				Condicion TEXT\
																				);"


#define CREAR_TABLA_BILLETERA_EMPRESA "CREATE TABLE IF NOT EXISTS Billetera_Empresa(id_EBilletera INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																				Numero_Billetera TEXT, \
																				Saldo FLOAT, \
																				Condicion TEXT, \
																				Identidad TEXT REFERENCES Datos_Empresa(Identidad));"

#define CREAR_TABLA_BILLETERA_PERSONA "CREATE TABLE IF NOT EXISTS Billetera_Persona(id_PBilletera INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																				Numero_Billetera TEXT, \
																				Saldo FLOAT, \
																				Condicion TEXT, \
																				Identidad TEXT REFERENCES Datos_Persona(Identidad));"

#define CREAR_TABLA_DATOS_EMPRESA "CREATE TABLE IF NOT EXISTS Datos_Empresa(id_Empresa INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																Identidad TEXT, \
																Razon_Social TEXT, \
																Telefono TEXT, \
																Correo TEXT,\
																Id_Usuario INTEGER REFERENCES Usuario(id_Usuario)\
																);"

#define CREAR_TABLA_DATOS_PERSONA "CREATE TABLE IF NOT EXISTS Datos_Persona(id_Persona INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																Identidad TEXT, \
																Primer_nombre TEXT, \
																Segundo_nombre TEXT, \
																Primer_apellido TEXT, \
																Segundo_apellido TEXT, \
																Edad INT, \
																Telefono TEXT, \
																Correo TEXT,\
																Id_Usuario INTEGER REFERENCES Usuario(id_Usuario)\
																);"

#define CREAR_TABLA_USUARIO "CREATE TABLE IF NOT EXISTS Usuario(id_Usuario INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
																Nombre TEXT , \
																Contrasena TEXT, \
																Tusuario TEXT);"

#define ROOT 0
#define ADMIN 1
#define CLIENTE3 3
#define CLIENTE2 2
#define ERROR "ERROR"
#define DATO_INVALIDO "DATO INVALIDO."
#define INTENTE_DE_NUEVO "INTENTE DE NUEVO"
#define NO_ENCONTRADO "DATO NO ENCONTRADO"
#define USUARIO_INVALIDO "USUARIO INVALIDO"
#define FUERA_DE_RANGO "VALOR FUERA DE RANGO"
#define NO_PERMITIDO "DATO NO PERMITIDO"
#define REPETIDO "DATO REPETIDO"
#define OPCION_INVALIDA "OPCION INVALIDA"
#define SALDO_INSUFICIENTE "SALDO INSUFICIENTE"
#define CONTRASENAS_DISTINTAS "LAS CONTRASEÑAS NO COINCIDEN"

typedef struct usuario
{

	int tipo;
	string usu;
	string contra;
	string Tusu;

}User;


typedef struct personales
{

	string cedula;
	string Pnombre;
	string Snombre;
	string Papellido;
	string Sapellido;
	int Edad;
	string telefono;
	string correo;

}Persona;

typedef struct empresa
{

	string RIF;
	string Razon_Social;
	string telefono;
	string correo;

}Empresa;

typedef struct B_Datos
{

	string nBilletera;
	float saldo;
	string Condicion;

}Billetera;

typedef struct DatosP
{

	User usuario;
	Persona persona;
	Billetera billetera;

} Datos_Persona;

typedef struct DatosE
{

	User usuario;
	Empresa empresa;
	Billetera billetera;

} Datos_Empresa;


//Un dato que permitira almacena toda
typedef struct cuenta
{
	int tipo;

	int id;
	User usuario;
	Empresa Dato_E;
	Persona Dato_P;
	Billetera billetera;

}Datos_Usuario;

Datos_Usuario CUENTA;


sqlite3 *database;

int numUsuario = 0;
string query = " ";
char *error;

int SqliteError(const char *error)
{

	cout << " Error: " << error << endl;
	getchar();
	exit(-1);
}

void CrearCarpeta(const char * dir)
{

	DIR *carpeta;

	//Si la carpeta no existe, se crea
	if((carpeta = opendir(dir)) == NULL)	mkdir(dir);

}

#define CLAVE 255
//CAMBIA LAS POSICIONES DE BITS DE LOS CARACTERES
#define Reflejarbyte(c)\
                   ( (((c) & 0x01) << 7) | (((c) & 0x02) << 5) | (((c) & 0x04) << 3) | (((c) & 0x08) << 1) | (((c) & 0x10) >> 1) |  (((c) & 0x20) >> 3) | (((c) & 0x40) >> 5) | (((c) & 0x80) >> 7))



unsigned char Cifrar(unsigned char byte, unsigned char clave)
		{

		//byte XOR clave

		byte = byte ^ clave;

		//byte = Reflejarbyte(byte)

		byte = (char)Reflejarbyte(byte);

		//complemento byte A1

		byte = ~byte;


		return byte;

}

string bloqueado;
void Aviso(const char *descripcion)
{
	
	LimpiarPantalla();
	
	cout << endl;
	cout << "\t AVISO" << endl;
	cout << endl;
	cout << descripcion << endl;
	cout << endl;
	cout << "Presione ENTER para continuar." << endl;
	getchar();
	
	return;
	
}

void DatosInvalidos(const char * DI, const char *R, const char *I)
{

	cout << DI << endl;
	cout << R << endl;

	cout << I << endl;
	getchar();

}


void MensajeSuspension(const char *cad)
{
	
	cout << cad << endl;
	sleep(1);
}

/*******************************************************************************/
/************************************        ***********************************/
/************************************  LOGOS ***********************************/
/************************************        ***********************************/
/*******************************************************************************/

void icono2()
{
                                                                                                
cout << endl;              
cout << "          (@@@@@@@@@*             " << endl;                                         
cout << "       (@@@@@@@@@@@@@@@           " << endl;                                            
cout << "     ,@@@@@@. @@@@@@@@@.          " << endl;                                            
cout << "    @@@@@@@@,  #@@@@@@@@*         " << endl;                                          
cout << "     @@@@@@@@#  *@@@@@@@@#        " << endl;                                          
cout << "      @@@@@@@@@  #@@@@@@@@        " << endl;                                         
cout << "       @@@@@@@@@@@@@@@@   #@      " << endl;                                          
cout << "        @@@@@@@@@@/     @@@@@     " << endl;                                          
cout << "         &@@@@@@@@.   @@@@@@@@    " << endl;                                          
cout << "          #@@@@@@@@*  (@@@@@@@@   " << endl;                                          
cout << "           ,@@@@@@@@#  &@@@@@@@   " << endl;                                          
cout << "             @@@@@@@@@@@@@@@@.    " << endl;                                          
cout << "              &@@@@@@@@@@#        " << endl;                                          
cout << endl;
                                                                                                              
}


void icono1()
{

cout << endl; 
cout << "                    @@@@@@@@@@@@@@.                                      " << endl;         
cout << "                    @@@@@@@@@@@@@@@@                                     " << endl;         
cout << "      @@@@@@@@@@    @@@@@@@  %@@@@@@%   /@@@@@@@@@@@                     " << endl;             
cout << "   .@@@@@@@@@@@@@@  @@@@@@@  %@@@@@@%  &@@@@@@@@@@@@@  &@@@@@@    @@@@@@ " << endl;          
cout << "   @@@@@@   @@@@@@@ @@@@@@@  %@@@@@@% /@@@@@@  @@@@@@&  @@@@@@.  #@@@@@& " << endl;          
cout << "   @@@@@@   @@@@@@@ @@@@@@@  %@@@@@@% /@@@@@&  @@@@@@@  *@@@@@@  @@@@@@  " << endl;          
cout << "   @@@@@@   @@@@@@@ @@@@@@@ /@@@@@@@* /@@@@@@  @@@@@@@   @@@@@@.,@@@@@@  " << endl;          
cout << "   @@@@@@@@@@@@@@@@ @@@@@@@@@@@@@@@&       @@@@@@@@@@@    @@@@@@@@@@@@   " << endl;          
cout << "   @@@@@@@@@@@@@@@@ @@@@@@@            %@@@@@@@@@@@@@@    @@@@@@@@@@@@   " << endl;          
cout << "   @@@@@@           @@@@@@@           @@@@@@@  @@@@@@@     @@@@@@@@@@    " << endl;          
cout << "   @@@@@@   @@@@@@@ @@@@@@@           @@@@@@@  @@@@@@@     /@@@@@@@@@    " << endl;          
cout << "   @@@@@@.  @@@@@@@ @@@@@@@           @@@@@@@  @@@@@@@      @@@@@@@@,    " << endl;          
cout << "   .@@@@@@@@@@@@@@  @@@@@@@           /@@@@@@@@@@@@@@@      ,@@@@@@@     " << endl;          
cout << "     .*########,                        ##############       @@@@@@,     " << endl;          
cout << "                                                         @@@@@@@@@@      " << endl;          
cout << "                                                          @@@@@@@(        " << endl;          
cout << endl;                                                                                          
                                                                                          
}

/*******************************************************************************/
/************************************        ***********************************/
/************************************ INICIO ***********************************/
/************************************        ***********************************/
/*******************************************************************************/


void titulo(string C)
{
	//Se limpia la pantalla
	LimpiarPantalla();
	
	// Icono de la e inclinada
	icono2();
	
	//Logo
	cout << C << endl;

}

int NumeroRegistros()
{
	int aux = 0;
	sqlite3_stmt *stmt;

	// Se crea la consulta
	query = "SELECT * FROM Usuario;";

	// Se envia la consulta al gestor
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
	{

		//Se extrae el dato
		while(sqlite3_step(stmt) != SQLITE_DONE) aux =  sqlite3_column_int(stmt, 0);

	}
	else SqliteError("No es posible realizar la función.");

	return aux;
}

//Se crean los usuarios privilegiados
void CrearUsuarioPrivilegiado(sqlite3 *database, string usuario, string contrasena)
{

	string aux = "";
	char *error;

	//Se cifran las contraseÃ±as
	for(int i = 0; i < contrasena.size(); i++) contrasena[i] = Cifrar(contrasena[i], CLAVE);

	//Se crea la consulta: Una consulta que crea el usuario ROOT, y que a su vez no pueda volver a ser creado
	query = "INSERT INTO Usuario(Nombre, Contrasena, Tusuario) SELECT '" +usuario +"', '" +contrasena +"', '" +usuario +"' WHERE NOT EXISTS(SELECT 1 FROM Usuario WHERE Nombre = '" +usuario +"');";

	//Se envia la consulta al gestor
	if(sqlite3_exec(database, query.c_str(), NULL, NULL, &error) == SQLITE_OK)

		cout << " Se creo el usuario " << usuario << "." << endl;

	else SqliteError(error);

	return;

}

bool inicio()
{

	bool bandera = false;
	//string aux = " ";

	/* Se crean la base de datos */
	//Se abre la carpeta para la base de datos
	//Si no existe, se crea
	CrearCarpeta(NOMBRE);


	if(sqlite3_open(CREAR_BASE_DATOS_RESTAURANTE(NOMBRE), &database) == (int)SQLITE_OK)
	{

		/* Se crean las tablas */
		query = CREAR_TABLA_USUARIO;
		query += CREAR_TABLA_DATOS_PERSONA;
		query += CREAR_TABLA_DATOS_EMPRESA;
		query += CREAR_TABLA_BILLETERA_PERSONA;
		query += CREAR_TABLA_BILLETERA_EMPRESA;
		query += CREAR_TABLA_REGISTRO_ORDENES;

		// Se envia la consulta al gestor
		if(sqlite3_exec(database, query.c_str(), NULL, NULL, &error) == SQLITE_OK)
		{

			cout << " Se crearon las tablas." << endl;

			//Se crea el usuario ROOT
			string usuario = "ROOT";
			string contrasena = "1234567890.";

			CrearUsuarioPrivilegiado(database, usuario, contrasena);

			//Se crea el usuario ADMIN
			usuario = "ADMIN";
			contrasena = "0987654321.";

			CrearUsuarioPrivilegiado(database, usuario, contrasena);

			//cout << numUsuario;
			//getchar();

		}
		else SqliteError(error);

		//Se cierra la base de datos
		sqlite3_close(database);

	}
	else
	SqliteError(" No es posible conectarse a la base de datos");

	return bandera;

}


/*******************************************************************************/
/************************************         **********************************/
/************************************  LOGIN  **********************************/
/************************************         **********************************/
/*******************************************************************************/

bool BuscarUsuario(const char *cadena, unsigned int condicion)
{

	bool bandera = false;

	sqlite3 *database;
	sqlite3_stmt *stmt;
	char *error;

	//Se conecta a la base de datos
	if(sqlite3_open(CREAR_BASE_DATOS_RESTAURANTE(NOMBRE), &database) == (int)SQLITE_OK)
	{

		query = "SELECT ";

		//Depende de la condicion, se ejecuta la funcion
		switch(condicion)
		{

			case 0: /* Nombre de usuario */

			query += "Nombre ";

			break;
			case 1: /* ContraseÃ±a */

			query += "Contrasena ";

			break;

		}

		query +="FROM Usuario;";

		sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0);

		//Se bucan las filas en la tabla
		while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
		{

			if(strcmp((const char *)sqlite3_column_text(stmt, 0), cadena) == 0) bandera = true;

		}

		//Hay que finalizar el uso del dato sqlite3_stmt por que si no, se bloquea la base de datos
		sqlite3_finalize(stmt);

		//Se cierra la base de datos
		sqlite3_close(database);

	}
	else
		SqliteError(" No es posible conectarse a la base de datos.");

	return bandera;

}

void RegistrarDatosUsuario(sqlite3 *database, const char *U)
{

	bool bandera = false;
	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT id_Usuario, Nombre, Tusuario FROM Usuario WHERE Nombre = '" +(string)U +"';";

		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {

			//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{

				if(strcmp((const char *)sqlite3_column_text(stmt, 1), U) == 0)
				{

					CUENTA.id = sqlite3_column_int(stmt, 0);
					CUENTA.usuario.usu = (char *)sqlite3_column_text(stmt, 1);
					CUENTA.usuario.Tusu = (char *)sqlite3_column_text(stmt, 2);

					bandera = true;

				}

			}

		//Dependiendo de Tusuario, se da valor al tipo de usuario
		if(CUENTA.usuario.Tusu == "CLIENTE2") CUENTA.tipo = CLIENTE2;
		if(CUENTA.usuario.Tusu == "CLIENTE3") CUENTA.tipo = CLIENTE3;
		if(CUENTA.usuario.Tusu == "ADMIN") CUENTA.tipo = ADMIN;
		if(CUENTA.usuario.Tusu == "ROOT") CUENTA.tipo = ROOT;

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

}

void RegistroDatosPersona(sqlite3 *database)
{

	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Identidad, Primer_nombre, Segundo_nombre, Primer_apellido, Segundo_apellido, Edad, Telefono, Correo FROM Datos_Persona WHERE id_Usuario = " +to_string(CUENTA.id) +";";
		
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {

			//Si la abstraccion se cumple sinn problemas, se almacenan los datos de los campos en las varibles
			// De la estructura CUENTA
			if(sqlite3_step(stmt))
			{

					CUENTA.Dato_P.cedula = (char *)sqlite3_column_text(stmt, 0);

					CUENTA.Dato_P.Pnombre = (char *)sqlite3_column_text(stmt, 1);
					CUENTA.Dato_P.Snombre = (char *)sqlite3_column_text(stmt, 2);
					CUENTA.Dato_P.Papellido = (char *)sqlite3_column_text(stmt, 3);
					CUENTA.Dato_P.Sapellido = (char *)sqlite3_column_text(stmt, 4);

					CUENTA.Dato_P.Edad = sqlite3_column_int(stmt, 5);

					CUENTA.Dato_P.telefono = (char *)sqlite3_column_text(stmt, 6);
					CUENTA.Dato_P.correo = (char *)sqlite3_column_text(stmt, 7);
					
			}
			
		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

}

void RegistroDatosePayPersona(sqlite3 *database)
{
	
	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Numero_Billetera, Saldo, Condicion FROM Billetera_Persona WHERE Identidad = '" +CUENTA.Dato_P.cedula +"';";
		
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {

			//Si la abstraccion se cumple sinn problemas, se almacenan los datos de los campos en las varibles
			// De la estructura CUENTA
			if(sqlite3_step(stmt))
			{
					
					CUENTA.billetera.nBilletera = (char *)sqlite3_column_text(stmt, 0);
					CUENTA.billetera.Condicion = (char *)sqlite3_column_text(stmt, 2);
					CUENTA.billetera.saldo = (float)sqlite3_column_double(stmt, 1);
				
			}
			
		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

	
}

void RegistroDatosempresa(sqlite3 *database)
{
	
	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Identidad, Razon_Social, Telefono, Correo FROM Datos_Empresa WHERE id_Usuario = " +to_string(CUENTA.id) +";";
		
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {

			//Si la abstraccion se cumple sinn problemas, se almacenan los datos de los campos en las varibles
			// De la estructura CUENTA
			if(sqlite3_step(stmt))
			{
					
					CUENTA.Dato_E.RIF = (char *)sqlite3_column_text(stmt, 0);
					CUENTA.Dato_E.Razon_Social = (char *)sqlite3_column_text(stmt, 1);
					CUENTA.Dato_E.telefono = (char *)sqlite3_column_text(stmt, 2);
					CUENTA.Dato_E.correo = (char *)sqlite3_column_text(stmt, 3);
					
			}
			
		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

	
}


void RegistroDatosePayEmpresa(sqlite3 *database)
{
	
	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Numero_Billetera, Saldo, Condicion FROM Billetera_Empresa WHERE Identidad = '" +CUENTA.Dato_E.RIF +"';";
		
		//Se prepara el gestor
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {

			//Si la abstraccion se cumple sinn problemas, se almacenan los datos de los campos en las varibles
			// De la estructura CUENTA
			if(sqlite3_step(stmt))
			{
					
					CUENTA.billetera.nBilletera = (char *)sqlite3_column_text(stmt, 0);
					CUENTA.billetera.Condicion = (char *)sqlite3_column_text(stmt, 2);
					CUENTA.billetera.saldo = (float)sqlite3_column_double(stmt, 1);
				
			}
			
		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

	
}



void ActualizarUsuario(const char *U)
{

	char *error;
	sqlite3 *database;

	//Se conecta a la base de datos
	if(sqlite3_open(CREAR_BASE_DATOS_RESTAURANTE(NOMBRE), &database) == (int)SQLITE_OK)
	{
		
		RegistrarDatosUsuario(database, U);
		
		if(CUENTA.usuario.Tusu == "CLIENTE2")          // Se almacenan los datos de la cuenta Persona
		{
		
			RegistroDatosPersona(database);
			RegistroDatosePayPersona(database);
		
		}
			else if(CUENTA.usuario.Tusu == "CLIENTE3") // Se almacenan los datos cuenta Empresa
		{
			
		RegistroDatosempresa(database);
		RegistroDatosePayEmpresa(database);
			
		}
		
		//Se cierra la base de datos
		sqlite3_close(database);

	}
	else
		SqliteError(" No es posible conectarse a la base de datos.");


}

string user;
string pass;

bool login()
{
	bool bandera = false;


	LimpiarPantalla();
	
	icono1();
	
	cout << endl;
	cout << "\t LOGIN" << endl;

	/* SE INGRESA EL USUARIO */
	do
	{

		bandera = true;

		cout << "USUARIO: "; getline(cin, user);

		//Si no se encuentra cadena, se repite el bucle
		if((bandera = BuscarUsuario(user.c_str(), 0)) == false)
			DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

	}while(bandera == false);
	
	cout << endl;

	/* SE INGRESA LA CONTRASEÃ‘A */
	do
	{

		bandera = true;

		cout << "CONTRASE" << (char)165 << "A: "; getline(cin, pass);

		//Se cifra la contraseÃ±a
		for(int i = 0; i < pass.size(); i++) pass[i] = Cifrar(pass[i], CLAVE);

		//Si no se encuentra cadena, se repite el bucle
		if((bandera = BuscarUsuario(pass.c_str(), 1)) == false)
			 DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

	}while(bandera == false);
	
	return bandera;

}

/*******************************************************************************/
/**************************                 ************************************/
/************************** CREA UNA CUENTA ************************************/
/**************************                 ************************************/
/*******************************************************************************/

Datos_Persona Dato_P;
Datos_Empresa Dato_E;

void ImprimirDatosUsuario(User T)
{

	cout << "\t USUARIO." << endl << endl;
	cout << "Usuario: " << T.usu << endl;
	cout << "Tipo: ";

	if(T.Tusu == "CLIENTE2") cout << "PERSONA" << endl;
	if(T.Tusu == "CLIENTE3") cout << "EMPRESA" << endl;

	cout << endl;

	return;
}



void ImprimirDatosBilletera(Billetera T)
{

	cout << "\t Billetera ePay." << endl << endl;
	cout << "N" << (char)163 << "mero de cuenta: " << T.nBilletera << endl;
	cout << "Saldo: " << T.saldo<< " UMT" << endl;

	cout << endl;

	return;
}

void ImprimirDatosPersonales(Persona T)
{

	cout << "\t DATOS PERSONALES." << endl << endl;
	cout << "C" << (char)130 << "dula: " << T.cedula << endl;
	cout << "Primer nombre: " << T.Pnombre << endl;
	cout << "Segundo nombre: " << T.Snombre << endl;
	cout << "Primer apellido: " << T.Papellido<< endl;
	cout << "Segundo apellido: " << T.Sapellido << endl;
	cout << "Edad: " << T.Edad << endl;
	cout << "Tel" << (char)130 << "fono: " << T.telefono << endl;
	cout << "Coreo electr" << (char)162 << "nico: " << T.correo << endl;
	cout << endl;

	return;
}

void ImprimirDatosEmpresa(Empresa T)
{

	cout << "\t DATOS DE LA EMPRESA." << endl << endl;
	cout << "R.I.F: " << T.RIF << endl;
	cout << "Raz" << (char)162 << "n social: " << T.Razon_Social << endl;
	cout << "Tel" << (char)130 << "fono: " << T.telefono << endl;
	cout << "Coreo electr" << (char)162 << "nico: " << T.correo << endl;
	cout << endl;

	return;
}

string DatosUsuario(int condicion)
{

	string datos = "";
	string auxStr;
	string contra;
	int auxInt;
	bool bandera = false;

	//Se crean los datos del usuario
	cout << "\t DATOS DE USUARIO." << endl;

	cout << " Ingrese el nombre de usuario: "; getline(cin, auxStr);
	if(condicion)
	Dato_P.usuario.usu = auxStr;
	else Dato_E.usuario.usu = auxStr;


	datos +="'" +auxStr +"', ";
	
	// Se ingresan la contraseña
	do
	{
		
		auxStr = " ";
		contra = " ";
		
		cout << " Ingrese la contrase" << (char)164 << "a: "; getline(cin, auxStr);

		cout << " Confirme la contrase" << (char)164 << "a: "; getline(cin, contra);
		
		if(auxStr != contra) bandera = true;
		else bandera = false;
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, CONTRASENAS_DISTINTAS, INTENTE_DE_NUEVO);
		
	}
	while(bandera);

	//Se cifra la contraseña
	for(int i = 0; i < auxStr.size(); i++) auxStr[i] = Cifrar(auxStr[i], CLAVE);
	
	//Un detallito
	Dato_P.usuario.contra = auxStr;
	datos +="'" +auxStr +"', ";

	cout << endl;

	return datos;
}

string DatosePayPersona()
{

	string datos = "";
	string auxStr = "";
	float inicial = 200.0;

	//Se genera el numero de la billetera
	Dato_P.billetera.nBilletera = "01-" + to_string(numUsuario + 1);

	auxStr = Dato_P.billetera.nBilletera;

	datos += "'" +auxStr +"'";
	datos +=",";

	//Saldo para la billetera para persona
	//El saldo inicial para sera de 200 Ds
	Dato_P.billetera.saldo = inicial;
	datos += to_string(inicial);
	datos += ", ";

	//Condicion de la cuenta
	datos += "'ACTIVA'";

	return datos;

}


bool VerificarCorreo(string C)
{
	
	bool bandera = false;
	int arroba = 0;
	string subcadena = "";
	
	//Se rrecorre la cadena, caracter por caracter
	for(int i = 0; i < C.size(); i++)
	{
		
		char car = C[i];
		
		bool Bcar = (isalnum(car) || (car == '.') || (car == '_') || (car == '-'));
		
		// Si se encuentra un arroba, se cuenta en un acumulador, y si el numero de caracteres del nombre
		// 	de usuario es 0, la bandera es true
		if(car == '@')
		{
			
			arroba++;
			
			if (subcadena.size() == 0) bandera = true;
			else				// Caso contrario, se limpia la cadena de caracteres
			subcadena = "";
			
		}
		// Si no es un arroba, y tampoco es un punto, un guion, guion bajo o un caracter alfanumerico,
		// 		la bandera es true
		else
		{
			if(!Bcar) bandera = true;
			else subcadena += car;
		}
		
	}
	
	// Si la bandera es falsa, y el numero de arrobas es diferente de 1, bandera es true
	if(bandera == false && arroba != 1) bandera = true;
		
	return bandera;
}

bool ValidarNumerodeTelefono(string N)
{
	
	bool bandera = false;
	int guion = 0;
	string subcadena = "";
	
	//Si el primer caracter no es 0, su bandera es true
	if(N[0] == '0')
	{
		
		//Se almacena los caracteres de la extension y los numeros restantes
		for(int i = 0; i < N.size() && bandera != true; i++)
		{
			
			char car = N[i];
					
			if(car == '-')
			{
				
				//Se acumula el número de guiones
				guion++;
				
				// Si el tamaño del numero de extension es diferente de 4, la bandera es true
				if(subcadena.size() != 4) bandera = true;
				subcadena = "";
				
			}
			else
			
			// Si el caracter no es un numero, la bandera es true
			 if(isdigit(car))subcadena += car;
			 	else bandera = true;
			
		}
		
		//los caracteres despues del guion
		if((bandera == false) && (guion != 1 || subcadena.size() != 7)) bandera = true;
		
	}
	else bandera = true;
	
	return bandera;
	
}

bool ValidarCadenaNoNumerica(string C)
{
	
	bool bandera = false;
	
	char car = ' ';
	
	bool Bcar = (car == 'ñ' || car == (int)-92 || car == 'Ñ' || car == -91 ) ||
				(car == 'á' || car == -96 || car == 'Á' || car == -75 ) ||
				(car == 'é' || car == -126 || car == 'É' || car == -112 ) ||
				(car == 'í' || car == -95 || car == 'Í' || car == -42 ) ||
				(car == 'ó' || car == -94 || car == 'Ó' || car == -32 ) ||
				(car == 'ú' || car == -93 || car == 'Ú' || car == -23 );
	
	if(C[0] == ' ') return bandera;
	
	//Si el primer caracter no es una Letra mayuscula, se retorna true
	if(isupper(C[0]))
	{
		
		//Se recorre toda la cadena caracter por caracter
		for(int i = 0; i < C.size() && bandera == false; i++)
		{
			
			char car = C[i];
			
			if(!isalpha(car)) bandera = true;
			
		}
		
	}else bandera = true;
	
	
	return bandera;
	
}

bool NacionalidadIdentidad(char N, int condicion)
{

	bool bandera = false;

	if(condicion == 0 || condicion == 1) bandera |= (N=='V') || (N=='E') || (N=='P');
	
		if(condicion == 0 || condicion == 2) bandera |= (N=='J') || (N=='G');

	return bandera;

}

bool VerificarNumeroIdentidad(string I, int condicion)
{
	
	bool bandera = false;
	int guion = 0;
	int numcar = 0;
	
	//Si el primer caracter no es ninguno de los tipos de RIF, entonces bandera = true
	if(NacionalidadIdentidad(I[0], condicion))
	{
		
		for(int i = 1; i < I.size() && bandera == false; i++)
		{
			
			char car = I[i];
			
			if (car == '-') guion++;
			else
				if(!isdigit(car)) bandera = true;
					else numcar++;
			
		}
		
		if(bandera == false && (guion > 2 || guion < 1) ) bandera = true;
		
			
		//Si todas las restricciones a la hora de escribir un rif, entonces bandera = true	
		if(condicion == 2 && (numcar != 9 && guion != 2 && I[I.size() - 2] != '-')) bandera = true;
	
	}
	else bandera = true;
	
	return bandera;
	
}

#define ENCONTRADO "DATO ENCONTRADO EN LA BASE DE DATOS"

bool BuscarDatoRepetido(string campo, string tabla, string C)
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	// Se genera la consulta
	query = "SELECT " +campo +" FROM " +tabla +";";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        	
        	//cout << "FUNCION" << endl;
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				string auxStr = (char *)sqlite3_column_text(stmt, 0);
				
				// Si se encuentra la misma cedula en la tabla, la bandera retorna true
				// 		Y la bandera es true 
				if(auxStr == C)	bandera = true;
				
			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	return bandera;
	
}

//Una funcion para guardar los datos personales
string DatosPersonales()
{

	string datos = "";
	string auxStr;
	int auxInt;
	bool bandera = false;
	
	cout << "\t DATOS PERSONALES." << endl;
	
	do
	{
		
		cout << " Ingrese la c" << (char)130 << "dula(V/E/P-XXXXXXXXX): "; getline(cin, auxStr);
		Dato_P.persona.cedula = auxStr;
		
		// Se busca en la base de datos si no hay una fila con la misma cedula
		bandera = BuscarDatoRepetido("Identidad", "Datos_Persona", auxStr);
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, ENCONTRADO, INTENTE_DE_NUEVO);
		else
		{
			//Se verifica la cadena si corresponde a ser una cedula
			bandera = VerificarNumeroIdentidad(auxStr, 1);
		
			if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
		}
		
	}
	while(bandera);
	
	
	datos += "'" +auxStr +"'" +", ";
	
	do
	{
		
		cout << " Ingrese el primer nombre: "; getline(cin, auxStr);
		Dato_P.persona.Pnombre = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos +="'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese el segundo nombre: "; getline(cin, auxStr);
		Dato_P.persona.Snombre = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese el primer apellido: "; getline(cin, auxStr);
		Dato_P.persona.Papellido = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese el segundo apellido: "; getline(cin, auxStr);
		Dato_P.persona.Sapellido = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese la edad (Mayor que 18): "; cin >> auxInt; getchar();
		Dato_P.persona.Edad = auxInt;
		
		if(auxInt > 100 || auxInt < 18)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(auxInt > 100 || auxInt < 18);
	
	datos += to_string(auxInt) +", ";
	
	do
	{
		
		cout << " Ingrese el tel" << (char)130 << "fono(0XXX-XXXXXXX): "; getline(cin, auxStr);
		Dato_P.persona.telefono = auxStr;
		
		// Se busca en la base de datos si no hay una fila con la misma cedula
		bandera = BuscarDatoRepetido("Telefono", "Datos_Persona", auxStr);
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, ENCONTRADO, INTENTE_DE_NUEVO);
		else
		{
			
			bandera = ValidarNumerodeTelefono(auxStr);
		
			if(bandera) DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
		}
		
	}
	while(bandera);
	
	datos += "'" +Dato_P.persona.telefono +"', ";

	//Se verifica si hay un solo arroba, y por lo menos un punto en el correo electronico
	
	do
	{
		
		cout << " Ingrese el correo electr" << (char)162 << "nico: "; getline(cin, auxStr);
		Dato_P.persona.correo = auxStr;
		
		bandera = VerificarCorreo(auxStr);
		
		if(bandera == true)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(bandera == true);
	
	datos += "'" +auxStr +"', ";

	cout << endl;

	return datos;

}

void EjecutarConsulta(const char *consulta)
{


	//Se envia la consulta al gestor de base de datos
		if(sqlite3_exec(database, query.c_str(), NULL, NULL, &error) == SQLITE_OK)
			{

				cout << " Datos procesados con " << (char)130 << "xito." << endl;
				cout << "Presione ENTER para continuar." << endl;

			} else SqliteError(error);

		getchar();

}

bool CrearCuentaPersona()
{

	bool bandera = false;

		/* Se crea una cuenta bancaria */
		int opc;
		do
		{

			//Se limpia la pantalla
			LimpiarPantalla();

			cout << "\t CREAR CUENTA" << endl;

			string auxStr = "";

			cout << endl;

			//Se crea la consulta
			query = "";

			// Primero, el usuario ingresa los datos personales

			auxStr = "INSERT INTO Datos_Persona(Identidad, Primer_nombre, Segundo_nombre, Primer_apellido, Segundo_apellido, Edad, Telefono, Correo, Id_Usuario) VALUES (";
			auxStr += DatosPersonales();
			auxStr += to_string((numUsuario + 1));
			auxStr += "); ";

			query += auxStr +"\n";

			// El usuario ingresa los datos bancarios

			auxStr = "INSERT INTO Billetera_Persona(Numero_Billetera, Saldo, Condicion, Identidad) VALUES (";
			auxStr += DatosePayPersona();
			auxStr += ", '" +Dato_P.persona.cedula +"'";
			auxStr += "); ";

			query += auxStr +"\n";


			//Datos de usuario en linea
			auxStr = "INSERT INTO Usuario(Nombre, Contrasena, TUsuario) VALUES(";
			auxStr += DatosUsuario(1);

			//Siendo una persona, el tipo de usuario es 2
			Dato_P.usuario.Tusu = "CLIENTE2";
			auxStr += "'" +Dato_P.usuario.Tusu +"'";
			auxStr += ");";


			query += auxStr;

			// Se muestran los datos en pantalla
			LimpiarPantalla(); cout << "\t CREAR CUENTA" << endl;
			ImprimirDatosPersonales(Dato_P.persona);
			ImprimirDatosBilletera(Dato_P.billetera);
			ImprimirDatosUsuario(Dato_P.usuario);

			cout << endl;

			// Se pregunta al cliente si los datos son correctos
			do
			{

				cout << (char)168 << "Los datos son correctos? (1-SI 0-NO): "; cin >> opc;
				getchar();

				//Si es un dato fuera de rango, se ejecuta el bucle
				if(opc < 0  || opc > 1)
					DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

			}while(opc < 0  || opc > 1);

		}
		while(!opc);

		//Se envia la consulta al gestor de base de datos
		EjecutarConsulta(query.c_str());

	return bandera;
}


bool VerificarRazonSocial(string C)
{
	
	bool bandera = false;
	
	char car = ' ';
	
	//Si el primer caracter no es una Letra mayuscula, se retorna true
	if(isupper(C[0]))
	{
		
		//Se recorre toda la cadena caracter por caracter
		for(int i = 0; i < C.size() && bandera == false; i++)
		{
			
			char car = C[i];
			
			// Si el caracter no esta en mayusculas, y no es un espacio, ni tampoco un punto, un guion, una coma, ni un aspersand, entonces bandera = true
			if(!isupper(car) && (!isdigit(car)) && (car !=' ' && car != '.' && car != ',' && car != '-' && car != '&') /*&& ()*/ ) bandera = true;
				
		}
		
	}else bandera = true;
	
	
	return bandera;
	
}


string DatosEmpresa()
{

	string datos;
	string auxStr;
	bool bandera = false;

	cout << "\t DATOS DE LA EMPRESA." << endl;
	
	do
	{
		
		cout << " Ingrese el R.I.F (J/G XXXXXXXX-X): "; getline(cin, auxStr);
		Dato_E.empresa.RIF = auxStr;
		
		// Se busca en la base de datos si no hay una fila con la misma cedula
		bandera = BuscarDatoRepetido("Identidad", "Datos_Empresa", auxStr);
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, ENCONTRADO, INTENTE_DE_NUEVO);
		else
		{
			
			//Se verifica la cadena si corresponde a ser una cedula
			bandera = VerificarNumeroIdentidad(auxStr, 2);
		
			if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
		}
		
	}
	while(bandera);
	
	datos +="'" +auxStr +"', ";

	do
	{
		
		cout << " Ingrese la raz" << (char)162 << "n social(en may" << (char)163 << "sculas): ";	getline(cin, auxStr);
		Dato_E.empresa.Razon_Social = auxStr;
		
		bandera = VerificarRazonSocial(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos +="'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese el tel" << (char)130 << "fono(0XXX-XXXXXXX): "; getline(cin, auxStr);
		Dato_E.empresa.telefono = auxStr;
		
		// Se busca en la base de datos si no hay una fila con la misma cedula
		bandera = BuscarDatoRepetido("Telefono", "Datos_Empresa", auxStr);
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, ENCONTRADO, INTENTE_DE_NUEVO);
		else
		{
			
			bandera = ValidarNumerodeTelefono(auxStr);
		
			if(bandera) DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
		}
		
	}
	while(bandera);
	
	datos +="'" +auxStr +"', ";
	
	do
	{
		
		cout << " Ingrese el correo electr" << (char)162 << "nico: "; getline(cin, auxStr);
		Dato_E.empresa.correo = auxStr;
		
		bandera = VerificarCorreo(auxStr);
		
		if(bandera == true)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(bandera == true);
	
	datos +="'" +auxStr +"', ";
	
	cout << endl;

	return datos;

}

string DatosePayEmpresa()
{


	string datos = "";
	string auxStr = "";
	float inicial = 400.0;

	//Se genera el numero de la billetera
	Dato_E.billetera.nBilletera = "10-" + to_string(numUsuario + 1);

	auxStr = Dato_E.billetera.nBilletera;

	datos += "'" +auxStr +"'";
	datos +=",";

	//Saldo para la billetera para empresa
	//El saldo inicial para sera de 400 Ds
	Dato_E.billetera.saldo = inicial;
	datos += to_string(inicial);
	datos += ", ";

	//Condicion de la cuenta
	datos += "'ACTIVA'";

	return datos;

}

bool CrearCuentaEmpresa()
{

	bool bandera = false;
	int opc;

	string auxStr = "";

	do
	{

		//Se ingresan los datos de la empresa
		query = "";

		//Se limpia la pantalla
		LimpiarPantalla();

		cout << "\t CREAR CUENTA" << endl;
		auxStr = "INSERT INTO Datos_Empresa(Identidad, Razon_Social, Telefono, Correo, Id_Usuario) VALUES (";
		auxStr += DatosEmpresa();
		auxStr += to_string((numUsuario + 1));
		auxStr += ");";

		query += auxStr +"\n";
		cout << endl;

		//SE crea la cuenta de ePay
		auxStr = "INSERT INTO Billetera_Empresa(Numero_Billetera, Saldo, Condicion, Identidad) VALUES (";
		auxStr += DatosePayEmpresa();
		auxStr += ", '" +Dato_E.empresa.RIF +"'";
		auxStr += "); ";

		query += auxStr +"\n";
		cout << endl;

		//Se ingresa los datos de usuario online
		auxStr = "INSERT INTO Usuario(Nombre, Contrasena, TUsuario) VALUES(";
		auxStr += DatosUsuario(0);

		//Siendo una persona, el tipo de usuario es 2
		Dato_E.usuario.Tusu = "CLIENTE3";
		auxStr += "'" +Dato_E.usuario.Tusu +"'";
		auxStr += ");";

		query += auxStr +"\n";


		//Se muestran los datos en panatalla
		LimpiarPantalla(); cout << "\t CREAR CUENTA" << endl;
		ImprimirDatosEmpresa(Dato_E.empresa);
		ImprimirDatosBilletera(Dato_E.billetera);
		ImprimirDatosUsuario(Dato_E.usuario);

		// Se pregunta al cliente si los datos son correctos
			do
			{

				cout << "Los datos son correctos (1-SI 0-NO): "; cin >> opc;
				getchar();

				//Si es un dato fuera de rango, se ejecuta el bucle
				if(opc < 0  || opc > 1)
					DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

			}while(opc < 0  || opc > 1);

	}while(!opc);

	//Se envia la consulta al gestor de base de datos
	EjecutarConsulta(query.c_str());

	return bandera;

}


bool CrearCuenta(const char *aux)
{

	bool bandera = false;

	int opc;

	titulo("\t CREAR CUENTA");
	cout << "Tipo de cuenta: " << endl;
	cout << "1)Persona \t 2)Empresa " << endl;
	cout << endl;
	cout << "0) " << aux << endl;
	cout << endl;

	cout << "Ingrese la opci" << (char)162 << "n: "; cin >> opc;
	getchar();

	switch(opc)
	{

		case 0: return bandera;
		break;

		case 1: CrearCuentaPersona();
		break;

		case 2: CrearCuentaEmpresa();
		break;

			default: DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);
			break;

	}

	return bandera;
}

/*******************************************************************************/
/**************************                 ************************************/
/**************************  ORDEN ABIERTA  ************************************/
/**************************                 ************************************/
/*******************************************************************************/

typedef struct Orden
{
	
	int id;
	string fecha;
	string hora;
	string emisor;
	float saldo;
	string receptor;
	string condicion;
	
} ORDEN;

ORDEN pendiente;

bool VerificarTablaOrden()
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	// Se genera la consulta
	query = "SELECT * FROM Registro_Ordenes WHERE Identidad_Receptor = '" +CUENTA.Dato_E.RIF +"';";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				string auxStr = (char *)sqlite3_column_text(stmt, 6);
				
				//Si hay una orden en espera, se almacenan todas las variables de la fila de la orden,
				// 		Y la bandera es true
				if(auxStr == "EN ESPERA")
				{
					
					pendiente.id = sqlite3_column_int(stmt, 0);
					pendiente.fecha = (char *)sqlite3_column_text(stmt, 1);
					pendiente.hora = (char *)sqlite3_column_text(stmt, 2);
					pendiente.emisor = (char *)sqlite3_column_text(stmt, 3);
					pendiente.saldo = (float)sqlite3_column_double(stmt, 4);
					pendiente.receptor = (char *)sqlite3_column_text(stmt, 5);
					pendiente.condicion = (char *)sqlite3_column_text(stmt, 6);
					
					bandera = true;
				
				}
				
			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	return bandera;
	
}

void ImprimirOrdenPendiente()
{
	
	cout << "N" << (char)163 << "mero de Orden: " << pendiente.id << endl;
	cout << "Fecha: " << pendiente.fecha << endl;
	cout << "Hora: " << pendiente.hora << endl;
	cout << "Emisor: " << pendiente.emisor << endl;
	cout << "Saldo: " << pendiente.saldo << endl;
	cout << "Receptor: " << pendiente.receptor << endl;
	cout << "Condici" << (char)162 << "n: " << pendiente.condicion << endl;
	
	cout << endl;
	
}

//Esta funcion permite recorrer la tabla Registro_Ordene, que verifican que no hayan filas concondicion "EN ESPERA"

void CancelarOrdenEnEspera(string Identidad)
{
	
	// Se genera la caonsulta
	query = "UPDATE Registro_Ordenes SET Condicion = 'CANCELADA' WHERE Identidad_Receptor ='" +Identidad  +"' AND Condicion = 'EN ESPERA';";
	
	EjecutarConsulta(query.c_str());
}

bool OrdenAbierta()
{
	
	bool bandera = false;
	
	// Se busca una orden cuya condicion sea "EN ESPERA". De encontrarse una orden, se guarda sus datos en una estructura
	// 		Y se avisa al usuario
	if(VerificarTablaOrden() == true)
	{
		
		int opc;
		
		// Se muestra el aviso
		LimpiarPantalla();
		cout << "\t AVISO" << endl;
		cout << endl;
		cout << "Se ha registrado una orden en espera: " << endl;
		cout << endl;
		
		ImprimirOrdenPendiente();
		
		cout << endl;
		do
			{
				
				cout << " " << (char)168 << "Desea cancelar la orden? De no ser asi, no podr" << (char)160 << " crear otra orden." << endl;
				cout << "(1-SI   0-NO): "; cin >> opc;
				getchar();
				
				if(opc < 0 || opc > 1) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
				
		}while(opc < 0 || opc > 1);
		
		if(opc) CancelarOrdenEnEspera(CUENTA.Dato_E.RIF);
			else bandera = true;
	}
	
	return bandera;
	
}

/*******************************************************************************/
/**************************                 ************************************/
/**************************  CREA UNA ORDEN ************************************/
/**************************                 ************************************/
/*******************************************************************************/

bool BuscarPersonaCedula(string cedula)
{

	bool bandera = false;

	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Identidad, Primer_Nombre, Segundo_nombre, Primer_apellido, Segundo_apellido FROM Datos_Persona WHERE Identidad = '" +cedula +"';";
		
		cout << " Persona. " << endl;
		
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        	
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{

				if(strcmp((const char *)sqlite3_column_text(stmt, 0), cedula.c_str()) == 0)
				{

					cout << " C" << (char)130 << "dula: " << cedula << endl;
					cout << " Nombre: " << sqlite3_column_text(stmt, 1) << " " << sqlite3_column_text(stmt, 2) << " " << sqlite3_column_text(stmt, 3) << " " << sqlite3_column_text(stmt, 4);
					cout << endl;
	
					bandera = true;
					
					break;

				}


			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

	return bandera;

}

bool BuscarEmpresaRIF(string RIF)
{

	bool bandera = false;

	sqlite3_stmt *stmt;
	//Se crea la consulta
		query = "SELECT Identidad, Razon_Social FROM Datos_Empresa WHERE Identidad = '" +RIF +"';";
		
		cout << " Empresa. " << endl;
		
		if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        	
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{

				if(strcmp((const char *)sqlite3_column_text(stmt, 0), RIF.c_str()) == 0)
				{

					cout << " R.I.F: " << RIF << endl;
					cout << " Raz" << (char)162 << "n Social: " << sqlite3_column_text(stmt, 1);
					cout << endl;
	
					bandera = true;
					
					break;

				}


			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");

	return bandera;

}

void InsertarOrden(string Identidad, float monto)
{
	
	query = "INSERT INTO Registro_Ordenes(Fecha, Hora, Identidad_Emisor, monto, Identidad_Receptor, Condicion) VALUES ((SELECT date(*)), (SELECT time(*)), '" +Identidad +"', " +to_string(monto) +", '" +CUENTA.Dato_E.RIF +"'" +", 'EN ESPERA');";
	
	EjecutarConsulta(query.c_str());
	
	return;
}

//PROCESAR ORDEN
bool CrearOrden()
{

	bool bandera = false;
	string auxStr;
	int opc;
	char nacionalidad = ' ';
	float monto = 0.0;

	//Si el usuario esta bloquedao, no sera posible confirmar ningun pago
	if(CUENTA.billetera.Condicion == "BLOQUEADA") 	
	{
		
		bloqueado = "Se ha detectado que la billetera de esta cuenta esta bloqueada, lo cu";
		bloqueado += (char)160;
		bloqueado += "l, no podr";
		bloqueado += (char)160;
		bloqueado += " emitir pagos. Para m"; 
		bloqueado += (char)160;
		bloqueado += "s informaci";
		bloqueado += (char)162;
		bloqueado += "n, comuniquese con nuestro servicio al cliente.";
		Aviso(bloqueado.c_str());

	}
	else
	{
		
	if(CUENTA.usuario.Tusu != "CLIENTE2")
	{
		
		LimpiarPantalla();
		titulo("\t CREAR ORDEN.");
		
		do
		{
			
			cout << " Ingrese el monto de la operaci" << (char)162 << "n (mayor igual a 1 UMT) (0-VOLVER): ";
			cin >> monto; getchar();
			
			if(monto == 0) return bandera;
			
			if(monto < 1) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
			
		}
		while(monto < 1);

		do
		{
			
			//Se Ingresa la cédula/RIF del comprador. Un dato es incorrecto cuando:
			//	Se ingresa el RIF que es el RIF de la empresa que crea la orden
			do
			{
				
				cout << " Ingrese la c" << (char)130 << "dula/R.I.F (V/P/E/J/G-XXXXXXXX(-X)) (0-VOLVER): "; 
				getline(cin, auxStr);
				
				// Si el rif es el mismo rif de la cuenta de la empresa, entonces se repite el bucle
				if(CUENTA.Dato_E.RIF  == auxStr) DatosInvalidos(DATO_INVALIDO, REPETIDO , INTENTE_DE_NUEVO);
			
				
			}while(CUENTA.Dato_E.RIF  == auxStr);
			
			//Si la cadena es "0", se regresa la funcion...
			if(auxStr == "0") return bandera;
			
			//Se verifica si el comprado es persona o empresa
			nacionalidad = auxStr[0];
			
			if((bandera = NacionalidadIdentidad(nacionalidad, 0)) == false) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
			
			
		}
		while(bandera == false);

		bandera = false;
		
		cout << endl;
		cout << " Monto: " << monto << endl;
		if(NacionalidadIdentidad(nacionalidad, 1)) bandera = BuscarPersonaCedula(auxStr);
		if(NacionalidadIdentidad(nacionalidad, 2)) bandera = BuscarEmpresaRIF(auxStr);
		 
		if(bandera == true)
		{
			
			do
			{
				
				cout << (char)168 << "Los datos son Correctos? (1-SI  0-NO): "; cin >> opc;
				getchar();
				
				if(opc < 0 || opc > 1) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
				
			}while(opc < 0 || opc > 1);
			
			if(opc) InsertarOrden(auxStr, monto);
			
			bandera = false;
			
		}
		else DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, "");
		
	} else SqliteError(" Usuario no permitido.");
	
	}
	
	return bandera;
	
}

/*******************************************************************************/
/*******************************				  ******************************/
/******************************* CONFIRMAR ORDEN  ******************************/
/*******************************				  ******************************/
/*******************************************************************************/

string Receptor;

bool VisualizarOrdenesEnEspera(string Identidad)
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	//Se hace la consulta
	query = " SELECT id_Orden, Monto, Identidad_Receptor, Identidad_Emisor FROM Registro_Ordenes WHERE Identidad_Emisor = '" +Identidad +"' AND Condicion = 'EN ESPERA';";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        	
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE)
			{
				
				if(strcmp((const char *)sqlite3_column_text(stmt, 3), Identidad.c_str()) == 0)
				{
					
					cout << " Orden: " << sqlite3_column_int(stmt, 0) << endl;
					cout << " Receptor: " << sqlite3_column_text(stmt, 2) << endl;
					cout << " Monto:" << sqlite3_column_double(stmt, 1) << endl; 
					cout << endl;
				
					bandera = true;
				
				}
				
			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
        
        return bandera;
        
}

float oMonto;

bool EnEspera(int orden, string identidad)
{
	
	bool bandera = false;
	
	sqlite3_stmt *stmt;
	
	//Se hace la consulta
	query = " SELECT id_Orden, Fecha, Hora, Monto, Identidad_Receptor, Condicion FROM Registro_Ordenes WHERE Identidad_Emisor = '" +identidad +"' AND Condicion = 'EN ESPERA';";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        
			Receptor = " ";
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				//int auxInt = sqlite3_column_int(stmt, 0);
				//cout << auxInt << " " << orden;
				//getchar();
				
				if(sqlite3_column_int(stmt, 0) == orden)
				{
					
				LimpiarPantalla();
				cout << " \t CONFIRMAR PAGO. " << endl;
				cout << endl;
				
				cout << " Orden: " << sqlite3_column_int(stmt, 0) << endl;
				Receptor = (char *)sqlite3_column_text(stmt, 4);
				cout << "Receptor: " << Receptor << endl;
				
				cout << " Fecha y Hora: " << sqlite3_column_text(stmt, 1) << " " << sqlite3_column_text(stmt, 2) << endl;
				cout << "Condici" << (char)162 << "n: " << sqlite3_column_text(stmt, 5) << endl;
				oMonto = (float)sqlite3_column_double(stmt, 3);
				cout << " Monto:" << oMonto << " UTM" << endl; 
				cout << endl;
				
				bandera = true; 
				
				}

			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	
	return bandera;
}

bool OrdenConfirmada(int orden, string R)
{
	
	bool bandera = false;
	string auxStr;
	
	query = " ";
	
	// Se suma el monto a la cuenta de la empresa
	query = "UPDATE Billetera_Empresa SET Saldo = (Saldo + (SELECT Monto FROM Registro_Ordenes WHERE id_Orden = " +to_string(orden) +")) WHERE Identidad =  '" +Receptor +"';";
	query += "\n";
	
	// Se resta el monto a la cuenta de la persona/empresa
	query += "UPDATE ";
	
	
	
	if(CUENTA.tipo == CLIENTE2)
	{
		
		query += "Billetera_Persona ";
		auxStr = CUENTA.Dato_P.cedula;
		
	 } 
	else if(CUENTA.tipo == CLIENTE3)
	{
		
		query += "Billetera_Empresa ";
		auxStr = CUENTA.Dato_E.RIF;
		
		}
	
	query += "SET Saldo = (Saldo - (SELECT Monto FROM Registro_Ordenes WHERE id_Orden = " +to_string(orden) +")) WHERE Identidad =  '" +auxStr +"';";
	query += "\n";
	
	//Se cambia la condicion de la orden a CONFIRMADA...
	
	query += "UPDATE Registro_Ordenes SET Condicion = 'CONFIRMADA' WHERE id_Orden = " +to_string(orden) +"; ";
	
	//Se ejecuta la funcion
	EjecutarConsulta(query.c_str());
	
	return bandera;
	
}

bool ConfirmarOrden()
{
	
	bool bandera = false;
	int auxInt;
	string auxStr = " ";
	
	titulo(" \t CONFIRMAR PAGO. ");
	cout << endl;
	
	//Si el usuario esta bloquedao, no sera posible confirmar ningun pago
	if(CUENTA.billetera.Condicion == "BLOQUEADA") 	
	{
		
		//Mensaje de error
		bloqueado = "Se ha detectado que la billetera de est";
		bloqueado += (char)160;
		bloqueado += " cuenta esta bloqueada, lo cual, no podr";
		bloqueado += (char)160;
		bloqueado += " confirmar pagos. Para m";
		bloqueado += (char)160;
		bloqueado += "s informaci";
		bloqueado += (char)162;
		bloqueado += "n, comuniquese con nuestro servicio al cliente.";
		
		Aviso(bloqueado.c_str());
	
		
	}
	else
	{
	
	// Se muestran los datos de la orden que estan en espera 
	if(CUENTA.tipo == CLIENTE2) auxStr = CUENTA.Dato_P.cedula;
	 if(CUENTA.tipo == CLIENTE3) auxStr = CUENTA.Dato_E.RIF;
	 
	 if(VisualizarOrdenesEnEspera(auxStr) == true)
	 {
		cout << endl << endl;
		cout << " Ingrese el n" << (char)163 << "mero de la orden(0- Salir ): "; cin >> auxInt;
		getchar();
		
		if(auxInt == 0) return bandera;
		
		if(EnEspera(auxInt, auxStr) == true)
		{
			
			int opc;
			
			do
			{
				
				cout << (char)168 << "Confirma la orden?(1-SI  0-NO): "; cin >> opc;
				getchar();
				
				if(opc < 0 || opc > 1) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
				
			}while(opc < 0 || opc > 1);
			
			if(opc)
			{
				//Hay que verificar SI EL MONTO DESPUES DE LA RESTA ES UN NUMERO NEGATIVO
				if((CUENTA.billetera.saldo - oMonto) < 0.0) DatosInvalidos(OPCION_INVALIDA, SALDO_INSUFICIENTE, "");
				else
				OrdenConfirmada(auxInt, Receptor);
				
			 } 
			
		}
		else DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, "");
		
	}
	else
	{
		
		cout << endl;
		cout << "Presione ENTER para continuar." << endl;
		getchar();
		
	}
	}
	
	return bandera;
}

bool ConfirmarROOTADMIN()
{
	
	bool bandera = false;
	string pass;
	
	do
	{

		bandera = true;

		cout << "CONTRASEÑA (0-VOLVER): "; getline(cin, pass);
		
		if(pass == "0") return bandera;

		//Se cifra la contraseÃ±a
		for(int i = 0; i < pass.size(); i++) pass[i] = Cifrar(pass[i], CLAVE);

		//Si no se encuentra cadena, se repite el bucle
		if((bandera = BuscarUsuario(pass.c_str(), 1)) == false)
			 DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

	}while(bandera == false);
	
	return bandera;
	
}


/*******************************************************************************/
/**************************                        *****************************/
/**************************   VISUALIZAR ORDENES   *****************************/
/**************************                        *****************************/
/*******************************************************************************/

void imprimirOrden(string Identidad)
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	//Se hace la consulta
	query = " SELECT id_Orden, Monto, Identidad_Receptor, Condicion FROM Registro_Ordenes WHERE Identidad_Emisor = '" +Identidad +"';";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
        	
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE)
			{
				
				//if(strcmp((const char *)sqlite3_column_text(stmt, 3), Identidad.c_str()) == 0)
				//{
					
					cout << " Orden: " << sqlite3_column_int(stmt, 0) << endl;
					cout << " Monto:" << sqlite3_column_double(stmt, 1) << endl;
					cout << " Receptor: " << sqlite3_column_text(stmt, 2) << endl;
					cout << " Condición: " << sqlite3_column_text(stmt, 3) << endl;
					 
					cout << endl;
				
					bandera = true;
				
				//}
				
			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
        
}

bool VisualizarOrdenes(string Identidad)
{
	
	bool bandera = false;
	
	titulo("\t VISUALIZAR ORDENES");
	
	// Si la entrada es vacia, se le pde al usuario el número de cédula
	if(Identidad == " ")
	{
		
		string auxStr;
		char nacionalidad = ' ';
		
		do
		{
			
			cout << " Ingrese la c" << (char)130 << "dula/R.I.F (V/P/E/J/G-XXXXXXXX(-X)) (0-VOLVER): "; 
			getline(cin, Identidad);
			
			//Si la cadena es "0", se regresa la función...
			if(Identidad == "0") return bandera;
		
			//Se verifica si el comprado es persona o empresa
			nacionalidad = Identidad[0];
		
			if(NacionalidadIdentidad(nacionalidad, 0)== false) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
		}while(NacionalidadIdentidad(nacionalidad, 0)== false);
	
	}
	
	//Se muestran todas las ordenes del emisor
	imprimirOrden(Identidad);
	
	cout << endl;
	
	cout << "Presione ENTER para continuar." << endl;
	getchar();
	
	return bandera;
	
}

/*******************************************************************************/
/**************************                   **********************************/
/**************************  MODIFICAR CUENTA **********************************/
/**************************                   **********************************/
/*******************************************************************************/

string auxIde;

string EditarDatosPersonales()
{
	
	string datos = "";
	string auxStr;
	int auxInt;
	bool bandera = false;
	
	// Cedula 
	do
	{
		
		cout << " Ingrese la c" << (char)130 << "dula(V/E/P-XXXXXXXXX): "; getline(cin, auxStr);
		Dato_P.persona.cedula = auxStr;
		
			//Se verifica la cadena si corresponde a ser una cedula
			bandera = VerificarNumeroIdentidad(auxStr, 1);
		
			if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos = " Identidad = '" +auxStr +"', ";
	auxIde = auxStr;
	
	//Primer nombre
	do
	{
		
		cout << " Ingrese el primer nombre: "; getline(cin, auxStr);
		Dato_P.persona.Pnombre = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "Primer_nombre = '" +auxStr +"', ";
	
	//Segundo nombre
	do
	{
		
		cout << " Ingrese el segundo nombre: "; getline(cin, auxStr);
		Dato_P.persona.Snombre = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "Segundo_nombre = '" +auxStr +"', ";
	
	// Primer apellido
	do
	{
		
		cout << " Ingrese el primer apellido: "; getline(cin, auxStr);
		Dato_P.persona.Papellido = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "Primer_apellido = '" +auxStr +"', ";
	
	// Segundo apellido
	do
	{
		
		cout << " Ingrese el segundo apellido: "; getline(cin, auxStr);
		Dato_P.persona.Sapellido = auxStr;
		
		bandera = ValidarCadenaNoNumerica(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += "Segundo_apellido = '" +auxStr +"', ";
	
	//Edad
	do
	{
		
		cout << " Ingrese la edad (Mayor que 18): "; cin >> auxInt; getchar();
		Dato_P.persona.Edad = auxInt;
		
		if(auxInt > 100 || auxInt < 18)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(auxInt > 100 || auxInt < 18);
	
	datos += "Edad = " +to_string(auxInt) +", ";
	
	//Telefono
	do
	{
		
		cout << " Ingrese el tel" << (char)130 << "fono(0XXX-XXXXXXX): "; getline(cin, auxStr);
		Dato_P.persona.telefono = auxStr;
		
		
			bandera = ValidarNumerodeTelefono(auxStr);
		
			if(bandera) DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += " Telefono = '" +auxStr +"', ";
	
	//Correo
	do
	{
		
		cout << " Ingrese el correo electr" << (char)162 << "nico: "; getline(cin, auxStr);
		Dato_P.persona.correo = auxStr;
		
		bandera = VerificarCorreo(auxStr);
		
		if(bandera == true)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(bandera == true);
	
	datos += "Correo = '" +auxStr +"' ";
	
	return datos;
 	
}

string EditarDatosUsuario()
{

	string datos = "";
	string auxStr;
	string contra;
	bool bandera = false;

	//Se crean los datos del usuario
	cout << "\t DATOS DE USUARIO." << endl;

	cout << " Ingrese el nombre de usuario: "; getline(cin, auxStr);

	datos +=" Nombre = '" +auxStr +"', ";
	
	// Se ingresan la contraseña
	do
	{
		
		auxStr = " ";
		contra = " ";
		
		cout << " Ingrese la contrase" << (char)164 << "a: "; getline(cin, auxStr);

		cout << " Confirme la contrase" << (char)164 << "a: "; getline(cin, contra);
		
		if(auxStr != contra) bandera = true;
		else bandera = false;
		
		if (bandera) DatosInvalidos(DATO_INVALIDO, CONTRASENAS_DISTINTAS, INTENTE_DE_NUEVO);
		
	}
	while(bandera);

	//Se cifra la contraseña
	for(int i = 0; i < auxStr.size(); i++) auxStr[i] = Cifrar(auxStr[i], CLAVE);
	
	datos +="Contrasena = '" +auxStr +"' ";

	cout << endl;

	return datos;
}

string ModificarSaldo()
{
	
	float auxFloat;
	
	do
	{
	
		cout << "Ingrese el saldo(UTM): "; cin >> auxFloat;
		getchar();
		
		if(auxFloat < 0.01) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);	
		
	}
	while(auxFloat < 0.01);
	
	return to_string(auxFloat);
}

string ModificarCondicion(string condicion)
{
	
	string aux = "";
	
	if(condicion == "ACTIVA") aux = "BLOQUEADA";
		else aux = "ACTIVA";
	
	return aux;
	
}

void EditarDatosBilleteraMenu(string condicion)
{
	
	cout << "\t MODIFICAR BILLETERA." << endl;
	cout << "1) Modificar saldo" << endl;
	cout << "2) ";
	
	// Si el usuario esta bloqueado, se pide activar el usuario; y viceversa
	if(condicion == "ACTIVA") cout << "Bloquear ";
	else cout << "Activar ";
	cout << "Usuario" << endl;
	cout << endl;
	cout << "0) Salir" << endl;
	cout << endl;
	
	return;
	
}

string EditarDatosBilletera(string condicion)
{
	
	string datos = "";
	int opc;
	
	do
	{
		
		//Se limpia la pantalla
		LimpiarPantalla();
		
		//Se muestran las opciones
		EditarDatosBilleteraMenu(condicion);
		
		cout << "Ingrese la opci" << (char)162 << "n: "; cin >> opc;
		getchar();
		
		if(opc < 0 || opc > 2) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}while(opc < 0 || opc > 2);
	
	switch(opc)
	{
		
		case 0: return datos;
		break;
		
		case 1: datos = "Saldo = " +ModificarSaldo() +" ";
		break;
		
		case 2: datos = "Condicion = '" +ModificarCondicion(condicion) +"' ";
		break;
		
	}
	
	return datos;
	
}

string EditarDatosEmpresa()
{
	
	string datos = "";
	string auxStr;
	int auxInt;
	bool bandera = false;
	
	// RIF
	do
	{
		
		cout << " Ingrese el R.I.F (J/G XXXXXXXX-X): "; getline(cin, auxStr);
		Dato_E.empresa.RIF = auxStr;
		
	
		//Se verifica la cadena si corresponde a ser una cedula
		bandera = VerificarNumeroIdentidad(auxStr, 2);
	
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
	
		
	}
	while(bandera);
	
	datos = " Identidad = '" +auxStr +"', ";
	auxIde = auxStr;
	
	// Razon social
	do
	{
		
		cout << " Ingrese la raz" << (char)162 << "n social(en mayúsculas): ";	getline(cin, auxStr);
		Dato_E.empresa.Razon_Social = auxStr;
		
		bandera = VerificarRazonSocial(auxStr);
		
		if(bandera)DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += " Razon_social = '" +auxStr +"', ";
	
	//Telefono
	do
	{
		
		cout << " Ingrese el tel" << (char)130 << "fono(0XXX-XXXXXXX): "; getline(cin, auxStr);
		Dato_P.persona.telefono = auxStr;
		
		
			bandera = ValidarNumerodeTelefono(auxStr);
		
			if(bandera) DatosInvalidos(DATO_INVALIDO, NO_PERMITIDO, INTENTE_DE_NUEVO);
		
	}
	while(bandera);
	
	datos += " Telefono = '" +auxStr +"', ";
	
	//Correo
	do
	{
		
		cout << " Ingrese el correo electr" << (char)162 << "nico: "; getline(cin, auxStr);
		Dato_P.persona.correo = auxStr;
		
		bandera = VerificarCorreo(auxStr);
		
		if(bandera == true)DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}
	while(bandera == true);
	
	datos += "Correo = '" +auxStr +"' ";
	
	return datos;
 	
}

void EditarCuentaMenu(string tusuario)
{
	
	cout << "Campos: " << endl;
	if(tusuario == "CLIENTE2")    cout << "1) Datos personales" << endl;
	else if (tusuario == "CLIENTE3") cout << "1) Datos de la empresa" << endl;
	cout << "2) Billetera" << endl;
	cout << "3) Cuenta de Usuario" << endl;
	cout << endl;
	cout << "0) Salir" << endl;
	
}


bool EditarCuenta(Datos_Usuario cuenta)
{
	
	bool bandera = false;
	
	int opc;
	
	do
	{
		
			query = "UPDATE ";
			
			titulo("\t MODIFICAR CAMPO");
			EditarCuentaMenu(cuenta.usuario.Tusu);
			cout << endl;
			cout << "Ingrese el campo: ";
			cin >> opc; getchar();	
			
			if(opc == 0) return bandera;
			
			if(opc == 1)
			{
				
				//Dependiendo del tipo de cliente, se crea unha consulta diferente
				if(cuenta.usuario.Tusu == "CLIENTE2")
				{
					
					//Se genera la consulta
					query += "Datos_Persona SET ";
					query += EditarDatosPersonales();
					query += "WHERE id_Usuario = " +to_string(cuenta.id) +";";
					query += "\n";
					
					//Se modifican el dato asociado a la billetera
					query += "UPDATE Billetera_Persona SET Identidad = '" +auxIde +"' WHERE Identidad = '" +cuenta.Dato_P.cedula +"';";
					query += "\n";
					
					//Se modifican las ordenes emitidas por todas las cuentas del 
					query += "UPDATE Registro_Ordenes SET Identidad_Emisor = '" +auxIde +"' WHERE Identidad_Emisor = '" +cuenta.Dato_P.cedula +"';";
				
					
				}
				else if(cuenta.usuario.Tusu == "CLIENTE3")
				{
					
					//Se genera la consulta
					query += "Datos_Empresa SET ";
					query += EditarDatosEmpresa();
					query += "WHERE id_Usuario = " +to_string(cuenta.id) +";";
					query += "\n";
					
					//Se modifican el dato asociado a la billetera
					query += "UPDATE Billetera_Empresa SET Identidad = '" +auxIde +"' WHERE Identidad = '" +cuenta.Dato_E.RIF +"';";
					query += "\n";
					
					//Se modifican las ordenes emitidas por todas las cuentas del 
					query += "UPDATE Registro_Ordenes SET Identidad_Emisor = '" +auxIde +"' WHERE Identidad_Emisor = '" +cuenta.Dato_E.RIF +"';";
				
				}
				
			}
			
			else if(opc == 2) // Epay
			{
				
				string tabla = "";
				string ide = "";
				string condicion = "";
				
				if(cuenta.usuario.Tusu == "CLIENTE2")
				{
					
					tabla = "Billetera_Persona";
					ide   = cuenta.Dato_P.cedula;
					
				}
				else if(cuenta.usuario.Tusu == "CLIENTE3")
				{
					
					tabla = "Billetera_Empresa";
					ide   = cuenta.Dato_E.RIF;
					
				}
				
				query += tabla +" SET ";
				//if(EditarDatosBilletera(cuenta.billetera.Condicion) == "") return bandera;
				query += EditarDatosBilletera(cuenta.billetera.Condicion);
				query += " WHERE Identidad = '" +ide +"';";
				
			}
			else if(opc == 3) // Cuenta de usuario
			{
				
				
				//Se genera la consulta
					query += "Usuario SET ";
					query += EditarDatosUsuario();
					query += "WHERE id_Usuario = " +to_string(cuenta.id) +";";
					query += "\n";
				
			}
			
			else DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO); 
			
		
	}
	while(opc > 3 || opc < 0);
	
	if(opc == 3 && (CUENTA.usuario.Tusu != "ROOT" && CUENTA.usuario.Tusu != "ADMIN"))
	{
		
		string auxStr = " Una vez modificado los datos de usuario, se saldr";
		auxStr += (char)160;
		auxStr += " de la aplicaci";
		auxStr += (char)162;
		auxStr += "n";
		
		cout << "\t AVISO" << endl;
		cout << auxStr << endl;
		cout << endl;
		
	}
	
	bandera = false;
	//Se pregunta al usuario si confirma estos datos
	int auxOpc;
	do
		{
			
			cout << (char)168 << "Desea continuar? (1-SI  0-NO): "; cin >> auxOpc;
			getchar();
			
			if(auxOpc < 0 || auxOpc > 1) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
			
		}while(auxOpc < 0 || auxOpc > 1);
		
	// Si la opcion es si, se modifican los datos en la base
	if(auxOpc)
	{
		
		EjecutarConsulta(query.c_str());
		
		if(opc == 3 && (CUENTA.usuario.Tusu != "ROOT" && CUENTA.usuario.Tusu != "ADMIN"))
		{
		
			//Mensaje de despedida
			MensajeSuspension("HASTA PRONTO");
			
			exit(0);
		}
		
	 } 
		
	return bandera;
	
}

/*******************************************************************************/
/**************************                   **********************************/
/**************************  ELIMINAR CUENTA  **********************************/
/**************************                   **********************************/
/*******************************************************************************/

bool EliminarCuenta(Datos_Usuario cuenta)
{
	
	bool bandera = false;
	
	if(cuenta.usuario.Tusu == "CLIENTE2") // Borrar cuenta Persona
	{
		query = "DELETE FROM Datos_Persona WHERE Identidad = '" +cuenta.Dato_P.cedula +"';" +"\n";
		query += "DELETE FROM Billetera_Persona WHERE Identidad = '" +cuenta.Dato_P.cedula +"';" +"\n";
		query += "DELETE FROM Usuario WHERE id_Usuario = " +to_string(cuenta.id) +";" + "\n";
	}
	else
	if(cuenta.usuario.Tusu == "CLIENTE3") // Borrar cuenta Empresa
	{
		
		query = "DELETE FROM Datos_Empresa WHERE Identidad = '" +cuenta.Dato_E.RIF +"';" +"\n";
		query += "DELETE FROM Billetera_Empresa WHERE Identidad = '" +cuenta.Dato_E.RIF +"';" +"\n";
		query += "DELETE FROM Usuario WHERE id_Usuario = " +to_string(cuenta.id) +";" + "\n";
		
	}
	
	EjecutarConsulta(query.c_str());
	
	return bandera;
	
}

bool BorrarCuenta(Datos_Usuario cuenta)
{
	
	bool bandera = false;
	int opc;
	
	do
	{
		
		cout << " ¿Desea eliminar esta cuenta? (1-SI 0-NO): "; cin >> opc;
		getchar();
		
		if(opc > 1 || opc < 0) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}while(opc > 1 || opc < 0);
	
	if (opc)
	{
		
		//Si el usuario es ROOT o ADMIN, se le pide la contraseña de usuario
		if((CUENTA.tipo == ROOT || CUENTA.tipo == ADMIN ));
			ConfirmarROOTADMIN();
		
		//Se hace la consulta y se envia al gestor
		EliminarCuenta(cuenta);
		
	}
	
	return bandera;
}

/*******************************************************************************/
/**************************                   **********************************/
/**************************   BUSCAR CUENTA   **********************************/
/**************************                   **********************************/
/*******************************************************************************/

Datos_Usuario auxDatos;

bool LecturaCuenta(int cliente, string Identidad)
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	if(cliente == CLIENTE2) query = "SELECT Identidad, Primer_nombre, Segundo_nombre, Primer_apellido, Segundo_apellido, Edad, Telefono, Correo, Id_Usuario FROM Datos_Persona ";
	
	if(cliente == CLIENTE3) query = "SELECT Identidad, Razon_Social, Telefono, Correo, Id_Usuario FROM Datos_Empresa ";
	
	query += "WHERE Identidad = '" +Identidad +"';";
	
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				string auxStr = (char *)sqlite3_column_text(stmt, 0);
				//cout << auxStr << " " << Identidad;
				//getchar();
				
				if(auxStr == Identidad)
				{
					
					if(cliente == CLIENTE2)
					{
						
						auxDatos.Dato_P.cedula    = (char *)sqlite3_column_text(stmt, 0);
						auxDatos.Dato_P.Pnombre   = (char *)sqlite3_column_text(stmt, 1);
						auxDatos.Dato_P.Papellido = (char *)sqlite3_column_text(stmt, 2);
						auxDatos.Dato_P.Snombre   = (char *)sqlite3_column_text(stmt, 3);
						auxDatos.Dato_P.Sapellido = (char *)sqlite3_column_text(stmt, 4);
						auxDatos.Dato_P.Edad      = sqlite3_column_int(stmt, 5);
						auxDatos.Dato_P.telefono  = (char *)sqlite3_column_text(stmt, 6);
						auxDatos.Dato_P.correo    = (char *)sqlite3_column_text(stmt, 7);
						auxDatos.id				  = sqlite3_column_int(stmt, 8);
					}
					else
					if(cliente == CLIENTE3)
					{
						
						// query = "SELECT Identidad, Razon_Social, Telefono, Correo FROM Datos_Empresa ";
						auxDatos.Dato_E.RIF 		 = (char *)sqlite3_column_text(stmt, 0);
						auxDatos.Dato_E.Razon_Social = (char *)sqlite3_column_text(stmt, 1);
						auxDatos.Dato_E.telefono 	 = (char *)sqlite3_column_text(stmt, 2);
						auxDatos.Dato_E.correo		 = (char *)sqlite3_column_text(stmt, 3);
						auxDatos.id                  = sqlite3_column_int(stmt, 4);
					}				
					
				bandera = true;
				
				}

			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	return bandera;
	
}

bool lecturaBilletera(string tabla, string Identidad)
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	query = "SELECT Numero_Billetera, Saldo, Condicion, Identidad FROM " +tabla +" WHERE Identidad = '" +Identidad +"';";
		
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				string auxStr = (char *)sqlite3_column_text(stmt, 3);
				//cout << auxStr << " " << Identidad;
				//getchar();
				
				if(auxStr == Identidad)
				{
					
					auxDatos.billetera.nBilletera    = (char *)sqlite3_column_text(stmt, 0);
					auxDatos.billetera.saldo  = (float)sqlite3_column_double(stmt, 1);
					auxDatos.billetera.Condicion = (char *)sqlite3_column_text(stmt, 2);
				
				bandera = true;
				
				}

			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	return bandera;
	
}

bool LecturaUsuario()
{
	
	bool bandera = false;
	sqlite3_stmt *stmt;
	
	query = "SELECT id_Usuario, Nombre, Tusuario FROM Usuario WHERE id_Usuario = " +to_string(auxDatos.id) +";";
		
	// Se prepara la base de datos
	if(sqlite3_prepare_v2(database, query.c_str(), -1, &stmt, 0) == SQLITE_OK)
        {
				
        	//Se bucan las filas en la tabla
			while(sqlite3_step(stmt) != SQLITE_DONE && bandera == false)
			{
				
				//string auxStr = (char *)sqlite3_column_text(stmt, 3);
				//cout << auxStr << " " << Identidad;
				//getchar();
				
				if(auxDatos.id == sqlite3_column_int(stmt, 0))
				{
					
					auxDatos.usuario.usu    = (char *)sqlite3_column_text(stmt, 1);
					auxDatos.usuario.Tusu   = (char *)sqlite3_column_text(stmt, 2);
					
				
				bandera = true;
				
				}

			}

		sqlite3_finalize(stmt); // Por si las dudas

        }
        else SqliteError(" No es posible realizar la ejecución.");
	
	return bandera;
	
}

void BuscarCuentaOpciones()
{
	
	cout << "OPCIONES" << endl;
	cout << "1) Borrar Cuenta" << endl;
	cout << "2) Modificar Cuenta" << endl;
	cout << "0) Salir" << endl;
	cout << endl;
	
}

void BuscarCuentaMenu()
{
	
	int opc;
	
	//Se muestran las opciones
	BuscarCuentaOpciones();
	
	//Se pide que ingrese una opcion
	do
	{
		
		cout << " Ingrese la opci" << (char)162 << "n: "; cin >> opc;
		getchar();
		
		if(opc > 2 || opc < 0) DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);
		
	}while(opc > 2 || opc < 0);
	
	//Depende de la opcion, se ejecuta la funcion.
	switch(opc)
	{
		
		case 0: return;
			break;
			
		case 1: 		BorrarCuenta(auxDatos);
			break;
			
		case 2:			EditarCuenta(auxDatos);
			break;
		
	}
	
	return;
	
}

bool BuscarCuenta(const char *aux)
{
	
	string datos;
	string auxStr;
	bool bandera = false;
	char nacionalidad = ' ';

	
			//Se Ingresa la cédula/RIF del comprador. Un dato es incorrecto cuando:
			//	Se ingresa el RIF que es el RIF de la empresa que crea la orden
			do
			{
				
				LimpiarPantalla();
				titulo("\t BUSCAR CUENTA");
				cout << " Ingrese la c" << (char)130 << "dula/R.I.F (V/P/E/J/G-XXXXXXXX(-X)) (0-" << aux << "): "; 
				getline(cin, auxStr);
				
				//Si la cadena es "0", se regresa la funcion...
				if(auxStr == "0") return bandera;
				
				if(CUENTA.Dato_E.RIF  == auxStr) DatosInvalidos(DATO_INVALIDO, REPETIDO, INTENTE_DE_NUEVO);
				else
				{
					
					//Se verifica si el comprado es persona o empresa
					nacionalidad = auxStr[0];
					if(!NacionalidadIdentidad(nacionalidad, 0)) bandera = true;
					
				}
				
			}while(CUENTA.Dato_E.RIF  == auxStr && bandera);
			
			bandera = false;
	
			//Si el cliente es un persona, se muestran sus datos personales
			if((NacionalidadIdentidad(nacionalidad, 1)))
			{
				
				//Se extraen los datos de la fila y se almacenan en la estructura auxDatos
				if (LecturaCuenta(CLIENTE2, auxStr) && lecturaBilletera("Billetera_Persona", auxStr) && LecturaUsuario())
				{
					
					// Se imprime los datos del usuario
					ImprimirDatosPersonales(auxDatos.Dato_P);
					ImprimirDatosBilletera(auxDatos.billetera);
					ImprimirDatosUsuario(auxDatos.usuario);
					//Se pregunta al usuario si quiere eliminar o modificar los campos del usuario
					BuscarCuentaMenu();
					
				}
				else DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, "");
				
			}
			
			//	Si la cuenta es una empresa, se muestra sus datos de empresa
			 else if((NacionalidadIdentidad(nacionalidad, 2)))
			 {
			 	
					if(LecturaCuenta(CLIENTE3, auxStr) && lecturaBilletera("Billetera_Empresa", auxStr) && LecturaUsuario())
					{
						
						// Se imprime los datos del usuario
						ImprimirDatosEmpresa(auxDatos.Dato_E);
						ImprimirDatosBilletera(auxDatos.billetera);
						ImprimirDatosUsuario(auxDatos.usuario);
						//Se pregunta al usuario si quiere eliminar o modificar los campos del usuario
						BuscarCuentaMenu();;
						
					 } 	
			 	
			}
			 else DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, "");
		
		//Se muestran las opciones
	
	return bandera;
	
}


/*******************************************************************************/
/********************************				 *******************************/
/******************************** MENU PRINCIPAL *******************************/
/********************************				 *******************************/
/*******************************************************************************/

// Menu para las opciones del usuario ROOT
void ROOTMainMenu()
{
	
	cout << "1) Crear Cuenta ePay" << endl;
	cout << "2) Buscar Cuenta ePay" << endl;
	cout << "3) Visualizar Ordenes" << endl;
	cout << endl;
	cout << "0) Salir" << endl;
	cout << endl;

}

bool ePayROOTMain()
{

	bool bandera = false;

	int opc;

	// Se muestran el menu de opciones
	ROOTMainMenu();

	cout << " Ingrese la opci" << (char)162 << "n: "; cin >> opc;
	getchar();

	//Dependiopendo de la opcion, se ejecuta la corespondiente opcion
		switch(opc)
		{

			case 0: bandera = true; 		 /* Salir */
			break;

			case 01: CrearCuenta("Volver");
			break;
			
			case 02: BuscarCuenta("VOLVER");
			break;
			
			case 03: VisualizarOrdenes(" ");
				break;
					
			default: DatosInvalidos(DATO_INVALIDO, NO_ENCONTRADO, INTENTE_DE_NUEVO);

		}

	return bandera;

}

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/

void EmpresaMainMenu()
{

	cout << "1) Crear Orden" << endl;
	cout << "2) Confirmar Orden" << endl;
	cout << "3) Mostrar ordenenes" << endl;
	cout << "9) Modificar Datos" << endl;
	cout << endl;
	cout << "0) Salir" << endl;

}



bool ePayEmpresaMain()
{

	bool bandera = false;

	int opc;

	// Se muestran el menu de opciones
	EmpresaMainMenu();

	cout << " Ingrese la opci" << (char)162 << "n: "; cin >> opc;
	getchar();

	//Dependiopendo de la opcion, se ejecuta la corespondiente opcion
	switch(opc)
	{

		case 00: bandera = true; 		 /* Salir */
		break;

		case 01: 
		
		// Primero, se busca una orden cuya condicion este "EN ESPERA"
		if (OrdenAbierta() == false)
			bandera = CrearOrden();
			
		break;
		
		case 02: bandera = ConfirmarOrden();
		break;
		
		case 03: VisualizarOrdenes(CUENTA.Dato_E.RIF);
		break;
		
		case 9:	EditarCuenta(CUENTA);
			break;

		default: DatosInvalidos(OPCION_INVALIDA, NO_ENCONTRADO, INTENTE_DE_NUEVO);

	}

	return bandera;

}

void PersonaMainMenu()
{

	cout << "1) Confirmar Orden" << endl;
	cout << "2) Mostrar Ordenes" << endl;
	cout << "9) Modificar Datos" << endl;
	cout << endl;
	cout << "0) Salir" << endl;

}

bool ePayPersonaMain()
{

	bool bandera = false;

	int opc;

	// Se muestran el menu de opciones
	PersonaMainMenu();

	cout << " Ingrese la opci" << (char)162 << "n: "; cin >> opc;
	getchar();

	//Dependiopendo de la opcion, se ejecuta la corespondiente opcion
		switch(opc)
		{

			case 0: bandera = true; 		 /* Salir */
			break;

			case 01: bandera = ConfirmarOrden();
			break;
			
			case 02: VisualizarOrdenes(CUENTA.Dato_P.cedula);
				break;
				
			case 9: EditarCuenta(CUENTA);
				break;

			default: DatosInvalidos(DATO_INVALIDO, FUERA_DE_RANGO, INTENTE_DE_NUEVO);

		}

	return bandera;

}

void TamanyoPantalla(int x, int y)
{
	
	string modo = "mode ";
		   modo += to_string(x);
		   modo += " ,";
		   modo += to_string(y);
	
	system(modo.c_str());
	
}

void Consola(int argc, char** argv)
{
	
	// Si hay cuatro argumentos de entrada por comandos, se pasa a la segunda condicion
	if(argc == 4)
	{
		
		string user = argv[1];
		string contra = argv[2];
		string argumento = argv[3];
		
		// Si el segundo argumento es el nombre de usuario ROOT o ADMIN, se pasa al tercer argumento
		if((user == "ROOT" && contra == "1234567890.") || (user == "ADMIN" && contra == "0987654321.") )
		{
			
			// Se conecta a la base de datos
			if(sqlite3_open(CREAR_BASE_DATOS_RESTAURANTE(NOMBRE), &database) == (int)SQLITE_OK)
			{
			
			//Se almacenan los datos de usuario
			ActualizarUsuario(user.c_str());
				
			//Se almacena en una variable el numero de la ultima cuenta registrada
			numUsuario = NumeroRegistros();
			
			// Mensaje de Bienvenida
			cout << "BIENVENIDO ";
			MensajeSuspension(CUENTA.usuario.usu.c_str());
			
			// Si el cuarto argumento es -c, se va a crear una nueva cuenta de Epay
			if(argumento == "-c" || argumento == "-C")
			{
				
				CrearCuenta("Salir");
				
			}
			else
			// Si el cuarto argumento es -b, se va a crear una nueva cuenta de Epay
			if(argumento == "-b" || argumento == "-B")
			{
				
				BuscarCuenta("SALIR");
				
			}else cout << "Epay : Argumentos incorrectos" << endl;
			
			
			// Se cierra la base de datos
			sqlite3_close(database); 
			
			}
			
			// Mensaje de despedida
			MensajeSuspension("HASTA PRONTO");
			LimpiarPantalla();
			
			// Se reinician los colores de la consola
			system("COLOR 07");
			exit(0);
		
		}
		else 
		
		// Si los argumemntos de consola van mas alla del nombre del programa, se muestra el mensaje de error
		cout << "Epay : Argumentos incorrectos" << endl;
		
	}
	else if(argc != 1) cout << "Epay : Argumentos incorrectos" << endl;
}

void ePay(int argc, char** argv)
{

	//sqlite3 *database;
	bool bandera; 
	
	Consola(argc, argv);
	
	//Si no hay argumentos de consola, se ejecuta la aplicacion
	if(argc == 1)
	{
		
	inicio(); 
	
	system("COLOR F1");
	TamanyoPantalla(75, 30);
	
	if(login() == true)
	{
		
		//Menaje de benvenida
		MensajeSuspension("BIENVENDO");

		//Se conecto con la base de datos
		if(sqlite3_open(CREAR_BASE_DATOS_RESTAURANTE(NOMBRE), &database) == (int)SQLITE_OK)
		{
			
			TamanyoPantalla(75, 50);
			
			//Se crea el bucle
			//Dependiendo del tipo de usuario, se muetra diferentes tipos de menu
			do
			{
				
				//Se actualiza los datos del usuario cada vez ue se repite el bucle
				ActualizarUsuario(user.c_str());
				
				//Se almacena en una variable el numero de la ultima cuenta registrada
				numUsuario = NumeroRegistros();
				
				//lETRERO DE BIENVENIDA
				string leyenda = "\t BIENVENIDO A ePay  " +CUENTA.usuario.usu;
				titulo(leyenda);
				
				switch(CUENTA.tipo)
				{
	
				case ROOT:
				case ADMIN:
				bandera = ePayROOTMain();
				break;
	
				case CLIENTE2:
				bandera = ePayPersonaMain();
				break;
	
				case CLIENTE3:
				bandera = ePayEmpresaMain();
				break;
			}

		}while(bandera == false);


		//Se cierra la base de datos
		sqlite3_close(database);

		}
		else
			SqliteError(" No es posible conectarse a la base de datos.");

	}
	
	//Mensaje de despedida
	MensajeSuspension("HASTA PRONTO");
	
	LimpiarPantalla();
	system("COLOR 07");
	}
	
	return;	

}

int main(int argc, char** argv) { ePay(argc, argv); return 0;}

/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/
