PROJECT_NAME=CsvTest

OBJ_DIR=../../../../_output/GCC/$(PROJECT_NAME)/
OUT_DIR=../../../../bin/GCC/

SRC_DIR=../../Tests/

SRC_FILES=main.cpp TestA.cpp TestB.cpp TestC.cpp z.cpp
OBJ_FILES=$(SRC_FILES:.cpp=.o)

SRCS=$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS=$(addprefix $(OBJ_DIR), $(OBJ_FILES))

CFLAGS=-c
LDFLAGS=

CsvTest: $(OBJS)
	$(CC) $(LFLAGS) $OBJS -o $(OUT_DIR)CsvTest

#.cpp.o:
#	$(CC) $(CFLAGS) $< -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) $< -o $@

#%.o : %.cpp
#	$(CC) $(CFLAGS) $< -o $@

OBJ_DIR:
	mkdir -p $(OBJ_DIR)

OUT_DIR:
	mkdir -p $(OUT_DIR)
