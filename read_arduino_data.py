import serial

# variable initialisation
arduino_port = "COM3"
baud = 9600
sensor_data = []
samples = 1000
line = 0

# open serial port
ser = serial.Serial(arduino_port, baud)
print("Start")

# read serial line #samples 
while line <= samples:
    # read serial line and format data 
    getData=ser.readline().decode('utf-8')
    data=getData[0:][:-2]            
    sensor_data.append(data.split(","))
    print(sensor_data)

    line = line+1

# write to text file
with open("data_arduino/arduino_data_{sys.argv[1]}.txt", "w") as f:
    f.write(str(sensor_data))

print("File is written, close connection")
f.close()

