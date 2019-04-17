#include<iostream>  //Funciones b�sicas de la consola
#include<fstream>   //Lectura y escritura de archivos
#include<iomanip>   
#include<string>    //Declaraci�n de variables tipo String
#include<vector>    //Contenedor de datos al leer informaci�n del archivo
#include<cstdio>
#include<stdlib.h>
#include<ctime>
#include<algorithm>
#include<sstream>

using namespace std;

int menuGeneral();
int submenuVuelos();
int submenuAviones();

bool adminPass();
void adminChange();

void limpiar();
bool checkHora(int hour, int min, bool tipo);
bool checkDestino(vector<string> destinos, string destino);
void showDestinos(vector<string> destinos);
void showHoraFecha(time_t time);

struct Avion{           //Registro para almacenar un avión
    string nombre;
    int tiempoCarga;    //Tiempo de descarga en minutos
    int tiempoDescarga; //Tiempo de carga en minutos
    int tiempoPista;    //Tiempo que pasa en la pista
};

struct Vuelo{           //Registro para almacenar un vuelo
    int hora, min;		//Hora en formatos hh:mm/hora = hh, min = mm
    bool tipo;          //0 si es salida, 1 si es entrada
    string destino;
    Avion avion;        //Avión que usará el vuelo
	int estado;
	int fHora, fMin; 	//Cálculo de la hora de finalización
	int duracion;		//Duración del vuelo en empezar
};

class Aerolinea{        //Clase para gestionar aerolíneas
    public:
    string nombre;
    vector <Vuelo> vuelos;
    int carga, descarga;

    //Agregar un vuelo con su hora, tipo y destino
    void agregar(int pHora, int pMin, bool pTipo, string pDestino){
        Vuelo contenedor;
        contenedor.hora = pHora;
        contenedor.min = pMin;
        contenedor.tipo = pTipo;
        contenedor.destino = pDestino;
		contenedor.estado = 0;
        vuelos.push_back(contenedor);

		cout<<"Vuelo agregado (Enter para continuar)"<<endl;
		getchar();
    }

    //Eliminar un vuelo segun su hora y destino
    void eliminar(int pHora, int pMin, bool pTipo, string pDestino){
        int i = 0, index;
        if(vuelos.empty()){
            cout<<"--- No hay vuelos asignados para esta aerolinea (Enter para continuar)"<<endl;
            getchar();
            return;
        }
        for(i=0; i<vuelos.size(); i++){
            if(vuelos[i].hora == pHora && vuelos[i].min == pMin && vuelos[i].tipo == pTipo && vuelos[i].destino == pDestino){
                index = i;
            }
        }
        vuelos.erase(vuelos.begin()+index);
        cout<<"Vuelo eliminado (Enter para continuar)"<<endl;
        getchar();
    }
};

