# import serial
# import sys

# arg = sys.argv[1]

# # variable initialisation
# arduino_port = "COM3"
# baud = 9600
# sensor_data = []
# samples = 90
# line = 0
# file_name = f"data_arduino/arduino_data_{arg}.txt"

# # open serial port
# ser = serial.Serial(arduino_port, baud)
# print("Start")

# # read serial line #samples 
# while line <= samples:
#     # read serial line and format data 
#     getData=ser.readline().decode('utf-8')
#     data=getData[0:][:-2]            
#     sensor_data.append(data.split(","))
#     print(sensor_data)

#     line = line+1

# # write to text file
# with open(file_name, "w") as f:
#     f.write(str(sensor_data))

# print("File is written, close connection")
# f.close()


import serial
import sys
import os

arg = sys.argv[1]

# variable initialisation
arduino_port = "COM3"
baud = 9600
sensor_data = []
samples = 90
line = 0
file_name = f"data_arduino/arduino_data_{arg}.txt"

# open serial port
ser = serial.Serial(arduino_port, baud)
print("Start")

# read serial line #samples 
while True:
    if os.path.exists("script_running.txt"):
    # read serial line and format data 
        getData=ser.readline().decode('utf-8')
        data=getData[0:][:-2]            
        sensor_data.append(data.split(","))
        #print(data)
        #print(line)
        line = line + 1 
        
    else:
        print("Script finished")
        break

# write to text file
with open(file_name, "w") as f:
    f.write(str(sensor_data))

print("File is written, close connection")
f.close()

