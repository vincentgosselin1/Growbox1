//GROWBOX library, includes all necessery classes. By Vincent Gosselin
#ifndef GROWBOX1_h 
#define GROWBOX1_h

#include "Arduino.h"
#include "dht.h"

class digital_pin
{
	public:
		digital_pin(int pin);
		bool get_state();
		void setinput();
		void setoutput();
		void high();
		void low();
		void blink(unsigned long ms);
		void init();
		bool read_input();
	private:
		int _pin;
		bool _state;
		unsigned long _previousMillis;
};
class analog_pin
{
	public:
		analog_pin(int pin);
		int read_adc();
	private:
		int _pin;	
};
class timer
{
	public:
	timer(const char * array);//Constructor. input is in the format of 00d00h00m00s.
	unsigned long get_count();//Returns count in seconds.
	bool get_status();//Returns status 0 not running.
	bool get_done();//IS timer done? 1 yes.
	bool get_ready_to_count();//IS timer ready to count? 1 yes.
	void set_ready_to_count(bool set);//Sets _ready_to_count value to 1.
	char * get_count_string();//Returns a string in the format of 00d00h00m00s.
	void reset();//Reset timer to its preset.
	void run();//Run timer.
	void stop();//Stop timer.
	void serialprint();//Print timer to serial port.
	void init(char * array);//Init timer at 00d00h00m00s.
	private:
	bool _status;//0 not running, 1 running.
	bool _done;//1 if done.	
	bool _ready_to_count;//1 if got init by using the init(char * array) funct.
	unsigned long _count;//Actual count in seconds.
	char * _countarray;//Actual count in days, hours, minutes, seconds.
	unsigned long _preset;//Preset value in seconds.
	unsigned long _previousMillis;
};
//AC Components
class LED
{	
	public:
		LED(int pin);//Constructor.
		void off();//command to turn off LED.
		void on();//command to turn on LED.
		void on_18h_off_6h();//ON for 18 hours, OFF for 6 hours.
		void on_12h_off_12h();//ON for 12 hours, OFF for 12 hours.
		bool get_status();//to check LED is on or off :).
		int get_pinconfig();
		void init();//Init.
	private:
		digital_pin *_pin;//digital_pin object.
		timer *_timer_18h_on;//timer object.
		timer *_timer_6h_off;//timer object.
		timer *_timer_12h_on;//timer object.
		timer *_timer_12h_off;//timer object.
		bool _status;
		int _pinconfig;//On what pin is LED.

};
class BULB
{
	public:
		BULB(int pin);//Constructor.
		void off();//command to turn off BULB.
		void on();//command to turn on BULB.
		void on_for(char * array);//Expected array format : 00d00h00m00s.
		bool get_status();//Return the status of the BULB 1 ON, 0 OFF.
		bool get_done();
		int get_pinconfig();
		void init();//Init mofooo.
	private:
		digital_pin *_pin;//digital_pin object.
		timer *_timer_on_for;//timer object.
		bool _status;
		bool _done;	
		int _pinconfig;
};
//DC Components 12VDC.
class FAN
{
	public:
		FAN(int pin);//Constructor.
		void off();//Command to turn off FAN.
		void on();//Command to turn ON FAN.
		void on_for(char * array);
		bool get_status();
		bool get_done();
		int get_pinconfig();
		void init();
	private:
		digital_pin *_pin;//digital_pin object.
		timer *_timer_on_for;//timer object.
		bool _status;		
		bool _done;
		int _pinconfig;
};
class VALVE
{
	public:
		VALVE(int pin);//Constructor.
		void close();//Command to close VALVE.
		void open();//Command to open VALVE.
		void open_for(char * array);
		bool get_status();
		bool get_done();
		int get_pinconfig();
		void init();
	private:
		digital_pin *_pin;//digital_pin object.
		timer *_timer_on_for;//timer object.
		bool _status;		
		bool _done;	
		int _pinconfig;
};
//FRONT PANEL
class FRONT_PANEL
{
	public:
		FRONT_PANEL(/* Front panel leds*/
					int FANled, int LEDled, int BULBled, 
					int VALVEled, int Germinationled, int Vegetationled, 
					int Floweringled, int phase_termineeled, int dryled,
					int moistled, int wetled, int nowaterled, int dooropenled,
					/*Front panel Switchs*/
					int FANswitch, int LEDswitch, int modeaswitch, int modebswitch,
					int BULBswitch, int VALVEpushbutton, int automanuelswitch,
					/*LCD*/
					 int lcd_serial_port);

