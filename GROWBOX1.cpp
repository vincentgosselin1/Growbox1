//GROWBOX1 Library, defitions of class.

//This File should copied to Documents/Arduino/Libraries/GROWBOX1/GROWBOX1.cpp for changes to happen in the Arduino.

#include "Arduino.h"
#include "GROWBOX1.h"
//Digital pin class.
Digital_pin::Digital_pin(int pin)
{
	_pin=pin;
	_state=0;//LOW at init.
	_previousMillis=0;//0 a start;
}
bool Digital_pin::get_state()
{
	return _state;
}
void Digital_pin::set_input()
{
	pinMode(_pin,INPUT);
}
void Digital_pin::set_output()
{
	pinMode(_pin,OUTPUT);
}
void Digital_pin::high()
{
	digitalWrite(_pin,1);
	_state=1;
}
void Digital_pin::low()
{
	digitalWrite(_pin,0);
	_state=0;
}
void Digital_pin::blink()
{
	unsigned long currentMillis = millis();
	if(currentMillis - _previousMillis >= 300)
	{
		_previousMillis = currentMillis;
		digitalWrite(_pin,_state);
		_state=!_state;
	}
	else;
}
void Digital_pin::init()
{
	set_output();
	low();
}
bool Digital_pin::read_input()
{
	bool state;
	state = digitalRead(_pin);
	return state;
}
analog_pin::analog_pin(int pin)
{
	_pin=pin;
}
int analog_pin::read_adc()
{
	return analogRead(_pin);
}
timer::timer(const char * array)
{
//Separate the char array and converts into seconds
	int days = (array[0] - '0')*10 + (array[1] - '0');//for days.
	int hours = (array[3] - '0')*10 + (array[4] - '0');//for hours.

	int minutes = (array[6] - '0')*10 + (array[7] - '0');//for minutes.
	int seconds = (array[9] - '0')*10 + (array[10] - '0');//for seconds.
//_preset gets initialized to the preset.
 	_preset = days*24*3600 + hours*3600 + minutes*60 + seconds;
	_previousMillis = 0;//no previous Millis.
	_status = 0;//Timer doesn't run at start.
	_done = 0;//Timer not done.
	_ready_to_count=0;//Used in the on_for functions of each classes.
//Reset timer at init.	
	reset();//Reset timer at init.
// Init array
	_countarray = new char[13];	
	for(int i=0; i<12; i++)
	{
		_countarray[i] = '0';
		_countarray[i+1] = '\0';//Ending string with NUL char.
	}
}

