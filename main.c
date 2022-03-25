#include <stdio.h>
#include <stdlib.h>

#include "analizadorsintactico.h"
#include "tablasimbolos.h"
#include "sistemadeentrada.h"
#include "analizadorlexico.h"
#include "gestionerrores.h"
//ALT+SHIFT+T

int main(int argc,char **argv){
    if(argc<2){
      errorParametros();
    }
    inicializarTabla();
    iniciaSistema(argv[1]);
    analizar_codigo();

    eliminarTabla();
    salirSistema();
    return 0;

}
