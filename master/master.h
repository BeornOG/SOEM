#define DEBUG

#pragma once
#include<iostream>
#ifndef INTMAX_MAX
    #define INTMAX_MAX MAXINT64
#endif // needed for thread on some platforms
#include"ethercat.h"
#include<thread>
#include<mutex>
#include<string.h>
constexpr int EC_TIMEOUTMON = 500;

class Master
{
    /* definition of CMMT_ST control_bits */
    typedef enum {
        control_switch_on = 0,
        control_enable_voltage = 1,
        control_quick_stop = 2,
        control_enable_operation = 3,
        control_fault_reset = 7,
        control_4 = 4,
        control_5 = 5,
        control_6 = 6,
        control_halt = 8,
        control_9 = 9,
    }control_bit_t;

    /* definition of CMMT_ST status_bits */
    typedef enum {
        status_ready_to_switch_on = 0,
        status_switched_on = 1,
        status_operation_enabled = 2,
        status_fault = 3,
        status_voltage_enabled = 4,
        status_quick_stop = 5,
        status_switch_on_disabled = 6,
        status_warning = 7,
        status_manfsp = 8,
        status_remote = 9,
        status_mc = 10, // motion complete / target reached // not jogging in jogging mode
        status_ack_start = 12,
        status_ref_reached = 12,
        status_ref = 15, //drive homed
    }statusword_bit_t;

    /* definition of CMMT_ST EtherCAT PDO process data */
    typedef enum {
        Controlword = 0,
        Statusword = 0,
        Mode_of_Operation = 2,
        Mode_of_Operation_Display = 2,
        Target_Position = 3,
        Position_Actual_Value = 3,
        Profile_velocity = 7,
        Target_velocity = 11,
        Velocity_Actual_Value = 11,
        Target_Torque = 15,
        Torque_Actual_Value = 15,
        Velocity_Offset = 17,
        Torque_Offset = 21,
    }mapped_PDO_t;

    /* definition of CMMT_ST EtherCAT Operation Mode */
    typedef enum {
        no_mode = 0,
        profile_position_mode = 1,
        velocity_mode = 2,
        profile_velocity_mode = 3,
        profile_torque_mode = 4,

        homing_mode = 6,
        interpolated_position_mode = 7,
        cyclic_sync_pos_mode = 8,
        cyclic_sync_vel_mode = 9,
        cyclic_sync_tor_mode = 10,
        
        jog_mode = 253, // uint8 = 253 || int8 = -3
    }Mode_of_Operation_t;

    /* PUBLIC functions: do only use these C++ functions (=API) */
public:
    
    /* constructor / destructor functions */
    /* 
        default constructor (with predetermined values, can be overwritten during instantiaton)

        char ifname[]: interface NIC name / Ethernet controller
        const uint32_t: cycle time (EtherCAT processdata cycle time)
        bool showNonError: extra debug information
    */
    
	Master(char ifname[] = "eth0", const uint32_t cycletime = 2000, bool showNonErrors = true);

    /* 
        default deconstructor        
    */
    
	~Master();

    
    /* status functions */
    /* 
        status function: get statusWord (ErrorCode)

        int slaveNr: indicate EtherCAT slave number / address
    */
        
    int getError(int slaveNr);

    /* 
        status function: get procesdata with defined offset

        int slaveNr: indicate EtherCAT slave number / address
        uint8_t byte: offset within EtherCAT processdata
    */

    int16_t get16(int slaveNr, uint8_t byte);

    /* 
        status function: get current postition in predefined units (Festo Automation Suite)

        int slaveNr: indicate EtherCAT slave number / address        
    */

    int32_t getPos(int slaveNr);

    /* 
        status function: get EtherCAT connection state (connected == true, not connected == false)

        int slaveNr: indicate EtherCAT slave number / address        
    */

    bool connected(); // Check if drives are ready to use (in operation mode)

	
    /* control functions: perform actions! */
    /* 
        control function: enable the CMMT-ST drive (set in EtherCAT OPERATIONAL state)

        int slaveNr: indicate EtherCAT slave number / address        

        return: EtherCAT status word/definition
    */

    int enable(int slaveNr);

    /* 
        control function: disable the CMMT-ST drive (set in EtherCAT SAFEOP state)

        int slaveNr: indicate EtherCAT slave number / address        

        return: EtherCAT status word/definition
    */

    int disable(int slaveNr);

