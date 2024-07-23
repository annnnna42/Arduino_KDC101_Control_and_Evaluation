from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import serial
import sys
import matplotlib as plt

arg = sys.argv[1]
rounds = int(sys.argv[2])


def stage_setup(scale_pos):
    acc = 3*scale_pos
    maxv = 1.7*scale_pos
    stage.setup_kcube_trigio(trig2_mode='in_gpio', trig2_pol = False)
    stage.setup_velocity(acceleration=acc, max_velocity=maxv)

def ser_config():
    arduino_port = "COM3"
    baud = 115200
    ser = serial.Serial(arduino_port, baud)
    return ser


def get_data():
    buffer = ""
    start_time = time.time()
    while stage.is_moving():
        if ser.in_waiting > 0:
            temp = []
            temp.append(stage.get_position()/scale_pos)
            getData = ser.readline().decode('utf-8').strip()
            temp.append(getData.split(","))
            print(temp)
            all_data.append(temp)


    t1 = time.time()
    # wait for 5 seconds
    while(time.time()-t1 <= 5):
        if ser.in_waiting > 0:
            temp = []
            temp.append(stage.get_position()/scale_pos)
            getData = ser.readline().decode('utf-8').strip()
            temp.append(getData.split(","))
            print(temp)
            all_data.append(temp)



with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    start_pos = 1.8
    end_pos = 49.8

    stage_setup(scale_pos)
    ser = ser_config() 
    trig1_state_after = False 

    file_name = f"merge_data/merge_data_{arg}.txt"

    all_data = []


    for i in range(rounds):
        stage.move_to(end_pos*scale_pos)
        get_data() 

        stage.move_to(start_pos*scale_pos)
        get_data()

        print(i)


    print(all_data)

    with open(file_name, "w") as f:
        f.write(str(all_data))

    stage.close()
