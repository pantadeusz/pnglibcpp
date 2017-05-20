# pnglibcpp

This is verry small library that depends only on libpng. It allows you
to load and save PNG files data represented as vector of chars. It is
probably the most simplified PNG loading method I can imagine.

### API ###
This is the whole API:

```
#!c++
// namespace puzniakowski::png
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
pngimage_t read_png_file(const std::vector<unsigned char> &file_contents_);
std::vector<unsigned char> write_png_file( const pngimage_t &image_);
```

(C) Tadeusz Puźniakowski, 2017
