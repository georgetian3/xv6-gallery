#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "../kernel/vga.h"
#include "user.h"

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a < b ? b : a)
#define abs(x)    (x < 0 ? -x : x)

const int halfwidth = WIDTH / 2;
const int halfheight = HEIGHT / 2;

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

int round(double n) {
    int i = (int)n;
    double decimal = n - i;
    if (decimal < 0.5) {
        return i;
    }
    return i + 1;
}

const char* filename;
struct Image img;
int fd;

char framebuffer[WIDTH * HEIGHT];

const char help[] = "\n\nGallery\n\
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

bool inbound(int x, int y) {
    return x >= 0 && x < img.width && y >= 0 && y < img.height;
}

void set_pixel(int x, int y, struct Pixel p) {
    if (!inbound(x, y)) {
        printf("set_pixel error\n");
        exit(0);
    }
    img.pixels[y * img.width + x] = p;
}
struct Pixel get_pixel(int x, int y) {
    if (!inbound(x, y)) {
        printf("get_pixel error\n");
        exit(0);
    } 
    return img.pixels[y * img.width + x];
}
void rotate(int direction){

}
void hflip() {
    int mid = img.width / 2;
    struct Pixel temp;
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < mid; x++) {
            temp = get_pixel(x, y);
            set_pixel(x, y, get_pixel(img.width - x - 1, y));
            set_pixel(img.width - x - 1, y, temp);
        }
    }
}
void vflip() {
    int mid = img.height / 2;
    struct Pixel temp;
    for (int x = 0; x < img.width; x++){
        for (int y = 0; y < mid; y++){
            temp = get_pixel(x, y);
            set_pixel(x, y, get_pixel(x, img.height - y - 1));
            set_pixel(x, img.height - y - 1, temp);
        }
    }
}


int xpos = 0;
int ypos = 0;
double zoom = 1;


void draw() {
    double ratio = img.width * 1.0 / img.height;
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    double scale;
    if (ratio >= RATIO) {
        x1 = img.width * (1 - 1 / zoom) / 2 + xpos;
        x2 = img.width - x1;
        scale = WIDTH * 1.0 / (x2 - x1);
        double new_height = img.height * scale;
        if (new_height > HEIGHT) {
            y1 = (new_height - HEIGHT) * 0.5 / scale;
            y2 = img.height - y1;
        }
        y1 += ypos;
        y2 += ypos;
    } else {
        y1 = img.height * (1 - 1 / zoom) / 2 + ypos;
        y2 = img.height - y1;
        scale = HEIGHT * 1.0 / (y2 - y1);
        double new_width = img.width * scale;
        if (new_width > HEIGHT) {
            x1 = round((new_width - WIDTH) * 0.5 / scale);
            x2 = img.width - x1;
        }
        x1 += xpos;
        x2 += xpos;
    }
    printf("xpos %d ypos %d x1 %d y1 %d x2 %d y2 %d\n", xpos, ypos, x1, y1, x2, y2);
    

    struct Pixel p;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            int x = x1 + i / scale;
            int y = y1 + j / scale;
            if (!inbound(x, y)) {
                setpixel(i, j, 0, 0, 0);
            } else {
                p = get_pixel(x, y);
                setpixel(i, j, p.r, p.g, p.b);
            }
        }
    }
    return;
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            int x = i;
            int y = j;
            if (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT) {
                p = get_pixel(i, j);
                setpixel(x, y, p.r, p.b, p.g);
            }
        }
    }

};

double zoom_scale = 1.2;

void testcolors() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            setpixel(i, j, i, j, (i + j * (1 - 2 * (j % 2))) % 256);
        }
    }
}
void eventloop() {
    setmsgstate(1);
    char msg = 0;
    while (msg != 'x') {
        msg = getmsg();
        if (msg == 0) {
            continue;
        }
        //printf("%x\n", msg);
        switch (msg) {
            case 'r': // reset
                xpos = 0;
                ypos = 0;
                zoom = 1;
                imgread();
                draw();
                break;
            case 'h':
                hflip();
                draw();
                break;
            case 'v':
                vflip();
                draw();
                break;
            case 'i':
                printf("Zooming in\n");
                zoom *= zoom_scale;
                draw();
                break;
            case 'o':
                printf("Zooming out\n");
                zoom /= zoom_scale;
                draw();
                break;
            case 0x41: // up
                ypos += 5;
                draw();
                break;
            case 0x42: // down
                ypos -= 5;
                draw();
                break;
            case 0x43: // right
                xpos += 5;
                draw();
                break;
            case 0x44: //left
                xpos -= 5;
                draw();
                break;
            case 0x64: // d, rotate left
                rotate(1);
                draw();
                break;
            case 0x66: // f, rotate right
                rotate(0);
                draw();
                break;

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

    printf("%s", help);

    filename = argv[1];

    imgread(filename);
    printf("img.width %d img.height %d\n", img.width, img.height);
    draw();

    

    eventloop();


    close(fd);
    
    exit(0);

}