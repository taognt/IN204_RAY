all:
	g++ -std=c++20 src/main_w_json_c.cpp -fopenmp -lpthread -o image_json.exe