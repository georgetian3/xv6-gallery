#include "gallery.h"


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