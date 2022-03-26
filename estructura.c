#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

//Estructura correspondiente a la información que guarda un nodo
typedef struct{
  char *lexema;
  short tipo;
}elemento;

//Estructura de un nodo del arbol
struct nodo{
  elemento info;
  struct nodo *izq,*der;
};

//Definimos el arbol binario como un puntero a un nodo
typedef struct nodo *abb;


//Inicializamos el nodo igualandolo a NULL
void crea(abb *a){
  *a = NULL;
}

//Función que nos sirve para liberar toda la memoria del árbol y destruírlo
void destruye(abb *a){
  if(*a != NULL){
    destruye(&(*a)->izq);
    destruye(&(*a)->der);
    (*a)->izq = NULL;
    (*a)->der = NULL;
    free(((*a)->info.lexema));
    (*a)->info.lexema = NULL;
    free(*a);
    *a = NULL;
  }
}

//Si el arbol es igual a NULL entonces esta vacío
unsigned vacio(abb a){
  return a==NULL;
}

//Esta función inserta y busca al mismo tiempo, si no encuentra el elemento
//entonces este es inserta en la tabla, si lo encuentra devuelve el tipo de componentes
//lexico que identifica al lexema
short inserta_busca(abb *a, char *lexema,short tipo){
  //Si el nodo esta vacío quiere decir que no existe en el árbol por lo que hay que insertarlo
  if(vacio(*a)){
    *a = (abb)malloc(sizeof(struct nodo));
    (*a)->info.lexema = (char*)malloc(strlen(lexema)+1*sizeof(char));
    strcpy((*a)->info.lexema,lexema);
    (*a)->info.tipo = tipo;
    (*a)->izq = NULL;
    (*a)->der = NULL;
    return (*a)->info.tipo;
  }else if(strcmp(lexema,(*a)->info.lexema)==0){
    return (*a)->info.tipo;
  }else if(strcmp(lexema,(*a)->info.lexema)<0){
    return inserta_busca(&(*a)->izq,lexema,tipo);
  }else{
    return inserta_busca(&(*a)->der,lexema,tipo);
  }
}

//Funcion que sirve para imprimir un nodo
void imprimirNodo(abb a){
  printf("\tLexema:%s\t",a->info.lexema);
  printf("\t\tComponente:%d\t\n",a->info.tipo);
}

//Funcion que cuando es llamda imprimer todo el arbol binario en un recorrido R-I-D
void imprimir(abb a){
    imprimirNodo(a);
    if(!vacio(a->izq)){
      imprimir(a->izq);
    }
    if(!vacio(a->der)){
      imprimir(a->der);
    }
}

//Funcion pública que nos permite imprimir el árbol
void imprimirArbol(abb a){
  printf("\n\n---------------------------Tabla de simbolos----------------------------\n");
  imprimir(a);
  printf("\n-----------------------------------------------------------------------\n\n");
}
