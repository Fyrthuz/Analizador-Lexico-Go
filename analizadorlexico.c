#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "analizadorlexico.h"
#include "sistemadeentrada.h"
#include "definiciones.h"
#include "tablasimbolos.h"
#include "gestionerrores.h"


long linea = 1;
long tam = 8;//Tamaño del lexema
long aumento = 8;//Aumento al realizar realloc de la memoria cuando se lee un lexema
comp_lexico comp;

void inicializarCompLexico(comp_lexico *comp){
  if(comp->lexema == NULL){
    comp->lexema = (char *)malloc(tam*sizeof(char));
  }
  memset(comp->lexema, '\0', tam);
  comp->tipo = 0;
}

void copiar(char caracter,comp_lexico *comp){
  char *aux;
  strncat(comp->lexema,&caracter,sizeof(char));
  if(strlen(comp->lexema)==tam-1){
    aux = (char *)malloc(tam*sizeof(char));
    //Inicializamos y copiamos el contenido del lexema a la cadena auxiliar
    memset(aux, '\0', tam);
    strncat(aux,comp->lexema,tam);
    //Aumentamos el tamaño máximo de lexema
    comp->lexema = (char *)realloc(comp->lexema,tam+aumento);
    tam +=aumento;
    //Inicializamos y volvemos a copiar a devuelta al lexema para evitar errores
    memset(comp->lexema, '\0', tam);
    strncat(comp->lexema,aux,tam);
    //Liberamos memoria
    free(aux);
    aux = NULL;
  }
}

short leerComentarios(comp_lexico comp){//pARA LEER TODOS LOS COMENTARIOS
  aceptar_lexema();//Podemos liberar los buffer ya que no tienen significado lexico
  char aux = sigCaracter();
  bucle:
  while((aux!='\n' && comp.tipo==COMENTARIO_LINEA) || (aux!='*' && comp.tipo==INICIO_COMENTARIO)){
    if(aux == EOF){
      errorLexico(linea);
      return -1;
    }
    aux = sigCaracter();
    if(aux=='\n'){
      linea ++;
    }
    aceptar_lexema();//Podemos liberar los buffer ya que no tienen significado lexico
  }
  if(comp.tipo == INICIO_COMENTARIO){
    aux=sigCaracter();
    if(aux=='\n'){
      linea ++;
    }
    if(aux!='/'){
      goto bucle;
    }

  }
  aceptar_lexema();//Podemos liberar los buffer ya que no tienen significado lexico
  return 0;
}

