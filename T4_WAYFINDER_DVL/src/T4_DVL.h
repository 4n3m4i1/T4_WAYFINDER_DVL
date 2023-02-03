#ifndef T4_DVL_H
#define T4_DVL_H
#include <stdint.h>
#include "DVL_CONSTANTS.h"

#ifndef DVLSERIAL
#define DVLSERIAL       Serial1 // Serial port on teensy hooked to MAX3232
#endif

#define DVLBAUD     115200

#define DVL_TX_BUF_LEN    256
#define DVL_RX_BUF_LEN    256


// Messages are expected, and send from DVL
//  in little-endian format
//
// This means: LSB is transmitted first!!
void init_DVL_serial();
void inline clear_DVL_RX();

//uint32_t updateVelocityTable(DVL_ &dvl_, DVL_VELOCITY &dest);

void inline serialSend(uint8_t val_2_send);
uint8_t serialRead();
void inline waitForSerial_TX_clear();

void send_float_over_serial(float val_);
uint16_t checksum_of_float(float val_);
float read_float_over_serial();

void send_uint16_over_serial(uint16_t val_2_send);
uint16_t read_uint16_over_serial();

void send_uint32_over_serial(uint32_t val_2_send);
uint32_t read_uint32_over_serial();

uint16_t chksum_of_string(const uint8_t *str_, uint8_t len);

void send_sop_dvl(const uint8_t *sop_);
void send_cmd_dvl(const uint8_t *cmd_);
void send_checksum_dvl(uint16_t chksum_);
void send_CMD_STRUC_ID(const uint8_t *str_);
bool read_and_match_sop_dvl(const uint8_t *sop_2_match);
bool read_and_match_RES_ID_dvl(const uint8_t *res_2_match);
bool read_and_match_PAYLOAD_HEADER(const uint8_t *pay_2_match);
bool read_and_match_CMD_STRUC_ID(const uint8_t *str_2_match);

void inline get_system_command_dvl();
int request_DVL_System_Config();
void print_DVL_System_Config();

void inline get_setup_command_dvl();
int request_DVL_Setup_Config();
void print_DVL_Setup_Config();

void inline set_setup_command_dvl();
int set_DVL_Setup_Config(float speed_of_sound_, float max_track_range, uint8_t baudrate, uint8_t soft_trig);


int DVL_DATA_UPDATE();
void print_current_vel();
void print_current_power();
void print_current_beam_range();
void print_current_BIT();

// General Use Struct
struct STATUS__{
    uint8_t major;
    uint8_t minor;
};

struct DVL_RTC{
    uint8_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
};

struct DVL_RTC_MS{
    uint8_t  year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint16_t millisecond;
};

struct DVL_FW{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
    uint8_t build;
};

struct DVL_VELOCITY{            // meters / sec
    float X;
    float Y;
    float Z;
    float error;
};

struct DVL_RANGE{               // meters
    float beam0;
    float beam1;
    float beam2;
    float beam3;
    float mean;
};

struct DVL_POWER{
    float input_voltage;
    float tx_voltage;
    float tx_current;
};

// Command/Response Structures
    // Get System Command
struct DVL_TX_GET_SYS_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_GET_SYS_TX_SOP;
    const uint8_t CMD_ID[CMD_ID_LEN] = DVL_GET_SYS_TX_CMD_ID;
    const uint16_t checksum = DVL_GET_SYS_TX_CHKSUM;
};
    // Get System Response
struct DVL_RX_GET_SYS_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_GET_SYS_RX_SOP;
    const uint8_t  RES_ID[RES_ID_LEN] = DVL_GET_SYS_RX_RES_ID;
    STATUS__ status;
    const uint8_t  payload_header[DVL_PAYLOAD_HEADER_LEN] = DVL_GET_SYS_RX_PAY_HEAD;
    float    frequency = DVL_PING_FREQ;
    uint32_t firmware_version;
    uint32_t FPGA_version;
    uint64_t unique_ID;
    uint8_t  XDCR_type;
    float    beam_angle;
    uint8_t  vertical_beam; // Bool
    uint8_t  RESERVED[101]; // 101 reserved bytes
    uint8_t  device_ID;     // Should be 76 for wayfinder
    uint8_t  subsys_type;   // Should be 0
    uint16_t checksum;
};

    // Get Setup Command
struct DVL_TX_GET_SET_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_GET_SET_TX_SOP;
    const uint8_t  CMD_ID[CMD_ID_LEN] = DVL_GET_SET_TX_CMD_ID;
    const uint16_t checksum = DVL_GET_SET_TX_CHKSUM;
};

    // Get Setup Response
struct DVL_RX_GET_SET_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_GET_SET_RX_SOP;
    const uint8_t  RES_ID[RES_ID_LEN] = DVL_GET_SET_RX_RES_ID;
    STATUS__ status;
    const uint8_t  payload_header[DVL_PAYLOAD_HEADER_LEN] = DVL_GET_SET_RX_PAY_HEAD;
    uint8_t  soft_trig;  // bool
    uint8_t  baud;
    float    speed_of_sound;   // m/s
    float    max_track_depth;  // m
    float    RESERVED;
    uint16_t checksum;
};

    // Set Setup Command
struct DVL_TX_SET_SET_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_SET_SET_TX_SOP;
    const uint8_t  CMD_ID[CMD_ID_LEN] = DVL_SET_SET_TX_CMD_ID;
    const uint8_t  CMD_STRUC_ID[DVL_CMD_STRUC_ID_LEN] = DVL_SET_SET_TX_STR_ID;
    uint8_t  soft_trig;  // bool
    uint8_t  baud = DVL_115200_BAUD;
    float    speed_of_sound = DVL_SOUND_SPEED_SALT;
    float    max_track_range = DVL_MAX_TRACK_RANGE;
    float    RESERVED;
    uint16_t checksum;
};

    // Set Setup Response
