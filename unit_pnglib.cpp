#include <pnglib.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <cstdio>

using namespace puzniakowski::png;

TEST_CASE( "File IO", "[pnglib][read_png_file][write_png_file]" ) {
    pngimage_t r = read_png_file("data/a.png");
    write_png_file("data/b.png",r);

    SECTION("file written must be identical to file read") {
        pngimage_t r2 = read_png_file("data/b.png");
        int diffs = 0;
        for (int x = 0; x < r.width; x++) {
            for (int y = 0; y < r.height; y++) {
                diffs += (r.get(x,y) == r2.get(x,y))?0:1;
            }
        }
        REQUIRE(diffs == 0);
    }

    SECTION("reading using raw data") {
        std::vector<char> file_contents;
        std::ifstream file("data/b.png");
        file.seekg(0, std::ios_base::end);
        file_contents.resize(file.tellg());
        file.seekg(0, std::ios_base::beg);
        file.read(file_contents.data(), file_contents.size());

        auto rf = read_png_file(file_contents);
        int diffs = 0;
        for (int x = 0; x < r.width; x++) {
            for (int y = 0; y < r.height; y++) {
                diffs += (r.get(x,y) == rf.get(x,y))?0:1;
            }
        }
        REQUIRE(diffs == 0);
    }
    SECTION("writing using raw data") {

        auto rf = read_png_file("data/b.png");
        std::vector<unsigned char> data_to_write = write_png_file( rf );
        
        std::ofstream f("data/b.png", std::ios::out | std::ofstream::binary);
        std::copy(data_to_write.begin(), data_to_write.end(), std::ostreambuf_iterator<char>(f));
        f.close();
        rf = read_png_file("data/b.png");
        int diffs = 0;
        for (int x = 0; x < r.width; x++) {
            for (int y = 0; y < r.height; y++) {
                diffs += (r.get(x,y) == rf.get(x,y))?0:1;
            }
        } 
        REQUIRE(diffs == 0);
    }


    std::remove("b.png"); 
}
TEST_CASE( "Data accessing methods", "[pnglib][get][set]" ) {
    auto r = read_png_file("data/a.png");
    
    REQUIRE(r.getI(32,2,r.A) == 0);
    REQUIRE(r.get(32,2) == 0x3789e2);
    for (int x = 0; x < r.width; x+=2) {
        for (int y = 0; y < r.height; y+=2) {
            r.getI(x,y,r.A) = 255;
        }
    }
    REQUIRE(r.get(32,2) == 0xff3789e2);
}


