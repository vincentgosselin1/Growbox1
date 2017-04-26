//GROWBOX library, includes all necessery classes. By Vincent Gosselin


//This File should copied to Documents/Arduino/Libraries/GROWBOX1/GROWBOX1.h for changes to happen in the Arduino.
#ifndef GROWBOX1_h 
#define GROWBOX1_h

#include "Arduino.h"

class Digital_pin
{
	public:
		Digital_pin(int pin);
		bool get_state();
		void set_input();
		void set_input_pullup();
		void set_output();
		void high();
		void low();
		void blink();
		void init();
		bool read_input();
	private:
		int _pin;
		bool _state;
		unsigned long _previousMillis;
};
class led
{
	public:
		led(int pin);
		void on();
		void off();
	private:
		Digital_pin *_pin;//Digital_pin object.
		bool _state;
};
class Pushbutton
{
	public:
		Pushbutton(int pin);
		void set_state();
		bool get_state();
	private:
		Digital_pin *_pin;//Digital_pin object.
		bool _state;
};
class Switch
{
	public:
		Switch(int pin);
		void listen();
		bool get_status();// 0 -> ON, 1 -> OFF.
	private:
	Digital_pin *_pin;//Digital_pin object.
	bool _status;
};
class Switch3WAY
{
	public:
	Switch3WAY(int pinA, int pinB);
	void listen();
	char get_position();// pinA=0,pinB=1 -> A (X,0,0)
						// pinA=1,pinB=0 -> B (0,0,X)
						// pinA=1,pinB=1 -> C (0,X,0)

	private:
	Digital_pin *_pinA;//Digital_pin object.
	Digital_pin *_pinB;//Digital_pin object.
	char _position;// Position A, B, C. 	

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
	timer(const char * array);//Constructor. String is in the format of 00d00h00m00s.
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
class Growlight//AC component of the growbox, replaces LED.
{	
	public:
		Growlight(int pin);//Constructor.
		void off();//command to turn off Growlight.
		void on();//command to turn on Growlight.
		void on_24h();;//On for 24 hours. (Germination mode)
		void on_18h_off_6h();//ON for 18 hours, OFF for 6 hours. (Vegetation mode)
		void on_12h_off_12h();//ON for 12 hours, OFF for 12 hours. (Flowering mode)

		void on_1min_off_1min();//On for 1 min, off 1min. FOR TESTS

		bool get_status();//to check Growlight is on or off :).
		char get_phase();//returns current phase of the growlight.
		int get_pinconfig();
		void init();//Init.
		void toggle();//ON->OFF,OFF->ON.
	private:
		Digital_pin *_pin;//Digital_pin object.
		timer *_timer_18h_on;//timer object.
		timer *_timer_6h_off;//timer object.
		timer *_timer_12h_on;//timer object.
		timer *_timer_12h_off;//timer object.
		timer *_timer_1min_on;//timer tests
		timer *_timer_1min_off;
		bool _status;// 0 -> OFF, 1 -> ON.
		char _phase; // G, V, F.
		int _pinconfig;//On what pin is Growlight.

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
		Digital_pin *_pin;//Digital_pin object.
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
		Digital_pin *_pin;//Digital_pin object.
		timer *_timer_on_for;//timer object.
		bool _status;		
		bool _done;
		int _pinconfig;
};
class  LCD
{
	public:
		LCD(int lcd_serial_port);
		void send_lcd(char * array);//In the format of : 12oC38%rh * MASTER TIMER HERE.

	private:
		int _lcd_serial_port;	
};
#endif
