/******************************************************************************
 *
 *  $Id$
 *
 *  Copyright (C) 2021 Veysi ADIN, UST KIST
 *
 *  This file is part of the IgH EtherCAT master userspace program in the ROS2 environment.
 *
 *  The IgH EtherCAT master userspace program in the ROS2 environment is free software; you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation; version 2
 *  of the License.
 *
 *  The IgH EtherCAT master userspace program in the ROS2 environment is distributed in the hope that
 *  it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the IgH EtherCAT master userspace program in the ROS environment. If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 *  Contact information: veysi.adin@kist.re.kr
 *****************************************************************************/
/*****************************************************************************
 * \file  ecat_definitions.hpp
 * \brief Header file for all include statements and global variables for EtherCAT
 *        communication.
 * 
 * This header file contains required include statements for IgH EtherCAT library,
 * global variables (e.g. ethercat master,master_state, domain,domain_state), 
 * structs for PDO offset and recieved data from slaves,
 * Communication period and number of slaves can be specified in here.
 *******************************************************************************/

#pragma once

#include <iostream>
#include <cstring>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <malloc.h>
#include <sched.h>
#include <chrono>
#include <memory>

/****************************************************************************/
/// IgH EtherCAT library header file the user-space real-time interface library.
/// IgH, EtherCAT related functions and data types.
#include "ecrt.h"  
     
/// Object dictionary paramaters PDO index and default values in here.
#include "object_dictionary.hpp" 
const uint32_t           g_kNsPerSec = 1000000000;     /// Nanoseconds per second.
/****************************************************************************/
/// Global variable declarations, definitions are in @file ethercat_node.cpp
static volatile sig_atomic_t sig = 1;
extern ec_master_t        * g_master ;  /// EtherCAT master
extern ec_master_state_t    g_master_state ; /// EtherCAT master state

extern ec_domain_t       * g_master_domain ; /// Ethercat data passing master domain
extern ec_domain_state_t   g_master_domain_state ;   /// EtherCAT master domain state

extern struct timespec      g_sync_timer ;                       /// timer for DC sync .
extern uint32_t             g_sync_ref_counter;                  /// To sync every cycle.
/****************************************************************************/
#define TEST_BIT(NUM,N)    ((NUM &  (1 << N))>>N)  /// Check specific bit in the data. 0 or 1.
#define SET_BIT(NUM,N)      (NUM |  (1 << N))  /// Set(1) specific bit in the data.
#define RESET_BIT(NUM,N)    (NUM & ~(1 << N))  /// Reset(0) specific bit in the data
/// Convert timespec struct to nanoseconds */ 
#define TIMESPEC2NS(T)      ((uint64_t) (T).tv_sec * g_kNsPerSec + (T).tv_nsec) 
#define DIFF_NS(A, B) (((B).tv_sec - (A).tv_sec) * g_kNsPerSec + (B).tv_nsec - (A).tv_nsec)
/// Using Monotonic system-wide clock.  */
#define CLOCK_TO_USE        CLOCK_MONOTONIC  

/**
 * @brief Add two timespec struct.
 * 
 * @param time1 Timespec struct 1
 * @param time2 Timespec struct 2
 * @return Addition result
 */

inline struct timespec timespec_add(struct timespec time1, struct timespec time2)
{
    struct timespec result;

    if ((time1.tv_nsec + time2.tv_nsec) >= g_kNsPerSec)
    {
        result.tv_sec = time1.tv_sec + time2.tv_sec + 1;
        result.tv_nsec = time1.tv_nsec + time2.tv_nsec - g_kNsPerSec;
    }
    else
    {
        result.tv_sec = time1.tv_sec + time2.tv_sec;
        result.tv_nsec = time1.tv_nsec + time2.tv_nsec;
    }

    return result;
}
/// Xbox Controller inputs
typedef struct
{
    float left_x_axis_;
    float left_y_axis_;
    float right_x_axis_;
    float right_y_axis_;
    uint8_t blue_button_;
    uint8_t green_button_;
    uint8_t red_button_;
    uint8_t yellow_button_;
    uint8_t left_r_button_;
    uint8_t left_l_button_;
    uint8_t left_u_button_;
    uint8_t left_d_button_ ;
    uint8_t left_rb_button_ ;
    uint8_t right_rb_button_ ;
    uint8_t left_start_button_ ;
    uint8_t right_start_button_ ; 
    uint8_t xbox_button_;
} Controller;


 /// Motor operation modes
typedef enum
{
    kProfilePosition = 1,
    kProfileVelocity = 3,
    kProfileTorque   = 4,
    kHoming = 6,
    kInterpolatedPosition = 7,
    kCSPosition = 8,
    kCSVelocity = 9,
    kCSTorque = 10,
} OpMode ;

