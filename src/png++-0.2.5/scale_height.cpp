#include "image.hpp"
#include "gray_pixel.hpp"
#include <string>
#include <set>
#include <iostream>

struct rgb {int r, b, g;};

int main(int argc, char const *argv[])
{
	png::image<png::gray_pixel_16> pngmap(argv[1]);

	int scale = atoi(argv[2]);

	size_t width = pngmap.get_width();
	size_t height = pngmap.get_height();

	std::set<png::gray_pixel_16> value_set;

	for (size_t i=0; i<width; i++) {
		for (size_t j=0; j<height; j++) {
			png::gray_pixel_16 cur_val = pngmap.get_pixel(i,j);
			pngmap.set_pixel(i,j,cur_val * scale);
		}
	}

	pngmap.write("scaled.png");

	return 0;
}