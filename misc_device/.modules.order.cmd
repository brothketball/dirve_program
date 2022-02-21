cmd_/home/pi/Gits/dirve_program/misc_device/modules.order := {   echo /home/pi/Gits/dirve_program/misc_device/misc.ko; :; } | awk '!x[$$0]++' - > /home/pi/Gits/dirve_program/misc_device/modules.order
