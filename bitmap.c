#include <stdio.h>

struct __attribute__((__packed__)) BitmapFileHeader {
    unsigned short bfType;
    unsigned bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned bfOffBits;
} fh;

struct __attribute__((__packed__)) BitmapInfoHeader {
    unsigned biSize;
    unsigned biWidth;
    unsigned biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned biCompression;
    unsigned biSizeImages;
    unsigned biXPelsPerMeter;
    unsigned biYPelsPerMeter;
    unsigned biClrUsed;
    unsigned biClrImportant;
} ih;

void
print_bitmap(struct BitmapFileHeader fh, struct BitmapInfoHeader ih)
{
    printf("%d %d %d %d %d\n", fh.bfType, fh.bfSize, fh.bfReserved1, fh.bfReserved2, fh.bfOffBits);
    printf("%d %d %d %d %d %d %d %d %d %d %d\n", ih.biSize, ih.biWidth, ih.biHeight, ih.biPlanes, ih.biBitCount, ih.biCompression, ih.biSize, ih.biXPelsPerMeter, ih.biYPelsPerMeter, ih.biClrUsed, ih.biClrImportant);
}

int main() {
    FILE* f = fopen("test.bmp", "rb");
    fread(&fh, sizeof(fh), 1, f);
    fread(&ih, sizeof(ih), 1, f);
    print_bitmap(fh, ih);
    fclose(f);
}