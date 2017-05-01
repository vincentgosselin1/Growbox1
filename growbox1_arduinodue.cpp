#include <GROWBOX1.h>
#include <arduinoduedht11.h>
#include <arduinotools.h>
//Growbox1_arduinodue Software by Vincent Gosselin, copyright 2016.	

//Last update : 1st of January 2017

//Minimal viable product.

//From arduinotools.h
SmartWait smartWait0;
SmartWait smartWait1;

//ALL CLASSES ARE FROM GROWBOX1.

//Components
Growlight Growlight(13);//PIN 13.
FAN FAN_INTAKE(9);//Pin 9.
FAN FAN_OUTTAKE(10);//PIN 10
//Unused components
BULB BULB(7);
//VALVE VALVE(8);
//Front pannel
Switch FANswitch(53);//PIN 53.
led FANled(50);//pin 50.
Switch Growlightswitch(47);//pin 47
Switch3WAY Growlightphaseswitch(51,49);// pin 51 + pin 49.
led Growlightled(48);//pin 48.
led Germinationled(40);
led Vegetationled(38);
led Floweringled(36);
LCD LCD(3);
//Unused pins, needs to be initialized since they are connected to MCU.
//pin 44,45,42,41,43,34,32,30,28,26,24.
led BULBled(44);
Switch BULBswitch(45);
led VALVEled(42);
Pushbutton pushbutton(41);
Switch VALVEswitch(43);
led phasetermineeled(34);
led secled(32);
led humideled(30);
led trempeeled(28);
led reservoirled(26);
led doorled(24);

//Sensors
dht11 dht11(2);//DHT11 object. PIN to attach to DHT11, here PIN2.

//Growbox1 Functions
void initiate();
void display_front_panel();
void listen_for_manual_commands();
void execute_manual_commands();
void display_welcome();
void send_data_serial();


//Pin config goes here.
void setup()
{
	initiate();
}
void loop()
{	
	//Based on new state machine from January 3rd 2017.
	display_front_panel();
	listen_for_manual_commands();
	execute_manual_commands();

	//Every 10 secs.
	if(smartWait0.wait(10000)) {
  		dht11.scan();
  	}

  	//Every 1 sec.
  	if (smartWait1.wait(1000)) {
  		send_data_serial(); 
  	}

  	
}

void initiate()
{
	//Start Serial communication for debugging.
	Serial.begin(9600);
	Serial.print("Connected");
	//Init components
	Growlight.init();
	BULB.init();
	FAN_INTAKE.init();
	FAN_OUTTAKE.init();
	//LCD message
	display_welcome();	
}
void send_data_serial()
{
	//Once every 10sec
	Serial.println("Start");
	Serial.print("Humidity is ->");			Serial.print(dht11.get_humidity(),DEC); 	Serial.println("%<-");
	Serial.print("Temperature is ->"); 		Serial.print(dht11.get_temperature(),DEC);	Serial.println("oC<-");
	Serial.print("Growlight is ->"); 
		if(Growlight.get_status()==1){ 
			Serial.print("ON");	Serial.println("<-"); 
		} else {
			Serial.print("OFF");	Serial.println("<-");
		}
	Serial.print("Growlight timer is for ->"); 	Serial.print(Growlight.get_phase());	Serial.println("<-");
		if(FAN_INTAKE.get_status()==1){ 
			Serial.print("Fan Intake is ->ON");	Serial.println("<-"); 
		} else {
			Serial.print("Fan Intake is ->OFF");	Serial.println("<-");
		}
		if(FAN_OUTTAKE.get_status()==1){ 
			Serial.print("Fan Outtake is ->ON");	Serial.println("<-"); 
		} else {
			Serial.print("Fan Outtake is ->OFF");	Serial.println("<-");
		}
	Serial.println("End");	

}
void display_welcome()
{
	LCD.send_lcd("Growbox1 by * Vincent Gosselin");
}
void display_front_panel()
{
	if(FAN_INTAKE.get_status()==1 && FAN_OUTTAKE.get_status()==1){ FANled.on(); }	else FANled.off(); 
 	if(Growlight.get_status()==1){ Growlightled.on(); } else Growlightled.off();
 	if(Growlight.get_phase()=='G') { Germinationled.on(); Vegetationled.off(); Floweringled.off();}
 	if(Growlight.get_phase()=='V') { Germinationled.off(); Vegetationled.on(); Floweringled.off();}
 	if(Growlight.get_phase()=='F') { Germinationled.off(); Vegetationled.off(); Floweringled.on();}
}
void listen_for_manual_commands()
{
	FANswitch.listen();
	Growlightswitch.listen();
	Growlightphaseswitch.listen();
}
void execute_manual_commands()
{
	//depending on switch positions.

	//Fans
	if(FANswitch.get_status() == 0)// 0 for ON.
	{
		FAN_INTAKE.on();
		FAN_OUTTAKE.on();
	}
	else
	{
		FAN_INTAKE.off();
		FAN_OUTTAKE.off();
	}

	//Growlight
	if(Growlightswitch.get_status() == 0)//Growlight switch ON.
	{
		if(Growlightphaseswitch.get_position() == 'A')// postion A is for Germination mode (24h)
		{
			Growlight.on_24h();
		}
		else if(Growlightphaseswitch.get_position() == 'B')// postion B is for Flowering mode (12h/12h).
		{
			Growlight.on_12h_off_12h();
		}
		else if(Growlightphaseswitch.get_position() == 'C')// postion B is for Vegetation mode (18h/6h).
		{
			Growlight.on_18h_off_6h();
		}
	}
	else Growlight.off();
}
