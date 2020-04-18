############
#          #
# makefile #
#          #
############

# Executable name
EXEC_NAME=census

# Compiler to use
CC=g++

# Default Options to use
# CFLAGS= -Wall -Wno-write-strings -O0 -g
CFLAGS= -Wno-write-strings -O3 -g -Wall
#CFLAGSFINAL= -O3
CLIBS=

#NAUTYSRC= nauty.c nautil.c naugraph.c

# Source files
SRC =                   		\
	CmdLine.cpp			\
	MultiplexGraphMatrix.cpp	\
	MultiplexGraphUtils.cpp		\
	Isomorphism.cpp			\
	ReducedSubgraph.cpp		\
	Count.cpp			\
	Timer.cpp			\
	GraphTree.cpp			\
	Results.cpp			\
	Error.cpp			\
	nauty27rc2/nauty.c		\
	nauty27rc2/nautil.c		\
	nauty27rc2/naugraph.c		\
	main.cpp

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} ${CLIBS} -o ${EXEC_NAME} ${OBJ}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf
