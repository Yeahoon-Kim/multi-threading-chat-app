# # gcc를 이용하여
# OBJECT 대상은 ../chatProgram.o chatClient.o main.o
# 최종 파일은 ../chatClient

CC = gcc
CFLAGS = -W -Wall
OBJECTS = ../chatProgram.o chatServer.o main.o
TARGET = ../bin/chatServer
LIBS = -lpthread

# TARGET 파일이 여러 개일 때 사용
all: $(TARGET)

main.o: ../chatProgram.h ../chatProgram.c chatServer.h chatServer.c main.c
chatServer.o: ../chatProgram.h ../chatProgram.c chatServer.h

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o ../$(@) $(OBJECTS) $(LIBS)

# 더미 타겟
clean: 
	rm -f *.o
	rm -f ../$(TARGET)

.PHONY: all clean