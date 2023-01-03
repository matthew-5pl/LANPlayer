all:
	gcc -o main src/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lcurl -lcjson -g

clean:
	rm -rf main.exe