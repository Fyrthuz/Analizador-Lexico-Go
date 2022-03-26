#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestionerrores.h"
#define N 32//potencia de la unidad de asignación (4096 bytes)

//estructura que nos sirve de buffer
typedef struct{
  char mem[N+1];//el ultimo para el centinela(EOF)
  short inicial;//puntero inicial
  short final;//puntero final
}centinela;

//Estructura del doble buffer, array de dos buffers
typedef struct{
  centinela centinelas[2];
}doblebuffer;


FILE *codigo;//Variable global que nos sirve para identificar al archivo del codigo fuente
char nombre_archivo[128];
doblebuffer buffer;//Variable global que contiene al doble buffer
short cent=0;//centinela activo primero o segundo
long caracteres=0L;//Posicion en el fichero
short cargar=1;//Para saber si puedo o no cargar el nuevo bufer

//Funcion que nos sirve para inicializar el doble buffer
void inicializarDobleBuffer(){
  buffer.centinelas[0].inicial = 0;
  buffer.centinelas[1].inicial = 0;
  buffer.centinelas[0].final = 0;
  buffer.centinelas[1].final = 0;
  cent=0;
 }

//Funcion que nos sirve para rellenar un buffer con los siguientes caracteres del codigo fuente
 void fill_buffer(){
   //Movemos el puntero del fichero al lugar donde habíamos acabado de leer la iteración anterior
   fseek(codigo,caracteres,SEEK_SET);
   //Inicializamos todos los caracteres del bufer a \o y
   //Copiamos los caracteres en el bufer correspondiente
   memset(buffer.centinelas[cent].mem,'\0',N); //NO HACE FALTA YA QUE SE LLEGA ANTES AL EOF
   caracteres += fread(buffer.centinelas[cent].mem, sizeof(char), N, codigo);
   //Inicializamos los punteros y confirmamos que en la ultima posicion del buffer se guarda el EOF
   buffer.centinelas[cent].inicial = 0;
   buffer.centinelas[cent].final = 0;
   buffer.centinelas[cent].mem[N]=EOF;
   if(ferror(codigo)){
     errorArchivo(nombre_archivo);
   }
   //printf("\n%ld   %ld\n",caracteres,tamanoano);
   //printf("\nBUFFER LLENADO: %s",buffer.centinelas[cent].mem);
 }

//Funcion que cambia el valor de la variable que nos indica el buffer que esta siendo utilizado
void cambiarBufer(){
  cent = (cent + 1)%2;
}

//Funcion que nos cierra el fichero la utilizamos cuando acaba el analisis lexico
void salirSistema(){
  fclose(codigo);
}

//Funcion que nos devuelve el siguiente caracter del codigo
char sigCaracter(){
  char caracter;
  if((caracter = buffer.centinelas[cent].mem[buffer.centinelas[cent].final]) == EOF){ //Comprueba que el siguiente caracter no sea EOF
    if(!feof(codigo)){ //Si no he llegado al final del código hay que cargar el siguiente búfer
      cambiarBufer();
      if(cargar){//Si se puede cargar cambiamos el bufer
        fill_buffer();
      }else{//Si no ponemos la variable a 1 para evitar que el codigo se pare,
        cargar = 1;
      }
      //Volvemos a pedir el siguiente caracter para no devolver un EOF que no se correspone con el siguiente caracter
      caracter = sigCaracter();
    }//No hace falta aumentar el puntero de lectura porque ya ha acabado el proceso al encontrar el fin de fichero
  }else{
    //Aumentamos el puntero final del buffer
    buffer.centinelas[cent].final += 1;
  }
  return caracter;

}

//Funcion que nos retrasa una posicion el puntero final del buffer
void devolverCaracter(){
  if(buffer.centinelas[cent].final==0){//Tengo que ir al otro buffer
    cambiarBufer();//Cambio bufer
    buffer.centinelas[cent].final -= 1; //retrocedo para que no apunte al EOF
    cargar = 0;//No quiero que me sobreescriba el bufer ya leído
  }else{
    //Retrocedo una posicion el puntero final
    buffer.centinelas[cent].final -= 1;
  }
}

//Inicializamos el sistema
void iniciaSistema(char *fichero){
  //Inicializamos los dos bufferes
  inicializarDobleBuffer();
  //Guardamos el nombre del fichero
  strcpy(nombre_archivo,fichero);
  //Abrimos el fichero
  if((codigo = fopen(fichero,"r")) == NULL){
    errorArchivo(nombre_archivo);
  }
  //Llenamos el primer bufer
  fill_buffer();

}

//Esta funcion nos indica que el lexema actual ha sido aceptado por que igualamos el puntero inicial con el final
void aceptar_lexema(){
  buffer.centinelas[cent].inicial = buffer.centinelas[cent].final;
}
