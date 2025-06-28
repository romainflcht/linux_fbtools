# linux_fbtools
This program use the linux framebuffer to draw painting and text onto the `STM32 Discovery board` screen. 

## Compile & install
Use the command `make` to compile the project. 
```bash
make
```

Use the command `make install` to copy the executable into the `rootfs` folder, compile the rootfs and install it to the `STM32`. 
```bash
make install
```

Make sure your path are setup correcty in the `makefile`. 
```
UCLINUX_PATH = /home/<user>/<path-to>/linux
ROOTFS_BIN_PATH = /home/<user>/<path-to>/rootfs/usr/bin

```
## Licence
- romainflcht