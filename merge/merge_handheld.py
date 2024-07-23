from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import serial
import sys
import matplotlib as plt

duration = int(sys.argv[1])
arg = sys.argv[2]


def ser_config():
    arduino_port = "COM3"
    baud = 115200
    ser = serial.Serial(arduino_port, baud)
    return ser


def get_data():
    buffer = ""
    start_time = time.time()

    while True:
        if ser.in_waiting > 0:
            getData = ser.readline().decode('utf-8').strip()
            data = getData.split(",")
            print(data)

            # Convert each element to float
            try:
                data = [float(x) for x in data]
                all_data.append(data)
            except ValueError:
                continue

        if time.time() - start_time > duration:
            break

ser = ser_config() 

file_name = f"merge_data/merge_data_{arg}.txt"

all_data = []

get_data()
    
print(all_data)

with open(file_name, "w") as f:
    f.write(str(all_data))

