all:
	g++ -std=c++20 src/main.cpp -fopenmp -lpthread -o image_json.exe