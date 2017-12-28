all:
	g++ ./module/MJtile.cpp ./module/MJhand.cpp ./module/MJplayer.cpp ./main.cpp -o main
clean:
	rm main
