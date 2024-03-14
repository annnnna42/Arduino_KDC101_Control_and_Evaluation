from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import sys
import os

arg = sys.argv[1]
file_name = f"data_stage/stage_data_{arg}.txt"
    
def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state

# home stage and wait while doing so
def stage_move(pos):
    stage.move_to(pos)
    stage.wait_move()

# get continuous position upon rising edge while stage is waiting
def wait_async(timeout, direction):
    t1 = time.time()
    while(time.time()-t1 <= timeout):
        if get_rising_edge_trig2():
            get_position_stage(direction)

# get position and append to trigger_positions
def get_position_stage(direction):
    global start_time
    global line
    #print(line)
    line = line + 1 

    position = stage.get_position()/scale_pos
    #print(position)

    if direction == "fw":
        trigger_positions.append([datetime.now()-start_time, position, None, None])
    elif direction == "bw":
        trigger_positions.append([datetime.now()-start_time, None, position, None])
    elif direction == "at start":
        trigger_positions.append([datetime.now()-start_time, None, None, position])
    elif direction == "at end":
        trigger_positions.append([datetime.now()-start_time, None, None, position])
    
    return trigger_positions

# return True only upon rising edge
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

def wait_for_init_edge(): 
    print(get_rising_edge_trig2())
    while True:
        state = return_trig2_state()
        print(state)
        if state:
            return True


# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    with open("script_running.txt", "w") as f:
        time.sleep(0.5)

    scale_pos = 34554.97192
    start_mm = 35.4*scale_pos
    end_mm = 45.4*scale_pos
    trigger_positions = []
    line = 0

    stage.setup_kcube_trigio(trig2_mode='in_gpio', trig2_pol=True)
    trig2_state_after = False 

    stage_move(start_mm)

    print("Wait for rising edge")
    wait_for_init_edge()
    print("Start")

    start_time = datetime.now()

    for i in range(1):
        print(f"Round: {i}")
        stage.move_to(end_mm)
        while stage.is_moving():  
            if (get_rising_edge_trig2()):  
                trigger_positions = get_position_stage("fw") 

        #time.sleep(2)
        wait_async(3, "at end")
                                                       
        stage.move_to(start_mm)
        while stage.is_moving():
            if (get_rising_edge_trig2()):
                trigger_positions = get_position_stage("bw")

        #time.sleep(2)
        wait_async(3, "at start")

    os.remove("script_running.txt")

    stage_move(25*scale_pos)

    print("End")

    with open(file_name, "w") as output:
        output.write(str(trigger_positions))

    print("File is written, close connection")
    stage.close()


