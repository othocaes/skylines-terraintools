#include "bitmap_image.hpp"
#include <string>
#include <set>
#include <iostream>

struct rgb {int r, b, g;};

int main(int argc, char const *argv[])
{
	std::string bitmap_filename = argv[1];
	bitmap_image bitmap = bitmap_image(bitmap_filename);

	int width = bitmap.width();
	int height = bitmap.height();

	//rgb values [bitmap.pixel_count()];

	//std::set< color_kernel;

	std::set<unsigned char> value_set;

	unsigned char r,g,b;

	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			bitmap.get_pixel(i,j,r,g,b);
			//std::cout
			//	<< i
			//	<< ", "
			//	<< j
			//	<< ": "
			//	<< r
			//	<< "\t"
			//	<< g
			//	<< "\t"
			//	<< b
			//	<< "\n";
			value_set.insert(r);
		}
	}

	unsigned char new_val = 0;
	int new_int = (int)(new_val);
	unsigned char min_val = *(value_set.begin());
	int min_int = (int)(min_val);

	std::cout
		<< "Min value: "
		<< min_val
		<< " ("
		<< min_int
		<< ")\n"
		<< "New value: "
		<< new_val
		<< " ("
		<< new_int
		<< ")\n"		
		<< "\n";

	int count = 0;

	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			bitmap.get_pixel(i,j,r,g,b);
			unsigned char cur_val = r;
			int cur_int = (int)cur_val;
			//std::cout << cur_int << "<? " << min_val << "\n";
			if ( cur_val <= (min_val+1) ) {
				bitmap.set_pixel(i,j,new_val,new_val,new_val);
				count++;
			}
			
		}
	}

	std::cout << "Changed " << count << " pixels\n";

	std::cout << "Printing ordered set of values\n";
	std::set<unsigned char>::iterator it = value_set.begin();
	while (it!= value_set.end()) {
		std::cout << (int)(*it) << "\t";
		it++;
	}

	std::cout << std::endl;

	std::string output_file = "new_bitmap.bmp";
	bitmap.save_image(output_file);


	//delete values [];
	return 0;
}