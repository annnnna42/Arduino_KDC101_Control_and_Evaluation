# # # thread1 - get_position, sleep(1)
# # # thread2 - move_to(0), sleep(0.6), move_to(20), sleep(0.6)

# from pylablib.devices import Thorlabs
# # import threading
# # import time

# # def print_position(device):
# #     while True:
# #         with device.lock:
# #             print("a")
# #         # position = stage.get_position()/scale_pos 
# #         # print(position)
# #             time.sleep(0.5)

# # def move_stage(device):
# #     while True:
# #         with device.lock:
# #             print("b")
# #         # stage.move_to(0)
# #         # stage.wait_move()
# #             time.sleep(1)
# #         # stage.move_to(50)
# #         # stage.wait_move()
# #         # time.sleep(1)


# # if __name__ == "__main__":
# #     with Thorlabs.KinesisMotor("27267730") as stage:
# #         scale_pos = 34554.97192

# #         stage.lock = threading.lock()

# #         position_thread = threading.Thread(target=print_position, args=(stage,))
# #         moving_thread = threading.Thread(target=move_stage, args=(stage,))

# #         position_thread.start()
# #         moving_thread.start()    
    
# #         position_thread.join()
# #         moving_thread.join()

# import threading
# import time
# import logging

# logging.basicConfig(level=logging.DEBUG,
#                     format='(%(threadName)-9s) %(message)s',)
                    
# def print_position(lock,stage):
#     logging.debug('Starting')
#     while True:
#         lock.acquire()
#         try:
#             logging.debug('Locking')
#             time.sleep(1.0)
#         finally:
#             logging.debug('Not locking')
#             lock.release()
#         #position = stage.get_position()
#         #print(position)
#         time.sleep(1.0)
#     return
                    
# def move_stage(lock,stage):
#     logging.debug('Starting')
#     num_tries = 0
#     num_acquires = 0
#     while num_acquires < 3:
#         time.sleep(0.5)
#         logging.debug('Trying to acquire')
#         acquired = lock.acquire(0)
#         try:
#             num_tries += 1
#             if acquired:
#                 logging.debug('Try #%d : Acquired',  num_tries)
#                 num_acquires += 1
#             else:
#                 logging.debug('Try #%d : Not acquired', num_tries)
#         finally:
#             if acquired:
#                 lock.release()
#     logging.debug('Done after %d tries', num_tries)

# if __name__ == '__main__':
#     with Thorlabs.KinesisMotor("27267730") as stage:

#         lock = threading.Lock()

#         locker = threading.Thread(target=print_position, args=(lock,stage,), name='Locker')
#         locker.setDaemon(True)
#         locker.start()

#         worker = threading.Thread(target=move_stage, args=(lock,stage,), name='Worker')
#         worker.start()


import threading
import time
from pylablib.devices import Thorlabs

# Define a lock for synchronizing access to the Thorlabs device
device_lock = threading.Lock()

def read_position_thread():
        # Acquire the lock before accessing the device
    while True:
    	with device_lock:        
        # Read the current position of the stage
            position = stage.get_position()
            print(f"Current position: {position/scale_pos}")

        # Release the lock after accessing the device
        
            #time.sleep(1)

def move_stage_thread():
        # Acquire the lock before accessing the device
    while True:
        with device_lock:
        
        
        # Move the stage to position 0
            #stage.move_to(0*scale_pos)
            
            stage.move_to(20*scale_pos)
            stage.wait_move()
            stage.move_to(10*scale_pos)
            stage.wait_move()

            print(f"Stage moved to position: {stage.get_position()/scale_pos}")
        # Release the lock after accessing the device



#if __name__ == "__main__":
    # Create threads for moving the stage and reading its position
    # Define a lock for synchronizing access to the Thorlabs device
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192


    move_thread = threading.Thread(target=move_stage_thread)
    read_thread = threading.Thread(target=read_position_thread)

    # Start both threads
    read_thread.start()
    move_thread.start()


    # Wait for both threads to finish
    move_thread.join()
    read_thread.join()
