################## GNU
        CC = g++
    CFLAGS = -O
DEBUGFLAGS = -g -Wall ${CFLAGS}

################## SUN Workshop
#        CC = CC
#    CFLAGS = -O
#DEBUGFLAGS = -g ${CFLAGS}

################## IBM XLC cc
#        CC = cc
#CFLAGS = -g -v
#DEBUGFLAGS = ${CFLAGS}

  LIBFLAGS = -lm
   STDOBJS = pi.o
   STDHDRS = 

all:
	make pi

pi: ${STDOBJS}
	${CC} ${DEBUGFLAGS} -o pi ${STDOBJS} ${LIBFLAGS}

pi.o: pi.c ${STDHDRS}

clean:
	rm -f *.o pi core *~
