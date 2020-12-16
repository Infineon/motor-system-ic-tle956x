# BLDC motor control with TLE9563
BLDC Motor Control Shield and Evaluation Board with TLE9563-3QX

<img src="/pictures/BLDC_SHIELD_TLE956X.jpg" width="500"> 

Library of Infineon's [BLDC motor control shield](https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_tle956x/) for Arduino IDE.
Refer also to the [Board manual](https://www.infineon.com/dgdl/Infineon-BLDC_Shield_TLE956x-UserManual-v01_00-EN.pdf?fileId=5546d46272e49d2a0173240cd6a32199).

## Summary
This evaluation board comes with one BLDC Motor Control IC of TLE9563-3QX. The purpose of this board is to provide a quick evaluation solution for costumers' lab assessments. The evaluation board offers a unique two in one solution. It can be connected via a UIO stick with the computer to evaluate its features via ConfigWizard (a graphical user interface) which can be found in the Infineon Toolbox. In addition, the evaluation board features an Arduino Shield interface for rapid prototyping.

## Key Features and Benefits
* Operating voltage range 5.5-28V with 5V compatible logic input
* Integrated reverse polarity protection with onboard EMC filter
* Three integrated half-bridges MOSFET (2 in 1 MOSFET package)
* 5 mΩ low-side current sense
* High-Power RGB led driven by integrated High-Side switches
* Onboard Back-EMF detection circuit and connector for HALL-sensor

## Supported Microcontroller Boards
|               | BLDC BEMF | BLDC Hall | BLDC FOC |
|       ---|---|---|---|
|**Arduino UNO**	| Yes   | Yes   | No	|
|**Arduino Mega**	| /		| /		| /		|
|**XMC4700**    	| /     | /   	| /		|

## Library Structure (for Wiki)
The Lib contains 3 parts:
* BLDC motor control functions (almost done, BEMF pattern missing)
* TLE956x basis lib for register configuration (Bitfield configuration missing)
* User frontend for debug output and error messages (PAL layer missing, error codes need to be defined)

## Usage
Please follow the example sketches in the /examples directory in this library to learn more about the usage of the library.