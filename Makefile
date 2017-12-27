all:
	g++ ./module/MJtile.cpp ./module/MJhand.cpp ./main.cpp -o main
clean:
	rm main
