#include "T4_DVL.h"
#include <stdint.h>
#include <HardwareSerial.h>
#include <Arduino.h>
// Remember: MSB IS SEND FIRST OVER UART

DVL_ dvl;
DVL_VELOCITY current_velocity;

uint8_t DVL_TX_BUF[DVL_TX_BUF_LEN]  DMAMEM;
uint8_t DVL_RX_BUF[DVL_RX_BUF_LEN]  DMAMEM;

void init_DVL_serial(){
    DVLSERIAL.begin(DVLBAUD);

    DVLSERIAL.addMemoryForWrite(DVL_TX_BUF, sizeof(DVL_TX_BUF));
    DVLSERIAL.addMemoryForRead(DVL_RX_BUF, sizeof(DVL_RX_BUF));

    clear_DVL_RX();
}

DVL_ *init_DVL_data_struct(){
    return &dvl;
}

void inline clear_DVL_RX(){
    DVLSERIAL.clear();
}

// Update velocity table
//  If return == 0 no updates have been made since last call to this function
/*
uint32_t updateVelocityTable(DVL_ &dvl_, DVL_VELOCITY &dest){
    uint32_t flags = 0;
    if(dvl_.responseData.binaryData.velocity.X != dest.X){          // If X changed
        flags |= (1 << 0);
        dest.X = dvl_.responseData.binaryData.velocity.X;
    }
    if(dvl_.responseData.binaryData.velocity.Y != dest.Y){          // If Y changed
        flags |= (1 << 1);
        dest.Y = dvl_.responseData.binaryData.velocity.Y;
    }
    if(dvl_.responseData.binaryData.velocity.Z != dest.Z){          // If Z changed
        flags |= (1 << 2);
        dest.Z = dvl_.responseData.binaryData.velocity.Z;
    }
    if(dvl_.responseData.binaryData.velocity.error != dest.error){  // If Error changed
        flags |= (1 << 3);
        dest.error = dvl_.responseData.binaryData.velocity.error;
    }
    return flags;
}
*/

void inline serialSend(uint8_t val_2_send){
    while(DVLSERIAL.availableForWrite() < 16);
    DVLSERIAL.write(val_2_send);
}

uint8_t serialRead(){
    while(!DVLSERIAL.available());
    return DVLSERIAL.read();
}

void inline waitForSerial_TX_clear(){
    DVLSERIAL.flush();
}

void send_float_over_serial(float val_){
    uint8_t *ptr_ = (uint8_t *) &val_;
    for(uint8_t n = 0; n < 4; n++){
        serialSend(ptr_[n]);
    }
}

uint16_t checksum_of_float(float val_){
    uint8_t *ptr_ = (uint8_t *) &val_;
    uint16_t tmp_ = 0;
    for(uint8_t n = 0; n < 4; n++){
        tmp_ += ptr_[n];
    }
    return tmp_;
}

float read_float_over_serial(){
    //while(!DVLSERIAL.available());
    float val_2_ret;
    uint8_t d_in[4];
    //while(!DVLSERIAL.available());
    d_in[0] = DVLSERIAL.read();
    d_in[1] = DVLSERIAL.read();
    d_in[2] = DVLSERIAL.read();
    d_in[3] = DVLSERIAL.read();

    // Copy read in data
    memcpy(&val_2_ret, &d_in, sizeof(float));
    return val_2_ret;
}

void send_uint16_over_serial(uint16_t val_2_send){
    while(!DVLSERIAL.availableForWrite());
    DVLSERIAL.write(val_2_send >> 8);
    DVLSERIAL.write(val_2_send & 0xFF);
}

uint16_t read_uint16_over_serial(){
    while(!DVLSERIAL.available());
    uint16_t val_2_ret = (uint16_t)DVLSERIAL.read() << 8;
    val_2_ret |= (uint16_t)DVLSERIAL.read();
    return val_2_ret;
}

void send_uint32_over_serial(uint32_t val_2_send){
    serialSend((val_2_send >> 24) & 0xFF);
    serialSend((val_2_send >> 16) & 0xFF);
    serialSend((val_2_send >> 8) & 0xFF);
    serialSend(val_2_send & 0xFF);
}

