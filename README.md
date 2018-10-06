# Brushing up my knowledge about Seeeduino or microcontrollers


# SETUP

Seeeduino Mega v1.23
it has two switches on the board
move them toward the center of the board

## install toolchain
```
sudo apt-get install gcc-avr avr-libc gdb-avr avrdude
```

## upload of the program is done through /dev/ttyUSB0
```
sudo chmod 666 /dev/ttyUSB0
```

# Compilation and Upload of the Program
```
make clean
make
make upload
```

