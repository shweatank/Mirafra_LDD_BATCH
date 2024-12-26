savedcmd_/home/tyr/ldd/log_driver/log_driver1.mod := printf '%s\n'   log_driver1.o | awk '!x[$$0]++ { print("/home/tyr/ldd/log_driver/"$$0) }' > /home/tyr/ldd/log_driver/log_driver1.mod
