WIDTH = 8
HEIGHT = 4

imgw = 8
imgh = 8

zoom = 1

xpos = imgw // 2
ypos = imgh // 2

x1 = max(xpos / zoom, 0)
x2 = min((imgw + xpos) / zoom, imgw)
y1 = max(ypos / zoom, 0)
y2 = min((imgh + ypos) / zoom, imgh)
scale = max(WIDTH * 1.0 / (x2 - x1), HEIGHT * 1.0 / (y2 - y1))

def pix(x, y):
    return (xpos + (x - WIDTH // 2) * scale, ypos + (y - HEIGHT // 2) * scale)

print(pix(0, 0))