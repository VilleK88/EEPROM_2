#include "log.h"

void write_byte(uint16_t const address, uint8_t const value) {
    uint8_t buffer[3];

    // MSB (Most Significant Byte) address
    buffer[0] = address >> 8 & 0xFF;
    // LSB (Least Significant Byte) address
    buffer[1] = address & 0xFF;
    // Actual data, one byte
    buffer[2] = value;

    // Send 3 bytes:
    // - 2 bytes of address
    // - 1 byte of data
    i2c_write_blocking(I2C, EEPROM_ADDRESS, buffer, 3, false);
    sleep_ms(5);
}

uint8_t read_byte(uint16_t const address) {
    uint8_t buffer[2];
    uint8_t data;
    buffer[0] = address >> 8 & 0xFF; // MSB
    buffer[1] = address & 0xFF; // LSB
    i2c_write_blocking(I2C, EEPROM_ADDRESS, buffer, 2,true);
    i2c_read_blocking(I2C, EEPROM_ADDRESS, &data, 1, false);
    return data;
}

void write_log_entry(const char *log) {
    printf("Write log entry\r\n");
    const int log_len = (int)strnlen(log, LOG_MAX_LEN);
    if (log_len <= LOG_MAX_LEN) {
        uint8_t buffer[LOG_ENTRY_SIZE];

        memcpy(buffer, log, log_len);
        buffer[log_len] = '\0';

        const uint16_t crc = crc16(buffer, log_len + 1);
        buffer[log_len + 1] = (uint8_t)(crc >> 8);
        buffer[log_len + 2] = (uint8_t) crc;

        const int total_log_len = log_len + 3;

        const uint16_t start_index = next_log_index();
        for (int i = 0; i < total_log_len; i++) {
            write_byte(start_index + i, buffer[i]);
        }
    }
}

uint16_t next_log_index() {
    uint16_t addr = 0;

    for (int i = 0; i < MAX_LOGS; i++) {
        if (!validate_log_entry(addr)) {
            return addr;
        }
        addr += LOG_ENTRY_SIZE;
    }
    erase_log_entry();
    return 0;
}

bool validate_log_entry(const uint16_t addr) {
    uint8_t buffer[LOG_ENTRY_SIZE];
    read_log_entry(addr, buffer);

    if (buffer[0] != 0) {
        bool end_mark = false;
        int index = 0;

        do {
            if (buffer[index] == '\0')
                end_mark = true;
            else index++;
        } while (index <= LOG_MAX_LEN && !end_mark);

        const size_t total_len = index + 3;
        if (end_mark && total_len <= LOG_ENTRY_SIZE) {
            const uint16_t crc = crc16(buffer, total_len);
            return crc == 0;
        }
    }
    return false;
}

void print_log_entries() {
    uint16_t j = 0;
    for (int i = 0; i < 32; i++) {
        if (validate_log_entry(j)) {
            uint8_t buffer[LOG_ENTRY_SIZE];
            read_log_entry(j, buffer);
            printf("%d. log: %s\r\n", i + 1, (char*)buffer);
        }
        j += 64;
    }
}

void read_log_entry(const uint16_t addr, uint8_t *buffer) {
    for (int i = 0; i < LOG_ENTRY_SIZE; i++) {
        buffer[i] = read_byte(addr + i);
    }
}

void erase_log_entry() {
    printf("Erase log entry.\r\n");
    int j = 0;
    for (int i = 0; i < 32; i++) {
        write_byte(j, '\0');
        j += 64;
    }
}

uint16_t crc16(const uint8_t *data_p, size_t length) {
    uint16_t crc = 0xFFFF;
    while (length--) {
        uint8_t x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = crc << 8 ^ (uint16_t) (x << 12) ^ (uint16_t) (x << 5) ^ (uint16_t) x;
    }
    return crc;
}