#include <pnglib.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace puzniakowski::png;

TEST_CASE( "Factorials are computed", "[factorial]" ) {
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
    REQUIRE(r.getI(32,2,r.A) == 0);
    for (int x = 0; x < r.width; x+=2) {
        for (int y = 0; y < r.height; y+=2) {
            r.getI(x,y,r.A) = 255;
        }
    }
    std::vector<unsigned char> data_to_write = write_png_file( r);

    std::ofstream f("b.png", std::ios::out | std::ofstream::binary);
    std::copy(data_to_write.begin(), data_to_write.end(), std::ostreambuf_iterator<char>(f));

    
}



