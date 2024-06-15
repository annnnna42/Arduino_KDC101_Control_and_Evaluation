import serial
import sys
import time

name = sys.argv[1]
run_time = sys.argv[2]

port = "COM3"
baud = 19200
serial_data = []

file_name = f"gyro/gyro_data/gyro_data_{name}.txt"

ser = serial.Serial(port, baud)

t_end = time.time() +  (60 * float(run_time))

while time.time() < t_end:
    getData = ser.readline().decode('utf-8')
    data = getData[0:][:-2]
    serial_data.append(data.split(","))
    print(data)


with open(file_name, "w") as f:
    f.write(str(serial_data))


