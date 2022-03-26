#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizadorlexico.h"
#include "sistemadeentrada.h"
#include "tablasimbolos.h"

void analizar_codigo(){
  comp_lexico lex;
  //Inicializamos a NULL el lexema del componente lexico
  lex.lexema = NULL;
  //Imprimimos la tabla de simbolos antes de empezar la ejecución
  imprimirTabla();
  //Pedimos el primer componente lexico
  siguiente_comp_lexico(&lex);
  //Comienza la ejecucion, ejecutaremos hasta encontrar el EOF
  while(lex.tipo != EOF){
    //Si el lexema es \n queremos imprimir literalmente este caracter, este es el motivo de este if
    if(lex.tipo != '\n'){
      printf("\n<%hd,%s>\n",lex.tipo,lex.lexema);
    }else{
      printf("\n<%hd,\\n>\n",lex.tipo);
    }
    //Pedimos el siguiente componente lexico
    siguiente_comp_lexico(&lex);
  }
  //Indicamos que la ejecución ha acabado
  printf("\n-------------Analisis lexico finalizado-----------\n");
  //Imprimimos la tabla
  imprimirTabla();
  //Liberamos la memoria correspondiente al lexema de la estructura que hemos utilizado
  free(lex.lexema);
  lex.lexema = NULL;

}
