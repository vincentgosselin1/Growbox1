//GROWBOX1 Library, defitions of class.
//Growbox1_arduinodue Software by Vincent Gosselin, copyright 2016.	

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
void Digital_pin::set_input_pullup()
{
	pinMode(_pin,INPUT_PULLUP);
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
	_state = state;
	return state;
}
//led class
led::led(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_pin->low();//puts pin to ground
	_state = 0;
}
void led::on()
{
	_pin->high();
	_state = 1;
}
void led::off()
{
	_pin->low();
	_state = 0;
}
//Pushbutton class
Pushbutton::Pushbutton(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_input_pullup();
	_state = 1;//Off by default.
}
void Pushbutton::set_state()
{
	bool state = _pin->read_input();// ON is 0 (Grounded), OFF is 1 (OPEN)
	_state = state;
}
bool Pushbutton::get_state()
{
	return _state;
}
//Switch class
Switch::Switch(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_input_pullup();
	_status = 1;//Off by default.
}
void Switch::listen()
{
	bool status = _pin->read_input();// ON is 0 (Grounded), OFF is 1 (OPEN)
	_status = status;
}
bool Switch::get_status()
{
	return _status;
}
//Switch3WAY class
Switch3WAY::Switch3WAY(int pinA, int pinB)
{
	_pinA = new Digital_pin(pinA);//Creating object Digital_pin.
	_pinA->set_input_pullup();
	_pinB = new Digital_pin(pinB);//Creating object Digital_pin.
	_pinB->set_input_pullup();
	_position = 'A';//Default
}
void Switch3WAY::listen()
{
	char position;
	bool tempA = _pinA->read_input();
	bool tempB = _pinB->read_input();

	if(tempA==0 && tempB == 1){ position = 'A';}
	if(tempA==1 && tempB == 0){ position = 'B';}
	if(tempA==1 && tempB == 1){ position = 'C';}

	_position = position;
}
char Switch3WAY::get_position()
{
	return _position;
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
	_countarray = new char[12];	
	for(int i=0; i<12; i++)
	{
		_countarray[i] = '0';
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
		//Condition to break overflow that happens after 50days when previousmillis is greater than currentmillis.
		if(_previousMillis > currentMillis)
		{
			_previousMillis = 0;
		}
		//Counts 1 sec.
		if(currentMillis - _previousMillis >= 1000)//1000ms = 1sec.
		{
			//Count my dear.
 			_count--;
 			_previousMillis=currentMillis;
 			//print to serial
 			serialprint();            
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
	Serial.println(get_count_string());
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
Growlight::Growlight(int pin)
{
	_pin = new Digital_pin(pin);//Creating object Digital_pin.
	_pin->set_output();//Pin will be an output.
	_timer_18h_on = new timer("00d18h00m00s");//18 hours ON.
	_timer_6h_off = new timer("00d06h00m00s");//6 hours OFF.
	_timer_12h_on = new timer("00d12h00m00s");//12 hours ON.
	_timer_12h_off = new timer("00d12h00m00s");//12 hours OFF.
	//Test timer
	_timer_1min_on = new timer("00d00h01m00s");//1min ON.
	_timer_1min_off = new timer("00d00h01m00s");//1min OFF.

	_status=0;//Growlight is off at start.
	_phase = 'G';//Germination by default.
	_pinconfig = pin;//On what pin is the Growlight.
}
void Growlight::off()
{
	_pin->high();//Growlight IS ON WHEN digital pin is LOW.
	_status=0;//Growlight is off.
}
void Growlight::on()
{
	_pin->low();//Growlight IS ON WHEN digital pin is LOW.
	_status=1;//Growlight is on.
}
void Growlight::on_1min_off_1min()
{
	if(_timer_1min_on->get_done()==0)
    {
        _timer_1min_on->run();
        on();//Growlight ON.
    }
    else if(_timer_1min_on->get_done()==1 && _timer_1min_off->get_done()==0)
    {
        _timer_1min_off->run();
        off();//Growlight OFF.
    }
    else
    {
        _timer_1min_on->reset();
        _timer_1min_off->reset();
    }
}
void Growlight::on_24h()
{
	_phase = 'G'; //mode Germination
	on();
}
void Growlight::on_18h_off_6h()
{
	_phase = 'V'; //mode Vegetation
	if(_timer_18h_on->get_done()==0)
    {
        _timer_18h_on->run();
        on();//Growlight ON.
    }
    else if(_timer_18h_on->get_done()==1 && _timer_6h_off->get_done()==0)
    {
        _timer_6h_off->run();
        off();//Growlight OFF.
    }
    else
    {
        _timer_18h_on->reset();
        _timer_6h_off->reset();
    }
}
void Growlight::on_12h_off_12h()
{
	_phase = 'F'; //mode flowering
	if(_timer_12h_on->get_done()==0)
	{
		_timer_12h_on->run();
		on();
	}
	else if(_timer_12h_on->get_done()==1 && _timer_12h_off->get_done()==0)
	{
		_timer_12h_off->run();
		off();
	}
	else
    {
        _timer_12h_on->reset();
        _timer_12h_off->reset();
    }
}
void Growlight::init()//Init.
{
	off();//Growlight OFF at start.
}
bool Growlight::get_status()
{
	return _status;
}
char Growlight::get_phase()
{
	return _phase;
}
int Growlight::get_pinconfig()
{
	return _pinconfig;
}
void Growlight::toggle()
{
	bool current_state = _status;
	if(current_state == 1)//If Growlight ON
	{
		off();//turn it off
	}
	else //If Growlight OFF
	{
		on();//turn it on
	}
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

LCD::LCD(int lcd_serial_port)
{
	_lcd_serial_port = lcd_serial_port;
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
void LCD::send_lcd(char * array)
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


