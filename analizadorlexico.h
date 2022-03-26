#include <stdio.h>
#include <stdlib.h>

//Estructuta correspondiente a un componente lexico
typedef struct{
  char *lexema;
  short tipo;
}comp_lexico;

//Funcion que nos devolverá el siguiente componente léxico del programa
short siguiente_comp_lexico(comp_lexico *comp);