uint32_t read_uint32_over_serial(){
    while(!DVLSERIAL.available());
    uint32_t val_2_ret = (uint32_t)DVLSERIAL.read() << 24;
    val_2_ret |= (uint32_t)DVLSERIAL.read() << 16;
    val_2_ret |= (uint32_t)DVLSERIAL.read() << 8;
    val_2_ret |= (uint32_t)DVLSERIAL.read() << 0;
    return val_2_ret;
}

void send_sop_dvl(const uint8_t *sop_){
    for(uint8_t n = 0; n < SOP_LEN; n++){
        serialSend(sop_[n]);
    }
}

void send_cmd_dvl(const uint8_t *cmd_){
    for(uint8_t n = 0; n < CMD_ID_LEN; n++){
        serialSend(cmd_[n]);
    }
}

void send_checksum_dvl(uint16_t chksum_){
    serialSend(chksum_ >> 8);
    serialSend(chksum_);
}

void send_CMD_STRUC_ID(const uint8_t *str_){
    for(uint8_t n = 0; n < DVL_CMD_STRUC_ID_LEN; n++){
        serialSend(str_[n]);
    }
}


bool read_and_match_sop_dvl(const uint8_t *sop_2_match){
    for(uint8_t n = 0; n < SOP_LEN; n++){
        if(sop_2_match[n] != serialRead()) return 0;
    }
    return 1;
}

bool read_and_match_RES_ID_dvl(const uint8_t *res_2_match){
    for(uint8_t n = 0; n < RES_ID_LEN; n++){
        if(res_2_match[n] != serialRead()) return 0;
    }
    return 1;
}

bool read_and_match_PAYLOAD_HEADER(const uint8_t *pay_2_match){
    for(uint8_t n = 0; n < DVL_PAYLOAD_HEADER_LEN; n++){
        if(pay_2_match[n] != serialRead()) return 0;
    }
    return 1;
}

bool read_and_match_CMD_STRUC_ID(const uint8_t *str_2_match){
    for(uint8_t n = 0; n < DVL_CMD_STRUC_ID_LEN; n++){
        if(str_2_match[n] != serialRead()) return 0;
    }
    return 1;
}

bool read_and_match_DATA_ID(const uint8_t *str_2_match){
    for(uint8_t n = 0; n < DVL_DATA_ID_LEN; n++){
        if(str_2_match[n] != serialRead()) return 0;
    }
    return 1;
}

uint16_t chksum_of_string(const uint8_t *str_, uint8_t len){
    uint16_t tmpchksum = 0;
    for(uint8_t n = 0; n < len; n++){
        tmpchksum += str_[n];
    }
    return tmpchksum;
}

// GET SYSTEM
void inline get_system_command_dvl(){
    send_sop_dvl(dvl.commandData.getSystem.SOP);
    send_cmd_dvl(dvl.commandData.getSystem.CMD_ID);
    send_checksum_dvl(dvl.commandData.getSystem.checksum);
}

int request_DVL_System_Config(){
    while((DVLSERIAL.availableForWrite() < GET_SYS_TX_LEN));   // Wait for enough TX buffer space
    get_system_command_dvl();

    clear_DVL_RX();         // Clear buffer bc we're reconfig
    while((DVLSERIAL.available() < GET_SYS_RX_LEN)){
        // DO some timeout thing here just in case
    }    // Wait for full message to come in

    if(!read_and_match_sop_dvl(dvl.responseData.getSystemRes.SOP))
                        return -1;          // SOP Does not match

    if(!read_and_match_RES_ID_dvl(dvl.responseData.getSystemRes.RES_ID))
                        return -2;          // Response ID does not match

    // Message should be valid after these checks.
    dvl.responseData.getSystemRes.status.major = serialRead();
    dvl.responseData.getSystemRes.status.minor = serialRead();

    if(!read_and_match_PAYLOAD_HEADER(dvl.responseData.getSystemRes.payload_header))
                        return -4;

    dvl.responseData.getSystemRes.frequency = read_float_over_serial();
    dvl.responseData.getSystemRes.firmware_version = read_uint32_over_serial();
    dvl.responseData.getSystemRes.FPGA_version = read_uint32_over_serial();
    dvl.responseData.getSystemRes.unique_ID = read_uint32_over_serial();
    dvl.responseData.getSystemRes.unique_ID |= ((uint64_t)read_uint32_over_serial() << 32);
    dvl.responseData.getSystemRes.XDCR_type = serialRead();
    dvl.responseData.getSystemRes.beam_angle = read_float_over_serial();
    dvl.responseData.getSystemRes.vertical_beam = serialRead();

    for(uint8_t n = 0; n < 101; n++){
        dvl.responseData.getSystemRes.RESERVED[n] = serialRead();
    }

    dvl.responseData.getSystemRes.device_ID = serialRead();
    dvl.responseData.getSystemRes.subsys_type = serialRead();
    dvl.responseData.getSystemRes.checksum = read_uint16_over_serial();

    return 0;   // TODO: Check checksum, check major and minor status
}