int main()
{
    //Declaraciones
    int i, j, k, cantidadVuelos = 0, opc = 0;
	int carga = 0, descarga = 0; 
    bool salir = false;
    vector <Avion> 	aviones;
	Avion desconocido;
    Avion avContenedor;

    //Inicializando temporizador
    clock_t c_inicio = clock();
	time_t tContenedor = time(0);
	tm *contenedor = localtime(&tContenedor);

    //Inicializando aerolíneas
    Aerolinea aerolinea[5];
        aerolinea[0].nombre = "AviancaBrasil";
        aerolinea[1].nombre = "AzulLineasAereas";
        aerolinea[2].nombre = "GOLLineasAereas";
        aerolinea[3].nombre = "PassaredOLineasAereas";
        aerolinea[4].nombre = "LATAMBrasil";

    //Posibles rutas de cada aerolínea
    string sAviancaBrasil[] = {"Aracaju", "BeloHorizonte", "Belem", "Brasilia", "Chapeco", "Campo",
            "Grande", "Cuiaba", "Curitiba", "Florianopolis", "Fortaleza", "FozDoIguacu", "Goiania", 
            "JoaoPessoa", "JuazeiroDoNorte", "Natal", "Navegantes", "Petrolina", "PortoAlegre", 
            "Recife", "RioDeJaneiro-Galeao", "SalvadorDeBahia", "Vitoria"},
            sAzulLineasAereas[] = {"BeloHorizonte", "Brasilia", "Cascavel", "Cuiaba", "Curitiba",
            "Londrina", "Maceio", "Maringa", "PortoAlegre", "Recife", "RioDeJaneiro", "Vitoria"},
            //
            sGolLineasAereas[] = {"Aracaju", "Belem", "BeloHorizonte", "Brasilia", "Campo Grande", 
            "Chapeco", "Cuiaba", "Curitiba", "Florianopolis", "Fortaleza", "FozDoIguacu", "Goiania",
            "JoaoPessoa", "JuazeiroDoNorte", "Londrina", "Maceio", "Manaos", "Maringa", "Natal",
            "Navegantes", "Petrolina", "PortoAlegre", "PortoSeguro", "Recife", "RioDeJaneiro-GIG",
            "RioDeJaneiro", "SalvadorDeBahia", "SaoLuis", "Teresina", "Uberlandia", "Vitoria"},
            //
            sPassaredLineasAereas[] = {"Cascavel", "RibeiraoPreto", "VitoriaDaConquista"},
            //
            sLatamBrasil[] = {"Aracaju", "Belem", "BeloHorizonte", "Brasilia", "CampoGrande", "Cuiaba",
            "Curitiba", "Florianopolis", "Fortaleza", "FozDoIguacu", "Goiania", "Ilheus", "JoaoPessoa",
            "Joinville", "Londrina", "Maceio", "Manaos", "Natal", "Navegantes", "PortoAlegre",
            "PortoSeguro", "PortoVelho", "Recife", "RioDeJaneiro-Galeao", "SalvadorDeBahia",
            "SaoLuis", "Teresina", "Uberlandia", "Vitoria"};
            
    vector <string> aviancaBrasil(sAviancaBrasil,sAviancaBrasil+23),
                    azulLineasAereas(sAzulLineasAereas,sAzulLineasAereas+12),
                    golLineasAereas(sGolLineasAereas,sGolLineasAereas+31),
                    passaredLineasAereas(sPassaredLineasAereas, sPassaredLineasAereas+3),
    	            latamBrasil(sLatamBrasil,sLatamBrasil+29);


	desconocido.nombre = "Desconocido";
	desconocido.tiempoCarga = 0;
	desconocido.tiempoDescarga = 0;
	desconocido.tiempoPista = 0;

    //
    //Men� principal

	int cargar = 2;

	//Carga de datos opcional de la ultima operaci�n
	
	//Carga de los datos de los vuelos
	while(!(cargar == 1 || cargar == 0)){
		limpiar();
		cout<<"Desea cargar los vuelos de la ultima operacion? 1 - Si / 0 - No"<<endl;
		cout<<"Escriba el numero exacto: ";
		cin>>cargar;
		if(cin.fail()){
			cin.clear();
			cin.ignore();
			cargar = 2;
		}
	}

	vector <string> vueloBackup;	
	if(cargar == 1){
		ifstream lVuelos;
		string linea;
		lVuelos.open("vuelos");
		while(lVuelos>>linea){
			vueloBackup.push_back(linea);
		}
		lVuelos.close();
	}

	int it = 0;
	k=0;
	if(!vueloBackup.empty()){
		for(i=0; i<5; i++){
			aerolinea[i].nombre = vueloBackup[k];
			istringstream iss1(vueloBackup[k+1]);
			iss1>>it;
			k+=2;
			for(j=0; j<it; j++){
				Vuelo container;
				container.destino = vueloBackup[k];
				istringstream iss2(vueloBackup[k+1]);
				iss2>>container.hora;
				istringstream iss3(vueloBackup[k+2]);
				iss3>>container.min;
				istringstream iss4(vueloBackup[k+3]);
				iss4>>container.tipo;
				container.estado = 0;
				aerolinea[i].vuelos.push_back(container);
				k+=4;
			}
		}
	}
	else{
		if(cargar == 1){
			cout<<"No hay vuelos en el archivo"<<endl;
			getchar(); getchar();
		}	
	}
	
	cargar = 2;
	//Carga de los datos de los aviones
	
	while(!(cargar == 1 || cargar == 0)){
		limpiar();
		cout<<"Desea cargar los aviones de la ultima operacion? 1 - Si / 0 - No"<<endl;
		cout<<"Escriba el numero exacto: ";
		cin>>cargar;
		if(cin.fail()){
			cin.clear();
			cin.ignore();
			cargar = 2;
		}
	}

	vector <string> avionBackup;	
	if(cargar == 1){
		ifstream lAviones;
		string linea;
		lAviones.open("aviones");
		while(lAviones>>linea) avionBackup.push_back(linea);
		lAviones.close();
	}

	j=0;
	if(!avionBackup.empty()){
		istringstream iss1(avionBackup[j]);
		iss1>>it; j++;
		for(i=0; i<it; i++){
			Avion container;
			container.nombre = avionBackup[j];
			
			istringstream iss2(avionBackup[j+1]);
			iss2>>container.tiempoCarga;
			
			istringstream iss3(avionBackup[j+2]);
			iss3>>container.tiempoDescarga;
			
			istringstream iss4(avionBackup[j+3]);
			iss4>>container.tiempoPista;
			
			aviones.push_back(container);
			j+=4;
		}
	}
	else{
		if(cargar == 1){
			cout<<"No hay aviones en el archivo"<<endl;
			getchar(); getchar();
		}	
	}
	
	//Ciclo principal
	
	while(salir == false){
		limpiar();
		time_t menu = time(0);
		opc = menuGeneral();
		char buffer[80];
		int input;
		time_t tInit = time(0);
		tm *init = localtime(&tInit);
		int initHora = init->tm_hour,
		initMin  = init->tm_min;
		switch(opc){
			case 1: //Gestión de vuelos por aerolínea
				while(salir == false){			
					int h,m; string d; bool t;
					bool eraseAdd;	
					limpiar();				
					opc = submenuVuelos();
					switch(opc){
						case 1: //Avianca Brasil
							// Chequeo del formato de fecha y hora 
							do{
								limpiar();
								cout<<"Intrododuzca la hora en el formato solicitado (hh-mm/24 Horas):"<<endl;
								cout<<"hh (0-23): "; cin>>h;
								cout<<"mm (0-59): "; cin>>m;
								cout<<"Introduzca el tipo de vuelo (1 para descarga/0 para carga): "; cin>>t;
								if(!checkHora(h,m,t)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								}
								if(cin.fail()){
									cin.clear();
									cin.ignore();
									d = 2; h = 100; m = 100;
								} 
							}
							while(!checkHora(h,m,t));
							// Chequeo del destino
							limpiar();
							do{
								limpiar();
								showDestinos(aviancaBrasil);
								cout<<"Destino: ";
								cin>>d;
								if(!checkDestino(aviancaBrasil,d)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								} 
							}while(!checkDestino(aviancaBrasil,d));
							do{
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Escriba con exactitud (1) <- Deseo agregar este vuelo / (0) <- Deseo eliminar este vuelo: ";
								cin>>eraseAdd;
							}
							while(cin.fail());
							//
							if(eraseAdd) aerolinea[0].agregar(h,m,t,d);
							else		 aerolinea[0].eliminar(h,m,t,d);
							break;
						case 2: //Azul Lineas Aereas
							// Chequeo del formato de fecha y hora 
							do{
								limpiar();
								cout<<"Intrododuzca la hora en el formato solicitado (hh-mm/24 Horas):"<<endl;
								cout<<"hh (0-23): "; cin>>h;
								cout<<"mm (0-59): "; cin>>m;
								cout<<"Introduzca el tipo de vuelo (1 para descarga/0 para carga): "; cin>>t;
								if(!checkHora(h,m,t)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								}
								if(cin.fail()){
									cin.clear();
									cin.ignore();
									d = 2; h = 100; m = 100;
								} 
							}
							while(!checkHora(h,m,t));
							// Chequeo del destino
							limpiar();
							do{
								limpiar();
								showDestinos(azulLineasAereas);
								cout<<"Destino: ";
								cin>>d;
								if(!checkDestino(azulLineasAereas,d)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								} 
							}while(!checkDestino(azulLineasAereas,d));
							do{
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Escriba con exactitud (1) <- Deseo agregar este vuelo / (0) <- Deseo eliminar este vuelo: ";
								cin>>eraseAdd;
							}
							while(cin.fail());
							//
							if(eraseAdd) aerolinea[1].agregar(h,m,t,d);
							else		 aerolinea[1].eliminar(h,m,t,d);
							break;
						case 3: //GOL Lineas Aereas
							// Chequeo del formato de fecha y hora 
							do{
								limpiar();
								cout<<"Intrododuzca la hora en el formato solicitado (hh-mm/24 Horas):"<<endl;
								cout<<"hh (0-23): "; cin>>h;
								cout<<"mm (0-59): "; cin>>m;
								cout<<"Introduzca el tipo de vuelo (1 para descarga/0 para carga): "; cin>>t;
								if(!checkHora(h,m,t)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								}
								if(cin.fail()){
									cin.clear();
									cin.ignore();
									d = 2; h = 100; m = 100;
								} 
							}
							while(!checkHora(h,m,t));
							// Chequeo del destino
							limpiar();
							do{
								limpiar();
								showDestinos(golLineasAereas);
								cout<<"Destino: ";
								cin>>d;
								if(!checkDestino(golLineasAereas,d)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								} 
							}while(!checkDestino(golLineasAereas,d));
							do{
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Escriba con exactitud (1) <- Deseo agregar este vuelo / (0) <- Deseo eliminar este vuelo: ";
								cin>>eraseAdd;
							}
							while(cin.fail());
							//
							if(eraseAdd) aerolinea[2].agregar(h,m,t,d);
							else		 aerolinea[2].eliminar(h,m,t,d);
							break;
						case 4: //Passared Lineas Aereas
							// Chequeo del formato de fecha y hora 
							do{
								limpiar();
								cout<<"Intrododuzca la hora en el formato solicitado (hh-mm/24 Horas):"<<endl;
								cout<<"hh (0-23): "; cin>>h;
								cout<<"mm (0-59): "; cin>>m;
								cout<<"Introduzca el tipo de vuelo (1 para descarga/0 para carga): "; cin>>t;
								cout<<"Introduzca la fecha del vuelo (D/M/A)"<<endl;
								if(!checkHora(h,m,t)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								}
								if(cin.fail()){
									cin.clear();
									cin.ignore();
									d = 2; h = 100; m = 100;
								} 
							}
							while(!checkHora(h,m,t));
							// Chequeo del destino
							limpiar();
							do{
								limpiar();
								showDestinos(passaredLineasAereas);
								cout<<"Destino: ";
								cin>>d;
								if(!checkDestino(passaredLineasAereas,d)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								} 
							}while(!checkDestino(passaredLineasAereas,d));
							do{
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Escriba con exactitud (1) <- Deseo agregar este vuelo / (0) <- Deseo eliminar este vuelo: ";
								cin>>eraseAdd;
							}
							while(cin.fail());
							//
							if(eraseAdd) aerolinea[3].agregar(h,m,t,d);
							else		 aerolinea[3].eliminar(h,m,t,d);
							break; 
						case 5: //LATAM Brasil
							// Chequeo del formato de fecha y hora 
							do{
								limpiar();
								cout<<"Intrododuzca la hora en el formato solicitado (hh-mm/24 Horas):"<<endl;
								cout<<"hh (0-23): "; cin>>h;
								cout<<"mm (0-59): "; cin>>m;
								cout<<"Introduzca el tipo de vuelo (1 para descarga/0 para carga): "; cin>>t;
								cout<<"Introduzca la fecha del vuelo (D/M/A)"<<endl;
								if(!checkHora(h,m,t)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								}
								if(cin.fail()){
									cin.clear();
									cin.ignore();
									d = 2; h = 100; m = 100;
								} 
							}
							while(!checkHora(h,m,t));
							// Chequeo del destino
							limpiar();
							do{
								limpiar();
								showDestinos(latamBrasil);
								cout<<"Destino: ";
								cin>>d;
								if(!checkDestino(latamBrasil,d)){
									cout<<"Formato incorrecto, intente otra vez (Enter para continuar)"<<endl;
									getchar();
								} 
							}while(!checkDestino(latamBrasil,d));
							do{
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Escriba con exactitud (1) <- Deseo agregar este vuelo / (0) <- Deseo eliminar este vuelo: ";
								cin>>eraseAdd;
							}
							while(cin.fail());
							//
							if(eraseAdd) aerolinea[4].agregar(h,m,t,d);
							else		 aerolinea[4].eliminar(h,m,t,d);
							break;
						case 6: //Salir
							salir = true;
							break; 
						default:
							limpiar();
							cout<<"---Debe introducir una opcion valida (Numero del 1 al 6 | Enter para continuar)"<<endl;
							getchar();
							break;
					}
				}
				salir = false;
				break;
			case 2: //Gestión de aviones en el aeropuerto
				while(salir == false){
					opc = submenuAviones();
					bool check = false;
					int index;
					string avion;
					limpiar();
					switch(opc){
						case 1: //Introducir un avi�n
							do{
								if(cin.fail())  cout<<"---Error, por favor introduzca el formato correcto"<<endl;
								limpiar();
								cin.clear();
								cin.ignore();
								cout<<"Introduzca el nombre del avion: ";
								cin>>avContenedor.nombre;
								cout<<"Tiempo de carga (En minutos): ";
								cin>>avContenedor.tiempoCarga;
								cout<<"Tiempo de descarga (En minutos): ";
								cin>>avContenedor.tiempoDescarga;
								cout<<"Tiempo de pista (En minutos): ";
								cin>>avContenedor.tiempoPista;
							}
							while(cin.fail());
							aviones.push_back(avContenedor);
							break;
						case 2: //Eliminar un avi�n
							if(!aviones.empty()){
								limpiar();
								cout<<"Introduzca el nombre del avion (Considere mayusculas): ";
								cin>>avion;
								for(i=0; i<aviones.size(); i++){
									if(aviones[i].nombre == avion){
										check = true;
										index = i;
									}
								}
								if(check){
									aviones.erase(aviones.begin()+index);
									cout<<"Avion eliminado"<<endl;
								}
								else{
									limpiar();
									cout<<"No se consiguio el avion (Presione enter para continuar)"<<endl;
								}
							}
							else{
								cout<<"No hay aviones activos en el sistema (Enter para continuar)"<<endl;
							}
							getchar();
							break;
						case 3: //Modificar un avi�n
							if(!aviones.empty()){
								limpiar();
								cout<<"Introduzca el nombre del avion a modificar (Considere mayusculas): ";
								cin>>avion;
								for(i=0; i<aviones.size(); i++){
									if(aviones[i].nombre == avion){
										check = true;
										index = i;
									}
								}
								if(check){
									do{
										if(cin.fail())  cout<<"---Error, por favor introduzca el formato correcto"<<endl;
										limpiar();
										cin.clear();
										cin.ignore();
										cout<<"Introduzca el nombre del avion: ";
										cin>>avContenedor.nombre;
										cout<<"Tiempo de carga (En minutos): ";
										cin>>avContenedor.tiempoCarga;
										cout<<"Tiempo de descarga (En minutos): ";
										cin>>avContenedor.tiempoDescarga;
										cout<<"Tiempo de pista (En minutos): ";
										cin>>avContenedor.tiempoPista;
									}
									while(cin.fail());
									aviones[index] = avContenedor;
									cout<<"Datos modificados para el avion "<<avContenedor.nombre<<" (Presione enter para continuar)";
									cout<<endl;
									cout<<endl;
								}
								else{
									limpiar();
									cout<<"No se consiguio el avion (Presione enter para continuar)"<<endl;
								}
							}
							else{
								cout<<"No hay aviones activos en el sistema (Enter para continuar)"<<endl;
							}
							getchar();
							break;
						case 4: //Lista de aviones
							if(!aviones.empty()){
								cout<<setfill('-');
								cout<<setw(15)<<"Nombre:"<<setw(10)<<"TCarga"<<setw(10)<<"TDescarga"<<setw(10)<<"TPista"<<endl;
								for(i=0; i<aviones.size(); i++){
									cout<<setfill(' ');
									cout<<setw(15)<<aviones[i].nombre.c_str();
									cout<<setw(10)<<aviones[i].tiempoCarga;
									cout<<setw(10)<<aviones[i].tiempoDescarga;
									cout<<setw(10)<<aviones[i].tiempoPista<<endl;
								}
								cout<<"---Enter para continuar"<<endl;
							}
							else{
								cout<<"No hay aviones activos en el sistema (Enter para continuar)"<<endl;
							}
							getchar(); getchar();
							break;
						case 5:
							salir = true;
							break;                             
						default:
							limpiar();
							cout<<"------Debe introducir una opcion valida (Numero del 1 al 5 | Enter para continuar)------"<<endl;
							getchar();
							break;
					}
				}
				salir = false;
				break;
			case 3: //Visualización del aeropuerto en tiempo real
				//Vuelos programados
				{			
					while(salir == false){
						limpiar();
						time_t tNow = time(0);
						tm *now = localtime(&tNow);
						showHoraFecha(tNow);

						cout<<setw(50)<<"*** VUELOS PROGRAMADOS ***"<<endl;
						for(i=0; i<5; i++){ //Ciclo por cada aerol�nea
							if(!aerolinea[i].vuelos.empty()){
								//Se chequean los vuelos programados
								for(j=0; j<aerolinea[i].vuelos.size(); j++){
									tNow = time(0);
									now = localtime(&tNow);
									
									if(now->tm_mday > init->tm_mday){
										int l,y;
										for(l=0; l<5; l++){
											for(y=0; y<aerolinea[l].vuelos.size(); y++){
												aerolinea[j].vuelos[y].estado = 0;
												init = now;
											}
										}
									}

									if(now->tm_hour >= aerolinea[i].vuelos[j].fHora && aerolinea[i].vuelos[j].estado == 2){
										if((now->tm_hour == aerolinea[i].vuelos[j].fHora && now->tm_min >= aerolinea[i].vuelos[j].fMin) || (now->tm_hour > aerolinea[i].vuelos[j].fHora)){
											aerolinea[i].vuelos[j].estado = 3;
											cantidadVuelos++;
											if(aerolinea[i].vuelos[j].tipo == 1){
												descarga--;
											}
											else{
												carga--;
											}
										}
									}

									if(aerolinea[i].vuelos[j].hora >= initHora && (aerolinea[i].vuelos[j].estado == 0)){
										if( (aerolinea[i].vuelos[j].hora == initHora && aerolinea[i].vuelos[j].min >= initMin) || (aerolinea[i].vuelos[j].hora > initHora)){
											if(now->tm_hour >= aerolinea[i].vuelos[j].hora){
												if(((now->tm_hour == aerolinea[i].vuelos[j].hora) && (now->tm_min >= aerolinea[i].vuelos[j].min)) || (now->tm_hour > aerolinea[i].vuelos[j].hora)){
													if(aviones.size() == 0){
														aerolinea[i].vuelos[j].avion = desconocido;
													}
													else{
														aerolinea[i].vuelos[j].avion = aviones[rand()%aviones.size()];
													}
													aerolinea[i].vuelos[j].avion.tiempoCarga += rand()%3;
													aerolinea[i].vuelos[j].avion.tiempoDescarga += rand()%3;
													aerolinea[i].vuelos[j].avion.tiempoPista += rand()%3;
													aerolinea[i].vuelos[j].estado = 1;
												}
											}		
										}
									}				
								}
								//Se muestran los vuelos que est�n programados
								cout<<"----"<<aerolinea[i].nombre<<"----"<<endl;
								cout<<setw(20)<<"_Destino"<<setw(12)<<"_Hora"<<setw(12)<<"_Tipo"<<setw(13)<<"_Estado"<<setw(15)<<"_Avion"<<setw(10)<<"_Fin"<<endl;
								for(j=0; j<aerolinea[i].vuelos.size(); j++){
									string sEstado;
									cout<<"|"<<setw(19)<<aerolinea[i].vuelos[j].destino.c_str();									
									contenedor->tm_hour = aerolinea[i].vuelos[j].hora;
									contenedor->tm_min  = aerolinea[i].vuelos[j].min;
									strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
									cout<<setw(12)<<buffer;
									if(aerolinea[i].vuelos[j].tipo){
										cout<<setw(12)<<"DESCARGA";
									}
									else{
										cout<<setw(12)<<"CARGA";
									}
									switch(aerolinea[i].vuelos[j].estado){
										case 0:
											sEstado = "PROGRAMADO";
											break;
										case 1:
											sEstado = "EN ESPERA";
											break;
										case 2:
											sEstado = "EN PROGRESO";
											break;
										case 3:
											sEstado = "ATENDIDO";
											break;
										default:
											sEstado = "ERROR";
											break;
									}
									cout<<setw(13)<<sEstado.c_str();
									if(aerolinea[i].vuelos[j].estado != 0){
										cout<<setw(15)<<aerolinea[i].vuelos[j].avion.nombre.c_str();
									}
									if(aerolinea[i].vuelos[j].estado == 2 || aerolinea[i].vuelos[j].estado == 3){
										contenedor->tm_hour = aerolinea[i].vuelos[j].fHora;
										contenedor->tm_min  = aerolinea[i].vuelos[j].fMin;
										strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
										cout<<setw(10)<<buffer;	
									}
									cout<<endl;
								}
								cout<<endl;
							}
						}
						cout<<endl;
						cout<<"= Cantidad de vuelos atendidos: "<<cantidadVuelos<<endl;
						cout<<endl;
						cout<<"- (1) para actualizar"<<endl;
						cout<<"- (2) para atender 1 vuelo en espera de carga"<<endl;
						cout<<"- (3) para atender 1 vuelo en espera de descarga"<<endl;
						cout<<"- (4) para salir"<<endl<<endl;
						cout<<"* Introduzca el numero exacto: "; cin>>input;
						if(cin.fail()){
							cin.clear();
							cin.ignore();
							input = 2;
						}
						switch(input){
							case 1:
								break;
							case 2: //Introduccion de 1 vuelo en espera de carga a su respectivo terminal
								{
									bool check = 0;
									for(i=0; i<5; i++){
										for(j=0; j<aerolinea[i].vuelos.size(); j++){
											tNow = time(0);
											now = localtime(&tNow);
											int cMin, cHora;
											if(aerolinea[i].vuelos[j].tipo == 0 && aerolinea[i].vuelos[j].estado == 1 && (carga < 5)){
												aerolinea[i].vuelos[j].duracion = aerolinea[i].vuelos[j].avion.tiempoCarga + aerolinea[i].vuelos[j].avion.tiempoPista;
												//Ajuste de la hora
												if((now->tm_min + aerolinea[i].vuelos[j].duracion) > 59){
													cout<<"Pass"<<endl;
													cMin = aerolinea[i].vuelos[j].duracion + now->tm_min - 60; 
													cHora = now->tm_hour + 1;
												}
												else{
													cout<<"Pass"<<endl;
													cMin = now->tm_min + aerolinea[i].vuelos[j].duracion;
													cHora = now->tm_hour;
												}
												//
												aerolinea[i].vuelos[j].fHora = cHora;
												aerolinea[i].vuelos[j].fMin	= cMin;
												aerolinea[i].vuelos[j].estado = 2;
												check = 1;
												carga++;
												break;
											}											
										}
										if(check == 1) break;
									}
								}								
								break;
							case 3: //Introduccion de 1 vuelo en espera de descarga a su respectivo terminal
								{
									bool check = 0;
									for(i=0; i<5; i++){
										for(j=0; j<aerolinea[i].vuelos.size(); j++){
											tNow = time(0);
											now = localtime(&tNow);
											int cMin, cHora;
											if(aerolinea[i].vuelos[j].tipo == 1 && aerolinea[i].vuelos[j].estado == 1 && (descarga < 3)){
												aerolinea[i].vuelos[j].duracion = aerolinea[i].vuelos[j].avion.tiempoCarga + aerolinea[i].vuelos[j].avion.tiempoPista;
												//Ajuste de la hora
												if((now->tm_min + aerolinea[i].vuelos[j].duracion) > 59){
													cout<<"Pass"<<endl;
													cMin = aerolinea[i].vuelos[j].duracion + now->tm_min - 60; 
													cHora = now->tm_hour + 1;
												}
												else{
													cout<<"Pass"<<endl;
													cMin = now->tm_min + aerolinea[i].vuelos[j].duracion;
													cHora = now->tm_hour;
												}
												//
												aerolinea[i].vuelos[j].fHora = cHora;
												aerolinea[i].vuelos[j].fMin	= cMin;
												aerolinea[i].vuelos[j].estado = 2;
												check = 1;
												descarga++;
												break;
											}										
										}
										if(check == 1) break;
									}
								}	
								break;
							case 4:
								salir = true;
								break;
							default:
								cout<<"---Debe introducir una opcion valida (Numero del 1 al 3 | Enter para continuar)"<<endl;
								getchar();
								break;
						}
					}
					salir = false;
				}
				break;
			case 4: //Interrumpir operacion
				limpiar();
				cout<<"(Presione enter para continuar)"<<endl;
				salir = true;
				break;
			default:
				cout<<"---Debe introducir una opcion valida (Numero del 1 al 5 | Enter para continuar)"<<endl;
				getchar();
				break;
		}
	}
	

    //Cerrando temporizador
    clock_t c_fin = clock();
	double tiempoOp, tiempoSeg;

	
	tiempoSeg = (c_fin - c_inicio)/CLOCKS_PER_SEC;
	string formato;

	if(tiempoSeg > 3600){
		tiempoOp = tiempoSeg/3600;
		formato = " horas";
	}
	else if(tiempoSeg > 60){
		tiempoOp = tiempoSeg/60;
		formato = " minutos";
	}
	else{
		tiempoOp = tiempoSeg;
		formato = " segundos";
	}

	cargar = 2;
	
	//Pregunta para saber si se desea poder reiniciar el sistema
	while(!(cargar == 1 || cargar == 0)){
		limpiar();
		cout<<"Desea que el sistema se pueda reanudar en el proximo inicio? 1 - Si / 0 - No"<<endl;
		cout<<"Escriba el numero exacto: ";
		cin>>cargar;
		if(cin.fail()){
			cin.clear();
			cin.ignore();
			cargar = 2;
		}
	}

	if(cargar == 1){
		//Generando archivo de recuperacion de los vuelos
		ofstream fVuelos;

		fVuelos.open("vuelos", ofstream::out | ofstream::trunc);
		for(i=0; i<5; i++){
			fVuelos<<aerolinea[i].nombre<<endl;
			fVuelos<<aerolinea[i].vuelos.size()<<endl;
			for(j=0; j<aerolinea[i].vuelos.size(); j++){
				fVuelos<<aerolinea[i].vuelos[j].destino<<endl;
				fVuelos<<aerolinea[i].vuelos[j].hora<<endl;
				fVuelos<<aerolinea[i].vuelos[j].min<<endl;
				fVuelos<<aerolinea[i].vuelos[j].tipo<<endl;
			}
		}
		fVuelos.close();
	}
	else{
		ofstream fVuelos;
		fVuelos.open("vuelos", ofstream::out | ofstream::trunc);
		fVuelos.close();
	}
	
	//Generando archivo de recuperacion de los aviones
	ofstream fAviones;

	fAviones.open("aviones", ofstream::out | ofstream::trunc);

	fAviones<<aviones.size()<<endl;
	for(i=0; i<aviones.size(); i++){
		fAviones<<aviones[i].nombre<<endl;
		fAviones<<aviones[i].tiempoCarga<<endl;
		fAviones<<aviones[i].tiempoDescarga<<endl;
		fAviones<<aviones[i].tiempoPista<<endl;
	}
	fAviones.close();
	

    //Generando reporte del archivo
	ofstream reporte;
	char buffer[100];
	bool check1 = 0, check2 = 0;

    reporte.open("reporte.txt", ofstream::out | ofstream::trunc);
	reporte<<"--- REPORTE DE LA OPERACION DEL TERMINAL: ---"<<endl<<endl;
	reporte<<"Cantidad de vuelos atendidos: "<<cantidadVuelos<<endl;
	reporte<<"Tiempo de operacion: "<<tiempoOp<<formato.c_str();
	reporte<<endl<<endl;

	for(i=0; i<5; i++){
		for(j=0; j<aerolinea[i].vuelos.size(); j++){
			if(aerolinea[i].vuelos[j].estado == 3){
				check1 = 1;
			}
		}
	}

	reporte<<"--- VUELOS ATENDIDOS ---"<<endl;
	if(check1 == 1){
		for(i=0; i<5; i++){
			check2 = 0;
			for(j=0; j<aerolinea[i].vuelos.size(); j++){
				if(aerolinea[i].vuelos[j].estado == 3) check2 = 1;
			}
			if(check2){
				reporte<<":::"<<aerolinea[i].nombre<<":::"<<endl;
				reporte<<setw(20)<<"_Destino"<<setw(12)<<"_Hora"<<setw(12)<<"_Tipo"<<setw(15)<<"_Avion"<<endl;
				for(j=0; j<aerolinea[i].vuelos.size(); j++){
					if(aerolinea[i].vuelos[j].estado == 3){
						reporte<<setw(20)<<aerolinea[i].vuelos[j].destino.c_str();
						contenedor->tm_hour = aerolinea[i].vuelos[j].hora;
						contenedor->tm_min  = aerolinea[i].vuelos[j].min;
						strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
						reporte<<setw(12)<<buffer;
						if(aerolinea[i].vuelos[j].tipo) reporte<<setw(12)<<"DESCARGA";
						else                        	reporte<<setw(12)<<"CARGA";
						reporte<<setw(15)<<aerolinea[i].vuelos[j].avion.nombre.c_str()<<endl;
					}
				}
			}
		}
	}
	else{
		reporte<<"No se atendieron vuelos durante la operación del terminal."<<endl;
	}

	reporte<<endl;

	check1 = 0; check2 = 0;

	for(i=0; i<5; i++){
		for(j=0; j<aerolinea[i].vuelos.size(); j++){
			if(aerolinea[i].vuelos[j].estado == 1){
				check1 = 1;
			}
		}
	}

	reporte<<"--- VUELOS EN ESPERA ---"<<endl;
	if(check1){
		for(i=0; i<5; i++){
			check2 = 0;
			for(j=0; j<aerolinea[i].vuelos.size(); j++){
				if(aerolinea[i].vuelos[j].estado == 1) check2 = 1;
			}
			if(check2){
				reporte<<":::"<<aerolinea[i].nombre<<":::"<<endl;
				reporte<<setw(20)<<"_Destino"<<setw(12)<<"_Hora"<<setw(12)<<"_Tipo"<<setw(15)<<"_Avion"<<endl;
				for(j=0; j<aerolinea[i].vuelos.size(); j++){
					if(aerolinea[i].vuelos[j].estado == 1){
						reporte<<setw(20)<<aerolinea[i].vuelos[j].destino.c_str();
						contenedor->tm_hour = aerolinea[i].vuelos[j].hora;
						contenedor->tm_min  = aerolinea[i].vuelos[j].min;
						strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
						reporte<<setw(12)<<buffer;
						if(aerolinea[i].vuelos[j].tipo) reporte<<setw(12)<<"DESCARGA";
						else                        	reporte<<setw(12)<<"CARGA";
						reporte<<setw(15)<<aerolinea[i].vuelos[j].avion.nombre.c_str()<<endl;
					}
				}
			}
		}
	}
	else{
		reporte<<"No quedaron vuelos en espera durante la operacion del terminal."<<endl;
	}
	
    reporte.close();

    //Fin del programa
    limpiar();

	cout<<"Reporte generado - (Enter para cerrar el programa)"<<endl;
	getchar(); 
    return 0;
}

