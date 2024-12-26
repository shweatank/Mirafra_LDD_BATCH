savedcmd_/home/tyr/ldd/i2c/i2c_driver1.mod := printf '%s\n'   i2c_driver1.o | awk '!x[$$0]++ { print("/home/tyr/ldd/i2c/"$$0) }' > /home/tyr/ldd/i2c/i2c_driver1.mod
