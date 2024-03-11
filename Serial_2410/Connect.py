import serial

ser = serial.Serial('/dev/tty.Plser', 9600, timeout=1)
ser.write("*RST")
ser.close()
