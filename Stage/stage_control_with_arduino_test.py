# from pylablib.devices import Thorlabs 
# import threading
# import time

# global trig1_state_before 
# global trig1_state_after 

# # Global variable to store the position of the stage
# stage_position = 0

# # trigger_positions = []

# def return_trig1_state():
#     status = stage.get_status()
#     trig1_state = "digio1" in status
#     return trig1_state


# # def get_position_on_trig1():
# #     # this needs to be inside multithreading
# #     trig1_state_before = return_trig1_state()
# #     if trig1_state_before:                                  # trig1 True
# #         if trig1_state_before != trig1_state_after:         # rising edge
# #             position = stage.get_position()/scale_pos       # get and append position
# #             trigger_positions.append(position)
# #             trig1_state_after = trig1_state_before          # set constant state
# #     else:                                                   # trig1 False
# #         if trig1_state_before != trig1_state_after:         # falling edge
# #             trig1_state_after = trig1_state_before          # set constant state
# # Function to monitor the input signal


# def monitor_signal():
#     global stage_position
    
#     while True:
#         # Simulate monitoring the input signal every 100ms
#         time.sleep(0.1)
        
#         trig1_state_before = return_trig1_state()
#         if trig1_state_before:                                  # trig1 True
#             if (trig1_state_before != trig1_state_after):         # rising edge
#                 signal_event.set()
#                 trig1_state_after = trig1_state_before          # set constant state
#         else:                                                   # trig1 False
#             if (trig1_state_before != trig1_state_after):         # falling edge
#                 trig1_state_after = trig1_state_before          # set constant state
#                 # Set the event to notify the main thread

# # Function to simulate moving the stage
# def move_stage():
#     global stage_position
    
#     while True:
#         # Your code to move the stage (replace this with your actual code)
#         print("Moving stage...")
#         time.sleep(1)  # Simulate moving the stage for 1 second
        
#         # Check if a notification from the signal monitoring thread is received
#         if signal_event.is_set():
#             # Handle the notification (e.g., save the position of the stage)
#             print("Signal high! Saving stage position:", stage_position)
#             position = stage.get_position()/scale_pos       # get and append position
#             trigger_positions.append(position)
#             # Clear the event
#             signal_event.clear()


# with Thorlabs.KinesisMotor("27267730") as stage:
#     scale_pos = 34554.97192
#     trigger_positions = []
#     stage.setup_kcube_trigio(trig1_mode='in_gpio', trig1_pol=True, trig2_mode='in_gpio', trig2_pol=True)

#     # Event to synchronize between threads
#     signal_event = threading.Event()

#     # Create and start the signal monitoring thread
#     signal_thread = threading.Thread(target=monitor_signal)
#     signal_thread.daemon = True  # Set as daemon so it will terminate when the main thread terminates
#     signal_thread.start()

#     # Execute the main code (moving the stage)
#     move_stage()



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
        #print(return_trig2_state())
        get_position_on_trig2()
        #time.sleep(0.001)


