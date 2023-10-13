build:
	gcc -Wall ./src/*.c -o game -lSDL2
run:
	./game
clean:
	rm game