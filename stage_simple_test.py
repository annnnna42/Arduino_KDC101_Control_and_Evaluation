from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time

def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state
            
# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    trigger_positions = []

    stage.setup_kcube_trigio(trig1_mode = 'in_gpio', trig2_mode='in_gpio')
    trig2_state_after = False 

    print("Start run")
    while True:
        status = stage.get_status()
        print(status)
        time.sleep(0.7)
        trig2_state = "digio2" in status
        #print(trig2_state)
        #print(return_trig2_state())
    # stage.move_to(40*scale_pos)
    # while stage.is_moving():  
    #     #print(stage.get_position()/scale_pos)
    #     print(return_trig2_state())  
    #     time.sleep(0.1)

    # stage.wait_move()

    # print("Start Stage")

    #  # forward movement           
    # stage.move_to(20*scale_pos)
    # while stage.is_moving():  
    #     print(stage.get_position()/scale_pos)
    #     print(return_trig2_state())  
    #     time.sleep(0.1)


    print("End")

    stage.close()

