/**
Tadeusz Puźnikowski 2017
*/


/*
 * Code inspired by ( 2002-2010 ) Guillaume Cottenceau.
 * http://zarb.org/~gc/html/libpng.html
 */

#include <pnglib.hpp>

#include <png.h>
#include <tuple>
#include <vector>
#include <string>
#include <functional>
#include <iterator>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace puzniakowski {
namespace png {

unsigned int pngimage_t::get(int x, int y) {
        if (bpp == 3) return 
               ((unsigned int)data[(y*width+x)*bpp])+
               ((unsigned int)data[(y*width+x)*bpp+1]<<8) + 
               ((unsigned int)data[(y*width+x)*bpp+2]<<16);
        return ((unsigned int)data[(y*width+x)*bpp])+
               ((unsigned int)data[(y*width+x)*bpp+1]<<8) + 
               ((unsigned int)data[(y*width+x)*bpp+2]<<16) + 
               ((unsigned int)data[(y*width+x)*bpp+3]<<24);
}

void pngimage_t::set(int x, int y, unsigned int c) {
        data[(y*width+x)*bpp] = c & 0x0ff;
        data[(y*width+x)*bpp+1] = (c >> 8 ) & 0x0ff;
        data[(y*width+x)*bpp+2] = (c >>16) & 0x0ff;
        if (bpp == 4) {
            data[(y*width+x)*bpp+3] = (c >>16) & 0x0ff;
        }
}

unsigned char &pngimage_t::getI(int x, int y, int p) {
	return data[(y*width+x)*bpp+p];
}

/**
 * Callback function for png library. This function feeds data from stream.
 * The pointer is of type std::pair<size_t *, std::vector<unsigned char> *> and it 
 * holds information about data read count, and the data itself
 */
void rdfis_f(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead){
    std::pair<size_t *, std::vector<unsigned char> *> &io_data_p =
    *(std::pair<size_t *, std::vector<unsigned char> * > *) png_get_io_ptr(png_ptr);
    size_t &filePointer = *io_data_p.first;
    std::vector<unsigned char> &file_contents = *io_data_p.second;
    for (int i = filePointer; (i < filePointer+byteCountToRead) && (i < file_contents.size()); i++,outBytes++)
        *outBytes = file_contents[i];
    filePointer+=byteCountToRead;
};

pngimage_t read_png_file(const std::vector<char> &file_contents_) {
    std::vector<unsigned char> fc;fc.reserve(file_contents_.size());
    for (auto c: file_contents_) fc.push_back(c);
    return read_png_file(fc);
}

pngimage_t read_png_file(const std::vector<unsigned char> &file_contents_) {
    std::vector<unsigned char> file_contents = file_contents_;
    png_infop info_ptr;
    png_structp png_ptr;
    pngimage_t image;

    size_t filePointer = 8;
    // superminified png check
    if (file_contents[1] != 'P') throw std::runtime_error( "not PNG file" );

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) throw std::runtime_error( "png_create_read_struct" );

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) throw std::runtime_error( "png_create_info_struct" );

    std::pair<size_t*, std::vector<unsigned char>* > io_data_p = {&filePointer, &file_contents};
    png_set_read_fn(png_ptr, &io_data_p, (png_rw_ptr)rdfis_f);

    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    image.width = png_get_image_width(png_ptr, info_ptr);
    image.height = png_get_image_height(png_ptr, info_ptr);

    if (png_get_bit_depth(png_ptr, info_ptr) != 8) throw std::runtime_error( "unsupported png bit depth" );

    png_read_update_info(png_ptr, info_ptr);

    std::vector < png_bytep > row_ptr(image.height);
    std::vector < png_byte > rows(png_get_rowbytes(png_ptr,info_ptr)*image.height);

    for (int y=0; y < image.height; y++) {
        row_ptr[y] = &rows[y*png_get_rowbytes(png_ptr,info_ptr)];
    }
    png_read_image(png_ptr, &row_ptr[0]);

    if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
        image.bpp = 3;
    } else if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
        image.bpp = 4;
    } else {
        throw std::runtime_error( "PNG_COLOR_TYPE -- unsupported" );
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    image.data = rows;
    return image;
}

std::vector<unsigned char> write_png_file( const pngimage_t &image_) {
    pngimage_t image = image_;
    png_infop info_ptr;
    png_structp png_ptr;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) throw std::runtime_error( "png_create_write_struct" );

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) throw std::runtime_error( "png_create_info_struct" );

    std::vector<unsigned char> file_content;
    png_set_write_fn(png_ptr, &file_content, [](png_structp png_ptr, png_bytep data, png_size_t length) {
        std::vector<unsigned char> &file_content = *(std::vector<unsigned char> *) png_get_io_ptr(png_ptr);
        for (int i = 0; i < length; i++) file_content.push_back(data[i]);
    }, NULL);

    if (image.bpp == 3) png_set_IHDR(png_ptr, info_ptr, image.width, image.height,
                                         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                                         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    if (image.bpp == 4) png_set_IHDR(png_ptr, info_ptr, image.width, image.height,
                                         8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                                         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    std::vector < png_bytep > row_pointers(image.height);
    for (int y = 0; y < image.height; y++) {
        row_pointers[y] = &image.data[y*png_get_rowbytes(png_ptr,info_ptr)];
    }
    png_write_image(png_ptr, &row_pointers[0]);
    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return file_content;
}



pngimage_t read_png_file (const std::string &fn)  {
        std::vector<char> file_contents;
        std::ifstream file(fn);
        file.seekg(0, std::ios_base::end);
        file_contents.resize(file.tellg());
        file.seekg(0, std::ios_base::beg);
        file.read(file_contents.data(), file_contents.size());
        return read_png_file(file_contents);
}

void write_png_file( const std::string &filename_, const pngimage_t &image_ ) {
    std::vector<unsigned char> data_to_write = write_png_file( image_ );
    std::ofstream f(filename_, std::ios::out | std::ofstream::binary);
    std::copy(data_to_write.begin(), data_to_write.end(), std::ostreambuf_iterator<char>(f));
}

unsigned int getColorFromRGBA(int8_t r, int8_t g, int8_t b, int8_t a){
    unsigned int color = 0;
    color|=((unsigned)r << (8*0));
    color|=((unsigned)g << (8*1));
    color|=((unsigned)b << (8*2));
    color|=((unsigned)a << (8*3));
    return color;
}

void getRGBAFromColor(unsigned int color, int8_t* r, int8_t* g, int8_t* b, int8_t* a){
    *r = (color >> 24) & 0xFF;
    *g = (color >> 16) & 0xFF;
    *b = (color >> 8) & 0xFF;
    *a = color & 0xFF;
}

}}


