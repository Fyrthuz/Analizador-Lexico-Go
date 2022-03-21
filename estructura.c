#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

typedef struct{
  char *lexema;
  short tipo;
}elemento;


struct nodo{
  elemento info;
  struct nodo *izq,*der;
};

typedef struct nodo *abb;



void crea(abb *a){
  *a = NULL;
}

void destruye(abb *a){
  if(*a != NULL){
    destruye(&(*a)->izq);
    destruye(&(*a)->der);
    (*a)->izq = NULL;
    (*a)->der = NULL;
    free(((*a)->info.lexema));
    free(*a);
    *a = NULL;
  }
}

unsigned vacio(abb a){
  return a==NULL;
}

short inserta(abb *a, char *lexema,short tipo){
  if(vacio(*a)){
    *a = (abb)malloc(sizeof(struct nodo));
    (*a)->info.lexema = (char*)malloc(strlen(lexema)+1*sizeof(char));
    strcpy((*a)->info.lexema,lexema);
    (*a)->info.tipo = tipo;
    (*a)->izq = NULL;
    (*a)->der = NULL;
    //printf("Se ha insertado el elemento %s del tipo %d\n",lexema,ID);
    return (*a)->info.tipo;
  }else if(strcmp(lexema,(*a)->info.lexema)==0){
    return (*a)->info.tipo;
  }else if(strcmp(lexema,(*a)->info.lexema)<0){
    return inserta(&(*a)->izq,lexema,tipo);
  }else{
    return inserta(&(*a)->der,lexema,tipo);
  }
}


void imprimirNodo(abb a){
  printf("\tLexema:%s\t",a->info.lexema);
  printf("\t\tComponente:%d\t\n",a->info.tipo);
}

void imprimir(abb a){
    imprimirNodo(a);
    if(!vacio(a->izq)){
      imprimir(a->izq);
    }
    if(!vacio(a->der)){
      imprimir(a->der);
    }
}

void imprimirArbol(abb a){
  printf("\n\n---------------------------Tabla de simbolos----------------------------\n");
  imprimir(a);
  printf("\n-----------------------------------------------------------------------\n\n");
}