    /* 
        control function: perform homing function on the CMMT-ST drive (function defined in Festo Automation Suite)

        int slaveNr: indicate EtherCAT slave number / address        

        return: status word/definition (-1 == failure, 0 == success)
    */

    int home(int slaveNr,bool always = false);

    /* 
        control function: perform jog function in positive direction on the CMMT-ST drive (orientation jog function defined in FAS)

        int slaveNr: indicate EtherCAT slave number / address        

    */

    void jogPos(int slaveNr);

    /* 
        control function: perform jog function in negative direction on the CMMT-ST drive (orientation jog function defined in FAS)

        int slaveNr: indicate EtherCAT slave number / address        

    */

    void jogNeg(int slaveNr);

    /* 
        control function: stop jog function in negative direction on the CMMT-ST drive 

        int slaveNr: indicate EtherCAT slave number / address        

    */

    void jogStop(int slaveNr);

    /* 
        control function: perform default absolute /relative move function on the CMMT-ST drive

        int slaveNr: indicate EtherCAT slave number / address        
        int32_t target: target position (in units defined in FAS)
        bool relative: relative and/or absolute movement

        return: EtherCAT status word/definition
    */

    int movePosition(int slaveNr, int32_t target, bool relative = false);
    
    /* 
        control function: perform absolute /relative move function with specified velocity on the CMMT-ST drive

        int slaveNr: indicate EtherCAT slave number / address        
        int32_t target: target position (in units defined in FAS)
        int32_t velocity: velocity (in units defined in FAS)
        bool relative: relative and/or absolute movement

        return: EtherCAT status word/definition
    */

    int movePosition(int slaveNr, int32_t target, int32_t velocity, bool relative = false);

    /* 
        control function: perform absolute /relative move function with specified velocity on the CMMT-ST drive

        int slaveNr: indicate EtherCAT slave number / address        
        int32_t target: target position (in units defined in FAS)
        uint32_t velocity: velocity (in units defined in FAS)
        bool relative: relative and/or absolute movement

        return: EtherCAT status word/definition
    */

    int movePosition(int slaveNr, int32_t target, uint32_t velocity, bool relative = false);

    /* 
        control function: perform absolute /relative move function with specified velocity & accelaration on the CMMT-ST drive

        int slaveNr: indicate EtherCAT slave number / address        
        int32_t target: target position (in units defined in FAS)
        uint32_t velocity: velocity (in units defined in FAS)
        uint32_t acceleration: acceleration (in units defined in FAS)
        bool relative: relative and/or absolute movement

        return: EtherCAT status word/definition
    */

    int movePosition(int slaveNr, int32_t target, uint32_t velocity, uint32_t acceleration, uint32_t deceleration, bool relative = false);
    
    /* 
        control function: perform reset on the CMMT-ST drive (set in EtherCAT INIT state)

        int slaveNr: indicate EtherCAT slave number / address        

        return: EtherCAT status word/definition
    */

    int reset(int slaveNr);

    /* 
        control function: perform wait for the EtherCAT cycle time

    */

    void waitCycle(); // Wait for the cycle time

    

    /* PRIVATE functions: do not use private C++ functions (ever) */
    /* EtherCAT: low level functions */

private:
    uint32_t ctime; // Store the cycle time in microseconds
    std::mutex m; // prevent acces to EC data at the same time
    
    char IOmap[4096];
    volatile int wkc;

	bool inOP;
    bool verbose; // Output to screen
	uint8_t currentgroup = 0;

    bool readyState(int slaveNr);

    // Data handling
    uint8_t setBit(int slaveNr, uint8_t bit, uint8_t byte = Controlword);
    uint8_t unsetBit(int slaveNr, uint8_t bit, uint8_t byte = Controlword);
    uint16_t unsetControl(int slaveNr);
    bool getBit(int slaveNr, uint8_t bit, uint8_t byte = Statusword);
    void setByte(int slaveNr, uint8_t value, uint8_t byte = Controlword);
    void set16(int slaveNr, int16_t value, uint8_t byte);
    void setPos(int slaveNr, int32_t target, uint8_t byte = Target_Position);
    void setProfileVelocity(int slaveNr, uint32_t velocity, uint8_t byte = Profile_velocity);
    int  startup();
    void cycle(); // send and recieve data, wait cycletime 
    int  setMode(int slaveNr, uint8_t mode);

    // create PDO's
    int mapCia402(uint16_t slaveNr);
    
    // Ethercat state
    void setPreOp(int slaveNr);

    //Thread
    std::thread cycle_thread;
 };
