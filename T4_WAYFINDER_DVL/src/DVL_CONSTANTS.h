
#include <stdint.h>

// SOP == Start Of Packet ID
//  precedes all transmissions with a message
//  type identifier.
//  TX and RX as 6 bytes

#define SOP_LEN             6
#ifdef REVERSE_ARRAYS
#define DVL_GET_SYS_TX_SOP  {0x02, 0x00, 0x0F, 0x01, 0x10, 0xAA}
#define DVL_GET_SYS_RX_SOP  {0x10, 0x00, 0x98, 0x01, 0x10, 0xAA}
#define DVL_GET_SET_TX_SOP  {0x02, 0x00, 0x0F, 0x01, 0x10, 0xAA}
#define DVL_GET_SET_RX_SOP  {0x10, 0x00, 0x25, 0x01, 0x10, 0xAA}
#define DVL_SET_SET_TX_SOP  {0x02, 0x00, 0x23, 0x01, 0x10, 0xAA}
#define DVL_SET_SET_RX_SOP  {0x10, 0x00, 0x11, 0x01, 0x10, 0xAA}
#define DVL_SOFT_TRIG_TX_SOP    {0x02, 0x00, 0x0F, 0x01, 0x10, 0xAA}
#define DVL_SOFT_TRIG_RX_SOP    {0x10, 0x00, 0x11, 0x01, 0x10, 0xAA}
#define DVL_SOUND_SPEED_TX_SOP  {0x02, 0x00, 0x13, 0x01, 0x10, 0xAA}
#define DVL_SOUND_SPEED_RX_SOP  {0x10, 0x00, 0x11, 0x01, 0x10, 0xAA}
#define DVL_GET_TIME_TX_SOP {0x02, 0x00, 0x0F, 0x01, 0x10, 0xAA}
#define DVL_GET_TIME_RX_SOP {0x01, 0x00, 0x1D, 0x01, 0x10, 0xAA}
#define DVL_SET_TIME_TX_SOP {0x02, 0x00, 0x1B, 0x01, 0x10, 0xAA}
#define DVL_SET_TIME_RX_SOP {0x10, 0x00, 0x11, 0x01, 0x10, 0xAA}
#define DVL_DATA_RX_SOP     {0x10, 0x00, 0x74, 0x01, 0x10, 0xAA}
#else
#define DVL_GET_SYS_TX_SOP  {0xAA, 0x10, 0x01, 0x0F, 0x00, 0x02}
#define DVL_GET_SYS_RX_SOP  {0xAA, 0x10, 0x01, 0x98, 0x00, 0x10}
#define DVL_GET_SET_TX_SOP  {0xAA, 0x10, 0x01, 0x0F, 0x00, 0x02}
#define DVL_GET_SET_RX_SOP  {0xAA, 0x10, 0x01, 0x25, 0x00, 0x10}
#define DVL_SET_SET_TX_SOP  {0xAA, 0x10, 0x01, 0x23, 0x00, 0x02}
#define DVL_SET_SET_RX_SOP  {0xAA, 0x10, 0x01, 0x11, 0x00, 0x10}
#define DVL_SOFT_TRIG_TX_SOP    {0xAA, 0x10, 0x01, 0x0F, 0x00, 0x02}
#define DVL_SOFT_TRIG_RX_SOP    {0xAA, 0x10, 0x01, 0x11, 0x00, 0x10}
#define DVL_SOUND_SPEED_TX_SOP  {0xAA, 0x10, 0x01, 0x13, 0x00, 0x02}
#define DVL_SOUND_SPEED_RX_SOP  {0xAA, 0x10, 0x01, 0x11, 0x00, 0x10}
#define DVL_GET_TIME_TX_SOP {0xAA, 0x10, 0x01, 0x0F, 0x00, 0x02}
#define DVL_GET_TIME_RX_SOP {0xAA, 0x10, 0x01, 0x1D, 0x00, 0x01}
#define DVL_SET_TIME_TX_SOP {0xAA, 0x10, 0x01, 0x1B, 0x00, 0x02}
#define DVL_SET_TIME_RX_SOP {0xAA, 0x10, 0x01, 0x11, 0x00, 0x10}
#define DVL_DATA_RX_SOP     {0xAA, 0x10, 0x01, 0x74, 0x00, 0x10}
#endif

