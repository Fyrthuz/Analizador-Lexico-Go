#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructura.h"
#include "gestionerrores.h"
#include "definiciones.h"

//Tamaño máximo de las palabras reservadas
#define TAM_MAXIMO 16

abb tabla;

void inicializarTabla(){
  short ids[] = {BREAK, DEFAULT, FUNC, INTERFACE, SELECT, CASE, DEFER, GO, MAP, STRUCT, CHAN, ELSE, GOTO, PACKAGE, SWITCH, CONST, FALLTHROUGH, IF, RANGE, TYPE, CONTINUE, FOR, IMPORT, RETURN, VAR};
  char nombre[25][12] = {"break", "default", "func", "interface", "select", "case", "defer", "go", "map", "struct", "chan", "else", "goto", "package", "switch", "const", "fallthrough", "if", "range", "type", "continue", "for", "import", "return", "var"};
  short elems = sizeof(ids)/sizeof(short);
  crea(&tabla);
  for(int i = 0 ; i < elems ; i++){
    inserta(&tabla,nombre[i],ids[i]);
  }

}

void imprimirTabla(){
  imprimirArbol(tabla);
}

void eliminarTabla(){
  printf("\n-----------------------Tabla destruida----------------------------\n");
  destruye(&tabla);
}

short insertar_buscaEnTabla(char *cadena,short tipo){
  return inserta(&tabla,cadena,tipo);
}
