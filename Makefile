OBJS = main.o util.o vm.o memory.o stack.o
CFLAGS = -Wall -DNDEBUG

all: $(OBJS)
	cc $(CFLAGS) -o vm $(OBJS)

test:
	./validate.sh codes.txt

clean:
	rm *.o vm

util.o: util.h
vm.o: vm.h
memory.o: memory.h
stack.o: stack.h
