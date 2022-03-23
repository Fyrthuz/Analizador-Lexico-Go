#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizadorlexico.h"
#include "sistemadeentrada.h"
#include "tablasimbolos.h"

void analizar_codigo(){
  comp_lexico lex;
  int i=0;
  lex.lexema = NULL;
  imprimirTabla();
  while((i=siguiente_comp_lexico(&lex))!=EOF){
    if(lex.tipo != '\n'){
      printf("\n<%hd,%s>\n",lex.tipo,lex.lexema);
    }else{
      printf("\n<%hd,\\n>\n",lex.tipo);
    }

  }

  printf("\n-------------Analisis lexico finalizado-----------\n");
  imprimirTabla();
  free(lex.lexema);
  lex.lexema = NULL;

}
