#include <GROWBOX1.h>
//DHT11 ARM lib https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino

//Debuging Sessions 31st of October 2016

//Growbox1_arduinodue Software by Vincent Gosselin
//Block schematic : 1. INIT.
//					2. SCAN SENSORS
//					3. STORE DATE
//					4. SEND DATA INTERNET.
//					5. DISPLAY FRONT PANEL
//					6. SCAN SWITCH
//					7. ORDERS		

//ALL CLASSES ARE FROM GROWBOX1.

// //Components
LED LED(13);//PIN 13.
BULB BULB(7);//PIN7
FAN FAN_INTAKE(9);//Pin 9.
FAN FAN_OUTTAKE(10);//PIN 10
VALVE VALVE(8);//PIN 8
Digital_pin * pin24 = new Digital_pin(24);//Pin 24, USB status.
Digital_pin * pin4 = new Digital_pin(4);//Pin 4, USB Mode button.


// //Front panel, inclues door led.
FRONT_PANEL FRONT_PANEL(/* Front panel leds*/
					50, 48, 44, 42, 40, 38, 
					36, 34, 32,
					30, 28, 26, 24,
					/*Front panel Switchs*/
					53, 47, 51, 49,
					45, 41, 43,
					/*LCD*/
					 3);//TX3
//SENSORS
//SENSORS SENSORS(7,2,1,4);
//SERVER
//SERVER SERVER(1);//TX1.

unsigned long _previousMillis = 0;//For LCD ....

//Growbox1 Functions
void initiate();
void store_data();
void send_data_internet();
void display_front_panel();
void scan_switch();
void orders();

//USB MODE additions
bool run_mode;//0 for normal mode (Commands via FRONTPANEL switches), 1 for USB mode (Commands via USB)
bool timer_on_1min_off_1min;//0 for inactive, 1 for active. Need to fix in future with OOP.


//Pin config goes here.
void setup()
{
	initiate();
	pin4->set_input_pullup();
	pin24->init();//USB mode led.

	run_mode = 0;//normal mode (default)
	timer_on_1min_off_1min = 0;//Timer inactive at start.
}
void loop()
{	
	if(run_mode) //USB MODE
	{
		pin24->high();
		wait_for_serial_commands();
		commands();
	}
	else		//FrontPanel Switches
	{
		pin24->low();	
	}
	//scan_sensors();
	//store_data();
	//send_data_internet();
	//display_front_panel();
	//scan_switch();
	//orders();
	if(pin4->read_input() == 0)//switch between normal/usb mode.
	{
		delay(1000);//Time to let button rebounce.
		run_mode = !run_mode;
	}
}
void wait_for_serial_commands()
{
	// Serial commands
	// a -> LED ON/OFF
	// s -> LED ON 1min, OFF 1min.
	// d -> FAN1 ON/OFF
	// f -> Fan2 ON/OFF
	char incomingChar = '0';   // for incoming serial data
	if (Serial.available() > 0) 
	{
    // read the incoming byte:
    incomingChar = Serial.read();
 	// say what you got:
    Serial.print("I received: ");
    Serial.println(incomingChar);
    //Now for orders
    	switch (incomingChar)
    	{
    	case 'a':
    		//toggle LED
    		//LED.toggle();
    		break;
    	case 's':
    		//timer on 1min, off 1min active/Desactivate
    		timer_on_1min_off_1min = !timer_on_1min_off_1min;
    		break;		
    	}
    }
}
void commands()
{
	//LED Commands
	if(timer_on_1min_off_1min)//If timer active.
	{
		LED.on_1min_off_1min();
	}
	else
	{
		LED.off();
	}
}
void initiate()
{
	//Start Serial communication for debugging.
	Serial.begin(9600);
	Serial.print("Connected");
	FRONT_PANEL.init();
	//SERVER.init();
	LED.init();
	BULB.init();
	FAN_INTAKE.init();
	FAN_OUTTAKE.init();
	VALVE.init();

	//FRONT_PANEL.display_welcome();	
}
// void scan_sensors()
// {
// 	SENSORS.scan_soil_moisture();
// 	SENSORS.scan_temp_humidity();
// 	SENSORS.scan_door();
// 	SENSORS.scan_floatswitch();
// }
// void store_data()
// {
// 	SERVER.Store_data(FAN_INTAKE.get_status(),FAN_OUTTAKE.get_status(),LED.get_status(),BULB.get_status(),
// 						VALVE.get_status(),SENSORS.get_soil_moisture(),SENSORS.get_temp(),SENSORS.get_humidity(),
// 						SENSORS.get_door(),SENSORS.get_floatswitch());
// 	Serial.print(SERVER.get_data());
// 	Serial.print("\r\n");
// }
// void send_data_internet()
// {
// 	// TO do....
// }
// void display_front_panel()
// {
	// if(FAN_INTAKE.get_status()==1 && FAN_OUTTAKE.get_status()==1){FRONT_PANEL.FAN_led_on();}	else FRONT_PANEL.FAN_led_off(); 
	// if(LED.get_status()==1){FRONT_PANEL.LED_led_on();} else FRONT_PANEL.LED_led_off();
	// if(SERVER.get_phase() == 'G'){FRONT_PANEL.Germination_mode();}
	// if(SERVER.get_phase() == 'V'){FRONT_PANEL.Vegetation_mode();}
	// if(SERVER.get_phase() == 'F'){FRONT_PANEL.Flowering_mode();}
	// if(BULB.get_status()==1){FRONT_PANEL.BULB_led_on();} else FRONT_PANEL.BULB_led_off();
	// if(VALVE.get_status()==1){FRONT_PANEL.VALVE_led_on();} else FRONT_PANEL.VALVE_led_off();
	// unsigned long currentMillis = millis();
	// if(currentMillis - _previousMillis >1000)//Every 1 sec.
	// {
	// _previousMillis = currentMillis;	
	// Serial3.print(SENSORS.get_temp(),0);
 //    Serial3.print("oC ");
 //    Serial3.print(SENSORS.get_humidity(),0);
 //    Serial3.print("% * Growbox1");
 //    Serial3.print("\n");
	// }
// }
// void scan_switch()
// {
// 	FRONT_PANEL.scan_switch();

// }
// void orders()
// {
// 	//From switch
// 	if(FRONT_PANEL.get_switch_register() & 0x80)//FAN switch ON
// 	{
// 		FAN_INTAKE.on();
// 		FAN_OUTTAKE.on();
// 	}
// 	else
// 	{
// 		FAN_INTAKE.off();
// 		FAN_OUTTAKE.off();
// 	}
// 	if(FRONT_PANEL.get_switch_register() & 0x40)//LED Switch ON
// 	{
// 		if(FRONT_PANEL.get_switch_register() & 0x20)//Germination mode
// 		{
// 			SERVER.set_phase('G');
// 			LED.on();
// 		}
// 		else if(FRONT_PANEL.get_switch_register() & 0x10)//Flowering mode
// 		{
// 			SERVER.set_phase('F');
// 			LED.on_12h_off_12h();
// 		}
// 		else if(FRONT_PANEL.get_switch_register() & 0x08)//Vegetation mode
// 		{
// 			SERVER.set_phase('V');
// 			LED.on_18h_off_6h();
// 		}
// 	}
// 	else LED.off();
// 	if(FRONT_PANEL.get_switch_register() & 0x04)//Bulb switch on
// 	{
// 		BULB.on();
// 	}
// 	else BULB.off();
// 	if(FRONT_PANEL.get_switch_register() & 0x01)//Valve pushbutton pressed.
// 	{
// 		VALVE.open();
// 	}
// 	else VALVE.close();
// }
