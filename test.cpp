#include<iostream>  //Funciones básicas de la consola
#include<ctime>
#include<stdlib.h>
#include<string>
#include<cstdio>

using namespace std;

void f()
{
    volatile double d = 0;
    for(int n=0; n<10000; ++n)
       for(int m=0; m<10000; ++m)
           d += d*n*m;
}

int main(){
   time_t now = time(NULL);
   tm *ltm = localtime(&now);

   cout<<asctime(ltm);

   getchar();
   system("cls");
}

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