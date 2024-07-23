This repository contains the code for the Laser Safety Mechanism implemented into the optoacoustic sensing probe. The Arduino implementation is included in the folder **Arduino**, the experiment control using the Thorlabs linear translation stage MTS50-Z8 and KDC101 stage controller is given, as well as the data analysis.

1) Arduino sensor data acquisition and decision making
   - _Arduino/control_arduino_all_sensors_kdc101/control_arduino_all_sensors_kdc101.ino_: Arduino implementation for Accelerometer MPU6050, Phototransistor BPY62 (interchangeable), Reed Switches (interchangeable). Execute with Arduino IDE, set baudrate in Arduino IDE to 115200. Frequency variable ```freq``` should be one of the values defined in the comments. 
   - Schematic for this script and Arduino implementation can be found in _Arduino/Schematic.pdf_
2) Stage control and data acquisition from Arduino and stage with host PC
   - _merge/merge_experiments.py_: Controls the stage, gathers data from the Arduino with the sensors mounted onto the stage. The data is output into a text file. Execute this script with
     ```
     python merge_experiments.py <rounds> <file_index>
     ```
     where ```rounds``` is an integer number that denotes the amount of cycles the stage moves up and down and ```file_index``` is a distinct naming for the file that is chosen by the user. If chosen twice, data can be overwritten. ```start_pos``` and ```end_pos``` need to be selected by manually adjusting the stage to start and end position of each round, given in milimeters.
   - _merge/merge_handheld.py_ This script gathers data from the sensors in the probe and stores them in a *.txt file. It is executed with
     ```
     python merge_handheld.py <duration> <file_index>
     ```
     where ```duration``` is the time of data acquisition in seconds. 
3) Data analysis with Jupyter notebooks 
   - _merge/merge_plots_all.ipynb_ the first two blocks analyse the hand held data from the accelerometer, phototransistor and reed switches. The third code block analyses the stage data from the integrated probe (reed switches and phototransistor)
   - _merge/merge_compare_thresh.ipynb_ compared different threshold and derivative values for one data set of phototransistor data 
