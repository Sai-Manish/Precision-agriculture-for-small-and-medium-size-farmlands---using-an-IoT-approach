import serial
import io
import httplib2

arduino_port = "COM3" #serial port of Arduino
baud = 9600 #arduino uno runs at 9600 baud
fileName="log-data.csv" #name of the CSV file generated
ser = serial.Serial(arduino_port, baud)

print("Connected to Arduino port:" + arduino_port)

#file = open(fileName, "a")
#print("Created file")
samples = 100 #how many samples to collect
line = 0 #start at 0 because our header is 0 (not real data)
while line <= samples:
    getData=str(ser.readline())
    gdata = getData[2:][:-3]
    data = gdata.split(',')
    #data=getData[2:][:-3]
    print(data)

    #a = float(data[0])
    #b = float(data[1])
    #c = float(data[2])
    #d = float(data[3])

    a = data[0]
    b = data[1]
    c = data[2]
    d = data[3]
    
    #file = open(fileName, "a")
    #file.write(data + "\n") #write data with a newline

    conn = httplib2.Http()
    conn.request("http://api.thingspeak.com/update?key=7GXBUU4G4HU20X9V"+"&field1="+a+"&field2="+ b +"&field3="+c+"&field4="+d)
    line = line+1

print("Data collection complete!")
