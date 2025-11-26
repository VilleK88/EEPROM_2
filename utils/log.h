#ifndef LOG_H
#define LOG_H

#include "../main.h"

#include "hardware/i2c.h"

void write_byte(uint16_t address, uint8_t value);
uint8_t read_byte(uint16_t address);
void write_log_entry(const char *log);
uint16_t next_log_index();
bool validate_log_entry(uint16_t addr);
void print_log_entries();
void read_log_entry(uint16_t addr, uint8_t *buffer);
void erase_log_entry();
uint16_t crc16(const uint8_t *data_p, size_t length);

#endif