# Updated by Upro March 2020. Had to add functionality to
# make on main/test port separate from each other.

CC = gcc
RM = rm
EXE = game

C_FLAGS = -O3 -ggdb3 -Wall
L_FLAGS = $(PROF) -lcrypt 

# Source Files
SRC_FILES := $(wildcard *.c)

# Object Files
OBJ_DIR = obj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

rom: $(OBJ_FILES)
	$(RM) -f $(EXE)
	$(CC) -o $(EXE) $(OBJ_FILES) $(L_FLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(C_FLAGS) -c -o $@ $<

clean:
	$(RM) -f $(OBJ_FILES) $(EXE) *~ *.bak *.orig *.rej

distro:
	make clean
	cd ../..
	$(RM) -f log/*.log
	tar zvcf strategy-`date -I`.tar.gz ../src	

main:
	cd ../../upro/strategy/src
	make clean
	make	
