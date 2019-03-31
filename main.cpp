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

struct Avion{           //Registro para almacenar un avión
    string nombre;
    int tiempoCarga;    //Tiempo de descarga en minutos
    int tiempoDescarga; //Tiempo de carga en minutos
    int tiempoPista;    //Tiempo que pasa en la pista
};

struct Vuelo{           //Registro para almacenar un vuelo
    tm *hora;           //Hora en formatos hh:mm, hora = hh, min = mm           
    bool tipo;          //0 si es salida, 1 si es entrada
    string destino;
    Avion avion;        //Avión que usará el vuelo
};

class Aerolinea{        //Clase para gestionar aerolíneas
    public:
    string nombre;
    vector <Vuelo> vuelos;

    //Agregar un vuelo con su hora, tipo y destino
    void agregar(int pHora, int pMin, bool pTipo, string pDestino){
        Vuelo contenedor;
        time_t now = time(NULL);
        contenedor.hora = localtime(&now);
        contenedor.hora->tm_hour = pHora;
        contenedor.hora->tm_min = pMin;
        contenedor.tipo = pTipo;
        contenedor.destino = pDestino;
        vuelos.push_back(contenedor);
    }

    //Eliminar un vuelo segun su hora y destino
    void eliminar(int pHora, int pMin, bool pTipo){
        int i = 0, index;
        if(!vuelos.empty()){
            cout<<"--- No hay vuelos asignados para esta aerolinea (Enter para continuar)"<<endl;
            getchar();
            return;
        }
        for(i=0; i<vuelos.size(); i++){
            if(vuelos[i].hora->tm_hour == pHora && vuelos[i].hora->tm_min == pMin && vuelos[i].tipo == pTipo){
                index = i;
            }
        }
        vuelos.erase(vuelos.begin()+index);
        cout<<index;
    }
};

int main()
{
    //Declaraciones
    int i, j, opc = 0; 
    bool salir = false;
    fstream datos;
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
    
        while(salir == false){
            opc = menuGeneral();
            time_t hora;
            char buffer[80];
            switch(opc){
                case 1: //Gestión de vuelos por aerolínea
                    while(salir == false){
                        int h,m; string d; bool t;
                        opc = submenuVuelos();
                        limpiar();
                        switch(opc){
                            case 1: //Avianca Brasil
                                do{
                                    limpiar();
                                    cout<<"Intrododuzca la hora en el formato solicitado (hh-mm):"<<endl;
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
                                //
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
                                //
                                aerolinea[0].agregar(h,m,t,d);
                                break;
                            case 2: //Azul Lineas Aereas
                                do{
                                    limpiar();
                                    cout<<"Intrododuzca la hora en el formato solicitado (hh-mm):"<<endl;
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
                                //
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
                                //
                                aerolinea[1].agregar(h,m,t,d);
                                break;
                            case 3: //GOL Lineas Aereas
                                do{
                                    limpiar();
                                    cout<<"Intrododuzca la hora en el formato solicitado (hh-mm):"<<endl;
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
                                //
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
                                //
                                aerolinea[2].agregar(h,m,t,d);
                                break;
                            case 4: //Passared Lineas Aereas
                                do{
                                    limpiar();
                                    cout<<"Intrododuzca la hora en el formato solicitado (hh-mm):"<<endl;
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
                                //
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
                                //
                                aerolinea[3].agregar(h,m,t,d);
                                break; 
                            case 5: //LATAM Brasil
                                do{
                                    limpiar();
                                    cout<<"Intrododuzca la hora en el formato solicitado (hh-mm):"<<endl;
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
                                //
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
                                //
                                aerolinea[4].agregar(h,m,t,d);
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
                case 3: //Visualización del aeropuerto
                    //Vuelos programados
                    while(salir == false){
                        limpiar();
                        cout<<setw(40)<<"*** VUELOS PROGRAMADOS ***"<<endl;
                        for(i=0; i<5; i++){
                            cout<<"----"<<aerolinea[i].nombre<<"----"<<endl;
                            for(j=0; j<aerolinea[i].vuelos.size(); j++){
                                cout<<setw(15)<<aerolinea[i].vuelos[j].destino.c_str();
                                cout<<setw(10)<<1 + aerolinea[i].vuelos[j].hora->tm_hour<<":"<<1 + aerolinea[i].vuelos[j].hora->tm_hour;
                                cout<<setw(6)<<strftime(buffer,80,"%H %M %p",aerolinea[i].vuelos[j].hora)<<endl;
                            }
                        }
                        cout<<setw(40)<<"*** VUELOS EN ESPERA ***"<<endl;
                        getchar();
                        salir = true;
                    }
                    salir = false;
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
    limpiar();
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
    limpiar();
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
    system("clear");
}

bool checkHora(int hour, int min, bool tipo){  //Chequear si la hora tiene valores correctos
    int i;
    if(hour > -1 && hour < 24 && min > 0 && min < 60 && (tipo == 0 || tipo == 1)) return true;
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