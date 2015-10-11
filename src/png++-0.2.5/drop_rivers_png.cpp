#include "image.hpp"
#include "gray_pixel.hpp"
#include <string>
#include <set>
#include <iostream>

struct rgb {int r, b, g;};

int main(int argc, char const *argv[])
{
	png::image<png::gray_pixel_16> pngmap(argv[1]);

	int threshold = atoi(argv[2]);

	size_t width = pngmap.get_width();
	size_t height = pngmap.get_height();

	std::set<png::gray_pixel_16> value_set;

	for (size_t i=0; i<width; i++) {
		for (size_t j=0; j<height; j++) {
			png::gray_pixel_16 cur_val = pngmap.get_pixel(i,j);
			//std::cout
			//	<< i
			//	<< ", "
			//	<< j
			//	<< ": "
			//	<< cur_val
			//	<< "\n";
			value_set.insert(cur_val);
		}
	}

	png::gray_pixel_16 new_val = 0;
	png::gray_pixel_16 min_val = *(value_set.begin());

	std::cout
		<< "Min value: "
		<< min_val
		<< "\nNew value: "
		<< new_val
		<< "\n";

	int count = 0;

	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			png::gray_pixel_16 cur_val = pngmap.get_pixel(i,j);
			if ( cur_val <= (min_val+threshold) ) {
				pngmap.set_pixel(i,j,cur_val - 150);
				count++;
			}
			
		}
	}

	std::cout << "Changed " << count << " pixels\n";

	//std::cout << "Printing ordered set of values\n";
	//std::set<png::gray_pixel_16>::iterator it = value_set.begin();
	//while (it!= value_set.end()) {
	//	std::cout << (int)(*it) << "\t";
	//	it++;
	//}

	std::cout << std::endl;

	pngmap.write("dropped.png");

	return 0;
}