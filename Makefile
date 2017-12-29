all:
	g++ -std=c++11 ./module/MJtile.cpp ./module/MJhand.cpp ./module/MJplayer.cpp ./main.cpp -o main
clean:
	rm main
