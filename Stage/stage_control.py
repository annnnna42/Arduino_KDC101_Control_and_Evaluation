from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time

# write functions!!! 

def return_trig1_state():
    status = stage.get_status()
    trig1_state = "digio1" in status
    return trig1_state

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

# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    trigger_positions = []

    stage.setup_kcube_trigio(trig1_mode='in_gpio', trig1_pol=True, trig2_mode='in_gpio', trig2_pol=True)

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
                if (return_trig1_state()):
                    position = stage.get_position()/scale_pos
                    print(position)
                    trigger_positions.append([datetime.now()-start_time, "fw", position])   
                time.sleep(0.07)

            wait_and_sleep()                                          
                                                           
            # backward movement
            stage.move_to(0*scale_pos)

            while stage.is_moving():
                if (return_trig1_state()):
                    position = stage.get_position()/scale_pos
                    print(position)
                    trigger_positions.append([datetime.now()-start_time, "bw", position])
                time.sleep(0.07)

            wait_and_sleep()

    stage_home()

    with open("test_file.txt", "w") as output:
        output.write(str(trigger_positions))

    stage.close()

