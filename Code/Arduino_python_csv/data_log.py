import serial
import io

arduino_port = "COM3" #serial port of Arduino
baud = 9600 #arduino uno runs at 9600 baud
fileName="new-soil-log-data.csv" #name of the CSV file generated
ser = serial.Serial(arduino_port, baud)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")
samples = 2001 #how many samples to collect
line = 0 #start at 0 because our header is 0 (not real data)
while line <= samples:

    getData=str(ser.readline())
    data=getData[2:][:-3]
    print(data)

    file = open(fileName, "a")
    file.write(data + "\n") #write data with a newline
    line = line+1

print("Data collection complete!")
file.close()
