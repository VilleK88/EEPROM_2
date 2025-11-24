# Exercise 2 – Store log strings in EEPROM      
  
Improve Exercise 1 by adding a persistent log that stores messages to EEPROM. When the program starts it
writes “Boot” to the log and every time when state or LEDs is changed the state change message, as
described in Exercise 1, is also written to the log.
The log must have the following properties:
-  Log starts from address 0 in the EEPROM.
-  First two kilobytes (2048 bytes) of EEPROM are used for the log.
-  Log is persistent, after power up writing to log continues from location where it left off last time.
The log is erased only when it is full or by user command.
-  Each log entry is reserved 64 bytes.
-  -  First entry is at address 0, second at address 64, third at address 128, etc.
-  -  Log can contain up to 32 entries.
-  A log entry consists of a string that contains maximum 61 characters, a terminating null character
(zero) and two-byte CRC that is used to validate the integrity of the data. A maximum length log
entry uses all 64 bytes. A shorter entry will not use all reserved bytes. The string must contain at
least one character.
-  When a log entry is written to the log, the string is written to the log including the terminating zero.
Immediately after the terminating zero follows a 16-bit CRC, MSB first followed by LSB.
-  -  Entry is written to the first unused (invalid) location that is available.
-  -  If the log is full then the log is erased first and then entry is written to address 0.
-  User can read the content of the log by typing read and pressing enter.
-  -  Program starts reading and validating log entries starting from address zero. If a valid string
is found it is printed and program reads string from the next location.
-  -  A string is valid if the first character is not zero, there is a zero in the string before index 62,
and the string passes the CRC validation.
-  -  Printing stops when an invalid string is encountered or the end log are is reached.
-  User can erase the log by typing erase and pressing enter.
-  -  Erasing is done by writing a zero at the first byte of every log entry.
