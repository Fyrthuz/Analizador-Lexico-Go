#include <stdio.h>
#include <stdlib.h>

void errorLexico(int linea){
  printf("\nError lexico en la linea: %d\n",linea);
}

void errorArchivo(char *s){
  printf("\nError al abrir el archivo: %s\n",s); 
}
