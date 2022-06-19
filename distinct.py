from distinctipy import distinctipy
from PIL import Image

# number of colours to generate
N = 256

# generate N visually distinct colours
#colors = distinctipy.get_colors(N, exclude_colors=[])

colors = []

rs = 8
gs = 8
bs = 4

for r in range(rs):
    for g in range(gs):
        for b in range(bs):
            colors.append((r * 32 + 16, g * 32 + 16, b * 64 + 32))

distinctipy.color_swatch([tuple(x[i] / 256 for i in range(3)) for x in colors])

exit()


r = 148
g = 210
b = 243


for r in range(256):
    for g in range(256):
        for b in range(256):

            x = r - r % 64
            y = g // 8
            y -= y % 4
            z = b // 64

            if x + y + z > 255:
                print(r, g, b)

                print(x, y, z)
                exit()

for i in range(256):
    r, g, b = i, i, i
    x = r - r % 64
    y = g // 8
    y -= y % 4
    z = b // 64
    print(x + y + z)