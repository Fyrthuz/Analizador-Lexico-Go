
typedef struct{
  char *lexema;
  short tipo;
}elemento;

//hay un short bloque

typedef void *abb;

void crea(abb *a);
void destruye(abb *a);
unsigned vacio(abb a);
short inserta(abb *a, char *lexema,short tipo);//Inserta y busca un lexema en la tabla de símbolos cuando se lee un lexema durante la ejecución del compildor
void imprimirArbol(abb a);
