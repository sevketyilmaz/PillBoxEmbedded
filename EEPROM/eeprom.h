#ifndef __EEPROM_H
#define __EEPROM_H

#include <stdlib.h>
#include <stdint.h>


void eeprom_write_byte( uint8_t deviceaddress, uint8_t eeaddress, uint8_t data );
void eeprom_write_page( uint8_t deviceaddress, uint8_t eeaddresspage, uint8_t* data, uint8_t length );

uint8_t eeprom_read_byte( uint8_t deviceaddress, uint8_t eeaddress );
void eeprom_read_buffer( uint8_t deviceaddress, uint8_t eeaddress, uint8_t *buffer, uint8_t length );

void eeprom_write_long( uint8_t deviceaddress, uint8_t eeaddresspage, uint32_t write_data);
void eeprom_read_long( uint8_t deviceaddress, uint8_t eeaddress, uint32_t *read_data);

#endif /* __EEPROM_H */
