TARGET=calc
ifeq ($(OS), Windows_NT)
TARGET:=$(TARGET).exe
endif

all : $(TARGET)

$(TARGET) : main.c calc.c helpers.c calc.h helpers.h struct.h
	gcc -std=c99 -o $(TARGET) main.c calc.c helpers.c -lm 

clean :
	-rm $(TARGET)
