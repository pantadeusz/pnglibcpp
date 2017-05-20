#ifndef __PUZNIAKOWSKI_PNG___
#define __PUZNIAKOWSKI_PNG___
/**
Tadeusz Puźnikowski 2017
*/
#include <vector>

namespace puzniakowski {
namespace png {

// kontener na dane PNG
class pngimage_t {
public:
    static const int R = 0;
    static const int G = 1;
    static const int B = 2;
    static const int A = 3;

    int width; ///< szerokosc
    int height; ///< wysokosc
    int bpp; ///< bajtow na piksel
    std::vector < unsigned char > data; ///< piksele
    
    unsigned int get(int x, int y);
    unsigned char &getI(int x, int y, int p);
};

/**
 * Read png file contents from vector (you can read the vector from any source you like)
 * @arg file_contents_ the bytes read that represents PNG file
 * */
pngimage_t read_png_file(const std::vector<unsigned char> &file_contents_);

/**
 * Writes data to PNG buffer
 * @return the raw PNG data
 * */
std::vector<unsigned char> write_png_file( const pngimage_t &image_);

}
}
#endif

