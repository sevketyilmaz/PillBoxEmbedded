#include "eeprom.h"
#include "i2c.h"

void clear_eeprom() {
	int esn;
	uint8_t byte_size = 8; // Number of byte for each boxes
	uint8_t start_address;
	for(esn=0; esn<21; esn++){
		start_address = 1 + byte_size * esn;
			
		eeprom_write_long(0x50<<1, start_address,   0);
		eeprom_write_byte(0x50<<1, start_address+4, 255);		
		eeprom_write_byte(0x50<<1, start_address+5, 0);		
		eeprom_write_byte(0x50<<1, start_address+6,	0);		
		eeprom_write_byte(0x50<<1, start_address+7,	0);	

	}		
}

void eeprom_write_byte( uint8_t deviceaddress, uint8_t eeaddress, uint8_t data ) {
    
	I2C_writeByte(deviceaddress, eeaddress, data);
}

 void eeprom_write_page( uint8_t deviceaddress, uint8_t eeaddresspage, uint8_t* data, uint8_t length ) {
	 
     I2C_writeBytes(deviceaddress, eeaddresspage, length, data);
}

void eeprom_write_long( uint8_t deviceaddress, uint8_t eeaddresspage, int write_data) {
	 
	uint8_t data[4];
  data[0] = (write_data & 0xFF000000) >> 24;
  data[1] = (write_data & 0x00FF0000) >> 16;
  data[2] = (write_data & 0x0000FF00) >>  8;
  data[3] = (write_data & 0x000000FF)      ;  
	I2C_writeBytes(deviceaddress, eeaddresspage, 4, data);
}
 
 uint8_t eeprom_read_byte( uint8_t deviceaddress, uint8_t eeaddress ) {
   
	 uint8_t rdata;
	 I2C_readByte(deviceaddress, eeaddress, &rdata);
   return rdata;
}

 void eeprom_read_buffer( uint8_t deviceaddress, uint8_t eeaddress, uint8_t *buffer, uint8_t length ) {
    
	 I2C_readBytes(deviceaddress, eeaddress, length, buffer);
}

 void eeprom_read_long( uint8_t deviceaddress, uint8_t eeaddress, int *read_data) {
 
	uint8_t data[4];  
	I2C_readBytes(deviceaddress, eeaddress, 4, data);
	*read_data  = (data[0] << 24);
  *read_data += (data[1] << 16);
  *read_data += (data[2] <<  8);
  *read_data += (data[3]      ); 
}

//void eeprom_write_alarm(){
//}

