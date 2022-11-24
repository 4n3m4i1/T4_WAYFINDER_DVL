# Wayfinder DVL Library for Teensy 4.X  
Bare minimum functional non blocking library for Teledyne Wayfinder DVL  
communicating over RS232 to a Teensy 4.X. A MAX3232 or equivalent is required  
for hardware connections.  
  
## Functions Provided  
### init_DVL_serial()
Initiates serial RX and TX buffers, sets up the selected  
LPUART port, associates the port RX ISR to fill the RX buffer.  
  
### request_DVL_System_Config() 
Will read hardware version information, beam frequency,  
unique system ID, beam angle. This data can be viewed over Serial  
with `print_DVL_System_Config()`.  
  
### request_DVL_Setup_Config()
Will read DVL soft trigger mode, BAUD, speed of sound  
used for Doppler calculations, max track range limit.  
This can be viewed over Serial with `print_DVL_Setup_Config()`  
  
### DVL_DATA_UPDATE()
This is a non-blocking check of the current UART buffer size.  
Once a message is fully read into the buffer by the associated  
UART ISR this function will run through parsing the message at  
full speed, not waiting for more serial data to arrive.  
If the message is deemed valid, read data can be viewed with:  
- `print_current_vel()` Prints current velocity vectors, and error
- `print_current_beam_range()` Prints current beam ranges to bottom, and the mean range
- `print_current_power()` Print input voltage, TX voltage, and TX current
- `print_current_BIT()` Prints current BIT error. Prints `NONE` if errors aren't present
  
## Default Configuration
By default `DVLSERIAL` is defined as `Serial1 / LPUART1`  
  
# TODO
- Fix checksums
- Reduce blocking more if possible
- Optimize serial buffer sizes
- functions to return data
- lots of polish
  
# Known Issues
Currently checksum calculation is not performing correctly,  
thus any message with variable checksum (ie. Set Setup)  
will not be accepted by the DVL. Once this is ironed out  
all message types will be fully supported.  
  
The current work around requires reading the system setup  
and system config, then adjusting software where needed  
to support the current hardware setup. 
