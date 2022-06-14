# 常用的图片格式

已经存在用于处理各种格式的图片的代码库，但需要问一下我们能不能使用这些库

- JPEG: https://github.com/libjpeg-turbo/libjpeg-turbo
- PNG: http://www.libpng.org/pub/png/libpng.html
- GIF: http://giflib.sourceforge.net/gif_lib.html
- BMP: https://code.google.com/archive/p/libbmp/

# 图片浏览器的功能

以下列出可以考虑实现的功能

- 图片的放大、缩小、平移

# 资料
https://github.com/YueDayu/Themis_GUI  
https://github.com/rtmrtmrtmrtm/xv6-vga  
https://github.com/KevinVan720/xv6-gui  
https://github.com/YueDayu/Themis_GUI  

- VNC Viewer: https://www.realvnc.com/en/connect/download/viewer/
编译方法：在qemu的参数里加-display vnc=localhost
         在vnc-viewer里面连接localhost:0即可  

# 提示

- When the `main` function finishes, use `exit(0)` instead of `return 0` to avoid error messages, see https://stackoverflow.com/questions/71583679/why-return-does-not-exit-a-process-in-xv6
- xv6-vga vs xv6-riscv:
    - vm.c
    - pci.c
    - vga.c
    - main.c