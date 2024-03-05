from pylablib.devices import Thorlabs 
import numpy as np  
import time

# connect to the devices
with Thorlabs.KinesisMotor("27267730") as stage:
    scale_pos = 34554.97192
    trigger_positions = []
    #stage.setup_kcube_trigio(trig1_mode='in_gpio', trig1_pol=False , trig2_mode='out_gpio', trig2_pol=False)


    print(stage.get_kcube_trigpos_parameters())
    print(stage.get_kcube_trigio_parameters())
    print(stage.get_status())

    stage.setup_kcube_trigio(trig1_mode="in_gpio", trig2_mode="in_gpio")





    # initial trigger from Arduino to start experiment series
    #while(True):
   #     if stage.get_kcube_trigio_parameters().trig1_pol == True:
    #        print(stage.get_kcube_trigpos_parameters())
    #        print(stage.get_kcube_trigio_parameters())
    #        print(True)
    while(True):
        #print(stage.get_kcube_trigio_parameters().trig1_pol)
        status = stage.get_status()
        print(status)
        #print(status)
        trig1_state ="digio1" in status
        trig2_state ="digio2" in status
        print(trig1_state)
        print(trig2_state)
        time.sleep(1)
            #print(stage.get_status())

    

 

    stage.close()








