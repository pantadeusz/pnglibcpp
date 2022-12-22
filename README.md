# pnglibcpp

This is verry small library that depends only on libpng. It allows you
to load and save PNG files data represented as vector of chars. It is
probably the most simplified PNG loading method I can imagine.

## Usage

### Example ###

You can build simple example using `make example`

### API ###
This is the basic API:

```cpp
// namespace puzniakowski::png
class pngimage_t {
public:

    int width; ///< image width
    int height; ///< image height
    int bpp; ///< bytes per pixel
    std::vector < unsigned char > data; ///< pixels
    
    // returns pixel value
    unsigned int get(int x, int y);

    // sets pixel value
    void set(int x, int y, unsigned int c);

    static const int R = 0;
    static const int G = 1;
    static const int B = 2;
    static const int A = 3;

    // returns color channel value
    unsigned char &getI(int x, int y, int p);
    // returns value which you can pass as argument c of set function
    unsigned int getColorFromRGBA(int8_t r, int8_t g, int8_t b, int8_t a);
    //takes color and split it into 4 rgba channels
    void getRGBAFromColor(unsigned int color, int8_t* r, int8_t* g, int8_t* b, int8_t* a);
};

pngimage_t read_png_file(const std::vector<unsigned char> &file_contents_);
pngimage_t read_png_file(const std::string &file_name_);

std::vector<unsigned char> write_png_file( const pngimage_t &image_);
void write_png_file( const std::string &file_name_, const pngimage_t &image_);

```

### Tests ###

At file unit_pnglib.cpp

## Authors

Tadeusz Puźniakowski, 2017  
Marat Nagayev, 2022