//Men�es del sistema

int menuGeneral(){      //Men� general de todo el programa
    int resp;
    cout<<"---"<<setw(30)<<"Menu principal"<<setw(21)<<"---"<<endl<<endl;
    cout<<"-1-"<<"  Gestionar vuelos en el aeropuerto"<<endl;
    cout<<"-2-"<<"  Gestionar aviones en funcionamiento"<<endl;
    cout<<"-3-"<<"  Revisar vuelos en tiempo real"<<endl;
    cout<<"-4-"<<"  Interrumpir la operacion"<<endl<<endl;
    cout<<"---"<<"  Introduzca el numero exacto de la opcion: ";
    cin>>resp; 
    if(cin.fail()){
        cin.clear();
        cin.ignore();
        resp = 10;
    } 
    cout<<endl;
    return resp;
}

int submenuVuelos(){    //Submen� para la gesti�n de vuelos
    int resp;
    cout<<"---"<<setw(30)<<"Seleccione aerolinea"<<setw(19)<<"---"<<endl<<endl;
    cout<<"-1-"<<"  Avianca Brasil"<<endl;
    cout<<"-2-"<<"  Azul Lineas Aereas"<<endl;
    cout<<"-3-"<<"  GOL Lineas Aereas"<<endl;
    cout<<"-4-"<<"  Passared o Lineas Aereas"<<endl;
    cout<<"-5-"<<"  LATAM Brasil"<<endl<<endl;
    cout<<"-6-"<<"  Regresar al menu anterior"<<endl<<endl;
    cout<<"---"<<"  Introduzca el numero exacto de la opcion: ";
    cin>>resp; 
    if(cin.fail()){
        cin.clear();
        cin.ignore();
        resp = 10;
    } 
    cout<<endl;
    return resp;
}

