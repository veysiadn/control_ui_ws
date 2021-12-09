#ifndef GUI_GLOBALS_HPP
#define GUI_GLOBALS_HPP

#define NUM_OF_SERVO_DRIVES 1
#define ID_CAMERA 0


#define TEST_BIT(NUM,N)    ((NUM &  (1 << N))>>N)  // Check specific bit in the data. 0 or 1.
#define SET_BIT(NUM,N)      (NUM |  (1 << N))  // Set(1) specific bit in the data.
#define RESET_BIT(NUM,N)    (NUM & ~(1 << N))  // Reset(0) specific bit in the data


    struct ReceivedData
    {
        int32_t   target_pos ;
        int32_t   target_vel ;
        int16_t   target_tor ;
        int16_t   max_tor ;
        uint16_t  control_word ;
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
        int32_t  right_x_axis;
        int32_t  left_x_axis;
        uint8_t  p_emergency_switch_val;
        uint8_t  com_status;
    };


    struct ControlUIButtonData{
        uint8_t b_init;
        uint8_t b_reinit;
        uint8_t b_enable_motor;
        uint8_t b_disable_motor;
        uint8_t b_enable_cyclic_pos;
        uint8_t b_enable_cyclic_vel;
        uint8_t b_enable_vel;
        uint8_t b_enable_pos;
        uint8_t b_enter_cyclic_pdo;
        uint8_t b_emergency_mode;
        uint8_t b_send;
    };

#endif // GUI_GLOBALS_HPP
