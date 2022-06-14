#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "user.h"


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

struct Pixel {
    char r;
    char g;
    char b;
};

struct Image {
    struct BitmapFileHeader fh;
    struct BitmapInfoHeader ih;
    struct Pixel* pixels;
};

struct Image readbitmap(const char* filename) {

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Invalid file\n");
        exit(0);
    }

    struct Image img;

    if (read(fd, &(img.fh), sizeof(struct BitmapFileHeader)) == -1 ||
        read(fd, &(img.ih), sizeof(struct BitmapInfoHeader)) == -1    ) {
        printf("Invalid bitmap headers\n");
        exit(0);
    }

    int padding = img.ih.biWidth % 4;
    img.pixels = malloc(img.ih.biWidth * img.ih.biHeight);

    struct Pixel discard[4];


    
    for (int y = 0; y < img.ih.biHeight; y++) {
        if (read(fd, &(img.pixels[img.ih.biWidth * y]), img.ih.biWidth * sizeof(struct Pixel)) == -1 ||
            read(fd, discard, padding) == -1) {
            printf("Invalid bitmap\n");
            exit(0);
        }
    }

    return img;

}

/*

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



struct Image imgopen(const char* filename) {
    /*
    int ext = -1;
    int len = 0;
    struct Image img;
    while (filename[len] != '\0') {
        len++;
        if (filename[len] == '.') {
            ext = len + 1;
        }
    }
    if (ext == -1 || ext == len) {
        printf("Wrong image extension\n");
        return img;
    }
    if (len == -1) {
        printf("Invalid filename\n");
        return img;
    }
    return img;
    */
   
}


/*void rotate(struct Image image, int angle) {
    if (angle == 0) {
        return;
    }
}*/

struct Image rotate(struct Image image){
    struct Image final;


    return final;
}


int pixel_index(struct Image image, int x, int y) {
    return 0;
}
/*
void flip_vertical(struct Image image) {
    int mid = image.width / 2;
    struct Pixel temp;
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < mid; x++) {
            //line = image.width * y;
            temp = image.pixels[pixel_index(image, x, y)];
            image.pixels[pixel_index(image, x, y)] = image.pixels[pixel_index(image, image.width - x - 1, y)];
            image.pixels[pixel_index(image, image.width - x - 1, y)] = temp;
        }
    }

}

void flip_horizontal(struct Image image) {
    int mid = image.height / 2;
    struct Pixel temp;
    for (int x=0; x<image.width; x++){
        for (int y=0; y<mid; y++){
            temp = image.pixels[pixel_index(image, x, y)];
            image.pixels[pixel_index(image, x, y)] = image.pixels[pixel_index(image, x, image.height - y -1)];
            image.pixels[pixel_index(image, x, image.height - y -1)] = temp;
        }
    }
    //test
}
*/


void
gui_init() {

    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 3; j++) {
            setpixel(i, j, 255);
        }
        for (int j = 197; j < 200; j++) {
            setpixel(i, j, 255);
        }
    }
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 3; j++) {
            setpixel(j, i, 255);
        }
        for (int j = 317; j < 320; j++) {
            setpixel(j, i, 255);
        }
    }

}


void
heart()
{
    for (int x = 140; x < 160; x++) {
        double slope = 1;
        setpixel(x, x * slope, 255);
        setpixel(320 - x, x * slope, 255);
    }

    int xs[] = {160, 158, 156, 153, 150, 147, 144, 142, 140, 140};
    int ys[] = {137, 134, 132, 130, 130, 130, 132, 134, 137, 140};
    int n = 10;

    for (int i = 0; i < n; i++) {
        setpixel(xs[i], ys[i], 255);
        setpixel(xs[i] + 20, ys[i], 255);
    }

}


int main(int argc, char** argv) {

    
    if (argc != 2) {
        printf("Specify a filename!\n");
        heart();
        exit(0);
    }

    gui_init();


    struct Image img = imgopen(argv[1]);


    exit(0);

}