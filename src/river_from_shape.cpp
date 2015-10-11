#include "shapefil.h"
#include "image.hpp"
#include "gray_pixel.hpp"
#include <string>
#include <set>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <utility>
#include <unordered_set>
#include <map>

# define M_PI           3.14159265358979323846  /* pi */

void dig_riverbed(int i, int j, png::gray_pixel_16 depth, png::image<png::gray_pixel_16>& topomap);

int main(int argc, char const *argv[]) {
	double xbnd [2] = {-85.699387,-85.480558};
	double ybnd [2] = {42.199006,42.360703};
	//double delta_factor = atoi(argv[4]);
	double delta_factor = 0.04;
	double x_offset = 0;
	double y_offset = 0;
	
	png::image<png::gray_pixel_16> topomap(argv[1]);
	size_t width = topomap.get_width();
	size_t height = topomap.get_height();
	std::set<png::gray_pixel_16> value_set;
	for (size_t i=0; i<width; i++) {
		for (size_t j=0; j<height; j++) {
			png::gray_pixel_16 cur_val = topomap.get_pixel(i,j);
			value_set.insert(cur_val);
		}
	}
	png::gray_pixel_16 min_val = *(value_set.begin());
	png::gray_pixel_16 max_val = *(value_set.rbegin());
	png::gray_pixel_16 delta_val = (max_val - min_val)*delta_factor;
	std::cout
		<< "TOPOGRAPH STATS"
		<< "\nmax value: "
		<< max_val 
		<< "\tmin value: "
		<< min_val
		<< "\tgrade delta: "
		<< delta_val
		<< "\n";

	SHPObject * rivershape_ptr = SHPReadObject(SHPOpen(argv[2],"rb"),0);
	std::cout
		<< "SHAPE OBJ STATS"
		<< "\ntype: "
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
		<< "\n";		

	int points_oob = 0;
	int points_ib = 0;
	int interpolations = 0;
	int vertices = rivershape_ptr->nVertices;
	for (int vertex=0; vertex<vertices; vertex++) {
		double x = (rivershape_ptr->padfX[vertex]);
		x += x_offset;
		double y = (rivershape_ptr->padfY[vertex]);
		y += y_offset;
		if (x < xbnd[0] || x > xbnd[1] || y < ybnd[0] || y > ybnd[1]) {
			points_oob++;
			continue;
		}
		points_ib++;
		double relx = (x - xbnd[0])/(xbnd[1]-xbnd[0]);
		double rely = (y - ybnd[0])/(ybnd[1]-ybnd[0]);
		static double old_relx = relx;
		static double old_rely = rely;
		double unit_len = 1.0/width;
		double max_len = unit_len*10;
		double delx = relx - old_relx;
		double dely = rely - old_rely;
		double dist = sqrt(delx*delx+dely*dely);
		if ( dist > unit_len && dist < max_len) {
			interpolations++;
			int num_pts = dist/unit_len;
			for(int k=0; k<num_pts; k++) {
				double delx_ = delx/num_pts*k;
				double dely_ = dely/num_pts*k;
				double relx_ = old_relx + delx_;
				double rely_ = old_rely + dely_;
				int i = (int)(relx_*width);
				int j = (int)(rely_*height);
				dig_riverbed(i,height-j,delta_val,topomap);
			}
		}
		int i = (int)(relx*width);
		int j = (int)(rely*height);
		dig_riverbed(i,height-j,delta_val,topomap);
		old_relx = relx;
		old_rely = rely;
	}
	std::cout
		<< "PROGRAM STATS"
		<< "\n points ib: "
		<< points_ib
		<< "\t points oob: "
		<< points_oob
		<< "\t interpolations: "
		<< interpolations
		<< "\n";

	topomap.write("applied.png");
	return 0;
}



void dig_riverbed(int i, int j, png::gray_pixel_16 depth, png::image<png::gray_pixel_16>& topomap) {
	png::gray_pixel_16 cur_val,new_val,brush_depth;
	int brush_radius=2;
	int width = topomap.get_width();
	int height = topomap.get_height();
	for (int i_=i-brush_radius; i_<=i+brush_radius; i_++) {
		for(int j_=j-brush_radius; j_<=j+brush_radius; j_++) {
			if (i_<0||i_>width||j_<0||j_>=height) continue;
			double i_dist=i_-i;
			double j_dist=j_-j;
			double dist=sqrt(i_dist*i_dist+j_dist*j_dist);
			if(dist>brush_radius) continue;
			//double brush_depth_dbl = depth - depth*sin(dist/brush_radius*M_PI/2);
			double brush_depth_dbl = depth*cos(dist/brush_radius*M_PI/2);
			if(brush_depth_dbl < 0) brush_depth_dbl = 0;
			brush_depth = brush_depth_dbl;
			cur_val = topomap.get_pixel(i_,j_); 
			if(brush_depth >= cur_val) new_val = 0;
			else
				new_val = cur_val - brush_depth;
			if (new_val < cur_val)
				topomap.set_pixel(i_,j_,new_val);
		}
	}
}
