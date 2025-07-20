CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = apc.exe
SRCS = main.c addition.c subtraction.c multiplication.c division.c digit_to_list.c print_list.c
OBJS = $(SRCS:.c=.o)
DEPS = apc.h

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