// All SOP IDs are led by 2 bytes 0xAA 0x10
//  by removing these we can fit every ID into
//  a standard uint32.
#ifdef REVERSE_ARRAYS
#define DVL_GET_SYS_TX_SOP_VAL  0x010F0002
#define DVL_GET_SYS_RX_SOP_VAL  0x01980010
#define DVL_GET_SET_TX_SOP_VAL  0x010F0002
#define DVL_GET_SET_RX_SOP_VAL  0x01250010
#define DVL_SET_SET_TX_SOP_VAL  0x01230002
#define DVL_SET_SET_RX_SOP_VAL  0x01110010
#define DVL_SOFT_TRIG_TX_SOP_VAL    0x010F0002
#define DVL_SOFT_TRIG_RX_SOP_VAL    0x01110010
#define DVL_SOUND_SPEED_TX_SOP_VAL  0x01130002
#define DVL_SOUND_SPEED_RX_SOP_VAL  0x01110010
#define DVL_GET_TIME_TX_SOP_VAL 0x010F0002
#define DVL_GET_TIME_RX_SOP_VAL 0x011D0010
#define DVL_SET_TIME_TX_SOP_VAL 0x011B0002
#define DVL_SET_TIME_RX_SOP_VAL 0x01110010
#define DVL_DATA_RX_SOP_VAL     0x01740010
#endif

// CMD ID == Command ID
// RES ID == Response ID
// These 7 byte values are representative
//  of specific messages within the overarching
//  message type defined by the SOP ID

#define CMD_ID_LEN          7
#define RES_ID_LEN          7
#ifdef REVERSE_ARRAYS
#define DVL_GET_SYS_TX_CMD_ID   {0x81, 0x00, 0x00, 0x01, 0x00, 0x08, 0x03}
#define DVL_GET_SYS_RX_RES_ID   {0x81, 0x00, 0x00, 0x01, 0x00, 0x91, 0x04}
#define DVL_GET_SET_TX_CMD_ID   {0x85, 0x00, 0x00, 0x01, 0x00, 0x08, 0x03}
#define DVL_GET_SET_RX_RES_ID   {0x85, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x04}
#define DVL_SET_SET_TX_CMD_ID   {0x87, 0x00, 0x00, 0x02, 0x00, 0x1C, 0x03}
#define DVL_SET_SET_RX_RES_ID   {0x87, 0x00, 0x00, 0x02, 0x00, 0x0A, 0x04}
#define DVL_SOFT_TRIG_TX_CMD_ID {0x00, 0x00, 0x00, 0x11, 0x00, 0x08, 0x03}
#define DVL_SOFT_TRIG_RX_RES_ID {0x00, 0x00, 0x00, 0x11, 0x00, 0x0A, 0x04}
#define DVL_SOUND_SPEED_TX_CMD_ID   {0x86, 0x00, 0x00, 0x03, 0x00, 0x0C, 0x03}
#define DVL_SOUND_SPEED_RX_RES_ID   {0x86, 0x00, 0x00, 0x03, 0x00, 0x0A, 0x04}
#define DVL_GET_TIME_TX_CMD_ID  {0x1D, 0x00, 0x00, 0x01, 0x00, 0x08, 0x03}
#define DVL_GET_TIME_RX_RES_ID  {0x1D, 0x00, 0x00, 0x01, 0x00, 0x16, 0x04}
#define DVL_SET_TIME_TX_CMD_ID  {0x1F, 0x00, 0x00, 0x02, 0x00, 0x14, 0x03}
#define DVL_SET_TIME_RX_RES_ID  {0x1F, 0x00, 0x00, 0x02, 0x00, 0x0A, 0x04}
#else
#define DVL_GET_SYS_TX_CMD_ID   {0x03, 0x08, 0x00, 0x01, 0x00, 0x00, 0x81}
#define DVL_GET_SYS_RX_RES_ID   {0x04, 0x91, 0x00, 0x01, 0x00, 0x00, 0x81}
#define DVL_GET_SET_TX_CMD_ID   {0x03, 0x08, 0x00, 0x01, 0x00, 0x00, 0x85}
#define DVL_GET_SET_RX_RES_ID   {0x04, 0x1E, 0x00, 0x01, 0x00, 0x00, 0x85}
#define DVL_SET_SET_TX_CMD_ID   {0x03, 0x1C, 0x00, 0x02, 0x00, 0x00, 0x87}
#define DVL_SET_SET_RX_RES_ID   {0x04, 0x0A, 0x00, 0x02, 0x00, 0x00, 0x87}
#define DVL_SOFT_TRIG_TX_CMD_ID {0x03, 0x08, 0x00, 0x11, 0x00, 0x00, 0x00}
#define DVL_SOFT_TRIG_RX_RES_ID {0x04, 0x0A, 0x00, 0x11, 0x00, 0x00, 0x00}
#define DVL_SOUND_SPEED_TX_CMD_ID   {0x03, 0x0C, 0x00, 0x03, 0x00, 0x00, 0x86}
#define DVL_SOUND_SPEED_RX_RES_ID   {0x04, 0x0A, 0x00, 0x03, 0x00, 0x00, 0x86}
#define DVL_GET_TIME_TX_CMD_ID  {0x03, 0x08, 0x00, 0x01, 0x00, 0x00, 0x1D}
#define DVL_GET_TIME_RX_RES_ID  {0x04, 0x16, 0x00, 0x01, 0x00, 0x00, 0x1D}
#define DVL_SET_TIME_TX_CMD_ID  {0x03, 0x14, 0x00, 0x02, 0x00, 0x00, 0x1F}
#define DVL_SET_TIME_RX_RES_ID  {0x04, 0x0A, 0x00, 0x02, 0x00, 0x00, 0x1F}
#endif


