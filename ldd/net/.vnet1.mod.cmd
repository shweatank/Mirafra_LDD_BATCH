savedcmd_/home/tyr/ldd/net/vnet1.mod := printf '%s\n'   vnet1.o | awk '!x[$$0]++ { print("/home/tyr/ldd/net/"$$0) }' > /home/tyr/ldd/net/vnet1.mod
