import time
import serial
import sys,tty,termios
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600, timeout=3)

s.write("/gogo/run \n".encode())
        
s = serial.Serial(sys.argv[1])
s.close()