// DVL Binary Data Output
//  has a 9 bit DATA ID
#define DVL_DATA_ID_LEN 9
#ifdef REVERSE_ARRAYS
#define DVL_DATA_RX_DATA_ID     {0x00, 0x00, 0x00, 0x69, 0x11, 0xAA, 0x00, 0x6D, 0x05}
#else
#define DVL_DATA_RX_DATA_ID     {0x05, 0x6D, 0x00, 0xAA, 0x11, 0x69, 0x00, 0x00, 0x00}
#endif

// Command Structure ID
//  precede command data within a SET message
#define DVL_CMD_STRUC_ID_LEN    6
#ifdef REVERSE_ARRAYS
#define DVL_SET_SET_TX_STR_ID   {0x00, 0x00, 0x00, 0x14, 0x10, 0x22}
#define DVL_SET_TIME_RX_STR_ID  {0x00, 0x00, 0x00, 0x0C, 0x10, 0x23}
#else
#define DVL_SET_SET_TX_STR_ID   {0x22, 0x10, 0x14, 0x00, 0x00, 0x00}
#define DVL_SET_TIME_RX_STR_ID  {0x23, 0x10, 0x0C, 0x00, 0x00, 0x00}
#endif

// Payload Headers
//  precede data bytes within a GET message
#define DVL_PAYLOAD_HEADER_LEN  6
#ifdef REVERSE_ARRAYS
#define DVL_GET_SYS_RX_PAY_HEAD {0x00, 0x00, 0x00, 0x87, 0x10, 0x22}
#define DVL_GET_SET_RX_PAY_HEAD {0x00, 0x00, 0x00, 0x14, 0x10, 0x22}
#define DVL_GET_TIME_RX_PAY_HEAD    {0x00, 0x00, 0x00, 0x0C, 0x10, 0x23}
#else
//#define DVL_GET_SYS_RX_PAY_HEAD {0x22, 0x10, 0x87, 0x00, 0x00, 0x00}  // Datasheet was wrong on this one
#define DVL_GET_SYS_RX_PAY_HEAD {0x21, 0x10, 0x87, 0x00, 0x00, 0x00}
//#define DVL_GET_SET_RX_PAY_HEAD {0x22, 0x10, 0x14, 0x00, 0x00, 0x00}
#define DVL_GET_SET_RX_PAY_HEAD {0x22, 0x12, 0x14, 0x00, 0x00, 0x00}
#define DVL_GET_TIME_RX_PAY_HEAD    {0x23, 0x10, 0x0C, 0x00, 0x00, 0x00}
#endif

// Wayfinder DVL System ID/Type
#define DVL_SYSTEM_ID   0x4C    // == 76

// Wayfinder DVL Ping Frequency
#define DVL_PING_FREQ   614400.0f 

// Speed of sound base
#define DVL_SOUND_SPEED_SALT    1500.0f    // m/s
#define DVL_SOUND_SPEED_FRESH   1481.0f    // m/s

// Maximum expected track depth
#define DVL_MAX_TRACK_RANGE     250.0f      // m
#define DVL_10M_TRACK_RANGE     10.0f       // m

// Checksums
//  unsigned 16 bit value, ignoring overflow
//  sum all data in message
#define DVL_GET_SYS_TX_CHKSUM   0x5901
#define DVL_GET_SET_TX_CHKSUM   0x5D01
#define DVL_SOFT_TRIG_TX_CHKSUM 0xE800
#define DVL_GET_TIME_TX_CHKSUM  0xF500

// Bytes per message
enum DVL_TX_MESSAGE_LEN_{
    GET_SYS_TX_LEN  = 15,
    GET_SET_TX_LEN  = 15,
    SET_SET_TX_LEN  = 35,
    SOFT_TRIG_TX_LEN = 15,
    SOUND_SPEED_TX_LEN = 19,
    GET_TIME_TX_LEN = 15,
    SET_TIME_TX_LEN = 27
};

