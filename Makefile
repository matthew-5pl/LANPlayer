all:
	gcc -o main src/*.c -lSDL2 -lSDL2_ttf -lcurl -lcjson -g

clean:
	rm -rf main.exe