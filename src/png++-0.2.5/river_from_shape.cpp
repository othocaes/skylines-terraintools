#include "shapefil.h"
#include "image.hpp"
#include "gray_pixel.hpp"
#include <string>
#include <set>
#include <iostream>

struct rgb {int r, b, g;};

int main(int argc, char const *argv[])
{
	png::image<png::gray_pixel_16> pngmap(argv[1]);
	SHPObject * rivershape_ptr = SHPReadObject(SHPOpen(argv[2],"rb"),0);

	size_t width = pngmap.get_width();
	size_t height = pngmap.get_height();

	std::set<png::gray_pixel_16> value_set;

	for (size_t i=0; i<width; i++) {
		for (size_t j=0; j<height; j++) {
			png::gray_pixel_16 cur_val = pngmap.get_pixel(i,j);
			pngmap.set_pixel(i,j,cur_val);
		}
	}

	pngmap.write("applied.png");

	std::cout
		<< "Shape Obj Info\ntype: "
		<< rivershape_ptr->nSHPType
		<< "\tnParts: "
		<< rivershape_ptr->nParts
		<< "\tnVertices: "
		<< rivershape_ptr->nVertices
		<< "\nx bnd: "
		<< rivershape_ptr->dfXMin
		<< "-"
		<< rivershape_ptr->dfXMax
		<< "\ty bnd: "
		<< rivershape_ptr->dfYMin
		<< "-"
		<< rivershape_ptr->dfYMax
		<< "\tz bnd: "
		<< rivershape_ptr->dfZMin
		<< "-"
		<< rivershape_ptr->dfZMax
		<< "\tm bnd: "
		<< rivershape_ptr->dfMMin
		<< "-"
		<< rivershape_ptr->dfMMax
		;		


	return 0;
}