		//Leds on front panel.
		void FAN_led_on();
		void FAN_led_off();
		void FAN_led_blink(unsigned long ms);
		void LED_led_on();
		void LED_led_off();
		void LED_led_blink(unsigned long ms);
		void BULB_led_on();
		void BULB_led_off();
		void BULB_led_blink(unsigned long ms);
		void VALVE_led_on();
		void VALVE_led_off();
		void VALVE_led_blink(unsigned long ms);

		void Germination_mode();
		void Vegetation_mode();
		void Flowering_mode();

		void phase_terminee_led_on();
		void phase_terminee_led_off();

		void dry();
		void moist();
		void wet();

		void no_water_led_on();
		void no_water_led_off();

		void door_led_on();
		void door_led_off();

		void all_led_off();
		void flash(int led_register);// flashes front panel by bits. 16bits, last 4 unused.

		//LCD
		void send_lcd(char * array);//In the format of : 12oC38%rh * MASTER TIMER HERE.
		void display_welcome();
		void display_error();
		void display_almostdone();

		//Switchs
		void scan_switch();//modifies _switch_register.
		char get_switch_register();//returns _switch_register.

		void init();

	private:
		/* Front panel leds*/
		digital_pin *_led1;//digital_pin object. Please see Front panel Image for pin Assignation.
		digital_pin *_led2;//digital_pin object.
		digital_pin *_led3;//digital_pin object.
		digital_pin *_led4;//digital_pin object.
		digital_pin *_led5;//digital_pin object.
		digital_pin *_led6;//digital_pin object.
		digital_pin *_led7;//digital_pin object.
		digital_pin *_led8;//digital_pin object.
		digital_pin *_led9;//digital_pin object.
		digital_pin *_led10;//digital_pin object.
		digital_pin *_led11;//digital_pin object.
		digital_pin *_led12;//digital_pin object.
		digital_pin *_led13;//digital_pin object.
		/*Front panel Switchs*/
		digital_pin *_switch1;//digital_pin object.
		digital_pin *_switch2;//digital_pin object.
		digital_pin *_switch3a;//digital_pin object.
		digital_pin *_switch3b;//digital_pin object.
		digital_pin *_switch4;//digital_pin object.
		digital_pin *_switch5;//digital_pin object.
		digital_pin *_pushbutton1;//digital_pin object.
		//Switch register.
		char _switch_register;//Holds Switch states on front panel. On 8bit (last unsused). (sw1,sw2,sw3a,sw3b,sw4,sw5,pb1, X) 
		int _lcd_serial_port;//0,1,2,3. Serials port on Arduino Due.
		unsigned long _previousMillis;//To allow switch debounce.
};
class SENSORS
{
	public:
		SENSORS(int soil_moisture_pin, int temp_humidity_pin, int door_sensor_pin, int floatswitch_pin);
		void scan_soil_moisture();//soil humidity sensor
		int get_soil_moisture();
		void scan_temp_humidity();// temperature/humidity sensor
		float get_temp();
		float get_humidity();
		void scan_door();
		int get_door();//adc pin!
		void scan_floatswitch();
		bool get_floatswitch();//0 empty, 1 full.
	private:
		analog_pin *_soil_moisture_sensor;
		DHT *_temp_humidity_sensor;
		analog_pin *_door_sensor;
		digital_pin *_reservoir_sensor;

		int _soil_moisture;
		float _temperature;
		float _humidity;// Add ,0 for no decimal!
		int _door;//Adc value boy!
		bool _floatswitch;//1 if reservoir is full, 0 empty!

		unsigned long _previousMillis;
		unsigned long _previousMillis1;
};
class SERVER
{
	public:
		SERVER(int nano_internet_pin);
		void Global_timer_run();
		void Global_timer_reset();
		void set_phase(char p);
		char get_phase();
		char *get_Global_timer();
		void Store_data(/* Components */
						bool FAN_intake, bool FAN_outake, bool LED, bool BULB, bool VALVE,
						/* Sensors */
						int soil_moisture, float temp, float humidity, int door, bool floatswitch);
		char * get_data();
		void Send_data_internet();
		void init();

	private:
		timer *_Global_timer;//For the phases timer!
		char _phase;//Current phase! G, V, F.
		char *_data;//To store data in a big fat array.	
		int _nano_internet_pin;
		unsigned long _previousMillis;//Thats right, sends data to internet every X seconds.

};

#endif












