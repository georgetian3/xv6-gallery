#include "../kernel/fcntl.h"
#include "../kernel/types.h"
#include "user.h"

int main() {

    printf("gallery\n");
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 200; j++) {
            setpixel(i, j, i + j);
        }
    }

    exit(0);

}