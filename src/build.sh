g++ -c ${1}.cpp `libpng-config --cflags` -I shapelib-1.3.0/ -I png++-0.2.5/
g++ -o ../bin/${1} ${1}.o `libpng-config --ldflags` shapelib-1.3.0/*.o
