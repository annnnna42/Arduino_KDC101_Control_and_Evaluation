from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import serial
import sys

arg = sys.argv[1]
rounds = sys.argv[2]

def get_rising_edge_trig2():                        # get rising edge
    global trig2_state_after
    trig2_state_before = return_trig2_state()
    if trig2_state_before:                                  # trig1 True
        if trig2_state_before != trig2_state_after:         # rising edge
            trig2_state_after = trig2_state_before          # set constant state
            return True
    else:                                                   # trig1 False
        if trig2_state_before != trig2_state_after:         # falling edge
            trig2_state_after = trig2_state_before          # set constant state

def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state

def wait_async(timeout, direction):
    t1 = time.time()
    while(time.time()-t1 <= timeout):
        if get_rising_edge_trig2():
            get_position_stage(direction)

def get_position_stage(direction):
    global start_time

    position = stage.get_position()/scale_pos

    if direction == "fw":
        position_data.append([position, None, None])
    elif direction == "bw":
        position_data.append([None, position, None])
    else:
        position_data.append([None, None, position])
    
    return position_data

def ser_config():
    arduino_port = "COM3"
    baud = 9600
    ser = serial.Serial(arduino_port, baud)
    return ser

def stage_config():
    scale_pos = 34554.97192
    start_mm = 32.4*scale_pos
    end_mm = 45.4*scale_pos
    return scale_pos, start_mm, end_mm

def get_serial():
    getData=ser.readline().decode('utf-8')
    data=getData[0:][:-2]  
    return data


with Thorlabs.KinesisMotor("27267730") as stage:
    file_name_stage = f"data_stage/stage_data_{arg}.txt"
    file_name_arduino = f"data_arduino/arduino_data_{arg}.txt"

    position_matrix = []
    serial_matrix = []

    ser = ser_config() 
    scale_pos, start_mm, end_mm = stage_config()

    stage.setup_kcube_trigio(trig2_mode='in_gpio', trig2_pol=True)
    trig2_state_after = False 

    start_time = datetime.now()

    print("Sleep")
    time.sleep(3)
    print("Start")

    for i in range(rounds):
        position_data = []
        serial_data = []

        print(f"Round: {i}")

        stage.move_to(end_mm)        
        while stage.is_moving():  
            if (get_rising_edge_trig2()): 
                position_data = get_position_stage("fw") 
                # getData=ser.readline().decode('utf-8')
                # data=getData[0:][:-2]   
                data = get_serial()         
                serial_data.append(data.split(","))

        time.sleep(2)
                                                                       
        stage.move_to(start_mm)
        while stage.is_moving():
            if (get_rising_edge_trig2()):
                position_data = get_position_stage("bw")
                # getData=ser.readline().decode('utf-8')
                # data=getData[0:][:-2]
                data = get_serial()                     
                serial_data.append(data.split(","))

        time.sleep(2)

        position_matrix.append(position_data)
        serial_matrix.append(serial_data)

    with open(file_name_arduino, "w") as fa:
        fa.write(str(serial_matrix))

    with open(file_name_stage, "w") as fs:
        fs.write(str(position_matrix))

    print("written to files")
    stage.close()