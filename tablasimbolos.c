#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estructura.h"
#include "gestionerrores.h"
#include "definiciones.h"

//variable global donde se almacena la tabla de símbolos
abb tabla;

void inicializarTabla(){
  //Arrayq ue contiene los valores que corresponden a los diferentes lexemas de las palabras reservadas
  short ids[] = {BREAK, DEFAULT, FUNC, INTERFACE, SELECT, CASE, DEFER, GO, MAP, STRUCT, CHAN, ELSE, GOTO, PACKAGE, SWITCH, CONST, FALLTHROUGH, IF, RANGE, TYPE, CONTINUE, FOR, IMPORT, RETURN, VAR};
  char nombre[25][12] = {"break", "default", "func", "interface", "select", "case", "defer", "go", "map", "struct", "chan", "else", "goto", "package", "switch", "const", "fallthrough", "if", "range", "type", "continue", "for", "import", "return", "var"};
  //Instaciamos la tabla de símbolos
  crea(&tabla);
  //Iteramos sobre los arrays para introducir la información sobre las palabras reservadas en la tabla de símbolos
  for(int i = 0 ; i < 25 ; i++){
    inserta_busca(&tabla,nombre[i],ids[i]);
  }

}

//Imprimimos la tabla de símbolos
void imprimirTabla(){
  imprimirArbol(tabla);
}

//Destruimos la tabla de simbolos
void eliminarTabla(){
  printf("\n-----------------------Tabla destruida----------------------------\n");
  destruye(&tabla);
}

//Introducimos un elemento en la tabla de símbolos
short insertar_buscaEnTabla(char *cadena,short tipo){
  return inserta_busca(&tabla,cadena,tipo);
}
