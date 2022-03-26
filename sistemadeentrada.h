//Funcion que inicializa el sistema de entrada
void iniciaSistema(char *fichero);
//Funcion que nos libera los recursos utilizados por el sistema de entradala utilizamos cuando acaba el analisis lexico
void salirSistema();
//Funcion que nos devuelve el siguiente caracter del codigo
char sigCaracter();
//Funcion que nos retrasa una posicion el puntero final del buffer
void devolverCaracter();
//Esta funcion nos indica que el lexema actual ha sido aceptado por que igualamos el puntero inicial con el final
void aceptar_lexema();