unsigned long timer::get_count()
{
	return _count;
}
char * timer::get_count_string()
{
//getting nb of days, hours, minutes, seconds.	
 	unsigned long count = _count;//in seconds.
	//nb of days, hours, minutes, seconds. all init at 0.
	int days=0;
	int hours=0;
	int minutes=0;

	//days nb.
	while(count>=24*3600)
	{
		count=count-(24*3600);
		days++;
	}
	//hours nb.
	while(count>=3600)
	{
		count=count-(3600);
		hours++;
	}
	//minutes nb.
	while(count>=60)
	{
		count=count-(60);
		minutes++;
	}
	//Count is in sec now.


//Building up _countarray
	int temp=0;
	while(days>=10)
	{
		days=days-10;
		temp++;
	}
	_countarray[0]= temp + '0';
	_countarray[1]=days + '0';
	_countarray[2]='d';
	temp=0;//Yes, I'm ready to do it again my master....
	while(hours>=10)
	{
		hours=hours-10;
		temp++;
	}
	_countarray[3]=temp+'0';
	_countarray[4]=hours + '0';
	_countarray[5]='h';
	temp=0;//PLEASE SOMEONE HELP ME.
	while(minutes>=10)
	{
		minutes=minutes-10;
		temp++;
	}
	_countarray[6]=temp+'0';
	_countarray[7]=minutes +'0';
	_countarray[8]='m';
	temp=0;//Ok, this is better be the last one...
	while(count>=10)
	{
		count=count-10;
		temp++;
	}	
	_countarray[9]=temp+'0';
	_countarray[10]=count+'0';
	_countarray[11]='s';
//Perfectooooooooo Copyright for Vincent Gosselin, Anyone cloning this code will owe me 100$.

return _countarray;
}
bool timer::get_status()
{
	return _status;
}
bool timer::get_done()
{
	return _done;
}
bool timer::get_ready_to_count()
{
	return _ready_to_count;
}
void timer::set_ready_to_count(bool set)
{
	_ready_to_count=set;
}
void timer::reset()
{
	_count=_preset;//
	_done=0;
	_status=0;
}
void timer::run()
{
	if(_count>0)//timer is not done.
	{
		_status = 1;
		unsigned long currentMillis = millis();
		if(currentMillis - _previousMillis >= 1000)//1000ms = 1sec.
		{
			//Count my dear.
 			_count--;
 			_previousMillis=currentMillis;            
		}
		else;//do nothing.
	}
	else//timer is done.
	{	
	_status = 0;
	_done = 1;
	}
}
void timer::serialprint()
{
	Serial.print(get_count_string());
	Serial.print("\r\n");
}
void timer::init(char * array)
{
	//Separate the char array and converts into seconds
	int days = (array[0] - '0')*10 + (array[1] - '0');//for days.
	int hours = (array[3] - '0')*10 + (array[4] - '0');//for hours.

	int minutes = (array[6] - '0')*10 + (array[7] - '0');//for minutes.
	int seconds = (array[9] - '0')*10 + (array[10] - '0');//for seconds.
	//_preset gets initialized to the preset.
 	_preset = days*24*3600 + hours*3600 + minutes*60 + seconds;
	_previousMillis = 0;//no previous Millis.
	_status = 0;//Timer doesn't run at start.
	_done = 0;//Timer not done.
	//Reset timer at init.	
	reset();//Reset timer at init.
}



