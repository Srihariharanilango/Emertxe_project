# Get all .c files and replace .c with .o
var := $(patsubst %.c, %.o, $(wildcard *.c))

# Compile all .o files into an executable
output.exe: $(var)
	gcc -o output.exe $(var)

# Clean generated files
clean:
	rm *.exe *.o