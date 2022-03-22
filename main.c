#include <stdio.h>
#include <stdlib.h>

#include "analizadorsintactico.h"
#include "tablasimbolos.h"
#include "sistemadeentrada.h"
#include "analizadorlexico.h"
//ALT+SHIFT+T

int main(int argc,char **argv){
    if(argc<2){
      printf("No introdujo el fichero a analizar\n");
      exit(0);
    }
    inicializarTabla();
    iniciaSistema(argv[1]);
    analizar_codigo();

    eliminarTabla();
    salirSistema();
    return 0;

}
