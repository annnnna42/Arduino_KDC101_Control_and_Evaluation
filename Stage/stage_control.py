from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time
import asynco

# write functions!!! 


def return_trig1_state():
    status = stage.get_status()
    trig1_state = "digio1" in status
    return trig1_state

def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state

def wait_for_init_trigger(): 
    print("wait_for_trig1_true()")
    while not return_trig1_state():
        time.sleep(0.001)

def stage_home():
    print("move to home")
    stage.move_to(0)
    stage.wait_move()

def wait_and_sleep():
    stage.wait_move()                                           
    time.sleep(1) 

async def wait_async():


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

# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    trigger_positions = []

    stage.setup_kcube_trigio(trig1_mode='in_gpio', trig1_pol=True, trig2_mode='in_gpio', trig2_pol=True)
    trig2_state_after = False 

    stage_home()
    wait_for_init_trigger()

    if (return_trig1_state()):
        print("inside if")
        duration = []
        start_time = datetime.now()
        time.sleep(1)

        for i in range(2):
             # forward movement           
            stage.move_to(50*scale_pos)

            while stage.is_moving():                                    # check ob das funktionier
                if (get_rising_edge_trig2()):                              # hier abfrage ob rising edge

                    position = stage.get_position()/scale_pos
                    print(position)
                    trigger_positions.append([datetime.now()-start_time, "fw", position])   
                #time.sleep(0.07)

            t1 = time.time()
            while(time.time()-t1 <= 2):
                                                                           
            # backward movement
            stage.move_to(0*scale_pos)

            while stage.is_moving():
                if (get_rising_edge_trig2()):
                    position = stage.get_position()/scale_pos
                    print(position)
                    trigger_positions.append([datetime.now()-start_time, "bw", position])
                #time.sleep(0.07)

            wait_and_sleep()

    stage_home()

    with open("test_file.txt", "w") as output:
        output.write(str(trigger_positions))

    stage.close()

