#ifndef __PUZNIAKOWSKI_PNG___
#define __PUZNIAKOWSKI_PNG___
/**
Tadeusz Puźnikowski 2017
*/
#include <vector>
#include <string>


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
    
    /**
     * returns the whole pixel value.
     */
    unsigned int get(int x, int y);

    /**
     * set pixel value
     */
    void set(int x, int y, unsigned int c);

    /**
     * get the pixel value of given channel.
     */
    unsigned char &getI(int x, int y, int p);
};

/**
 * Read png file contents from vector (you can read the vector from any source you like)
 * @arg file_contents_ the bytes read that represents PNG file
 * */
pngimage_t read_png_file(const std::vector<unsigned char> &file_contents_);
pngimage_t read_png_file(const std::vector<char> &file_contents_);
pngimage_t read_png_file (const std::string &file_name_);

/**
 * Writes data to PNG buffer
 * @return the raw PNG data
 * */
std::vector<unsigned char> write_png_file( const pngimage_t &image_);
void write_png_file( const std::string &file_name_, const pngimage_t &image_ );
unsigned int getColor(int8_t r, int8_t g, int8_t b, int8_t a);

}
}
#endif

