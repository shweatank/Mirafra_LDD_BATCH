#include <stdio.h>
#include <string.h>

int main() {
    char csrc[100] = "Geeksfor";
    memmove(csrc + 8, csrc, strlen(csrc) + 1); // Overlapping memory handled
    printf("%s\n", csrc);
    return 0;
}

