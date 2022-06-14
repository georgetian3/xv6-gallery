#include "../kernel/types.h"
#include "gallery.h"

struct BitmapFileHeader {
    uint16 bfType;
    uint32 bfSize;
    uint16 bfReserved1;
    uint16 bfReserved2;
    uint32 bfOffBits;
};

struct BitmapInfoHeader {
    uint32 biSize;
    uint32 biWidth;
    uint32 biHeight;
    uint16 biPlanes;
    uint16 biBitCount;
    uint32 biCompression;
    uint32 biSizeImages;
    uint32 biXPelsPerMeter;
    uint32 biYPelsPerMeter;
    uint32 biClrUsed;
    uint32 biClrImportant;
};


struct Image readbitmap(const char* filename) {

    open(

    struct Image img;
    

    img_in.read((char*)(&file_header), sizeof(file_header));
    img_in.read((char*)(&info_header), sizeof(info_header));

    _width = info_header.biWidth;
    _height = info_header.biHeight;

    int padding = _width % 4;

    _pixels = new Pixel[_width * _height];
    
    for (int y = 0; y < _height; y++) {
        img_in.read((char*)(_pixels + _width * y), _width * sizeof(Pixel));
        img_in.ignore(padding);
    }

}


void save(const std::string& filename) {

    file_header.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + _height * _width * sizeof(Pixel);
    info_header.biHeight = _height;
    info_header.biWidth = _width;

    std::ofstream fout(filename, std::ios::binary);
    fout.write((char*)(&file_header), sizeof(BitmapFileHeader));
    fout.write((char*)(&info_header), sizeof(BitmapInfoHeader));

    char temp[] = {0, 0, 0, 0};
    int padding = _width % 4;

    for (int y = 0; y < _height; y++) {
        fout.write((char*)(_pixels + _width * y), _width * sizeof(Pixel));
        fout.write(temp, padding);
    }

}
    

*/