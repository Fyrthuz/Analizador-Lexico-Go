#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionerrores.h"
#define N 16//potencia de la unidad de asignación (4096 bytes)

typedef struct{
  char mem[N+1];//el ultimo para el centinela
  short inicial;
  short final;
}centinela;

typedef struct{
  centinela centinelas[2];
}doblebuffer;


FILE *codigo;
char nombre_archivo[64];
doblebuffer buffer;
short cent=0;//centinela activo primero o segundo
long caracteres=0L;//Posicion en el fichero
short cargar=1;//Para saber si puedo o no cargar el nuevo bufer


void inicializarDobleBuffer(){
  buffer.centinelas[0].inicial = 0;
  buffer.centinelas[1].inicial = 0;
  buffer.centinelas[0].final = 0;
  buffer.centinelas[1].final = 0;
  cent=0;
 }

 void fill_buffer(){
   fseek(codigo,caracteres,SEEK_SET);
   memset(buffer.centinelas[cent].mem,'\0',N); //NO HACE FALTA YA QUE SE LLEGA ANTES AL EOF
   caracteres += fread(buffer.centinelas[cent].mem, sizeof(char), N, codigo);
   buffer.centinelas[cent].inicial = 0;
   buffer.centinelas[cent].final = 0;
   buffer.centinelas[cent].mem[N]=EOF;
   if(ferror(codigo)){
     errorArchivo(nombre_archivo);
     exit(EXIT_FAILURE);
   }
   //printf("\n%ld   %ld\n",caracteres,tamanoano);
   //printf("\nBUFFER LLENADO: %s",buffer.centinelas[cent].mem);
 }

void cambiarBufer(){
  cent = (cent + 1)%2;
}

void salirSistema(){
  fclose(codigo);
}

char sigCaracter(){
  char caracter;
  if((caracter = buffer.centinelas[cent].mem[buffer.centinelas[cent].final]) == EOF){ //Comprueba que el siguiente caracter no sea EOF
    //printf("\n%ld  %ld  %d\n",caracteres ,tamanoano, buffer.centinelas[cent].final);
    if(!feof(codigo)){ //Si no he llegado al final del código hay que cargar el siguiente búfer
      cambiarBufer();
      if(cargar){
        fill_buffer();
      }else{
        cargar = 1;
      }
      caracter = sigCaracter();
    }//No hace falta aumentar el puntero de lectura porque ya ha acabado el proceso al encontrar el fin de fichero
  }else{
    buffer.centinelas[cent].final += 1;
  }
  return caracter;

}

void devolverCaracter(){
  if(buffer.centinelas[cent].final==0){//Tengo que ir al otro buffer
    cambiarBufer();//Cambio bufer
    buffer.centinelas[cent].final -= 1; //retrocedo para que no apunte al EOF
    cargar = 0;//No quiero que me cambie le bufer ya leído
  }else{
    buffer.centinelas[cent].final -= 1;
  }
}


void iniciaSistema(char *fichero){
  inicializarDobleBuffer();
  strcpy(nombre_archivo,fichero);
  if((codigo = fopen(fichero,"r")) == NULL){
    errorArchivo(nombre_archivo);
    exit(EXIT_FAILURE);
  }
  //Llenamos el primer bufer
  fill_buffer();

}

void aceptar_lexema(){
  buffer.centinelas[cent].inicial = buffer.centinelas[cent].final;
}