int submenuAviones(){   //Submen� para la gesti�n de aviones
    int resp;
    limpiar();
    cout<<"---"<<setw(33)<<"Seleccione una opcion"<<setw(15)<<"---"<<endl<<endl;
    cout<<"-1-"<<"  Introducir un avion"<<endl;
    cout<<"-2-"<<"  Eliminar un avion"<<endl;
    cout<<"-3-"<<"  Modificar un avion"<<endl;
    cout<<"-4-"<<"  Lista de aviones activos"<<endl<<endl;
    cout<<"-5-"<<"  Regresar al menu anterior"<<endl<<endl;
    cout<<"---"<<"  Introduzca el numero exacto de la opcion: ";
    cin>>resp; 
    if(cin.fail()){
        cin.clear();
        cin.ignore();
        resp = 10;
    } 
    cout<<endl;
    return resp;
}

//Utilidades:
void limpiar(){
    system("cls");
    //system("clear");
}

bool checkHora(int hour, int min, bool tipo){  //Chequear si la hora y fecha tienen valores correctos
	time_t update = time(0);
   	tm *utm = localtime(&update);
    if(hour > -1 && hour < 24 && min >= 0 && min < 60 && (tipo == 0 || tipo == 1)) return true;
    return false;
}

bool checkDestino(vector<string> pDestinos, string destino){ //Chequear si el destino es correcto
    int i;
    for(i = 0; i < pDestinos.size(); i++){
            if(pDestinos[i] == destino) return true;
    }
    return false;
}

