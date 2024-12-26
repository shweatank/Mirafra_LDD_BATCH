savedcmd_/home/tyr/ldd/net/vnet6.mod := printf '%s\n'   vnet6.o | awk '!x[$$0]++ { print("/home/tyr/ldd/net/"$$0) }' > /home/tyr/ldd/net/vnet6.mod
