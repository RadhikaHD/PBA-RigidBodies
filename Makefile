CC      = g++
C	= cpp
H	= h

CFLAGS = -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm
  endif
endif

HFILES = Vector.${H} Utility.${H} Camera.${H} Matrix.${H} Quaternion.${H} rigid.${H}
OFILES = Vector.o Utility.o Camera.o Matrix.o Quaternion.o rigid.o
PROJECT = RigidBodies

${PROJECT}:	${PROJECT}.o ${OFILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS}

${PROJECT}.o: ${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} ${CFLAGS} -c Utility.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H}
	${CC} ${CFLAGS} -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H}
	${CC} ${CFLAGS} -c Matrix.${C}

Quaternion.o: Quaternion.${C} Quaternion.${H} Matrix.${H}
	${CC} ${CFLAGS} -c Quaternion.${C}

rigid.o: rigid.${C} rigid.${H} Quaternion.${H} Camera.${H}
	${CC} ${CFLAGS} -c rigid.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm core.*; rm *.o; rm *~; rm ${PROJECT}