struct DVL_RX_SET_SET_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_SET_SET_RX_SOP;
    const uint8_t  RES_ID[RES_ID_LEN] = DVL_SET_SET_RX_RES_ID;
    STATUS__ status;
    uint16_t checksum;
};

    // Software Trigger Command
struct DVL_TX_SOFT_TRIG_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_SOFT_TRIG_TX_SOP;
    const uint8_t  CMD_ID[CMD_ID_LEN] = DVL_SOFT_TRIG_TX_CMD_ID;
    const uint16_t checksum = DVL_SOFT_TRIG_TX_CHKSUM;
};

    // Software Trigger Response
struct DVL_RX_SOFT_TRIG_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_SOFT_TRIG_RX_SOP;
    const uint8_t  RES_ID[RES_ID_LEN] = DVL_SOFT_TRIG_RX_RES_ID;
    STATUS__ status;
    uint16_t checksum;
};

    // Speed of Sound Command
struct DVL_TX_SOUND_SPEED_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_SOUND_SPEED_TX_SOP;
    const uint8_t CMD_ID[CMD_ID_LEN] = DVL_SOUND_SPEED_TX_CMD_ID;
    float    speed_of_sound;
    uint16_t checksum;
};

    // Speed of Sound Response
struct DVL_RX_SOUND_SPEED_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_SOUND_SPEED_RX_SOP;
    const uint8_t RES_ID[RES_ID_LEN] = DVL_SOUND_SPEED_RX_RES_ID;
    STATUS__ status;
    uint16_t checksum;
};

    // Get Time Command
struct DVL_TX_GET_TIME_STRUCT{
    const uint8_t  SOP[SOP_LEN] = DVL_GET_TIME_TX_SOP;
    const uint8_t  CMD_ID[CMD_ID_LEN] = DVL_GET_TIME_TX_CMD_ID;
    const uint16_t checksum = DVL_GET_TIME_TX_CHKSUM;
};

    // Get Time Response
struct DVL_RX_GET_TIME_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_GET_TIME_RX_SOP;
    const uint8_t RES_ID[RES_ID_LEN] = DVL_GET_TIME_RX_RES_ID;
    STATUS__ status;
    const uint8_t payload_header[DVL_PAYLOAD_HEADER_LEN] = DVL_GET_TIME_RX_PAY_HEAD;
    DVL_RTC  rtc;
    uint16_t checksum;
};

    // Set Time Command
struct DVL_TX_SET_TIME_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_SET_TIME_TX_SOP;
    const uint8_t CMD_ID[CMD_ID_LEN] = DVL_SET_TIME_TX_CMD_ID;
    const uint8_t CMD_STRUC_ID[DVL_CMD_STRUC_ID_LEN] = DVL_SET_SET_TX_STR_ID;
    DVL_RTC  rtc;
    uint16_t checksum;
};

    // Set Time Response
struct DVL_RX_SET_TIME_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_SET_TIME_RX_SOP;
    const uint8_t RES_ID[RES_ID_LEN] = DVL_SET_TIME_RX_RES_ID;
    STATUS__ status;
    uint16_t checksum;
};

    // Binary Data Stream
struct DVL_RX_BIN_DATA_STRUCT{
    const uint8_t SOP[SOP_LEN] = DVL_DATA_RX_SOP;
    const uint8_t DATA_ID[DVL_DATA_ID_LEN] = DVL_DATA_RX_DATA_ID;
    uint8_t  device_ID;     // Should be 76 for wayfinder
    uint8_t  subsys_type;   // Should be 0
    DVL_FW firmware;
    DVL_RTC_MS rtc;
    uint8_t coordinate_system;
    DVL_VELOCITY velocity;
    DVL_RANGE   range;
    float speed_of_sound;
    uint16_t BT_status;
    uint16_t BIT;           // Built in test
    DVL_POWER power;
    uint8_t system_serial[6];
    uint8_t RESERVED[20];       // 20 bytes reserved
    uint16_t checksum_data;
    uint16_t checksum;          // All non-checksum bytes
};


struct DVL_COMMANDS{
    DVL_TX_GET_SYS_STRUCT getSystem;
    DVL_TX_GET_SET_STRUCT getSetup;
    DVL_TX_SET_SET_STRUCT setSetup;
    DVL_TX_SOFT_TRIG_STRUCT softTrigger;
    DVL_TX_SOUND_SPEED_STRUCT setSoundSpeed;
    DVL_TX_GET_TIME_STRUCT getTime;
    DVL_TX_SET_TIME_STRUCT setTime;
};


struct DVL_RESPONSES{
    DVL_RX_GET_SYS_STRUCT getSystemRes;
    DVL_RX_GET_SET_STRUCT getSetupRes;
    DVL_RX_SET_SET_STRUCT setSetupRes;
    DVL_RX_SOFT_TRIG_STRUCT softTriggerRes;
    DVL_RX_SOUND_SPEED_STRUCT setSoundSpeedRes;
    DVL_RX_GET_TIME_STRUCT getTimeRes;
    DVL_RX_SET_TIME_STRUCT setTimeRes;
    DVL_RX_BIN_DATA_STRUCT binaryData;
};

struct DVL_{
    DVL_COMMANDS commandData;
    DVL_RESPONSES responseData;
};












#endif