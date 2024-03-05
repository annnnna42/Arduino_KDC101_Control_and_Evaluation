from pylablib.devices import Thorlabs
import time

trig2_state_after = False 

def return_trig2_state():
    status = stage.get_status()
    trig2_state = "digio2" in status
    return trig2_state

def get_position_on_trig2():
    global trig2_state_after
    # this needs to be inside multithreading
    trig2_state_before = return_trig2_state()
    if trig2_state_before:                                  # trig1 True
        if trig2_state_before != trig2_state_after:         # rising edge
            position = stage.get_position()/scale_pos       # get and append position
            trigger_positions.append(position)
            print(position)
            trig2_state_after = trig2_state_before          # set constant state
    else:                                                   # trig1 False
        if trig2_state_before != trig2_state_after:         # falling edge
            trig2_state_after = trig2_state_before          # set constant state

with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    trigger_positions = []
    stage.setup_kcube_trigio(trig1_mode='in_gpio',trig2_mode='in_gpio')

    while True:
        get_position_on_trig2()


