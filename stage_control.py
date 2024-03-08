from pylablib.devices import Thorlabs 
import numpy as np  
from datetime import datetime
import time


def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state

# home stage and wait while doing so
def stage_home():
    print("move to home")
    stage.move_to(25*scale_pos)
    stage.wait_move()

# get continuous position upon rising edge while stage is waiting
def wait_async(timeout, direction):
    t1 = time.time()
    while(time.time()-t1 <= timeout):
        #await asyncio.sleep(0.001)
        if get_rising_edge_trig2():
            get_position_stage(direction)

# get position and append to trigger_positions
def get_position_stage(direction):
    global start_time
    position = stage.get_position()/scale_pos
    print(position)
    trigger_positions.append([datetime.now()-start_time, direction, position])
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

# obsolet? 
def wait_for_init_edge(): 
    print(get_rising_edge_trig2())
    while True:
        state = return_trig2_state()
        #print(state)
        if state:
            return True
            
# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192

    start_mm = 35.4*scale_pos
    end_mm = 45.4*scale_pos

    trigger_positions = []

    stage.setup_kcube_trigio(trig2_mode='in_gpio', trig2_pol=True)
    trig2_state_after = False 

    stage_home()

    time.sleep(2)

    wait_for_init_edge()
    print("Start")
    start_time = datetime.now()

    stage.move_to(start_mm)
    while stage.is_moving():  
            #print(get_rising_edge_trig2())                                  # check ob das funktionier
            if (get_rising_edge_trig2()):                              # hier abfrage ob rising edge
                trigger_positions = get_position_stage("fw") 
    
    wait_async(5, "at end")

    for i in range(3):
         # forward movement           
        stage.move_to(start_mm)
        while stage.is_moving():  
            #print(get_rising_edge_trig2())                                  # check ob das funktionier
            if (get_rising_edge_trig2()):                              # hier abfrage ob rising edge
                trigger_positions = get_position_stage("fw") 

        #print("wait at end")
        wait_async(5, "at end")
                                                       
        # backward movement
        stage.move_to(end_mm)
        while stage.is_moving():
            if (get_rising_edge_trig2()):
                trigger_positions = get_position_stage("bw")

        wait_async(2, "at start")
        #print("wait at start")


    # hier wäre es nice ein trig1 out zu senden aka END
        
    stage_home()

    print("End")

    with open("data_stage/stage_data_{sys.argv[1]}.txt", "w") as output:
        output.write(str(trigger_positions))

    stage.close()

