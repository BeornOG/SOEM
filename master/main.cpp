#include "master.h" // include the EtherCat Master library
#include <ctime> 
#include <cerrno>
#include <chrono>
#include <thread>

//added Linux sleep function for ICP&B 
void delay(unsigned msec) {
	//STEP7: create sleep msec timer 
	std::this_thread::sleep_for(std::chrono::milliseconds(msec)); //lang leve stackoverflow antwoorden
}

int main(int argc, char* argv[])
{
    
	//STEP1: select the proper EtherCAT network interface 
	
   	//STEP2: instantiate the EtherCAT master object (with proper network interface & cycle time)
	Master.Master();
	//STEP3: check EtherCAT connection to EtherCAT slaves (and check OPERATIONAL state)
   	if (Master.connected) { 
		   for (int i = 0; i < 65535; ++i){
			   //STEP4: detect & determine the amount of connected slaves and RESET all drives
			   Master.reset(i);
		   }
		//STEP5: enable the first slave (0 = EtherCAT master) && power the drive (OPERATIONAL state && powerstage enable)
		Master.enable(1);
		//STEP6: perform homing
		Master.home(1, false)
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
