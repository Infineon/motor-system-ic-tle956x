.. _arduino-api:

Arduino API
===========

TODO - will be replaced by code reference


Start with the example sketch **TLE9563_BLDCM-control**.

Here we go through each function and variable used in this sketch and show up other control possibilities.

void setup()
------------

Include the library and create an instance of the class **BLDCMcontrolIno**::

	#include <Arduino.h>
	#include <BLDCM-control-ino.hpp>

	uint16_t speed = 400;
	uint8_t direction = 0;
	uint8_t weakening = 0;

	// Create an instance of BLDCMcontrolIno called 'MyMotor'. 
	BLDCMcontrolIno MyMotor = BLDCMcontrolIno();


Set up a GPIO interrupt routine bound to Pin 2 (the interrupt Pin of the TLE956x shield).
By default, the library configures the TLE to throw an interrupt if an error in one or more status register occurs::

	// Enable GPIO interrupt for pin 2
	attachInterrupt(digitalPinToInterrupt(2), TLEinterrupt, LOW);


When you jump to the last lines in the example sketch, you see, the TLEinterrupt function only sets the variable *interrupt_status_changed* to one. Just keep that in mind for now, we come to the reason later on.

**Important note:** In order to use the interrupt function properly, make sure the HSS switch of the board is in position *Static*.
Otherwise the interrupt is bound to the PWM of HSS1 and thus called periodically, if this HSS is used (e.g. the green LED is on).

.. image:: /img/board_HSS_Switch.jpg
    :height: 200


Motor control functions
------------------------

First, we need to call the *begin()* function, that configures all input/output pins, PWM frequencies and so on. The function *setLED(red, green, blue)* let us set the color of the onboard RGB-LED, driven by the high-side-switches of the TLE. It takes 10-bit values as argument, so you can enter values from 0 (off) to 1024 (max brightness). Make sure your HSS-jumpers are in place. ::

	MyMotor.begin();
	MyMotor.setLED(0,20,0); 

Now comes the important part: You need to select which position-feedback and which speedmode you want to use::

	MyMotor.MotorParam.feedbackmode = BLDCMcontrol::BLDC_HALL;
	MyMotor.MotorParam.speedmode = BLDCMcontrol::BLDC_PERCENTAGE;
	MyMotor.MotorParam.MotorPolepairs = 4;

.. list-table::
	:header-rows: 1

	* - MotorParam
	  - type
	  - arguments
	  - input range
	* - feedbackmode
	  - mandatory
	  - BLDC_HALL
	  - 
	* - 
	  - 
	  - BLDC_BEMF
	  - 
	* - speedmode
	  - mandatory
	  - BLDC_DUTYCYCLE
	  - 0 - 100
	* - 
	  - 
	  - BLDC_RPM
	  - 0- 2E32
	* - MotorPolepairs
	  - mandatory for BLDC_RPM
	  - integer value
	  - 0-255
	* - PI_reg_P
	  - optional (default = 0.01)
	  - float value
	  - 
	* - PI_reg_I
	  - optional (default = 0.01)
	  - float value
	  - 

If you don't know the amount of pole-pairs in your BLDC, you can use the *find_polepairs_BLDCM.ino* sketch provided in the *examples* folder. If you use a wrong number, your actual RPM-speed might be imprecise.

In order to actually set the previous defined parameters, call the following function::

	MyMotor.configBLDCshield();

Finally, we can set our default speed and direction and start the BLDC motor::

	MyMotor.setBLDCspeed(speed, direction);
	MyMotor.startBLDCM();

Depending on your configuration above, the *speed* - parameter will be interpreted as a percentage-value (a permil-value to be precise) or as a desired RPM-speed. *direction* can be 0 or 1. A third argument *weakening range* would be possible as well that can be 0 (default) or 1, but is only applicable if BLDC_HALL was selected.
*weakening range* uses a different commutation pattern, that let's the motor turn with its double speed but less torque.

*startBLDCM()* applies an open-loop commutation to your motor and enables the usage of *serveBLDCshield()* which actually commutates the motor.

void loop()
------------

In order to change speed, direction, weakening range (only for BLDC_HALL), start or stop the motor, a if-routine has been implemented, that scans the Serial-input line. 
You can directly extract, what affect it has, when you press which key::

	if (Serial.available() > 0)
	{
		uint8_t in = Serial.read();
		if(in == '+'){
		speed += 100;
		Serial.println(speed);}
		if(in == '-'){
		speed -= 100;
		Serial.println(speed);}
		if(in == 'd'){
		direction = 0;
		Serial.println("forward");}
		if(in == 'e'){
		direction = 1;
		Serial.println("backward");}
		if(in == 's'){
		weakening = 0;
		Serial.println("Field weakening disabled");}
		if(in == 'w'){
		weakening = 1;
		Serial.println("Field weakening enabled");}
		if(in == 'h'){
		MyMotor.stopBLDCM(BRAKEMODE_PASSIVE);
		Serial.println("Motor stopped");}
		if(in == 'g'){
		MyMotor.startBLDCM();
		Serial.println("Motor started");}
		MyMotor.setBLDCspeed(speed, direction, weakening);
	}

For example, if you press 'h', the function *stopBLDCM(brakemode)* is called. As the name says, is stops the commutation and prohibits the use of *serveBLDCshield()*, where brakemode defines, wether the phases are left floating (*BRAKEMODE_PASSIVE*) or actively tied to ground (*BRAKEMODE_ACTIVE*).

Last but not least, you may not forget to call the most important function, where all the magic happens: *serveBLDCshield()*

Depending on the previously defined configuration, this function checks, if the hall-sensor or BEMF-sensor state changed since the last time the function was called and if so, it commutates the output phases. This means, this function needs to be called **as often as possible** and the time between calling this function must be **as short as possible**. ::

	MyMotor.serveBLDCshield();
	MyMotor.checkBLDCshield();

The function *checkBLDCshield()* is not mandatory to run the BLDC, but handles error codes and prints debug messages. If you remind the interrupt setting at the beginning, I can now tell you, this function will only be executed if *interrupt_status_changed* was set to 1.
