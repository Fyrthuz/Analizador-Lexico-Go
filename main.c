#include <stdio.h>
#include <stdlib.h>

#include "analizadorsintactico.h"
#include "tablasimbolos.h"
#include "sistemadeentrada.h"
#include "gestionerrores.h"

int main(int argc,char **argv){
    //Pasamos como parametros el nombre del fichero, de otro modo ocurre un error
    if(argc!=2){
      errorParametros();
    }
    //Inicializamos la tabla de simbolos
    inicializarTabla();
    //Inicializamos el sistema de sistema de entrada
    iniciaSistema(argv[1]);
    //Empezamos a analizar el codigo
    analizar_codigo();

    //Liberamos la tabla de símbolos y liberamos el sistema de sistema de entrada
    //liberando también los recursos utilizados
    eliminarTabla();
    salirSistema();
    return 0;

}
