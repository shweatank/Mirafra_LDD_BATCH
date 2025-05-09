#include <stdio.h>
#include <string.h>

int main() {
    char csrc[100] = "Geeksfor";
    memcpy(csrc + 8, csrc, strlen(csrc) + 1); // Overlapping memory
    printf("%s\n", csrc);
    return 0;
}