#ifndef NO_DVL_DEBUG
void print_DVL_System_Config(){
    Serial.printf("DVL System Configuration\n");
    Serial.printf("Major Status:\t\t%2X\n", dvl.responseData.getSystemRes.status.major);
    Serial.printf("Minor Status:\t\t%2X\n", dvl.responseData.getSystemRes.status.minor);
    Serial.printf("Beam Frequency:\t\t%.2f hz\n", dvl.responseData.getSystemRes.frequency);
    Serial.printf("Firmware Version:\t%u\n", dvl.responseData.getSystemRes.firmware_version);
    Serial.printf("FPGA Version:\t\t%u\n", dvl.responseData.getSystemRes.FPGA_version);
    Serial.printf("Unique ID:\t\t%llu\n", dvl.responseData.getSystemRes.unique_ID);
    Serial.printf("Transducer:\t\t%u\n", dvl.responseData.getSystemRes.XDCR_type);
    Serial.printf("Beam Angle:\t\t%.2f\n", dvl.responseData.getSystemRes.beam_angle);
    Serial.printf("Vertical Beam:\t\t%d\n", dvl.responseData.getSystemRes.vertical_beam);
    Serial.printf("Device ID:\t\t%d\n", dvl.responseData.getSystemRes.device_ID);
    Serial.printf("Subsystem Type:\t\t%d\n", dvl.responseData.getSystemRes.subsys_type);
    Serial.printf("Checksum:\t\t%u\n", dvl.responseData.getSystemRes.checksum);
}
#endif

// GET SETUP
void inline get_setup_command_dvl(){
    send_sop_dvl(dvl.commandData.getSetup.SOP);
    send_cmd_dvl(dvl.commandData.getSetup.CMD_ID);
    send_checksum_dvl(dvl.commandData.getSetup.checksum);
}

int request_DVL_Setup_Config(){
    while((DVLSERIAL.availableForWrite() < GET_SET_TX_LEN));       // Wait for TX buffer space
    get_setup_command_dvl();

    clear_DVL_RX();
    while((DVLSERIAL.available() < GET_SET_RX_LEN)){
        // DO some timeout thing here just in case
    }    // Wait for full message to come in

    if(!read_and_match_sop_dvl(dvl.responseData.getSetupRes.SOP))
                        return -1;          // SOP Does not match

    if(!read_and_match_RES_ID_dvl(dvl.responseData.getSetupRes.RES_ID))
                        return -2;          // Response ID does not match

    // Message should be valid after these checks.
    dvl.responseData.getSetupRes.status.major = serialRead();
    dvl.responseData.getSetupRes.status.minor = serialRead();

    if(!read_and_match_PAYLOAD_HEADER(dvl.responseData.getSetupRes.payload_header))
                        return -4;          // Payload Header doesn't match

    dvl.responseData.getSetupRes.soft_trig = serialRead();
    dvl.responseData.getSetupRes.baud = serialRead();
    dvl.responseData.getSetupRes.speed_of_sound = read_float_over_serial();
    dvl.responseData.getSetupRes.max_track_depth = read_float_over_serial();
    dvl.responseData.getSetupRes.RESERVED = read_float_over_serial();

    dvl.responseData.getSetupRes.checksum = read_uint16_over_serial();

    return 0;   // TODO: Check checksum, check major and minor status
}

