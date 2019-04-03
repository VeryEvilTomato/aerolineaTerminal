#include<iostream>  //Funciones básicas de la consola
#include<fstream>   //Lectura y escritura de archivos
#include<iomanip>   
#include<string>    //Declaración de variables tipo String
#include<vector>    //Contenedor de datos al leer información del archivo
#include<cstdio>
#include<ctime>
#include<algorithm>

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

struct Tiempo{
	int duracion, i, j;
	int hora,min;
};

struct Vuelo{           //Registro para almacenar un vuelo
    int hora, min;		//Hora en formatos hh:mm/hora = hh, min = mm
    bool tipo;          //0 si es salida, 1 si es entrada
    string destino;
    Avion avion;        //Avión que usará el vuelo
	int estado;
};

class Aerolinea{        //Clase para gestionar aerolíneas
    public:
    string nombre;
    vector <Vuelo> vuelos;

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
    int i, j, k, opc = 0; 
    bool salir = false;
    fstream datos;
	vector <Tiempo> vuelosDescarga;
	vector <Tiempo> vuelosCarga;
    vector <Avion> aviones;
    Avion avContenedor;

    //Inicializando temporizador
    clock_t c_inicio = clock();

    //Inicializando aerolíneas
    Aerolinea aerolinea[5];
        aerolinea[0].nombre = "Avianca Brasil";
        aerolinea[1].nombre = "Azul Lineas Aereas";
        aerolinea[2].nombre = "GOL Lineas Aereas";
        aerolinea[3].nombre = "Passared o Lineas Aereas";
        aerolinea[4].nombre = "LATAM Brasil";

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

    //
    //Menú principal

