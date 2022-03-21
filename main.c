#include <stdio.h>
#include <stdlib.h>
#include "analizadorsintactico.h"
#include "tablasimbolos.h"
#include "sistemadeentrada.h"

#include "definiciones.h"
#include "analizadorlexico.h"
//ALT+SHIFT+T

int main(){

    inicializarTabla();
    iniciaSistema("./concurrentSum.go");
    analizar_codigo();

    eliminarTabla();
    salirSistema();
    return 0;

}