//COMPONENTS CLASSES
LED::LED(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_timer_18h_on = new timer("00d18h00m00s");//18 hours ON.
	_timer_6h_off = new timer("00d06h00m00s");//6 hours OFF.
	_timer_12h_on = new timer("00d12h00m00s");//12 hours ON.
	_timer_12h_off = new timer("00d12h00m00s");//12 hours OFF.
	_status=0;//LED is off at start.
	_pinconfig = pin;//On what pin is the LED.
}
void LED::off()
{
	_pin->high();//LED IS ON WHEN digital pin is LOW.
	_status=0;//LED is off.
}
void LED::on()
{
	_pin->low();//LED IS ON WHEN digital pin is LOW.
	_status=1;//LED is on.
}
void LED::on_18h_off_6h()
{
	if(_timer_18h_on->get_done()==0)
    {
        _timer_18h_on->serialprint();
        _timer_18h_on->run();
        on();//LED ON.
    }
    else if(_timer_18h_on->get_done()==1 && _timer_6h_off->get_done()==0)
    {
        _timer_6h_off->serialprint();
        _timer_6h_off->run();
        off();//LED OFF.
    }
    else
    {
        _timer_18h_on->reset();
        _timer_6h_off->reset();
    }
}
void LED::on_12h_off_12h()
{
	if(_timer_12h_on->get_done()==0)
	{
		_timer_12h_on->serialprint();
		_timer_12h_on->run();
		on();
	}
	else if(_timer_12h_on->get_done()==1 && _timer_12h_off->get_done()==0)
	{
		_timer_12h_off->serialprint();
		_timer_12h_off->run();
		off();
	}
	else
    {
        _timer_12h_on->reset();
        _timer_12h_off->reset();
    }
}
void LED::init()//Init.
{
	off();//LED OFF at start.
}
bool LED::get_status()
{
	return _status;
}
int LED::get_pinconfig()
{
	return _pinconfig;
}
BULB::BULB(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_timer_on_for = new timer("00d00h00m00s");//on_for default.
	_status = 0;//BULB is off at start.
	_done = 0;
	_pinconfig = pin;
}
void BULB::on()
{
	_pin->low();//BULB IS ON WHEN digital pin is LOW.
	_status=1;//BULB is on.
}
void BULB::off()
{
	_pin->high();//BULB IS ON WHEN digital pin is LOW.
	_status=0;//LED is off.
}
void BULB::init()
{
	off();//OFF at init.
}
void BULB::on_for(char * array)
{
	if(_timer_on_for->get_ready_to_count()==0)
	{
		_done = 0;
		_timer_on_for->init(array);
		_timer_on_for->set_ready_to_count(1);
	}
	else if(_timer_on_for->get_ready_to_count()==1 && _timer_on_for->get_done()==0)
	{
		_timer_on_for->run();
		_timer_on_for->serialprint();
		on();
	}
	else 
	{
		_timer_on_for->set_ready_to_count(0);
		off();
		_done = 1;
	}
}
bool BULB::get_status()
{
	return _status;
}
bool BULB::get_done()
{
	return _done;
}
int BULB::get_pinconfig()
{
	return _pinconfig;
}
//DC Components.
FAN::FAN(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_timer_on_for = new timer("00d00h00m00s");//on_for default.
	_done = 0;//FAN is not done at start :PPP.
	_status=0;//FAN is off at start.
	_pinconfig = pin;
}
void FAN::on()
{
	_pin->low();//FAN IS ON WHEN digital pin is LOW.
	_status=1;//FAN is on.
}
void FAN::off()
{
	_pin->high();//FAN IS ON WHEN digital pin is LOW.
	_status=0;//FAN is off.
}
void FAN::init()
{
	off();//OFF at init.
}
void FAN::on_for(char * array)
{
	if(_timer_on_for->get_ready_to_count()==0)
	{
		_done = 0;
		_timer_on_for->init(array);
		_timer_on_for->set_ready_to_count(1);
	}
	else if(_timer_on_for->get_ready_to_count()==1 && _timer_on_for->get_done()==0)
	{
		_timer_on_for->run();
		_timer_on_for->serialprint();
		on();
	}
	else 
	{
		_timer_on_for->set_ready_to_count(0);
		_done = 1;
		off();
	}
}
bool FAN::get_status()
{
	return _status;
}
bool FAN::get_done()
{
	return _done;
}
int FAN::get_pinconfig()
{
	return _pinconfig;
}
VALVE::VALVE(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_timer_on_for = new timer("00d00h00m00s");//on_for default.
	_done = 0;//VALVE is not done at start :PPP.
	_status=0;//VALVE is off at start.
	_pinconfig = pin;
}
void VALVE::open()
{
	_pin->low();//VALVE IS Open WHEN digital pin is LOW.
	_status=1;//VALVE is open.
}
void VALVE::close()
{
	_pin->high();//VALVE IS OPEN WHEN digital pin is LOW.
	_status=0;//FAN is closed.
}
void VALVE::init()
{
	close();
}
void VALVE::open_for(char * array)
{
	if(_timer_on_for->get_ready_to_count()==0)
	{
		_done = 0;
		_timer_on_for->init(array);
		_timer_on_for->set_ready_to_count(1);
	}
	else if(_timer_on_for->get_ready_to_count()==1 && _timer_on_for->get_done()==0)
	{
		_timer_on_for->run();
		_timer_on_for->serialprint();
		open();
	}
	else 
	{
		_timer_on_for->set_ready_to_count(0);
		_done = 1;
		close();
	}
}
bool VALVE::get_status()
{
	return _status;
}
bool VALVE::get_done()
{
	return _done;
}
int VALVE::get_pinconfig()
{
	return _pinconfig;
}
FRONT_PANEL::FRONT_PANEL(/* Front panel leds*/
					int FANled, int LEDled, int BULBled, 
					int VALVEled, int Germinationled, int Vegetationled, 
					int Floweringled, int phase_termineeled, int dryled,
					int moistled, int wetled, int nowaterled, int dooropenled,
					/*Front panel Switchs*/
					int FANswitch, int LEDswitch, int modeaswitch, int modebswitch,
					int BULBswitch, int VALVEpushbutton, int automanuelswitch,
					/*LCD*/
					int lcd_serial_port)
{
	//INITIATE ALL PINS TO DESIRED INITIAL CONFIG.
	//Front Panel leds.
	_led1 = new Digital_pin(FANled);
	_led1->set_output();
	_led1->init();
	_led2 = new Digital_pin(LEDled);
	_led2->set_output();
	_led2->init();
	_led3 = new Digital_pin(BULBled);
	_led3->set_output();
	_led3->init();
	_led4 = new Digital_pin(VALVEled);
	_led4->set_output();
	_led4->init();
	_led5 = new Digital_pin(Germinationled);
	_led5->set_output();
	_led5->init();
	_led6 = new Digital_pin(Vegetationled);
	_led6->set_output();
	_led6->init();
	_led7 = new Digital_pin(Floweringled);
	_led7->set_output();
	_led7->init();
	_led8 = new Digital_pin(phase_termineeled);
	_led8->set_output();
	_led8->init();
	_led9 = new Digital_pin(dryled);
	_led9->set_output();
	_led9->init();
	_led10 = new Digital_pin(moistled);
	_led10->set_output();
	_led10->init();
	_led11 = new Digital_pin(wetled);
	_led11->set_output();
	_led11->init();
	_led12 = new Digital_pin(nowaterled);
	_led12->set_output();
	_led12->init();
	_led13 = new Digital_pin(dooropenled);
	_led13->set_output();
	_led13->init();
	//Front Panel Switchs
	_switch1 = new Digital_pin(FANswitch);
	_switch1->set_input();
	_switch2 = new Digital_pin(LEDswitch);
	_switch2->set_input();
	_switch3a = new Digital_pin(modeaswitch);
	_switch3a->set_input();
	_switch3b = new Digital_pin(modebswitch);
	_switch3b->set_input();
	_switch4 = new Digital_pin(BULBswitch);
	_switch4->set_input();
	_switch5 = new Digital_pin(automanuelswitch);
	_switch5->set_input();
	_pushbutton1 = new Digital_pin(VALVEpushbutton);
	_pushbutton1->set_input();
	//LCD
	_lcd_serial_port = lcd_serial_port;
	//Switchs
	_switch_register = 0x00;//
}
//leds
void FRONT_PANEL::FAN_led_on()
{
	_led1->high();
}
void FRONT_PANEL::FAN_led_off()
{
	_led1->low();
}
void FRONT_PANEL::FAN_led_blink()
{
	_led1->blink();
}
void FRONT_PANEL::LED_led_on()
{
	_led2->high();
}
void FRONT_PANEL::LED_led_off()
{
	_led2->low();
}
void FRONT_PANEL::LED_led_blink()
{
	_led2->blink();
}
void FRONT_PANEL::BULB_led_on()
{
	_led3->high();
}
void FRONT_PANEL::BULB_led_off()
{
	_led3->low();
}
void FRONT_PANEL::BULB_led_blink()
{
	_led3->blink();
}
void FRONT_PANEL::VALVE_led_on()
{
	_led4->high();
}
void FRONT_PANEL::VALVE_led_off()
{
	_led4->low();
}
void FRONT_PANEL::VALVE_led_blink()
{
	_led4->blink();
}
void FRONT_PANEL::Germination_mode()
{
	_led5->high();
	_led6->low();
	_led7->low();
}
void FRONT_PANEL::Vegetation_mode()
{
	_led5->low();
	_led6->high();
	_led7->low();
}
void FRONT_PANEL::Flowering_mode()
{
	_led5->low();
	_led6->low();
	_led7->high();
}
void FRONT_PANEL::phase_terminee_led_on()
{
	_led8->high();
}
void FRONT_PANEL::phase_terminee_led_off()
{
	_led8->low();
}
void FRONT_PANEL::dry()
{
	_led9->high();
	_led10->low();
	_led11->low();
}
void FRONT_PANEL::moist()
{
	_led9->low();
	_led10->high();
	_led11->low();
}
void FRONT_PANEL::wet()
{
	_led9->low();
	_led10->low();
	_led11->high();
}
void FRONT_PANEL::no_water_led_on()
{
	_led12->high();
}
void FRONT_PANEL::no_water_led_off()
{
	_led12->low();
}
void FRONT_PANEL::door_led_on()
{
	_led13->high();
}
void FRONT_PANEL::door_led_off()
{
	_led13->low();
}
void FRONT_PANEL::all_led_off()
{
	_led1->low();
	_led2->low();
	_led3->low();
	_led4->low();
	_led5->low();
	_led6->low();
	_led7->low();
	_led8->low();
	_led9->low();
	_led10->low();
	_led11->low();
	_led12->low();
	_led13->low();

}
void FRONT_PANEL::flash(int led_register)//Last 3 bits unused.
{	
	all_led_off();
	if(led_register & 0x8000){_led1->high();}
	if(led_register & 0x4000){_led2->high();}
	if(led_register & 0x2000){_led3->high();}
	if(led_register & 0x1000){_led4->high();}
	if(led_register & 0x0800){_led5->high();}
	if(led_register & 0x0400){_led6->high();}
	if(led_register & 0x0200){_led7->high();}
	if(led_register & 0x0100){_led8->high();}
	if(led_register & 0x0080){_led9->high();}
	if(led_register & 0x0040){_led10->high();}
	if(led_register & 0x0020){_led11->high();}
	if(led_register & 0x0010){_led12->high();}
	if(led_register & 0x0008){_led13->high();}
}
//LCD
void FRONT_PANEL::send_lcd(char * array)
{
	switch (_lcd_serial_port)
	{
		case 0:
		Serial.print(array);
		Serial.print("\n");
		break;
		case 1:
		Serial1.print(array);
		Serial1.print("\n");
		break;
		case 2:
		Serial2.print(array);
		Serial2.print("\n");
		break;
		case 3:
		Serial3.print(array);
		Serial3.print("\n");
		break;
	}
}
void FRONT_PANEL::display_welcome()
{
	delay(1000);
	//Welcome!!! My Friend
	send_lcd(" Welcome!!! * ");
	delay(1000);
	send_lcd(" Welcome!!! * M ");
	delay(1000);
	send_lcd(" Welcome!!! * My ");
	delay(1000);
	send_lcd(" Welcome!!! * My F");
	delay(1000);
	send_lcd(" Welcome!!! * My Fr");
	delay(1000);
	send_lcd(" Welcome!!! * My Fri");
	delay(1000);
	send_lcd(" Welcome!!! * My Frie");
	delay(1000);
	send_lcd(" Welcome!!! * My Frien");
	delay(1000);
	send_lcd(" Welcome!!! * My Friend");
	delay(1000);
	//GrowBox1 by Vincent Gosselin
	send_lcd(" GrowBox1 * By ");
	delay(5000);
	send_lcd(" Vincent * Gosselin ");
	delay(1000);

	int uperbooster = 80; 
	for(int y=0;y<5;y++)//Flash 20 times full panel.
	{
		for(int i=3; i<16; i++)
		{
		flash(1<<i);
		delay(uperbooster);
		uperbooster--;
		}
	}
}
void FRONT_PANEL::display_error()
{
	send_lcd(" Error! * ");
}
void FRONT_PANEL::display_almostdone()
{
	send_lcd(" Almost * Done!"); 
}
//Switchs
void FRONT_PANEL::scan_switch()
{
	//Scan switch every 100ms to allow switch debounce.
	unsigned long currentMillis = millis();
	if(currentMillis - _previousMillis > 100)//Every 100ms.
	{
	//When SWITCH IS ON, PIN IS GROUNDED.
	char temp = 0x00;//Temp for generation of _switch_register.
	//Switch1.
	if(_switch1->read_input()==0)/*Switch1 ON*/{temp = 0x80;}//1000 000X for Switch1.
	else						/*Switch1 OFF*/{temp = 0x00;}//0000 000X for Switch1.
	//Switch2.
	if(_switch2->read_input()==0)/*Switch2 ON*/{temp = 0x40 | temp;}//0100 000X for Switch2.
	else						/*Switch2 OFF*/{temp = 0x00 | temp;}//0000 000X for Switch2.
	//Switch3a POSITION ON 24H 
	if(_switch3a->read_input()==0 && _switch3b->read_input()==1)/*Switch3A on 24h position*/{temp = 0x20 | temp;}
	else													/*Switch3a NOT on 24h position*/{temp = 0x00 | temp;}
	//Switch3b POSITION ON 12h/12h
	if(_switch3b->read_input()==0 && _switch3a->read_input()==1)/*Switch3B on 12h12h position*/{temp = 0x10 | temp;}
	else													/*Switch3b not on 12h12h position*/{temp = 0x00 | temp;}
	//Switch3c Position ON 16h/8h
	if(_switch3b->read_input()==1 && _switch3a->read_input()==1)/*Switch3C on 16h8h position*/{temp = 0x08 | temp;}
	else														/*Switch3C not on 16h8h position*/{temp = 0x00 | temp;}
	//Switch4 
	if(_switch4->read_input()==0)/*Switch4 ON*/{temp = 0x04 | temp;}
	else						/*Switch4 OFF*/{temp = 0x00 | temp;}
	//Switch5
	if(_switch5->read_input()==0)/*Switch5 ON*/{temp = 0x02 | temp;}
	else						/*Switch6 OFF*/{temp = 0x00 | temp;}
	//Pushbutton1
	if(_pushbutton1->read_input()==0)/*Pushbutton is pressed*/{temp = 0x01 | temp;}
	else							/*Pushbutton is not pressed*/{temp = 0x00 | temp;}

	//Pceee mofooo.

	//Pow pow modifies register. We are in business my dear.
	_switch_register = temp;
	}
}
char FRONT_PANEL::get_switch_register()
{
	return _switch_register;
}
void FRONT_PANEL::init()
{
	switch (_lcd_serial_port)
	{
		case 0:
		Serial.begin(9600);
		break;
		case 1:
		Serial1.begin(9600);
		break;
		case 2:
		Serial2.begin(9600);
		break;
		case 3:
		Serial3.begin(9600);
		break;
	}
}
SENSORS::SENSORS(int soil_moisture_pin, int temp_humidity_pin, int door_sensor_pin, int floatswitch_pin)
{
	_soil_moisture_sensor = new analog_pin(soil_moisture_pin);
	_temp_humidity_sensor = new DHT(temp_humidity_pin, 11);
	_door_sensor = new analog_pin(door_sensor_pin);
	_reservoir_sensor = new Digital_pin(floatswitch_pin);
	_reservoir_sensor->set_input();
	_previousMillis = 0;//yess
	_previousMillis1 = 0;//Uhhh, yes I guess.
	_soil_moisture = 0;
	_temperature = 0;
	_humidity= 0;// Add ,0 for no decimal!
	_door = 0;//Adc value boy!
	_floatswitch = 0;//1 if reservoir is full, 0 empty!
}
void SENSORS::scan_soil_moisture()
{
	unsigned long currentMillis = millis();
	if(currentMillis - _previousMillis1 > 1000)//Once every 1 sec.
	{
	_previousMillis1 = currentMillis;
	_soil_moisture = _soil_moisture_sensor->read_adc();
	}
}
int SENSORS::get_soil_moisture()
{
	return _soil_moisture;
}
void SENSORS::scan_temp_humidity()
{
	unsigned long currentMillis = millis();
	if(currentMillis- _previousMillis>1000)//once every 1sec.
	{
	_previousMillis = currentMillis;
	_temperature = _temp_humidity_sensor->readTemperature();
	_humidity = _temp_humidity_sensor->readHumidity();
	}
}
float SENSORS::get_temp()
{
	return _temperature;
}
float SENSORS::get_humidity()
{
	return _humidity;
}
void SENSORS::scan_door()
{
	_door = _door_sensor->read_adc();
}
int SENSORS::get_door()
{
	return _door;
}
void SENSORS::scan_floatswitch()
{
	_floatswitch = _reservoir_sensor->read_input();
}
bool SENSORS::get_floatswitch()
{
	return _floatswitch;
}
SERVER::SERVER(int nano_internet_pin)
{
	_Global_timer = new timer("28d00h00m00s");//Thats right init it that way dawg.
	_phase = 'G';//Default
	_data = new char[500];//Thats right, nice big fatt array.
	_nano_internet_pin = nano_internet_pin;
	_previousMillis = 0;
}
void SERVER::Global_timer_run()
{
	if(_Global_timer->get_done()==0)
	{
		_Global_timer->run();
	}
}
void SERVER::Global_timer_reset()
{
	switch (_phase)
	{
		case 'G':
		_Global_timer->init("28d00h00m00s");
		break;
		case 'V':
		_Global_timer->init("30d00h00m00s");
		break;
		case 'F':
		_Global_timer->init("40d00h00m00s");
		break;
	}
}
void SERVER::set_phase(char p)
{
	_phase = p;
}
char SERVER::get_phase()
{
	return _phase;
}
char * SERVER::get_Global_timer()
{
	return _Global_timer->get_count_string();
}
void SERVER::Store_data(/* Components */
						bool FAN_intake, bool FAN_outake, bool LED, bool BULB, bool VALVE,
						/* Sensors */
						int soil_moisture, float temp, float humidity, int door, bool floatswitch)
{
	
	/*First One.*/strncpy(_data,"FAN_intake is ",sizeof("FAN_intake is ")); if(FAN_intake == 1) {strcat(_data,"ON ,");} else strcat(_data,"OFF ,");
	/*The rest.*/	strcat(_data, " FAN_outake is ");						if(FAN_outake == 1)	{strcat(_data,"ON ,");} else strcat(_data,"OFF ,");
					strcat(_data, " LED is ");						if(LED == 1)	{strcat(_data,"ON ,");} else strcat(_data,"OFF ,");
					strcat(_data, " BULB is ");						if(BULB == 1)	{strcat(_data,"ON ,");} else strcat(_data,"OFF ,");
					strcat(_data, " VALVE is ");						if(VALVE == 1)	{strcat(_data,"ON ,");} else strcat(_data,"OFF ,");

	/*Bunch of sensors*/
					/*Moisture sensor*/ 
					String str;
					char array[5];
					str = String(soil_moisture);
					str.toCharArray(array,5);
					strcat(_data, " soil_moisture is ");strcat(_data, array);strcat(_data, " ,");	
					/*Temperature*/
					String str1;
					char array1[3];
					str1 = String(temp);
					str1.toCharArray(array1,3);
					strcat(_data, " Temperature is ");strcat(_data, array1);strcat(_data, "oC ,");
					/*Humidity*/
					String str2;
					char array2[3];
					str2 = String(humidity);
					str2.toCharArray(array2,3);
					strcat(_data, " Humidity is ");strcat(_data, array2);strcat(_data, "% ,");
					/*Door sensor*/
					String str3;
					char array3[5];
					str3 = String(door);
					str3.toCharArray(array3,5);
					strcat(_data, " Door is ");strcat(_data, array3);strcat(_data, " ,");
					/*Reservoir*/
					strcat(_data, " Reservoir is ");						if(floatswitch == 1)	{strcat(_data,"Full ,");} else strcat(_data,"Empty ,");


}
char * SERVER::get_data()
{
	return _data;
}
void SERVER::Send_data_internet()
{
	unsigned long currentMillis = millis();
	if(currentMillis - _previousMillis > 5000)
	{
		_previousMillis = currentMillis;
		switch (_nano_internet_pin)
		{
			case 0:
			Serial.print("START : ");
			Serial.print(_data);
			Serial.print(" END\r\n");
			break;
			case 1:
			Serial1.print("START : ");
			Serial1.print(_data);
			Serial1.print(" END\r\n");
			break;
			case 2:
			Serial2.print("START : ");
			Serial2.print(_data);
			Serial2.print(" END\r\n");
			break;
			case 3:
			Serial3.print("START : ");
			Serial3.print(_data);
			Serial3.print(" END\r\n");
			break;
		}
	}
}
void SERVER::init()
{
	switch (_nano_internet_pin)
	{
		case 0:
		Serial.begin(9600);
		break;
		case 1:
		Serial1.begin(9600);
		break;
		case 2:
		Serial2.begin(9600);
		break;
		case 3:
		Serial3.begin(9600);
		break;
	}
}