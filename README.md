SCL -> PB8
SDA -> PB9

PB6 -> TX
PB7 -> RX


MOTOR
PE7
PE8
PE9
PE10

4 pin Stepper Motor

STEPS | STEP1 | STEP2 | STEP3 | STEP4 |
---------------------------------------
PE7   |   1   |   0   |   0   |   0   |
---------------------------------------
PE8   |   0   |   0   |   1   |   0   |
---------------------------------------
PE9   |   0   |   1   |   0   |   0   |
---------------------------------------
PE10  |   0   |   0   |   0   |   1   |
---------------------------------------

5 pin Stepper Motor

STEPS | STEP1 | STEP2 | STEP3 | STEP4 |
---------------------------------------
PE7   |   0   |   0   |   1   |   1   |
---------------------------------------
PE8   |   1   |   1   |   0   |   0   |
---------------------------------------
PE9   |   1   |   0   |   0   |   1   |
---------------------------------------
PE10  |   0   |   1   |   1   |   0   |
---------------------------------------

//How to write and read data from EEPROM
I2C_writeBytes(0x50<<1, 0, 1, &write_byte);		
I2C_readBytes(0x50<<1, 0, 1, &read_byte);	
if(read_byte == 255)
  STM_EVAL_LEDOn(LED6);
	
//How to get milisecond from time
t.tm_year = 2015-1900;
t.tm_mon = 9;           // Month, 0 - jan
t.tm_mday = 13;          // Day of the month
t.tm_hour = 12-3;     
t.tm_min = 01;
t.tm_sec = 02;
t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
t_of_day = mktime(&t);
printf("seconds since the Epoch: %ld\n", (long) t_of_day);

//How to get date from milisecond
time_t current_time;
time_date = localtime(&current_time);
printf("The Date is : %s", ctime(&current_time));
				
date = (time_date->tm_mday);
month = (time_date->tm_mon)+1;
year = (time_date->tm_year)+1900;
				
dayofweek = (time_date->tm_wday);
if(dayofweek == 0)
	dayofweek = 7;
				
hours = (time_date->tm_hour);
minutes = (time_date->tm_min);
seconds = (time_date->tm_sec);

//How to use json //
char text[] = "{\"foo\":\"balina\",\"quux\":1234}";
json_t *root,*foo,*quux;
int boolean;
char *str_value;
root = json_loads(text, 0, &error);
if(error.line != 0xFFFFFFFF) {
  printf("json error on line %d: %s\n", error.line, error.text);
}
//Method 1 
json_unpack(root, "{s:s, s:b}", "foo", &str_value, "quux", &boolean);
//Method 2 
foo = json_object_get(root, "foo");
str_value = json_string_value(foo);
quux = json_object_get(root, "quux");
boolean = json_integer_value(quux);
