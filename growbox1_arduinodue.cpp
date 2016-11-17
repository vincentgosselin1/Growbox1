#include <GROWBOX1.h>
//Growbox1_arduinodue Software by Vincent Gosselin, copyright 2016.	

//Last update : 1st of November 2016

//No more Valve for water injection, reservoir with floatswitch (no point),
//pushbutton to active valve, led for valve, led for floatswitch, switch for automatic/manual mode.

//Plant grows in soil, no need for automatic water. 
//Cellphone App tells you how to grow like a pro.

//This is not a hydroponic system.


//ALL CLASSES ARE FROM GROWBOX1.

// //Components
LED LED(13);//PIN 13.
BULB BULB(7);//PIN7
FAN FAN_INTAKE(9);//Pin 9.
FAN FAN_OUTTAKE(10);//PIN 10
USB_led Digital_pin(24);//Pin 24, USB status.
USB_button Digital_pin(4);//Pin 4, USB Mode button.


// //Front panel, inclues door led.
FRONT_PANEL FRONT_PANEL(/* Front panel leds*/ 
					50, 48, 44, 40, 38, 
					36, 34, 32,
					30, 28, 24,
					/*Front panel Switchs*/
					53, 47, 51, 49,
					45,
					/*LCD*/
					 3);//TX3
//SENSORS
//SENSORS SENSORS(7,2,1);
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
void listen_for_serial_commands();
void execute_serial_commands();

//Pin config goes here.
void setup()
{
	initiate();

	run_mode = 0;//normal mode (default)
	timer_on_1min_off_1min = 0;//Timer inactive at start.
}
void loop()
{	
	//Based on new state machine from November 1st 2016.
	store_data();
	send_data_internet();//every 1min.
	display_front_panel();
	scan_sensors();//every 5sec.
	if(run_mode) //USB MODE
	{
		USB_led.high();
		listen_for_serial_commands();
		execute_serial_commands();
	}
	else		//FrontPanel Switches
	{
		USB_led.low();
		listen_for_manual_commands();
		execute_manual_commands();	
	}
	//scan_sensors();
	//store_data();
	//send_data_internet();
	//display_front_panel();
	//scan_switch();
	//orders();
	if(USB_button.read_input() == 0)//switch between normal/usb mode.
	{
		delay(1000);//Time to let button rebounce.
		run_mode = !run_mode;
	}

}
void listen_for_serial_commands()
{
	// Serial commands
	// a -> LED ON/OFF
	// s -> LED ON 1min, OFF 1min.
	// d -> FAN1 ON/OFF
	// f -> Fan2 ON/OFF
	char incomingChar;   // for incoming serial data
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
void execute_serial_commands()
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
	//USB mode addition
	USB_button.set_input_pullup();
	USB_led.init();//USB mode led.

	//FRONT_PANEL.display_welcome();	
}
//Watch millis() if it overflows.
void overflow_watcher()
{
	
}
// void scan_sensors()
// {
// 	SENSORS.scan_soil_moisture();
// 	SENSORS.scan_temp_humidity();
// 	SENSORS.scan_door();
// }
void store_data()
{
	SERVER.Store_data(FAN_INTAKE.get_status(),FAN_OUTTAKE.get_status(),LED.get_status(),BULB.get_status(),
						SENSORS.get_soil_moisture(),SENSORS.get_temp(),SENSORS.get_humidity(),
						SENSORS.get_door());
	Serial.print(SERVER.get_data());
	Serial.print("\r\n");
}
// void send_data_internet()
// {
// 	// TO do....
// }
// void display_front_panel()
// {
// 	if(FAN_INTAKE.get_status()==1 && FAN_OUTTAKE.get_status()==1){FRONT_PANEL.FAN_led_on();}	else FRONT_PANEL.FAN_led_off(); 
// 	if(LED.get_status()==1){FRONT_PANEL.LED_led_on();} else FRONT_PANEL.LED_led_off();
// 	if(SERVER.get_phase() == 'G'){FRONT_PANEL.Germination_mode();}
// 	if(SERVER.get_phase() == 'V'){FRONT_PANEL.Vegetation_mode();}
// 	if(SERVER.get_phase() == 'F'){FRONT_PANEL.Flowering_mode();}
// 	if(BULB.get_status()==1){FRONT_PANEL.BULB_led_on();} else FRONT_PANEL.BULB_led_off();
// 	unsigned long currentMillis = millis();
// 	if(currentMillis - _previousMillis >1000)//Every 1 sec.
// 	{
// 	_previousMillis = currentMillis;	
// 	Serial3.print(SENSORS.get_temp(),0);
//     Serial3.print("oC ");
//     Serial3.print(SENSORS.get_humidity(),0);
//     Serial3.print("% * Growbox1");
//     Serial3.print("\n");
// 	}
// }
void scan_switch()
{
	FRONT_PANEL.scan_switch();
}
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
// }
