#include "master.h" // include the EtherCat Master library
#include <ctime> 
#include <cerrno>

//added Linux sleep function for ICP&B 
void my_sleep(unsigned msec) {
	//STEP7: create sleep msec timer 
}

int main(int argc, char* argv[])
{
    
	//STEP1: select the proper EtherCAT network interface 
	
   	//STEP2: instantiate the EtherCAT master object (with proper network interface & cycle time)

	//STEP3: check EtherCAT connection to EtherCAT slaves (and check OPERATIONAL state)
   	if (1==1) { //NOTE: change me!!!!

    	//STEP4: detect & determine the amount of connected slaves and RESET all drives

	//STEP5: enable the first slave (0 = EtherCAT master) && power the drive (OPERATIONAL state && powerstage enable)
	
  	//STEP6: perform homing

	//STEP7: assignment MOTION PROFILE
	/*
	//MOTION PROFILE: assignment
	//
	//perform motion profile by using a separate C++ function (use a separate function or C++ class for this!)
	//NOTE: my_sleep (msec) function needs to be created 
	*/

	//STEP8: disable the first slave (0 = EtherCAT master) && power down the drive (powerstage disable)

        return EXIT_SUCCESS; // exit the program
    }
    else
    {
        return EXIT_FAILURE; // exit the program
    }
}
