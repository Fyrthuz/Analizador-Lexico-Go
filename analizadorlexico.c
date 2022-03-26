#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "analizadorlexico.h"
#include "sistemadeentrada.h"
#include "definiciones.h"
#include "tablasimbolos.h"
#include "gestionerrores.h"


long linea = 1;//Variable que nos servirá para llevar la cuenta de la linea en la que estamos situados
long tam = 8;//Tamaño del lexema
long aumento = 8;//Aumento al realizar realloc de la memoria cuando se lee un lexema

void inicializarCompLexico(comp_lexico *comp){
  //Reservamos memoria para el lexema en caso de que no este inicializado
  if(comp->lexema == NULL){
    comp->lexema = (char *)malloc(tam*sizeof(char));
  }
  //Inicializamos todos los valores del lexema a \0 para evitar posibles problemas
  memset(comp->lexema, '\0', tam);
  //Inicializamos el tipo de componente al que se corresponde como 0
  comp->tipo = 0;
}

void copiar(char caracter,comp_lexico *comp){
  //Si se llega al limite del tamaño del lexema se realiza un realloc para seguir copiando el lexema
  if(strlen(comp->lexema)==tam-1){
    //Aumentamos la variable que nos indica el tamaño del lexema
    tam += aumento;
    //Reservamos mas memoria
    comp->lexema = realloc(comp->lexema,sizeof(char)*tam);
    //Inicializamos las nuevas posiciones de memoria a \0 para evitar posibles problemas
    for(int i=tam-aumento;i<tam;i++){
      comp->lexema[i] = '\0';
    }
  }
  //Concatenamos el nuevo caracter al lexema
  strncat(comp->lexema,&caracter,sizeof(char));
}


//Funcion que sirve para leer los comentarios
short leerComentarios(comp_lexico comp){
  //Podemos permitir la carga de cualquier buffer ya que no tienen significado lexico por eso aceptamos el lexema
  aceptar_lexema();
  //Pedimos el siguiente caracter
  char aux = sigCaracter();
  //Saltaremos aqui con la funcion goto en un caso particular
  bucle:
  //Empezamos a iterar hasta acabar de leer el comentario
  while((aux!='\n' && comp.tipo==COMENTARIO_LINEA) || (aux!='*' && comp.tipo==INICIO_COMENTARIO)){
    //Si llegamos a EOF entonces acabamos ejecución y el compilador acabará su ejecucion
    if(aux == EOF){
      return -1;
    }
    //Pedimos el siguiente caracter
    aux = sigCaracter();
    //Si es \n aumentamos la variable linea
    if(aux=='\n'){
      linea ++;
    }
    //Podemos permitir la carga de cualquier buffer ya que no tienen significado lexico por eso aceptamos el lexema
    aceptar_lexema();
  }

  //Si el comentario es del tipo /* hay que comprobar que el siguiente caracter sea una / para
  //confirmar que el comentario ha finalizado, en caso contrario hay que volver al bucle, mediante
  //goto
  if(comp.tipo == INICIO_COMENTARIO){
    aux=sigCaracter();
    if(aux=='\n'){
      linea ++;
    }
    if(aux!='/'){
      goto bucle;
    }

  }
  //Podemos permitir la carga de cualquier buffer ya que no tienen significado lexico por eso aceptamos el lexema
  aceptar_lexema();
  //Retornamos 0 para que el estado del automata vuelva a ser el inivial
  return 0;
}

