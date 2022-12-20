#include <iostream>
#include <pnglib.hpp>
using namespace std;
using namespace puzniakowski::png;

int main(){
    pngimage_t im;
    im = read_png_file("data/a.png");
    for (int i=0;i<im.width;i++){
        for (int j=0;j<im.height;j++){
            //Convert image to greyscale
            unsigned int color = im.get(i,j);
            int8_t r,g,b,a,middle;
            getRGBAFromColor(color,&r,&g,&b,&a);
            middle = (r+g+b)/3;
            color = getColorFromRGBA(middle,middle,middle,a);
            im.set(i,j,color);
        }
    }
    write_png_file("b.png",im);
    return 0;
}
