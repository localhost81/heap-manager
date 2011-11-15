
CC =  gcc
CFLAGS = -g 

OUTPUT_FN_ALL = heapman
OBJ_FILES_ALL = memory_model.o main.o list.o

OUTPUT_FN_LIST_TESTS = list_tests
OBJ_FILES_LIST_TESTS = memory_model.o list_tests.o list.o

all: ${OBJ_FILES_ALL}
	@echo "Make all"
	@echo "Compiling begins..."
	${CC} ${CFLAGS} -o ${OUTPUT_FN_ALL} ${OBJ_FILES_ALL} 
	@echo "Done!"

list_tests: ${OBJ_FILES_LIST_TESTS}
	@echo "Make list tests"
	@echo "Compiling begins..."
	${CC} ${CFLAGS} -o ${OUTPUT_FN_LIST_TESTS} ${OBJ_FILES_LIST_TESTS} 
	@echo "Done!"

memory_model.o: memory_model.c memory_model.h
	${CC} ${CFLAGS} -c memory_model.c

main.o: main.c
	${CC} ${CFLAGS} -c main.c

list_tests.o: list_tests.c
	${CC} ${CFLAGS} -c list_tests.c

list.o: list.h list.c
	${CC} ${CFLAGS} -c list.c

clean:
	@echo "Cleaning..."
	rm -rf *~ *.o *.p *.a ${OUTPUT_FN_LIST_TESTS} ${OBJ_FILES_LIST_TESTS}