CC= gcc -Wall
HEADER_FILES_DIR = .
INCLUDES = -I $(HEADER_FILES_DIR)
MAIN= compilador
SRCS = main.c analizadorlexico.c analizadorsintactico.c estructura.c sistemadeentrada.c tablasimbolos.c gestionerrores.c
DEPS = $(HEADER_FILES_DIR)/analizadorlexico.h analizadorsintactico.h estructura.h definiciones.h sistemadeentrada.h tablasimbolos.h gestionerrores.h
OBJS = $(SRCS:.c=.o)
$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) -g
	rm -f *.o *~
%.o: %.c $(DEPS)
	$(CC) -c $< $(INCLUDES)
cleanall: clean
	rm -f $(MAIN)
clean:
	rm -f *.o *~
