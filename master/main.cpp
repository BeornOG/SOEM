#include "master.h" // include the EtherCat EthCat library
#include <ctime> 
#include <cerrno>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector> 
#include <string>

using namespace std;



//added Linux sleep function for ICP&B 
void delay(unsigned msec) {
	//STEP7: create sleep msec timer 
	std::this_thread::sleep_for(std::chrono::milliseconds(msec)); //lang leve stackoverflow antwoorden
}
vector<int> openmotionprofile() {
	string fname;
	cout << "Enter the motion profile file name: ";
	cin >> fname;
	ifstream MotionProfileFile(fname);
	vector<int> FileData;
	string myText;
	while (getline (MotionProfileFile, myText)) {
  		// Output the text from the file
  		cout << myText;
		int myNumber = stoi(myText);
		FileData.push_back(myNumber);
	}
	return FileData;
}

/*int executemotionprofile(vector<int> MotionData, Master Ethcat) {
		int i = 0;
	// extern Master EthCat;
	while(i < MotionData.size()-1) {
		Ethcat.movePosition(1, MotionData[i], false);
		cout << "moving to: " << MotionData[i] << "\n";
		delay(5000);
		int ErrorCode = Ethcat.getError(1);
		if (ErrorCode =! 0) {
			return ErrorCode;
		}
	}
	return 0;
} */

int main(int argc, char* argv[])
{    
	//STEP1: select the proper EtherCAT network interface 
	
	
   	//STEP2: instantiate the EtherCAT Master object (with proper network interface & cycle time)
	Master EthCat("eth0", 2000, true);
	
	//STEP3: check EtherCAT connection to EtherCAT slaves (and check OPERATIONAL state)
   	if (EthCat.connected()) { 
		   for (int i = 0; i < 65535; ++i){
			   //STEP4: detect & determine the amount of connected slaves and RESET all drives
			   EthCat.reset(i);
		   }
		//STEP5: enable the first slave (0 = EtherCAT EthCat) && power the drive (OPERATIONAL state && powerstage enable)
		EthCat.enable(1);
		//STEP6: perform homing
		EthCat.home(1, false);
		//STEP7: assignment MOTION PROFILE

		/*
		//MOTION PROFILE: assignment
		//
		//perform motion profile by using a separate C++ function (use a separate function or C++ class for this!)
		//NOTE: my_sleep (msec) function needs to be created 
		*/
		vector<int> MotionProfileData = openmotionprofile();

		// int MovementReturnCode = executemotionprofile(MotionProfileData, EthCat);
			int i = 0;
	// extern Master EthCat;
	int ErrorCode
	while(i < MotionData.size()-1) {
		Ethcat.movePosition(1, MotionData[i], false);
		cout << "moving to: " << MotionData[i] << "\n";
		delay(5000);
		ErrorCode = Ethcat.getError(1);
		if (ErrorCode =! 0) { break;
		}
	}
		
		//STEP8: disable the first slave (0 = EtherCAT EthCat) && power down the drive (powerstage disable)

		EthCat.disable(1);

		if (ErrorCode =! 0) {
			return EXIT_FAILURE;
		}
        return EXIT_SUCCESS; // exit the program
    }
    else
    {
        return EXIT_FAILURE; // exit the program
    }
}