#ifndef NO_DVL_DEBUG
void print_DVL_Setup_Config(){
    Serial.printf("DVL Setup Configuration\n");
    Serial.printf("Major Status:\t\t%2X\n", dvl.responseData.getSetupRes.status.major);
    Serial.printf("Minor Status:\t\t%2X\n", dvl.responseData.getSetupRes.status.minor);
    Serial.printf("Soft Trigger:\t\t%u\n", dvl.responseData.getSetupRes.soft_trig);
    if(dvl.responseData.getSetupRes.baud == DVL_115200_BAUD){
        Serial.printf("BAUD:\t\t\t115200\n");
    } else if(dvl.responseData.getSetupRes.baud == DVL_9600_BAUD){
        Serial.printf("BAUD:\t\t\t9600\n");
    } else {
        Serial.printf("BAUD:\t\t\t%u\n", dvl.responseData.getSetupRes.baud);
    }
    Serial.printf("Speed of Sound:\t\t%.2f\n", dvl.responseData.getSetupRes.speed_of_sound);
    Serial.printf("Max Track Depth:\t%.2f\n", dvl.responseData.getSetupRes.max_track_depth);
    Serial.printf("Checksum:\t\t%u\n", dvl.responseData.getSetupRes.checksum);
}
#endif

// SET SETUP CONFIGURATION VARIABLES
void inline set_setup_command_dvl(){
    send_sop_dvl(dvl.commandData.setSetup.SOP);
    send_cmd_dvl(dvl.commandData.setSetup.CMD_ID);
    send_CMD_STRUC_ID(dvl.commandData.setSetup.CMD_STRUC_ID);
}

int set_DVL_Setup_Config(float speed_of_sound_, float max_track_range, uint8_t baudrate, uint8_t soft_trig){
    // Bounds limit speed of sound
    if(speed_of_sound_ < 1400.0f || speed_of_sound_ > 1600.0f){
        speed_of_sound_ = DVL_SOUND_SPEED_SALT;
    }
    
    // Bounds limit max track range [0,250] meters
    if(max_track_range < 0.0f){
        max_track_range = 0.0f;
    } else if(max_track_range > 250.0f){
        max_track_range = 250.0f;
    }
    

    // Bounds limit BAUD
    if(baudrate != DVL_115200_BAUD && baudrate != DVL_9600_BAUD){
        baudrate = DVL_115200_BAUD;
    }

    // Bounds limit soft trig (bool)
    if(soft_trig > 1){
        soft_trig = 1;
    }

    while((DVLSERIAL.availableForWrite() < SET_SET_TX_LEN));   // Wait for TX buffer space
    set_setup_command_dvl();

    serialSend(soft_trig);
    serialSend(baudrate);
    send_float_over_serial(speed_of_sound_);
    send_float_over_serial(max_track_range);
    send_float_over_serial(0.0f);

    uint16_t chksum_tmp = checksum_of_float(speed_of_sound_);
    chksum_tmp += checksum_of_float(max_track_range);
    chksum_tmp += baudrate;
    chksum_tmp += soft_trig;

    //send checksum here, calculate constants for SOP and stuff
    chksum_tmp += chksum_of_string(dvl.commandData.setSetup.SOP, SOP_LEN);
    chksum_tmp += chksum_of_string(dvl.commandData.setSetup.CMD_ID, CMD_ID_LEN);
    chksum_tmp += chksum_of_string(dvl.commandData.setSetup.CMD_STRUC_ID, DVL_CMD_STRUC_ID_LEN);

    // chksum is somehow wrong here idk

    send_uint16_over_serial(chksum_tmp);                // Send checksum data

    clear_DVL_RX();
    while((DVLSERIAL.available() < SET_SET_RX_LEN)){
        // DO some timeout thing here just in case
    }    // Wait for full message to come in                                                    // Flush this data

    if(!read_and_match_sop_dvl(dvl.responseData.setSetupRes.SOP))
                    return -1;

    if(!read_and_match_RES_ID_dvl(dvl.responseData.setSetupRes.RES_ID))
                    return -2;

    dvl.responseData.setSetupRes.status.major = serialRead();
    dvl.responseData.setSetupRes.status.minor = serialRead();

    dvl.responseData.setSetupRes.checksum = read_uint16_over_serial();

    return 0;
}







