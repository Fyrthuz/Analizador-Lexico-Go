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
  //Arrayq ue contiene los valores que corresponden a los diferentes lexemas de las palabras reservadas las insertamos ene ste orden para equilibrar lo máximo posible el árbol
    short ids[] = {GOTO, ELSE, RETURN, CONST, FUNC, INTERFACE, SWITCH, CASE, DEFAULT, FOR, GO, IF, PACKAGE, STRUCT, TYPE, BREAK, CHAN, CONTINUE, DEFER, FALLTHROUGH, IMPORT, MAP, RANGE, SELECT, VAR};
  char nombre[25][12] = {"goto", "else", "return", "const", "func", "interface", "switch", "case", "default", "for", "go", "if", "package", "struct", "type", "break", "chan", "continue", "defer", "fallthrough", "import", "map", "range", "select", "var"};
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
