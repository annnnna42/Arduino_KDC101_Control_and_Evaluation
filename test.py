#script a
import os
import time

def do_something():
    print("Script A is doing something...")

def monitor_script_b():
    while True:
        if os.path.exists("script_b_running.txt"):
            print("Script B is running...")
        else:
            print("Script B is not running. Doing something else...")
            # Do something else when Script B stops
            break  # Exit the loop when Script B stops
        
        time.sleep(1)  # Add a short delay between checks

if __name__ == "__main__":
    monitor_script_b()
    print("Stop")
    time.sleep(1)
    print("end")