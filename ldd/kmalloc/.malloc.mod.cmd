savedcmd_/home/tyr/ldd/kmalloc/malloc.mod := printf '%s\n'   malloc.o | awk '!x[$$0]++ { print("/home/tyr/ldd/kmalloc/"$$0) }' > /home/tyr/ldd/kmalloc/malloc.mod
