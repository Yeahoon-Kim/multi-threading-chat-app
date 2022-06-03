# gcc를 이용하여
# OBJECT 대상은 ../chatProgram.o chatClient.o main.o
# 최종 파일은 ../chatClient

CC = gcc
CFLAGS = -W -Wall
OBJECTS = ../chatProgram.o chatClient.o main.o
TARGET = chatClient

# TARGET 파일이 여러 개일 때 사용
all: $(TARGET)

main.o: ../chatProgram.h ../chatProgram.c ./chatClient.h ./chatClient.c ./main.c
chatClient.o: ../chatProgram.h ../chatProgram.c ./chatClient.h

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o ../$(@) $(OBJECTS)

# 더미 타겟
clean: 
	rm -f *.o
	rm -f ../$(TARGET)

.PHONY: all clean