from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import serial
import sys

# arg1 = sys.argv[1]
# arg2 = sys.argv[2]

def return_trig1_state():
    status = stage.get_status()
    #print(status)
    trig1_state = "digio1" in status
    print(trig1_state)
    return trig1_state

def return_trig2_state():
    status = stage.get_status()
    print(status)
    trig2_state = "digio2" in status
    print(trig2_state)
    return trig2_state
            
# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    acc = 4.5*scale_pos
    maxv = 2.4*scale_pos
    trigger_positions = []

    #stage.setup_kcube_trigio(trig1_mode = 'in_gpio', trig1_pol=True, trig2_mode='in_gpio', trig2_pol=False)
    stage.setup_kcube_trigio(trig1_mode='in_gpio', trig1_pol = False)

    stage.setup_velocity(acceleration=acc, max_velocity=maxv)
    print(stage.get_velocity_parameters())

    # stage.move_to(20*scale_pos)
    # stage.wait_move()  

    # stage.move_to(0*scale_pos)
    #  stage.wait_move()    
    #while stage.is_moving():
    while True:
        return_trig1_state()
        #return_trig2_state()
    #    stage.send_comm(0x0213, 0xFF)
    #    time.sleep(1)
    #    stage.send_comm(0x0213, 0x00)
        time.sleep(0.05)
    print("End")
    stage.close()
############################################

# ### test two scripts (two )
# #script b
# import time
# import os

# # Simulate Script B running for 5 seconds
# with open("script_b_running.txt", "w") as f:
#     # Do something in Script B
#     time.sleep(7)

# print("sleep again")
# time.sleep(5)
# print("Stop")

# # Delete the file to signal that Script B has stopped
# os.remove("script_b_running.txt")
##################################################
# arg = sys.argv[1]


# def get_rising_edge_trig2():                        # get rising edge
#     global trig2_state_after
#     trig2_state_before = return_trig2_state()
#     if trig2_state_before:                                  # trig1 True
#         if trig2_state_before != trig2_state_after:         # rising edge
#             trig2_state_after = trig2_state_before          # set constant state
#             return True
#     else:                                                   # trig1 False
#         if trig2_state_before != trig2_state_after:         # falling edge
#             trig2_state_after = trig2_state_before          # set constant state

# def return_trig2_state():
#     status = stage.get_status()
#     trig2_state = "digio2" in status
#     return trig2_state

# def wait_async(timeout, direction):
#     t1 = time.time()
#     while(time.time()-t1 <= timeout):
#         if get_rising_edge_trig2():
#             get_position_stage(direction)

# def get_position_stage(direction):
#     global start_time

#     position = stage.get_position()/scale_pos
#     #print(position)

#     if direction == "fw":
#         #trigger_positions.append([datetime.now()-start_time, position, None, None])
#         trigger_positions.append([position, None, None])
#     elif direction == "bw":
#         #trigger_positions.append([datetime.now()-start_time, None, position, None])
#         trigger_positions.append([None, position, None])
#     else:
#         #trigger_positions.append([datetime.now()-start_time, None, None, position])
#         trigger_positions.append([None, None, position])

    
#     return trigger_positions

# def ser_config():
#     arduino_port = "COM3"
#     baud = 9600
#     ser = serial.Serial(arduino_port, baud)
#     return ser

# def stage_config():
#     scale_pos = 34554.97192
#     start_mm = 32.4*scale_pos
#     end_mm = 45.4*scale_pos
#     return scale_pos, start_mm, end_mm

# def get_serial():
#     getData=ser.readline().decode('utf-8')
#     data=getData[0:][:-2]  
#     data.split(",") 
#     return data


# with Thorlabs.KinesisMotor("27267730") as stage:
#     file_name_stage = f"data_stage/stage_data_{arg}.txt"
#     file_name_arduino = f"data_arduino/arduino_data_{arg}.txt"

#     position_matrix = []
#     serial_matrix = []

#     ser = ser_config() 
#     scale_pos, start_mm, end_mm = stage_config()

#     stage.setup_kcube_trigio(trig2_mode='in_gpio', trig2_pol=True)
#     trig2_state_after = False 

#     start_time = datetime.now()

#     print("Sleep")
#     time.sleep(3)
#     print("Start")

#     for i in range(5):
#         trigger_positions = []
#         sensor_data = []

#         print(f"Round: {i}")

#         stage.move_to(end_mm)        
#         while stage.is_moving():  
#             if (get_rising_edge_trig2()): 
#                 trigger_positions = get_position_stage("fw") 
#                 getData=ser.readline().decode('utf-8')
#                 data=getData[0:][:-2]            
#                 sensor_data.append(data.split(","))

#         time.sleep(2)
                                                                       
#         stage.move_to(start_mm)
#         while stage.is_moving():
#             if (get_rising_edge_trig2()):
#                 trigger_positions = get_position_stage("bw")
#                 getData=ser.readline().decode('utf-8')
#                 data=getData[0:][:-2]            
#                 sensor_data.append(data.split(","))

#         time.sleep(2)

#         position_matrix.append(trigger_positions)
#         serial_matrix.append(sensor_data)

#     with open(file_name_arduino, "w") as fa:
#         fa.write(str(serial_matrix))

#     with open(file_name_stage, "w") as fs:
#         fs.write(str(position_matrix))

#     print("written to files")
#     stage.close()
