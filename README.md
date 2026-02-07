To code the raspberry pico using wsl, run those powershell commands as admin:

``
usbipd bind --force --busid 2-13
usbipd attach --busid 2-13 --auto-attach --wsl
``

The busid may change, check it with this command:

``
usbipd list
``# NeatoD7_pico
# NeatoD7_pico
