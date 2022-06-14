#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "user.h"


struct Pixel {
    char r;
    char g;
    char b;
};

struct Image {
    int width;
    int height;
    struct Pixel* pixels;
};

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
    for (int j = image.height - 1, k = 0; j >= 0, k < image.height; j--, k++){
        for (int i = 0; i < image.width; i++){
            final.pixels[pixel_index(final, k, i)] = image.pixels[pixel_index(image, i, j)];
        }
    }

    return final;
}


int pixel_index(struct Image image, int x, int y) {
    return 0;
}

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


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Specify a filename\n");
        exit(0);
    }


    struct Image img = imgopen(argv[1]);

    gui_init();

    exit(0);

}