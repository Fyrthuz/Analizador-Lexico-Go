#include <stdio.h>
#include <stdlib.h>

void errorLexico(int linea){
  printf("\nError lexico en la linea: %d\n",linea);
}

void errorArchivo(char *s){
  printf("\nError al abrir el archivo: %s\n\nParando ejecucion del programa...",s);
  exit(EXIT_FAILURE);
}

void errorParametros(){
  printf("\nIntroduzca el codigo fuente que quiere compilar\n\nParando ejecucion del programa...\n");
  exit(EXIT_FAILURE);
}