/// constant declarations for lifecycle node
static constexpr uint8_t PRIMARY_STATE_UNKNOWN =
0u;
static constexpr uint8_t PRIMARY_STATE_UNCONFIGURED =
1u;
static constexpr uint8_t PRIMARY_STATE_INACTIVE =
2u;
static constexpr uint8_t PRIMARY_STATE_ACTIVE =
3u;
static constexpr uint8_t PRIMARY_STATE_FINALIZED =
4u;
static constexpr uint8_t TRANSITION_STATE_CONFIGURING =
10u;
static constexpr uint8_t TRANSITION_STATE_CLEANINGUP =
11u;
static constexpr uint8_t TRANSITION_STATE_SHUTTINGDOWN =
12u;
static constexpr uint8_t TRANSITION_STATE_ACTIVATING =
13u;
static constexpr uint8_t TRANSITION_STATE_DEACTIVATING =
14u;
static constexpr uint8_t TRANSITION_STATE_ERRORPROCESSING =
15u;
/// CIA 402 state machine motor states
enum MotorStates{
	kReadyToSwitchOn = 1,
	kSwitchedOn,
	kOperationEnabled,
	kFault,
	kVoltageEnabled,
	kQuickStop,
	kSwitchOnDisabled,
	kWarning,
	kRemote,
	kTargetReached,
	kInternalLimitActivate
};

enum ErrorRegisterBits{
    kGenericError = 0,
    kCurrentError,
    kVoltageError,
    kTemperatureError,
    kCommunicationError,
    kDeviceProfileSpecificError,
    kReserved,
    kMotionError
};
/// Sensor Configuration for motor for more information \see EPOS4-Firmware-Specification Pg.138
enum SensorConfig{
    kSensor1TypeNone=0,
    kSensor1TypeDigitalIncrementalEncoder1=1,
    kSensor2TypeNone=0, 
    kSensor2TypeDigitalIncrementalEncoder2=256,
    kSensor2TypeAnalogIncrementalEncoderSinCos=512,
    kSensor2TypeSSIAbsoluteEncoder=768,
    kSensor3TypeNone=0,
    kSensor3TypeDigitalHallSensor=131072  //EC motors only 

};
/// Control structure configuration for control mechanism to select sensor structure specific to hardware. \see EPOS4-Firmware-Specification pg. 140
enum ControlStructureBits{
    /// These are bit locations not values for values  \see EPOS4-Firmware-Specification pg. 140 !!!
    kCurrentControlStructure  = 0,    // 0-3 , 4 bits. Val : 1 - PI current controller
    kVelocityControlStructure = 4,   // 4-7,   4bits.  Val : 0 - None | 1 - PI Vecolity controller (low pass filter) | 2 - PI velocity controller (observer) 
    kPositionControlStructure = 8,   // 8-11 , 4bits.  Val : 0 - None | 1 - PID position controller
    kGearLocation             = 12,  // 1 bit          Val : 0 - None | 1 - Gear Mounted on system     
    kProcessValueReference    = 14,  // 14-15 2 bits.  Val : 0 - On motor (or undefined) | 1 - On gear  
    kMainSensor               = 16,  // 16-19 4 bits.  Val : 0 - None | 1 - Sensor 1  | 2 - Sensor 2 | 3 - Sensor 3
    kAuxiliarySensor          = 20,  // 20-23 4 bits.  Val : 0 - None | 1 - Sensor 1  | 2 - Sensor 2 | 3 - Sensor 3
    kMountingPositionSensor1  = 24,  // 24-25 2 bits.  Val : 0 - On motor (or undefined) | 1 - On gear 
    kMountingPositionSensor2  = 26,  // 26-27 2 bits.  Val : 0 - On motor (or undefined) | 1 - On gear 
    kMountingPositionSensor3  = 28,  // 28-29 2 bits.  Val : 0 - On motor 
};
/// Offset for PDO entries to register PDOs.
typedef struct
{
    uint32_t target_pos ;
    uint32_t target_vel ;
    uint32_t target_tor ;
    uint32_t max_tor  ;
    uint32_t control_word ;
    uint32_t op_mode ;
    uint32_t profile_acc ;
    uint32_t profile_dec ;
    uint32_t quick_stop_dec ;
    uint32_t profile_vel ;

    uint32_t actual_pos ;
    uint32_t pos_fol_err ;
    uint32_t actual_vel ;
    uint32_t actual_cur ;
    uint32_t actual_tor ;
    uint32_t status_word ;
    uint32_t op_mode_display ;
    uint32_t error_code ;
    uint32_t extra_status_reg ;
    uint32_t torque_offset;

    uint32_t r_limit_switch;
    uint32_t l_limit_switch;
    uint32_t emergency_switch;
} OffsetPDO ;


/// Received feedback data from slaves
typedef struct
{
    int32_t   target_pos ;
    int32_t   target_vel ;
    int16_t   target_tor ;
    int16_t   max_tor ;
    uint16_t  control_word ;
    OpMode    op_mode ;
    int32_t   vel_offset ;
    int16_t   tor_offset ;

    int32_t  actual_pos ;
    int32_t  actual_vel ;
    int16_t  actual_cur ;
    int16_t  actual_tor ;
    uint16_t status_word ;
    int8_t   op_mode_display ;
    uint8_t  left_limit_switch_val ;
    uint8_t  right_limit_switch_val ;
    uint8_t  s_emergency_switch_val;
}ReceivedData;

