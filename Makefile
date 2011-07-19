# Find the flex/bison implementations for the system

LEX = ${shell which flex}
YACC = ${shell which bison}


#
# Variables to control the various tools
# CFLAGS go to cc
# LDFLAGS and LDLIBS go to the linker (passed through cc)
# YFLAGS go to yacc
#

INCLUDEPATH=\
    -Iinclude\
    -I/usr/local/include\

CFLAGS+=  -D_POSIX_C_SOURCE -std=c99 ${INCLUDEPATH}
LDFLAGS+= -L/usr/local/lib
LDLIBS+=  -lgsl -lgslcblas
YFLAGS+=  --defines=work/y.tab.h -o y.tab.c

# Targets:

# Do everything by default, if it isn't done already
all: bin/escher

bin/escher: obj/escher $(filter-out $(wildcard bin), bin)
	mv obj/escher bin/escher

obj/escher: work/parser.o work/lexer.o obj/escher.o obj/statistics.o obj/simulation.o

obj/%.o:  src/%.o include/%.h $(filter-out $(wildcard obj), obj)
	mv src/$*.o obj/

work/%.c: src/%.c ${filter-out ${wildcar work}, work}
	mv src/$*.c work/

clean: 
	if [ -e work ]; then rm -r work; fi
	if [ -e obj ]; then rm -r obj; fi
purge: clean
	if [ -e bin ]; then rm -r bin; fi

obj: 
	mkdir obj
bin:   
	mkdir bin
work:
	mkdir work
