#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "../kernel/vga.h"
#include "user.h"

struct __attribute__((__packed__)) BitmapFileHeader
{
  uint16 bfType;
  uint32 bfSize;
  uint16 bfReserved1;
  uint16 bfReserved2;
  uint32 bfOffBits;
} fh;

struct __attribute__((__packed__)) BitmapInfoHeader
{
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

struct Pixel
{
  char b;
  char g;
  char r;
};

struct Image
{
  int width, height;
  struct Pixel *pixels;
};

int round(double n)
{
  int i = (int)n;
  double decimal = n - i;
  if (decimal < 0.5)
  {
    return i;
  }
  return i + 1;
}

const char *filename;
struct Image img;
int fd;

struct Pixel *pa;
struct Pixel *pb;
bool a = true;

const char help[] = "\n\nGallery\n\
     x: exit\n\
     r: reset view\n\
     h: flip horizontally\n\
     v: flip vertically\n\
     i: zoom in\n\
     o: zoom out\n\
     d: rotate left\n\
     f: rotate right\n\
     up, down, left, right arrows: pan\n\
     \n";

void imgread()
{

  fd = open(filename, O_RDONLY);
  if (fd == -1)
  {
    printf("Invalid file\n");
    exit(0);
  }

  if (read(fd, &fh, sizeof(struct BitmapFileHeader)) == -1 ||
    read(fd, &ih, sizeof(struct BitmapInfoHeader)) == -1)
  {
    printf("Invalid bitmap headers\n");
    close(fd);
    exit(0);
  }

  img.height = ih.biHeight;
  img.width = ih.biWidth;

  pa = sbrk(img.width * img.height * sizeof(struct Pixel));
  pb = sbrk(img.width * img.height * sizeof(struct Pixel));

  int padding = img.width % 4;
  img.pixels = pa;

  struct Pixel discard[4];

  for (int y = img.height - 1; y >= 0; y--)
  {
    if (read(fd, img.pixels + y * img.width, img.width * sizeof(struct Pixel)) == -1 ||
      read(fd, discard, padding) == -1)
    {
      printf("Invalid bitmap\n");
      close(fd);
      exit(0);
    }
  }
}

bool outside(int x, int y)
{
  return x < 0 || x >= img.width || y < 0 || y >= img.height;
}

void set_pixel(int x, int y, struct Pixel p)
{
  if (outside(x, y))
  {
    printf("set_pixel error\n");
    exit(0);
  }
  img.pixels[y * img.width + x] = p;
}

struct Pixel get_pixel(int x, int y)
{
  static struct Pixel black = {0, 0, 0};
  if (outside(x, y))
  {
    return black;
  }
  return img.pixels[y * img.width + x];
}

void hflip()
{
  int mid = img.width / 2;
  struct Pixel temp;
  for (int y = 0; y < img.height; y++)
  {
    for (int x = 0; x < mid; x++)
    {
      temp = get_pixel(x, y);
      set_pixel(x, y, get_pixel(img.width - x - 1, y));
      set_pixel(img.width - x - 1, y, temp);
    }
  }
}

void vflip()
{
  int mid = img.height / 2;
  struct Pixel temp;
  for (int x = 0; x < img.width; x++)
  {
    for (int y = 0; y < mid; y++)
    {
      temp = get_pixel(x, y);
      set_pixel(x, y, get_pixel(x, img.height - y - 1));
      set_pixel(x, img.height - y - 1, temp);
    }
  }
}

void lrotate()
{
  for (int i = 0; i < 3; i++)
  {
    rrotate();
  }
}

void rrotate()
{
  struct Pixel *new_pixels = a ? pb : pa;
  a = !a;
  for (int i = 0; i < img.width; i++)
  {
    for (int j = 0; j < img.height; j++)
    {
      new_pixels[i * img.height + img.height - j + 1] = get_pixel(i, j);
    }
  }

  img.pixels = new_pixels;
  int temp = img.width;
  img.width = img.height;
  img.height = temp;
}

int xpos = 0;
int ypos = 0;
int zoom = 1;

void draw()
{
  int  xmax, ymax;
  struct Pixel p;
  xmax = WIDTH / zoom;
  ymax = HEIGHT / zoom;
  for (int i = 0; i < xmax; ++i)
  {
    for (int j = 0; j < ymax; ++j)
    {
      p = get_pixel(xpos + i, ypos + j);
      for (int k = 0; k < zoom; ++k)
      {
        for (int l = 0; l < zoom; ++l)
        {
          setpixel(i * zoom + k, j * zoom + l, p.r, p.g, p.b);
        }
      }
    }
  }
};

void testcolors()
{
  for (int i = 0; i < WIDTH; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      setpixel(i, j, i, j, (i + j * (1 - 2 * (j % 2))) % 256);
    }
  }
}

void eventloop()
{
  static const int pan = 8;
  listenmsgs(1);
  char msg = 0;
  while (msg != 'x')
  {
    msg = getmsg();
    if (msg == 0)
    {
      continue;
    }
    switch (msg)
    {
    case 'r':
      printf("Resetting\n");
      xpos = 0;
      ypos = 0;
      zoom = 1;
      imgread();
      draw();
      break;
    case 'h':
      printf("Flipping horizontally\n");
      hflip();
      draw();
      break;
    case 'v':
      printf("Flipping vertically\n");
      vflip();
      draw();
      break;
    case 'i':
      printf("Zooming in\n");
      zoom *= 2;
      draw();
      break;
    case 'o':
      if (zoom > 1)
      {
        printf("Zooming out\n");
        zoom /= 2;
        draw();
      }
      break;
    case 0x41: // up
      printf("Panning up\n");
      ypos -= pan;
      draw();
      break;
    case 0x42: // down
      printf("Panning down\n");
      ypos += pan;
      draw();
      break;
    case 0x43: // right
      printf("Panning right\n");
      xpos += pan;
      draw();
      break;
    case 0x44: // left
      printf("Panning left\n");
      xpos -= pan;
      draw();
      break;
    case 'd':
      printf("Rotating left\n");
      lrotate();
      draw();
      break;
    case 'f':
      printf("Rotating right\n");
      rrotate();
      draw();
      break;
    }
  }
  listenmsgs(0);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    testcolors();
    printf("Usage: gallery [filename]\n");
    exit(0);
  }
  filename = argv[1];
  imgread(filename);
  printf("%s", help);
  draw();
  eventloop();
  close(fd);
  exit(0);
}
