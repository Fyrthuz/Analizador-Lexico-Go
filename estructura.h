//Puntero a una direccion de memoria es un tipo opaco desde fuera no se sabe la estructura del abb
typedef void* abb;

//Creamos el arbol binarios de busqueda
void crea(abb *a);
//Destruimos yel arbol binario y liberamos la memoria
void destruye(abb *a);
//Inserta y busca (sirve para ambas funciones)un lexema en la tabla de símbolos cuando se lee un lexema durante la ejecución del compildor
short inserta_busca(abb *a, char *lexema,short tipo);
//Imprimimos el arbol binario
void imprimirArbol(abb a);