//Funcion que nos devolverá el siguiente componente léxico del programa
short siguiente_comp_lexico(comp_lexico *comp){
  char c = EOF;//Char que usaremos para cargar los Caracteres
  short state = 0;//Estado del automata
  short especial = 0;//Caracteres especiales strings como pueden ser \b,\n,etc
  short flag_number = 0;//Nos indica si un numero es entero o flotante
  short flag_sign = 0;//Nos indica si un numero tiene signo en el exponente
  short flag_exponent = 0;//Nos indica si un numero tiene exponente
  short flag_imaginary = 0;//Nos indica si un numero es imaginario
  short num_type = 0;//Indica si un numero empieza por 0x,ob,oO,etc
  short show__ = 0;//Indica si aparecio una barrabaja
  short done = 0;//Nos indica si el automata acepta una cadena

  //Inicializar componente lexico para evitar problemas a la hora de copiar los nuevos caracteres
  inicializarCompLexico(comp);
  //Mientras no acabamos el analisis iteramos
  while(!done){
    //Switch con diferentes estados del automata
    switch(state){
      //Primer estado del automata de automatas
      case 0:
        c = sigCaracter();

        if(c == '\n'){//Si es \n acepto y ivoy al estado de aceptacion
          linea++;
          copiar(c,comp);
          comp->tipo='\n';
          aceptar_lexema();
          state=5;
        }else if(c == ' ' || c == '\t'){
          //Si es un espacio o una tabulacion continuo leyendo caracteres ya que carecen de significado lexico en este automata
          state=0;
        }else if(c == EOF){
          //Llegamos al final de fichero acabamos proceso, si llegamos aquí hemos encontrado el EOF o un caracter nulo
          state = 5;
          comp->tipo = EOF;
        }else if(c==';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c==','){
          //Si el caracter es uno de estos operadores podemos aceptar el lexema ya que no hay lexemas de mayor longitud
          //que 1 que empiecen por estos caracteres
          state = 5;
          comp->tipo = c;
          copiar(c,comp);
        }else if(isalpha(c) || c == '_'){
          //Si la cadena empieza por una letra o una _ nos dirigimos al automata de cadenas alfanumericas
          state = 1;
        }else if(isdigit(c) || c == '.'){
          //Si la cadena empieza por un numero o un . nos dirigimos al automata de numeros
          state = 2;
          //Si empieza por punto tenemos que comprobar que el siguiente caracter del lexema nos permite identificar un numero
          if(c=='.'){
            c = sigCaracter();
            //Si no es un numero decimal o hexadecimal o el caracter es una _ entonces tendríamos que identificar un operador
            //E iríamos al automata de operadores
            if(!(isdigit(c) || ((c>=65 && c<=70) || (c>=97 && c<=102)) || c=='_')){
              state = 4;
              //Hay que recuperar el . que ya habiamos leido
              c = '.';
              //Devolvemos el caracter al anterior
              devolverCaracter();
            }else{
              //En cualquier otro caso tenemos un numero y vamos al automata de numeros copiando el . al lexema
              flag_number = 1;
              copiar('.',comp);
            }
          }
        }else if(c == '"'){//Si leemos unas comillas vamos al automata de strings
          state = 3; //Strings
        }else if(!isalpha(c) && !isdigit(c) && c != '\0'){ //Si el caracter no es un numero ni una letra tendríamos que ir al automata de operadores
          state = 4;
        }
        break;
      case 1://Automata de cadenas alfanuméricas
        do{
          //Copiamos el caracter al lexema
          copiar(c,comp);
          //Pedimos el siguiente caracter
          c = sigCaracter();
          //Iteramos mientras encontremos un numero una letra o una _
        }while(isalpha(c) || isdigit(c) || c == '_');
        //Cuando salimos del bucle tenemos que devolver el caracter para no perderlo en la siguiente palabra
        devolverCaracter();
        //Si resulta que ya existe en la tabla el lexema nos devolverá el tipo de lexema que es y se guradará
        //en la variable tipo de la struct
        comp->tipo = insertar_buscaEnTabla(comp->lexema,ID);
        //Vamos al estado de aceptación
        state = 5;
        break;
      case 2://Autómata de números
        //Copiamos el caracter al lexema
        copiar(c,comp);
        //Pedimos el siguiente caracter
        c=sigCaracter();
        //Mientras sea una letra un numero un . una _ o un simbolo leemos el numero
        while(isdigit(c) || isalpha(c) || c=='_' || c == '.' || c=='+' || c=='-'){
          //Copiamos el numero
          copiar(c,comp);
          if(c=='x' || c=='X'){//Si es hexadecimal el exponente será con la letra p
            num_type=1;
          }
          if(c=='_'){
            //Solo puede aparecer un _ seguido, por eso tenemos en cuenta esto
            switch (show__) {
              case 0:
                show__ = 1;
                break;
              case 1:
                errorLexico(linea);
            }
          }else{//En cualquier otro caso puede aparecer de nuevo la _
            show__ = 0;
          }
          if(c=='.'){
            if(flag_number==0){//Solo puede haber un punto en otro caso habria un error lexico
              flag_number = 1;
            }else{
              errorLexico(linea);
            }
          }
          //El exponente tiene que estar antes que el signo
          if((c=='e' || c=='E' || c == 'p' || c=='P') && flag_sign==0){
            if(flag_exponent){//Si ya apareció un exponente hay un error lexico
              errorLexico(linea);
            }
            //Si no es un numero hexadecimal entonces el exponente es con la letra e
            if(flag_exponent == 0 && (c == 'e'|| c == 'E') && num_type==0){
              flag_exponent = 1;
              flag_number = 1;
            }
            //Si es un numero hexadecimal entonces el exponente es con la letra p
            if(flag_exponent == 0 && (c == 'p' || c == 'P') && num_type==1){
              flag_exponent = 1;
              flag_number = 1;
            }
          }

          //Si aparecio el exponente y aparece un numero cambiamos el flag de exponente a 2
          //Esto lo hacemos por si aparece un signo, ya que en este caso el signo o pertencería
          //al lexema de este número
          if(flag_exponent==1 && isdigit(c)){
            flag_exponent=2;
          }

          //Si aparece un signo este tiene que ir despues del exponente
          if((c == '+' || c == '-')){
            if(flag_sign == 0 && flag_exponent==1){
              flag_sign = 1;
            }else if(flag_sign==1 || flag_exponent==2){
              //Si ya aparecio el signo o no puede haber ya un signo porque despues del exponente no hubo ningún significado
              //Entonces no podemos tomar este signo como parte del lexema por lo que hay que borrar el signo del lexema
              comp->lexema[strlen(comp->lexema)-1]='\0';
              break;
            }
          }
          if(c=='i' || c=='I'){//Nos reconocera los numeros imaginarios, si aparece una i entonces el numero será imaginario
            if(flag_imaginary){
              errorLexico(linea);
            }
            flag_imaginary = 1;

          }
          //Pedimos el siguiente caracter
          c=sigCaracter();
        };
        //Devolvermos el caracter ya que este no pertenece al lexema y si no lo perderíamos para el siguiente
        devolverCaracter();
        //Comprobamos que tipo de numero tenemos en función de los flags que hemos ido definiendo
        if(flag_imaginary){
          comp->tipo = IMAGINARIO;
        }else if(flag_number){//Flotantes
          comp->tipo = FLOTANTE;
        }else{//decimales normales
          comp->tipo = ENTERO;
        }
        //Vamos al estado de aceptacion
        state = 5;
        break;
      case 3://Automata que reconoce Strings
        c = sigCaracter();
        while(!(c == '"' && especial == 0) && c!=EOF){
          copiar(c,comp);
          aceptar_lexema();

          especial = 0;
          if(c == '\\'){//Nos sirve para indicar caracteres especiales
            especial = 1;
          }
          c = sigCaracter();
        }
        if(c==EOF){//Si hemos encontrado el EOF el string no está correctamente formado
          errorLexico(linea);
          //Acabamos la ejecución del programa
          return EOF;
        }
        comp->tipo = STRING;
        //Vamos al estado de aceptación
        state = 5;
        break;
      case 4://Automata operadores
        //Los que no inicien comentarios tendrán que irse copiando,
        //Los operadores que solo tienen una posibilidad ya fueron tratados anteriorente en el estado 0
        if(c!='/' ){
          do{
            copiar(c,comp);
            c = sigCaracter();
          }while(c != ' ' && c != '\t' && c != '\n' && c != ';' && !isdigit(c) && !isalpha(c));
          devolverCaracter();
        }else{
          copiar(c,comp);
        }
        //Hacemos un switch con las diferentes posibilidades, el primer caracter nos servirá para filtrar el tipo de operador que es
        //Dentro de cada case identificaremos el operador al que se correponde el lexema, en caso de que ocurra un errorLexico entonces
        //identificaremos el lexema como un identificador pero únicamente por comodidad, este lexema sería incorrecto al fin y al cabo
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
            //En el caso del caracter / tenemos que tener en cuenta que puede ser el iniciao de un comentario o una division,
            //por lo que tenemos que pedir el siguiente caracter y en función de cúal sea el siguiente identificar el lexema
            c = sigCaracter();
            if(c=='='){
              copiar(c,comp);
              comp->tipo = DIV_Y_ASIGNACION;
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
          case 58://:
          //El caso de los puntos es parecido al del caracter /
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
          default://Si no se corresponde con ninguna de las anteriores hay un error lexico
            errorLexico(linea);
            comp->tipo = ID;
            state = 5;
        }
        break;
      case 5://aceptamos cadena
        aceptar_lexema();
        //salimos del bucle
        done = 1;
    }

  }
  return comp->tipo;
}
