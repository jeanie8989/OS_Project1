all: 
	gcc process.c scheduler.c queue.c main.c -o main

debug:
	gcc process.c scheduler.c queue.c main.c -DDEBUG -o main

clean:
	@rm process scheduler main

run:
	sudo ./main