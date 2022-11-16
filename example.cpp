#include <iostream>
#include <pnglib.hpp>
using namespace std;
using namespace puzniakowski::png;

int main(){
    pngimage_t im;
    im = read_png_file("data/a.png");
    for (int i=0;i<im.width;i++){
        for (int j=0;j<im.height;j++){
            //set color to R = 0, G = 255, B = 255, A = 0
            unsigned int color = getColor(0,255,255,0);
            im.set(i,j,color);
        }
    }
    write_png_file("b.png",im);
    return 0;
}