	avContenedor.nombre = "Boeing747";
	avContenedor.tiempoCarga = 2;
	avContenedor.tiempoDescarga = 1;
	avContenedor.tiempoPista = 2;
	aviones.push_back(avContenedor);
	avContenedor.nombre = "AirbusA320";
	avContenedor.tiempoCarga = 2;
	avContenedor.tiempoDescarga = 1;
	avContenedor.tiempoPista = 2;
	aviones.push_back(avContenedor);
	avContenedor.nombre = "TupolevTu-204";
	avContenedor.tiempoCarga = 2;
	avContenedor.tiempoDescarga = 1;
	avContenedor.tiempoPista = 2;
	aviones.push_back(avContenedor);
    
	
	while(salir == false){
		limpiar();
		time_t menu = time(0);
		opc = menuGeneral();
		char buffer[80];
		int input;
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
						case 1: //Introducir un avión
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
						case 2: //Eliminar un avión
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
						case 3: //Modificar un avión
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
							getchar();
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
				break;
			case 3: //Visualización del aeropuerto en tiempo real
				//Vuelos programados
				{
					time_t tInit = time(0);
					tm *init = localtime(&tInit);
					int initHora = init->tm_hour,
						initMin  = init->tm_min;
					tm *contenedor = localtime(&tInit);			
					while(salir == false){
						limpiar();
						time_t tNow = time(0);
						tm *now = localtime(&tNow);
						showHoraFecha(tNow);

						if(!vuelosCarga.empty()){
							for(i=0; i<vuelosCarga.size(); i++){
								tNow = time(0);
								now = localtime(&tNow);
								if(now->tm_hour >= vuelosCarga[i].hora){
									if((now->tm_hour == vuelosCarga[i].hora && now->tm_min >= vuelosCarga[i].min) || now->tm_hour > vuelosCarga[i].hora){
										aerolinea[vuelosCarga[i].i].vuelos[vuelosCarga[i].j].estado = 3;
										for(j=0; j<vuelosCarga.size()-1; j++){
											vuelosCarga[i] = vuelosCarga[i+1];
										}
										vuelosCarga.erase(vuelosCarga.begin()+vuelosCarga.size());
									}
								}
							}
						}

						if(!vuelosDescarga.empty()){
							for(i=0; i<vuelosDescarga.size(); i++){
								tNow = time(0);
								now = localtime(&tNow);
								if(now->tm_hour >= vuelosDescarga[i].hora){
									if((now->tm_hour == vuelosDescarga[i].hora && now->tm_min >= vuelosDescarga[i].min) || now->tm_hour > vuelosDescarga[i].hora){
										aerolinea[vuelosDescarga[i].i].vuelos[vuelosDescarga[i].j].estado = 3;
										for(j=0; j<vuelosDescarga.size()-1; j++){
											vuelosDescarga[i] = vuelosDescarga[i+1];
										}
										vuelosDescarga.erase(vuelosDescarga.begin()+vuelosDescarga.size());
									}
								}
							}
						}

						cout<<setw(50)<<"*** VUELOS PROGRAMADOS ***"<<endl;
						for(i=0; i<5; i++){ //Ciclo por cada aerolínea
							if(!aerolinea[i].vuelos.empty()){
								//Se chequean los vuelos programados
								for(j=0; j<aerolinea[i].vuelos.size(); j++){
									tNow = time(0);
									now = localtime(&tNow);
									if(aerolinea[i].vuelos[j].hora >= initHora && (aerolinea[i].vuelos[j].estado == 0)){
										if( (aerolinea[i].vuelos[j].hora == initHora && aerolinea[i].vuelos[j].min >= initMin) || (aerolinea[i].vuelos[j].hora > initHora)){
											if(now->tm_hour >= aerolinea[i].vuelos[j].hora){
												if(((now->tm_hour == aerolinea[i].vuelos[j].hora) && (now->tm_min >= aerolinea[i].vuelos[j].min)) || (now->tm_hour > aerolinea[i].vuelos[j].hora)){
													aerolinea[i].vuelos[j].avion = aviones[rand() % aviones.size()];
													aerolinea[i].vuelos[j].avion.tiempoCarga += rand()%3;
													aerolinea[i].vuelos[j].avion.tiempoDescarga += rand()%3;
													aerolinea[i].vuelos[j].avion.tiempoPista += rand()%3;
													aerolinea[i].vuelos[j].estado = 1;
												}
											}		
										}
									}				
								}
								//Se muestran los vuelos que están programados
								cout<<"----"<<aerolinea[i].nombre<<"----"<<endl;
								cout<<setw(20)<<"_Destino"<<setw(12)<<"_Hora"<<setw(12)<<"_Tipo"<<setw(13)<<"_Estado"<<setw(15)<<"_Avion"<<setw(10)<<"_Fin"<<endl;
								for(j=0; j<aerolinea[i].vuelos.size(); j++){
									string sEstado;
									contenedor->tm_hour = aerolinea[i].vuelos[j].hora;
									contenedor->tm_min  = aerolinea[i].vuelos[j].min;
									cout<<"|"<<setw(19)<<aerolinea[i].vuelos[j].destino.c_str();
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
									for(k=0; k<vuelosDescarga.size(); k++){
										if(vuelosDescarga[k].i == i && vuelosDescarga[k].j == j){
											contenedor->tm_hour = vuelosDescarga[k].hora;
											contenedor->tm_min  = vuelosDescarga[k].min;
											strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
											cout<<setw(10)<<buffer;
										}
									}
									for(k=0; k<vuelosCarga.size(); k++){
										if(vuelosCarga[k].i == i && vuelosCarga[k].j == j){
											contenedor->tm_hour = vuelosCarga[k].hora;
											contenedor->tm_min  = vuelosCarga[k].min;
											strftime(buffer,sizeof(buffer),"%I:%M%p",contenedor);
											cout<<setw(10)<<buffer;
										}
									}
									cout<<endl;
								}
								cout<<endl;
							}
						}
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
							case 2: //Introducción de 1 vuelo en Espera a su respectivo terminal (Carga/Descarga)
								{
									Tiempo contenedor;
									bool check = 0;
									for(i=0; i<5; i++){
										for(j=0; j<aerolinea[i].vuelos.size(); j++){
											tNow = time(0);
											now = localtime(&tNow);
											int cMin, cHora;
											if(aerolinea[i].vuelos[j].tipo == 0 && aerolinea[i].vuelos[j].estado == 1 && (vuelosCarga.size() < 5)){
												contenedor.duracion = aerolinea[i].vuelos[j].avion.tiempoCarga + aerolinea[i].vuelos[j].avion.tiempoPista;
												contenedor.i = i;
												contenedor.j = j;
												//Ajuste de la hora
												if((now->tm_min + contenedor.duracion) > 59){
													cout<<"Pass"<<endl;
													cMin = contenedor.duracion + now->tm_min - 60; 
													cHora = now->tm_hour + 1;
												}
												else{
													cout<<"Pass"<<endl;
													cMin = now->tm_min + contenedor.duracion;
													cHora = now->tm_hour;
												}
												//
												contenedor.hora = cHora;
												contenedor.min	= cMin;
												vuelosCarga.push_back(contenedor);
												aerolinea[i].vuelos[j].estado = 2;
												check = 1;
												break;
											}											
										}
										if(check == 1) break;
									}
								}								
								break;
							case 3:
								{
									Tiempo contenedor;
									bool check = 0;
									for(i=0; i<5; i++){
										for(j=0; j<aerolinea[i].vuelos.size(); j++){
											tNow = time(0);
											now = localtime(&tNow);
											int cMin, cHora;
											if(aerolinea[i].vuelos[j].tipo == 1 && aerolinea[i].vuelos[j].estado == 1 && (vuelosDescarga.size() < 3)){
												contenedor.duracion = aerolinea[i].vuelos[j].avion.tiempoDescarga + aerolinea[i].vuelos[j].avion.tiempoPista;
												contenedor.i = i;
												contenedor.j = j;
												//Ajuste de la hora
												if((now->tm_min + contenedor.duracion) > 59){
													cMin = contenedor.duracion + now->tm_min - 60; 
													cHora = now->tm_hour + 1;
												}
												else{
													cMin = now->tm_min + contenedor.duracion;
													cHora = now->tm_hour;
												}
												//
												contenedor.hora = cHora;
												contenedor.min	= cMin;
												vuelosDescarga.push_back(contenedor);
												aerolinea[i].vuelos[j].estado = 2;
												check = 1;
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
    cout<<"El programa funciono por: "<<(c_fin - c_inicio)/CLOCKS_PER_SEC<<" segundos"<<endl;
    getchar();  

    //Recaudando información del archivo
    datos.open("datos", ios::out | ios::in);
    datos.close();

    //Fin del programa
    limpiar();
    return 0;
}

//Menúes del sistema

int menuGeneral(){      //Menú general de todo el programa
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

int submenuVuelos(){    //Submenú para la gestión de vuelos
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

int submenuAviones(){   //Submenú para la gestión de aviones
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

bool checkDestino(vector<string> pDestinos, string destino){
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

//Documentación de estudio
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
//Leer múltiples líneas
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