// Bytes per message
enum DVL_RX_MESSAGE_LEN_{
    GET_SYS_RX_LEN  = 152,
    GET_SET_RX_LEN  = 37,
    SET_SET_RX_LEN  = 17,
    SOFT_TRIG_RX_LEN = 17,
    SOUND_SPEED_RX_LEN = 17,
    GET_TIME_RX_LEN = 29,
    SET_TIME_RX_LEN = 17,
    DVL_BIN_DATA_LEN = 112
};

// Constant checksum values
//  ie. SOP + CMD ID, etc..
enum DVL_CHKSUM_CONSTANTS{

};

enum DVL_BAUD{
    DVL_9600_BAUD   = 3,
    DVL_115200_BAUD = 7
};

enum MAJOR_BYTE_STATUS{
    BIN_RSP_SUCCESS = 1,
    BIN_RSP_UNKNOWN_CMD,
    BIN_RSP_PARAM_INVALID,
    BIN_RSP_CMD_EXEC_ERR,
    BIN_RSP_CMD_SET_ERR,
    BIN_RSP_CMD_GET_ERR,
    BIN_RSP_NORUN_WITH_PING
};

enum MINOR_BYTE_STATUS{
    BIN_RSP_INVALID_NONE,
    BIN_RSP_INVALID_PARAM_SIZE,
    BIN_RSP_INVALID_STRUCT_HDR,
    BIN_RSP_INVALID_BAUD,
    BIN_RSP_INVALID_TRIGGER,
    BIN_RSP_INVALID_SOS,
    BIN_RSP_INVALID_MAXDEPTH,
    BIN_RSP_INVALID_DATETIME,
    BIN_RSP_INVALID_PARAM_GENERIC
};

enum BIT_FLAGS{
    AB_NO_ERROR,
    AB_POST_FAULT_DSC,
    AB_POST_FAULT_DPFRAM,
    AB_POST_FAULT_SDRAM,
    AB_POST_FAULT_DPEEPROM,
    AB_POST_FAULT_RTC,
    AB_FAULT_RTC,
    AB_CLK_NOT_LOCKED = 0x10,
    AB_FAULT_REG_FILE_SCK_ADC,
    AB_FAULT_REG_FILE_DSP,
    AB_FAULT_REG_FILE_ADC,
    AB_FAULT_RAW_RD_EMPTY,
    AB_FAULT_RAW_WR_FULL,
    AB_FAULT_FILTER,
    AB_FAULT_OX_RD_EMPTY,
    AB_FAULT_OS_WR_FULL,
    AB_FAULT_OS_FULL,
    AB_FAULT_IN_FIFO,
    AB_FAULT_TX,
    AB_QSPI_ERROR,
    AB_QSPI_FIFO_RD_EMPTY,
    AB_FAULT_FPGA_14,
    AB_FAULT_FPGA_15,
    AB_FAULT_VOLTAGE_OUT_OF_RANGE,
    AB_DP_FAULT_MEMORY = 0xE5,
    AB_DP_FAULT_OOB,
    AB_DP_FAULT_START_PING,
    AB_DP_FAULT_PING_WAIT_EVT_FAIL,
    AB_DP_FAULT_PING_FIFO,
    AB_DP_FAULT_BOTDET_FISH,
    AB_DP_FAULT_BOTDET_BOUNCE,
    AB_DP_FAULT_BOTDET_FAIL,
    AB_DP_FAULT_COR_FAIL,
    AB_DP_FAULT_VEL_OVR,
    AB_DP_FAULT_NVMEM_FAILURE,
    AB_DP_FAULT_SCHED_EVT_DESCR,
    AB_DP_FAULT_SCHED_EVT_ERR,
    AB_DP_FAULT_SCHED_TRIG_EVT_ERR,
    AB_DP_FAULT_SCHED_PING_EVT_ERR,
    AB_DP_FAULT_SCHED_EVT_RESET_ERR,
    AB_DP_FAULT_OUT_EVTWAIT_ERR,
    AB_DP_FAULT_PING_EVT_ERR,
    AB_DP_FAULT_TIMER,
    AB_DP_FAULT_IQ_ABORT,
    AB_DP_FAULT_IQ_READ,
    AB_DP_FAULT_IQ_EVT_SET,
    AB_DP_FAULT_FPGA_IND_FAULT,
    AB_DP_FAULT_FIFO_EVT_WAIT,
    AB_DP_FAULT_IQ_CKSUM_FAIL,
    AB_DP_FAULT_WDREG_ERR,
    AB_DP_FAULT_WDRPT_ERR,
    AB_MAX_FAULT_CODES
};

const char AD_DP_FAULT_START_PING_STR[] = "Ping Start Error\0";
const char AD_DP_FAULT_PING_WAIT_EVT_STR[] = "Ping Wait for Data Event Error\0";

const char AD_DP_FAILT_BOTDET_FAIL_STR[] = "Bottom detect failed to find >2 good beams\0";