void showDestinos(vector<string> destinos){
    int i,j=0;
    cout<<"Introduzca el destino de la siguiente lista (Considere mayusculas): "<<endl;;
    cout<<endl;
    for(i=0; i<destinos.size(); i++){
        cout<<setw(26)<<destinos[i].c_str();
        j++;
        if(j == 3){
            cout<<endl;
            j = 0;
        }
    }
    cout<<endl;
}

void showHoraFecha(time_t time){
	char buffer[80];
	tm *contenedor = localtime(&time);
	strftime(buffer,sizeof(buffer),"%d/%m/%y - %I:%M%p",contenedor);
	cout<<setw(37)<<"FECHA/HORA ACTUAL: "<<buffer<<endl<<endl;
}

//Documentaci�n de estudio
//
//------------------------Archivos------------------------------------
//--Declarando la variable de lectura/escritura
//fstream contenedor;
//
//--Inicializando
//contenedor.open("Nombre el archivo", ios::in || ios::out);
//
//--Escribiendo en el archivo
//contenedor<<texto<<endl;
//
//--Leyendo del archivo
//contenedor>>texto;
//
//Leer m�ltiples l�neas
/*
  if (archivo.is_open()) {
      std::string linea;
      while (getline(archivo, linea)) {
          // using printf() in all tests for consistency
          printf("%s", linea.c_str());
      }
      archivo.close();
  }
*/
//--Cerrando el archivo
//contenedor.close();
//
//
//--------------------Gestion de Tiempo-------------------------------
//Condiciones de tiempo
/*

time_t now = time(0);

tm *ltm = localtime(&now);

cout << "Current min is " << ltm->tm_min<<endl;
getchar();

while(ltm->tm_min < 23){
   time_t update = time(0);
   tm *utm = localtime(&update);
   cout << "Current min is " << ltm->tm_min<<endl;
   getchar();
}

*/
/*
struct tm {
  int tm_sec;
  int tm_min
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
*/