/// CKim - SDO_data Structure holding all data needed to send/receive an SDO object.
typedef struct {
    uint16_t slave_position;    // Position based addressing.
    uint16_t index;		        // Index in Object dictionary
    uint8_t  sub_index;	        // Subindex in Object dictionary
    uint32_t data ;             // Actual data to sent/receive
    size_t   data_sz;	        // Size
    size_t   result_sz;         // Resulted data size
    uint32_t err_code;	        // Error code
} SDO_data;

/// EtherCAT SDO request structure for configuration phase.
typedef struct 
{
    ec_sdo_request_t  * com_status;
    ec_sdo_request_t  * target_pos ;
    ec_sdo_request_t  * target_vel ;
    ec_sdo_request_t  * target_tor ;
    ec_sdo_request_t  * max_tor ;
    ec_sdo_request_t  * control_word ;
    ec_sdo_request_t  * op_mode ;
    ec_sdo_request_t  * vel_offset ;
    ec_sdo_request_t  * tor_offset ;

    ec_sdo_request_t  * actual_pos ;
    ec_sdo_request_t  * actual_vel ;
    ec_sdo_request_t  * actual_cur ;
    ec_sdo_request_t  * actual_tor ;
    ec_sdo_request_t  * status_word ;
    ec_sdo_request_t  * op_mode_display ;
    ec_sdo_request_t  * left_limit_switch_val ;
    ec_sdo_request_t  * right_limit_switch_val ;
    ec_sdo_request_t  * emergency_switch_val ;
}SdoRequest;

/// Parameters that should be specified in position mode.
typedef struct 
{
    uint32_t profile_vel ;
    uint32_t profile_acc ;
    uint32_t profile_dec ;
    uint32_t max_fol_err ;
    uint32_t max_profile_vel ; 
    uint32_t quick_stop_dec ;
    uint16_t motion_profile_type ; 

} ProfilePosParam ;
/**
 * @brief Struct contains configuration parameters for cyclic sync. position mode.
 * 
 */
typedef struct 
{
    uint32_t nominal_current ;
    uint16_t torque_constant ;
    uint32_t current_controller_gain ;
    uint32_t position_control_parameter_set ;
    uint32_t software_position_limit ; 
    uint16_t motor_rated_torque ;
    uint32_t max_gear_input_speed ; 
    uint32_t profile_vel ;
    uint32_t profile_acc ;
    uint32_t profile_dec ;
    uint32_t max_fol_err ;
    uint32_t max_profile_vel ; 
    uint32_t quick_stop_dec ;
    uint32_t interpolation_time_period ;
} CSPositionModeParam ;
/**
 * @brief Struct containing 'velocity control parameter set' 0x30A2
 * Has 4 sub index. Default values are from EPOS4 firmware manual
 * 
 */
 typedef struct
 {
    uint32_t Pgain = 20000;     // micro amp sec per radian
    uint32_t Igain = 500000;    // micro amp per radian
    uint32_t FFVelgain = 0;
    uint32_t FFAccgain = 0;
 } VelControlParam;
/**
 * @brief Struct contains configuration parameters for cyclic sync. velocity mode.
 * 
 */
typedef struct 
{
    VelControlParam velocity_controller_gain ;
    uint32_t quick_stop_dec ;
    uint32_t profile_dec ;
    uint32_t software_position_limit ; 
    uint32_t interpolation_time_period ;
} CSVelocityModeParam ;
 /**
 * @brief Struct contains configuration parameters for cyclic sync. torque mode.
 * 
 */
typedef struct 
{
    uint32_t nominal_current ;
    uint16_t torque_constant ;
    uint32_t max_motor_speed ; 
    uint32_t max_gear_input_speed ;
    uint32_t current_controller_p_gain ;
    uint32_t current_controller_i_gain ; 
    uint32_t quick_stop_dec ;
    uint32_t profile_dec ;
    uint16_t motor_rated_torque ;
    uint32_t software_position_limit ; 

} CSTorqueModeParam ;

/// Parameters that should be specified in homing mode.
typedef struct
{
	uint32_t	max_fol_err;
	uint32_t	max_profile_vel;
	uint32_t	quick_stop_dec;
	uint32_t	speed_for_switch_search;
	uint32_t	speed_for_zero_search;
	uint32_t	homing_acc;
    // Used when homing by touching mechanical limit and sensing current
	uint16_t	curr_threshold_homing;
    // Amount to move away from the sensed limit	
	int32_t		home_offset;
	int8_t		homing_method;
} HomingParam;

/// Parameters that should be specified in velocity mode.
typedef struct
{
    uint32_t	max_profile_vel;
    uint32_t	quick_stop_dec;
    uint32_t	profile_acc;
    uint32_t	profile_dec;
    uint16_t    motion_profile_type;
} ProfileVelocityParam ;