int DVL_DATA_UPDATE(){
    if(DVLSERIAL.available() < DVL_BIN_DATA_LEN){
        return -5;                              // Not enough data ready
    }

    while(DVLSERIAL.peek() != 0xAA) DVLSERIAL.read();   // Read until data SOP start

    if(!read_and_match_sop_dvl(dvl.responseData.binaryData.SOP))
                        return -1;

    if(!read_and_match_DATA_ID(dvl.responseData.binaryData.DATA_ID))
                        return -3;

    dvl.responseData.binaryData.device_ID   = serialRead();
    dvl.responseData.binaryData.subsys_type = serialRead();
    dvl.responseData.binaryData.firmware.major = serialRead();
    dvl.responseData.binaryData.firmware.minor = serialRead();
    dvl.responseData.binaryData.firmware.patch = serialRead();
    dvl.responseData.binaryData.firmware.build = serialRead();
    dvl.responseData.binaryData.rtc.year = serialRead();
    dvl.responseData.binaryData.rtc.month = serialRead();
    dvl.responseData.binaryData.rtc.day = serialRead();
    dvl.responseData.binaryData.rtc.hour = serialRead();
    dvl.responseData.binaryData.rtc.minute = serialRead();
    dvl.responseData.binaryData.rtc.second = serialRead();
    dvl.responseData.binaryData.rtc.millisecond = read_uint16_over_serial();
    dvl.responseData.binaryData.coordinate_system = serialRead();
    dvl.responseData.binaryData.velocity.X = read_float_over_serial();
    dvl.responseData.binaryData.velocity.Y = read_float_over_serial();
    dvl.responseData.binaryData.velocity.Z = read_float_over_serial();
    dvl.responseData.binaryData.velocity.error = read_float_over_serial();
    dvl.responseData.binaryData.range.beam0 = read_float_over_serial();
    dvl.responseData.binaryData.range.beam1 = read_float_over_serial();
    dvl.responseData.binaryData.range.beam2 = read_float_over_serial();
    dvl.responseData.binaryData.range.beam3 = read_float_over_serial();
    dvl.responseData.binaryData.range.mean = read_float_over_serial();
    dvl.responseData.binaryData.speed_of_sound = read_float_over_serial();
    dvl.responseData.binaryData.BT_status = read_uint16_over_serial();
    dvl.responseData.binaryData.BIT = read_uint16_over_serial();
    dvl.responseData.binaryData.power.input_voltage = read_float_over_serial();
    dvl.responseData.binaryData.power.tx_voltage = read_float_over_serial();
    dvl.responseData.binaryData.power.tx_current = read_float_over_serial();
    for(uint8_t n = 0; n < 6; n++){
        dvl.responseData.binaryData.system_serial[n] = serialRead();
    }
    for(uint8_t n = 0; n < 20; n++){
        dvl.responseData.binaryData.RESERVED[n] = serialRead();
    }
    dvl.responseData.binaryData.checksum_data = read_uint16_over_serial();
    dvl.responseData.binaryData.checksum_data = read_uint16_over_serial();


    current_velocity.X = dvl.responseData.binaryData.velocity.X;
    current_velocity.Y = dvl.responseData.binaryData.velocity.Y;
    current_velocity.Z = dvl.responseData.binaryData.velocity.Z;
    current_velocity.error = dvl.responseData.binaryData.velocity.error;



    return 0;
}

#ifndef NO_DVL_DEBUG
// Debug only
void print_current_vel(){
    Serial.printf("X:\t\t%.4f\n", current_velocity.X);
    Serial.printf("Y:\t\t%.4f\n", current_velocity.Y);
    Serial.printf("Z:\t\t%.4f\n", current_velocity.Z);
    Serial.printf("E:\t\t%.4f\n", current_velocity.error);
}

void print_current_power(){
    Serial.printf("V in:\t\t%.4f V\n", dvl.responseData.binaryData.power.input_voltage);
    Serial.printf("V TX:\t\t%.4f V\n", dvl.responseData.binaryData.power.tx_voltage);
    Serial.printf("I TX:\t\t%.4f A\n", dvl.responseData.binaryData.power.tx_current);
}

void print_current_beam_range(){
    Serial.printf("0:\t\t%.4f m\n", dvl.responseData.binaryData.range.beam0);
    Serial.printf("1:\t\t%.4f m\n", dvl.responseData.binaryData.range.beam1);
    Serial.printf("2:\t\t%.4f m\n", dvl.responseData.binaryData.range.beam2);
    Serial.printf("3:\t\t%.4f m\n", dvl.responseData.binaryData.range.beam3);
    Serial.printf("M:\t\t%.4f m\n", dvl.responseData.binaryData.range.mean);
}

void print_current_BIT(){
    if(dvl.responseData.binaryData.BIT & 0x100){
        Serial.printf("BIT:\t\t0x%2X\n", dvl.responseData.binaryData.BIT & 0xFF);
    } else {
        Serial.printf("BIT:\t\tNo Errors\n");
    }
}
#endif
