
# C compiler
CC = gcc
# Compiling flags
ifneq ($(NDEBUG), 1)
	CFLAGS = -g -O0 -Wall -std=c99
else 
	CFLAGS = -O3 -std=c99 -DNDEBUG=1
endif
# Linking flags
LDFLAGS = 

# Directory that contains object files
OBJ_DIR = obj
# Directory that contains source files
SRC_DIR = src
# Directory that contains binary files
BIN_DIR = bin

# Result program filename
BIN_FN = heap-manager

# Project's obejct files
OBJ_FILES = $(OBJ_DIR)/memory_model.o $(OBJ_DIR)/main.o $(OBJ_DIR)/list.o 
# Project's source files 
# (generated automatically using the existing object files list)
SRC_FILES = ${OBJ_FILES:.o=.c}

all: mkdirs $(OBJ_FILES)
	@echo "Building the project..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$(BIN_FN) $(OBJ_FILES)
	@echo "Done!"

$(OBJ_DIR)/%.o:
	$(CC) $(CFLAGS) -c $(SRC_DIR)/$*.c -o $@

mkdirs:
	@echo "Making directories..."
	mkdir -p $(OBJ_DIR) $(BIN_DIR)
	@echo "Done!"

clean:
	@echo "Cleaning..."
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Done!"