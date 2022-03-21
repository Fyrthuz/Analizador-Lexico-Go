#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizadorlexico.h"
#include "sistemadeentrada.h"
#include "tablasimbolos.h"
#define N 16//tamaño maximo permitido lexema tamaño de un buffer del doble buffer

void analizar_codigo(){
  comp_lexico lex;
  int i=0;
  lex.lexema = NULL;
  imprimirTabla();
  while((i=siguiente_comp_lexico(&lex))!=EOF){
    if(lex.tipo != -2){
      printf("\n<%hd,%s>\n",lex.tipo,lex.lexema);
      //printf("\n--------------%d----------",i);
      i++;
      //if(i==120)break;
      //
    }

  }

  printf("\n-------------Analisis lexico finalizado-----------\n");
  imprimirTabla();
  free(lex.lexema);
  lex.lexema = NULL;

}
