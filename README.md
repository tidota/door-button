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

# Pin configuration
Be careful about the corresponde of pin names.
On the board, Seeeduino's pin names are printed, but
the code must follow the names defined for the chip (ATmega1280).

Seeeduino: Dx
ATmega1280 (chip): PAx PBx etc


# PC program
Change the baud rate
```
sudo stty -F /dev/ttyUSB0 9600
```

and start the program
