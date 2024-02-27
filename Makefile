

build:
	gcc -g -std=c99 task1.c task2.c task3.c main.c header.h -o quadtree -lm -Wall

clean:
	rm tema2.out quadtree tema2.in

run:
	make build
	./quadtree