short siguiente_comp_lexico(comp_lexico *comp){
  char c = EOF;

  short state = 0;//Estado del automata

  short especial = 0;//Caracteres especiales strings

  short flag_number = 0;//Nos indica si un numero es entero o flotante
  short flag_sign = 0;//Nos indica si un numero tiene signo en el exponente
  short flag_exponent = 0;//Nos indica si un numero tiene exponente
  short flag_imaginary = 0;//Nos indica si un numero es imaginario
  short num_type = 0;//Indica si un numero empieza por 0x,ob,oO,etc
  //Inicializar componente lexico
  inicializarCompLexico(comp);
  while(1){
    switch(state){
      case -1://Llegamos al final de fichero acabamos proceso
        return EOF;
        break;
      case 0://Primer estado del automata de automatas
        c = sigCaracter();
        if(c == ' ' || c == '\t' || c == '\n'){
          if(c == '\n'){
            linea++;
            aceptar_lexema();
          }
          state=0;
        }else if(c == EOF){//Si es caracter nulo  o EOF se acaba la ejecucion
          state = -1;
        }else if(c==';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c==','){
          state = 4;
          break;
        }else if(isalpha(c) || c == '_'){
          state = 1; //Cadenas alfanumericas
        }else if(isdigit(c) || c == '.'){
          state = 2; //Numeros
          if(c=='.'){
            flag_number = 1;
            c = sigCaracter();
            if(!(isdigit(c) || ((c>=65 && c<=70) || (c>=97 && c<=102)) || c=='_')){
              state = 4;
              //Hay que recuperar el . que ya habiamos leido
              c = '.';
              devolverCaracter();
            }else{
              copiar('.',comp);
            }
          }
        }else if(c == '"'){
          state = 3; //Strings
        }else if(!isalpha(c) && !isdigit(c) && c != 0){ //Para reconocer los operadores
          //copiar(c,comp);
          state = 4;
        }
        break;
      case 1://Palabras clave o identificadores
        do{
          copiar(c,comp);
          c = sigCaracter();
        }while(isalpha(c) || isdigit(c) || c == '_');
        devolverCaracter();//el _ es una identificador nulo
        comp->tipo = insertar_buscaEnTabla(comp->lexema,ID);//Si resulta que ya existe en la tabla el lexema nos devolverá el tipo de lexema que es
        state = 5;
        break;
      case 2://numeros
      copiar(c,comp);
      c=sigCaracter();
        while(isdigit(c) || isalpha(c) || c=='_' || c == '.' || c=='+' || c=='-'){
          copiar(c,comp);
          if(c=='x' || c=='X'){//Si es hexadecimal el exponente será con la letra p
            num_type=1;
          }
          if(c=='.'){
            if(flag_number==0){//Solo puede haber un punto
              flag_number = 1;
            }else{
              errorLexico(linea);
            }
          }
          if((c=='e' || c=='E' || c == 'p' || c=='P') && flag_sign==0){
            if(flag_exponent){
              errorLexico(linea);
            }
            if(flag_exponent == 0 && (c == 'e'|| c == 'E') && num_type==0){
              flag_exponent = 1;
              flag_number = 1;
            }
            if(flag_exponent == 0 && (c == 'p' || c == 'P') && num_type==1){
              flag_exponent = 1;
              flag_number = 1;
            }

          }
          if((c == '+' || c == '-')){
            if(flag_sign == 0){
              flag_sign = 1;
            }else{
              errorLexico(linea);
            }
          }
          if(c=='i' || c=='I'){//Nos reconocera los numeros imaginarios
            if(flag_imaginary){
              errorLexico(linea);
            }
            flag_imaginary = 1;

          }

          c=sigCaracter();
        };
        devolverCaracter();
        if(flag_imaginary){
          comp->tipo = IMAGINARIO;
        }else if(flag_number){//Flotantes
          printf("YOOOS\n");
          comp->tipo = FLOTANTE;
        }else{//decimales normales
          comp->tipo = ENTERO;
        }
        state = 5;
        break;
      case 3://Strings
        c = sigCaracter();
        while(!(c == '"' && especial == 0)){
          if((c=='\\' && especial != 0) || c!='\\'){
            copiar(c,comp);
            aceptar_lexema();
          }
          especial = 0;
          if(c == 92){
            especial = 1;
          }
          c = sigCaracter();
        }
        comp->tipo = STRING;
        state = 5;
        break;
      case 4://Automata operadores
        //Los que no tenga una única posibilidad, no inicien comentarios o el tratamiento de la cadena sea mas simple en su correspondiente if
        if(!(c==';' || c==':' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c==',' || c=='=' || c=='/')){
          do{
            copiar(c,comp);
            c = sigCaracter();
          }while(c != ' ' && c != '\t' && c != '\n' && c != ';' && !isdigit(c) && !isalpha(c));
          devolverCaracter();
        }else{
          copiar(c,comp);
        }

        switch(comp->lexema[0]){
          case 43://+
            if(strcmp(comp->lexema,"++")==0){//++
              comp->tipo = AUTOSUMA;
              state = 5;
            }else if(strcmp(comp->lexema,"+=")==0){//+=
              comp->tipo = SUMA_Y_ASIGNACION;
              state = 5;
            }else if(strcmp(comp->lexema,"+")==0){
              comp->tipo = 43;
              state = 5;
            }else{
              errorLexico(linea);
              comp->tipo = ID;
              state=5;
            }
            break;
          case 45://-
            if(strcmp(comp->lexema,"--")==0){//++
              comp->tipo = AUTORESTA;
              state = 5;
            }else if(strcmp(comp->lexema,"-=")==0){//+=
              comp->tipo = RESTA_Y_ASIGNACION;
              state = 5;
            }else if(strcmp(comp->lexema,"-")==0){
              comp->tipo = 45;
              state = 5;
            }else{
              errorLexico(linea);
              comp->tipo = ID;
              state=5;
            }
            break;
          case 42://*
            if(strcmp(comp->lexema,"*=")==0){
              comp->tipo = MULT_Y_ASIGNACION;
              state = 5;
            }else if(strcmp(comp->lexema,"*")==0){
              comp->tipo = 42;
              state = 5;
            }else{
              errorLexico(linea);
              comp->tipo = ID;
              state=5;
            }
            break;
          case 47:// /
            c = sigCaracter();
            if(c=='='){
              copiar(c,comp);
              comp->tipo = DIV_Y_ASIGNACION;
              state = 5;
            }else if (c == ' ' && c == '\t' && c == '\n'){
              comp->tipo = 47;
              state = 5;
            }else if(c=='*'){
              comp->tipo = INICIO_COMENTARIO;
              state = leerComentarios(*comp);
              inicializarCompLexico(comp);
            }else if(c=='/'){
              comp->tipo = COMENTARIO_LINEA;
              state = leerComentarios(*comp);
              inicializarCompLexico(comp);
            }else{//Es una / solo
              devolverCaracter();
              comp->tipo = 47;
              state=5;
            }
            break;
          case 37:// %
            if(strcmp(comp->lexema,"%=")==0){
              comp->tipo = DIV_Y_ASIGNACION;
              state = 5;
            }else if (strcmp(comp->lexema,"%")==0){
              comp->tipo = 37;
              state = 5;
            }else{
              errorLexico(linea);
              comp->tipo = ID;
              state=5;
            }
            break;
          case 61://=
            if(strlen(comp->lexema)==1){
              state = 5;
              comp->tipo = 61;
            }else{//==
              comp->tipo = ES_IGUAL;
              if(strcmp(comp->lexema,"==")!=0){
                errorLexico(linea);
                comp->tipo = ID;
              }
              state = 5;
            }

            break;
          case 38:// &
            if(strlen(comp->lexema)==1){
              comp->tipo = 38;
              state = 5;
            }else if(strcmp(comp->lexema,"&=")==0){
              comp->tipo = AMPERSAND_IGUAL;
              state = 5;
            }else if(strcmp(comp->lexema,"&&")==0){
              comp->tipo = AMPERSAND_AMPERSAND;
              state = 5;
            }else if(strcmp(comp->lexema,"&^")==0){
              comp->tipo = AMPERSAND_ELEVADO;
              state = 5;
            }else if(strcmp(comp->lexema,"&^=")==0){
              comp->tipo = AMPERSAND_ELEVADO_IGUAL;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 124:// |
            if(strlen(comp->lexema)==1){
              comp->tipo = 124;
              state = 5;
            }else if(strcmp(comp->lexema,"|=")==0){
              comp->tipo = OR_IGUAL;
              state = 5;
            }else if(strcmp(comp->lexema,"||")==0){
              comp->tipo = OR;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 94:// ^
            if(strlen(comp->lexema)==1){
              comp->tipo = 94;
              state = 5;
            }else if(strcmp(comp->lexema,"^=")==0){
              comp->tipo = ELEVADO_IGUAL;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 33:// !
            if(strlen(comp->lexema)==1){
              comp->tipo = 33;
              state = 5;
            }else if(strcmp(comp->lexema,"!=")==0){
              comp->tipo = EXCLAMACION_IGUAL;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 60:// <
            if(strlen(comp->lexema)==1){
              comp->tipo = 60;
              state = 5;
            }else if(strcmp(comp->lexema,"<-")==0){
              comp->tipo = OPERADOR_FLECHA;
              state = 5;
            }else if(strcmp(comp->lexema,"<<")==0){
              comp->tipo = MENOR_MENOR;
              state = 5;
            }else if(strcmp(comp->lexema,"<=")==0){
              comp->tipo = MENOR_IGUAL;
              state = 5;
            }else if(strcmp(comp->lexema,"<<=")==0){
              comp->tipo = MENOR_MENOR_IGUAL;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 62:// >
            if(strlen(comp->lexema)==1){
              comp->tipo = 62;
              state = 5;
            }else if(strcmp(comp->lexema,">=")==0){
              comp->tipo = MAYOR_IGUAL;
              state = 5;
            }else if(strcmp(comp->lexema,">>")==0){
              comp->tipo = MAYOR_MAYOR;
              state = 5;
            }else if(strcmp(comp->lexema,">>=")==0){
              comp->tipo = MAYOR_MAYOR_IGUAL;
              state = 5;
            }else{
              comp->tipo = ID;
              errorLexico(linea);
              state = 5;
            }
            break;
          case 46://.
            if(strlen(comp->lexema)==3 && comp->lexema[1]=='.' && comp->lexema[2]=='.'){
              comp->tipo = TRIPLE_PUNTO;
              state = 5;
            }else if(strlen(comp->lexema)==1){
              comp->tipo = 46;
              state=5;
            }else{
              errorLexico(linea);
              comp->tipo = ID;
              state=5;
            }
            break;
          case 44://,
            comp->tipo = 44;
            state = 5;
            break;
          case 91://[
            comp->tipo = 91;
            state = 5;
            break;
          case 93://]
            comp->tipo = 93;
            state = 5;
            break;
          case 40://(
            comp->tipo = 40;
            state = 5;
            break;
          case 41://)
            comp->tipo = 41;
            state = 5;
            break;
          case 123://{
            comp->tipo = 123;
            state = 5;
            break;
          case 125://}
            comp->tipo = 125;
            state = 5;
            break;
          case 58://:
            c=sigCaracter();
            if(c=='='){
              comp->tipo = DOS_PUNTOS_IGUAL;
              copiar(c,comp);
              state = 5;
            }else{
                devolverCaracter();
                comp->tipo = 58;
                state = 5;
            }
            break;
          case 59://;
            comp->tipo = 59;
            state = 5;

            break;

        }
        break;
      case 5://aceptamos cadena
        aceptar_lexema();
        return comp->tipo;
        break;
    }

  }
  return comp->tipo;
}
