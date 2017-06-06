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
        if (bpp == 3) return ((unsigned int)data[(y*width+x)*bpp])+((unsigned int)data[(y*width+x)*bpp+1]<<8) + ((unsigned int)data[(y*width+x+2)*bpp]<<16);
        return ((unsigned int)data[(y*width+x)*bpp])+((unsigned int)data[(y*width+x)*bpp+1]<<8) + ((unsigned int)data[(y*width+x+2)*bpp]<<16) + ((unsigned int)data[(y*width+x+2)*bpp]<<24);
}

unsigned char &pngimage_t::getI(int x, int y, int p) {
	return data[(y*width+x)*bpp+p];
}


void rdfis_f(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead){
    std::pair<size_t *, std::vector<unsigned char> *> &io_data_p =
    *(std::pair<size_t *, std::vector<unsigned char> * > *) png_get_io_ptr(png_ptr);
    size_t &filePointer = *io_data_p.first;
    std::vector<unsigned char> &file_contents = *io_data_p.second;
    for (int i = filePointer; (i < filePointer+byteCountToRead) && (i < file_contents.size()); i++,outBytes++)
        *outBytes = file_contents[i];
    filePointer+=byteCountToRead;
};

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
        std::cout << "data..." << file_content.size() << std::endl;

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
}}


#ifdef UNITTEST
using namespace puzniakowski::png;

int main(int argc, char **argv)
{

    std::vector<unsigned char> file_contents;
    {
        // the following 7 lines is based on stack-overflow article
        std::ifstream file("a.png");
        if (!file.eof() && !file.fail())
        {
            file.seekg(0, std::ios_base::end);
            std::streampos fileSize = file.tellg();
            file_contents.resize(fileSize);

            file.seekg(0, std::ios_base::beg);
            file.read((char*)&file_contents[0], fileSize);
        }
    }

    auto r = read_png_file(file_contents);
    for (int x = 0; x < r.width; x+=2) {
        for (int y = 0; y < r.height; y+=2) {
            r.getI(x,y,r.A) = 255;
        }
    }
    std::vector<unsigned char> data_to_write = write_png_file( r);

    std::ofstream f("b.png", std::ios::out | std::ofstream::binary);
    std::copy(data_to_write.begin(), data_to_write.end(), std::ostreambuf_iterator<char>(f));

    return 0;
}
#endif

