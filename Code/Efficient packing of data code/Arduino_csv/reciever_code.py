import serial
import io

arduino_port = "COM3" #serial port of Arduino
baud = 9600 #arduino uno runs at 9600 baud
fileName="packing-log-data.csv" #name of the CSV file generated
ser = serial.Serial(arduino_port, baud)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

print("Connected to Arduino port:" + arduino_port)
file = open(fileName, "a")
print("Created file")
samples = 5 #how many samples to collect
line = 0 #start at 0 because our header is 0 (not real data)
atm_temp_thresh = 0
soil_temp_thresh = 0
while line <= samples:
    if(line == 0):
        getData=str(ser.readline())
        print(getData);
        data=getData[2:][:-3]
        print(data)
    
        file = open(fileName, "a")
        file.write(data + "\n") #write data with a newline
        line = line+1
    
    elif(line == 1):
        getData=str(ser.readline())
        print(getData);
        data=getData[2:][:-3]
        print(data)
        xdata = data.split(",")
        atm_temp_thresh = int(xdata[0])
        soil_temp_thresh = int(xdata[1])
        line = line + 1
        
    else:
        getData=str(ser.readline())
        print(getData);
        data1=getData[2:][:-3]
        xdata = data1.split(",")
        soil_temp = (int(xdata[0])&15)
        soil_temp = soil_temp + soil_temp_thresh - 8

        air_temp = (((int(xdata[0]) >> 4)&15))
        air_temp = air_temp + atm_temp_thresh - 8

        data =str(soil_temp) + "," + str(air_temp) + "," + str(xdata[1]) +"," + str(xdata[2])
        print(data)
        file = open(fileName, "a")
        file.write(data + "\n") #write data with a newline
        line = line+1

    
print("Data collection complete!")
file.close()
