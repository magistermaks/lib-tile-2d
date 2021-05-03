
#include <string>

class Layer {
private:
	unsigned char* data;
	int data_width;
	int data_height;
	int tiles_count = 0;

public:
	Layer(int width, int height) {
		data_width = width;
		data_height = height;
		data = new unsigned char[width * height];
		memset(data, 0, width * height * sizeof(unsigned char));
	}

	~Layer() {
		delete[] data;
	}

	unsigned char* get_data() {
		return data;
	}

	int get_data_width() {
		return data_width;
	}

	int get_data_height() {
		return data_height;
	}

	int get_tiles_count() {
		return data_width;
	}

	void addTile(int x, int y) {
		tiles_count++;
		data[x * data_width + y] = 1;
	}
};