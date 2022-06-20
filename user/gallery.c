#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "../kernel/vga.h"
#include "user.h"

struct __attribute__((__packed__)) BitmapFileHeader {
    uint16 bfType;
    uint32 bfSize;
    uint16 bfReserved1;
    uint16 bfReserved2;
    uint32 bfOffBits;
} fh;

struct __attribute__((__packed__)) BitmapInfoHeader {
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
} ih;

struct Pixel {
    char b;
    char g;
    char r;
};

struct Image {
    int width, height;
    struct Pixel* pixels;
};

const char* filename;
struct Image img;
int fd;

const char help[] =
    "\n\nGallery\n\
         x: exit\n\
         h: help\n\
         r: reset view\n\
         \n";



void imgread() {

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Invalid file\n");
        exit(0);
    }

    struct Image img;

    if (read(fd, &fh, sizeof(struct BitmapFileHeader)) == -1 ||
        read(fd, &ih, sizeof(struct BitmapInfoHeader)) == -1 ) {
        printf("Invalid bitmap headers\n");
        close(fd);
        exit(0);
    }


    img.height = ih.biHeight;
    img.width  = ih.biWidth;

    int padding = img.width % 4;
    img.pixels = (struct Pixel*)sbrk(img.width * img.height * sizeof(struct Pixel));

    struct Pixel discard[4];

    
    for (int y = img.height - 1; y >= 0; y--) {
        if (read(fd, img.pixels + y * img.width, img.width * sizeof(struct Pixel)) == -1 ||
            read(fd, discard, padding) == -1) {
            printf("Invalid bitmap\n");
            close(fd);
            exit(0);
        }
    }

}

void rotate(int direction){

}


int pixel_index(struct Image image, int x, int y) {
    return 0;
}

void vflip() {
    int mid = img.width / 2;
    struct Pixel temp;
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < mid; x++) {
            //line = img.width * y;
            temp = img.pixels[pixel_index(img, x, y)];
            img.pixels[pixel_index(img, x, y)] = img.pixels[pixel_index(img, img.width - x - 1, y)];
            img.pixels[pixel_index(img, img.width - x - 1, y)] = temp;
        }
    }

}

void hflip() {
    int mid = img.height / 2;
    struct Pixel temp;
    for (int x =0; x<img.width; x++){
        for (int y=0; y<mid; y++){
            temp = img.pixels[pixel_index(img, x, y)];
            img.pixels[pixel_index(img, x, y)] = img.pixels[pixel_index(img, x, img.height - y -1)];
            img.pixels[pixel_index(img, x, img.height - y -1)] = temp;
        }
    }
    //test
}



void guiinit() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < 3; j++) {
            setpixel(i, j, 255, 255, 255);
        }
        for (int j = 197; j < HEIGHT; j++) {
            setpixel(i, j, 255, 255, 255);
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < 3; j++) {
            setpixel(j, i, 255, 255, 255);
        }
        for (int j = 317; j < WIDTH; j++) {
            setpixel(j, i, 255, 255, 255);
        }
    }

}

void testcolors() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            setpixel(i, j, i, j, (i + j) % 256);
        }
    }
}


void draw(int x1, int x2, int y1, int y2) {
    double ratio = img.width * 1.0 / img.height;
    int width, height;
    if (ratio >= RATIO) {
        width = WIDTH;
        height = WIDTH / ratio;
    } else {
        width = HEIGHT / ratio;
        height = HEIGHT;
    }
    if (img.height < HEIGHT && img.width < WIDTH) {
        //stretch(img, width, height);
    } else {
       // shrink(img, width, height);
    }
    
};



void eventloop() {
    setmsgstate(1);
    char c = 255;
    while (c != 'x') {
        c = getmsg();
        switch (c) {
            case 255: // no input
                break;
            case 'r': // reset
                imgread();
                break;
            default:
                printf("%s", help);
        }
    }
    setmsgstate(0);
}

int main(int argc, char** argv) {

    if (argc != 2) {
        testcolors();
        printf("Usage: gallery [filename]\n");
        exit(0);
    }

    filename = argv[1];

    guiinit();

    imgread(filename);
    //draw();

    

    eventloop();


    close(fd);
    exit(0);

}