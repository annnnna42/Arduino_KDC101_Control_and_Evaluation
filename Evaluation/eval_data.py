import ast
import matplotlib.pyplot as plt
from datetime import datetime, timedelta
import sys

arg = sys.argv[1]
file_name_arduino = f"data_arduino/arduino_data_{arg}.txt"
file_name_stage = f"data_stage/stage_data_{arg}.txt"

# Read the content of the file
with open(file_name_arduino, "r") as fda:
    arduino_data = fda.read()

with open(file_name_stage, "r") as fds:
    stage_data = fds.read()

# Safely evaluate the string as a Python expression
arduino_data = ast.literal_eval(arduino_data)
x = [None for _ in range(len(arduino_data))]

# Convert strings to integers
ard_data_int = [[int(value) for value in sublist] for sublist in arduino_data]

reed_f0 = [sublist[0] for sublist in ard_data_int]
reed_f1 = [sublist[1] for sublist in ard_data_int]
reed_b0 = [sublist[2] for sublist in ard_data_int]
reed_b1 = [sublist[3] for sublist in ard_data_int]
reed_b2 = [sublist[4] for sublist in ard_data_int]

stage_data = eval(stage_data.replace("datetime.datetime", "datetime").replace("datetime.timedelta", "timedelta"))

timing = [sublist[0].total_seconds() for sublist in stage_data]            # index doesnt make much sense in this context
position_fw = [sublist[1] for sublist in stage_data]
position_bw = [sublist[2] for sublist in stage_data]


print(ard_data_int)
print(timing)
print(position_fw)
print(position_bw)
print(len(ard_data_int))
print(len(stage_data))

plt.scatter(x, reed_f0)
plt.scatter(x, reed_f1)
plt.scatter(x, reed_b0)
plt.scatter(x, reed_b1)
plt.scatter(x, reed_b2)

