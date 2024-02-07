TARGET = enctypt.exe
CC = gcc


.PHONY: clean run build all

PREFF_SRC = ./src/
PREFF_INC = ./inc

MAIN =$(wildcard *.c)
INC = $(wildcard $(PREFF_INC)*.h)
SRC = $(wildcard $(PREFF_SRC)*.c)
OBJ = $(patsubst $(PREFF_SRC)%.c, %.o, $(SRC)) $(patsubst %.c, %.o, $(MAIN))

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)


%.o : $(PREFF_SRC)%.c 
	$(CC) -I $(PREFF_INC) -c $< -o $@

main.o : main.c
	$(CC) -I $(PREFF_INC) -c main.c -o main.o


all : clean build

build: $(TARGET) clean

run:
	$(TARGET)		

clean:
	rm -rf *.o
