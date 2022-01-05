
Hardware Platforms
==================
There are two PCBs available using TLE956x motor control ICs.

BLDC Motor Shield with TLE9563
-------------------------------

Pinout Diagram
^^^^^^^^^^^^^^^

.. image:: /img/BLDC_Shield_TLE9563_Pinout.png

Pin Description
^^^^^^^^^^^^^^^^

.. list-table::
	:header-rows: 1

	* - Arduino Pin
	  - Symbol
	  - Type
	  - Function
	* - GND
	  - GND
	  - \-
	  - Ground
	* - D2
	  - INTN
	  - Output/digital
	  - Interrupt output of TLE956x configurable via SPI.
	* - D3
	  - PWM1
	  - Input/PWM
	  - Input PWM for Phase 1
	* - D5
	  - BEMF_U_IO
	  - Output/digital
	  - Output of BEMF comparator for Phase 1
	* - D6
	  - BEMF_V_IO
	  - Output/digital
	  - Output of BEMF comparator for Phase 2
	* - D7
	  - BEMF_W_IO
	  - Output/digital
	  - Output of BEMF comparator for Phase 3
	* - D8
	  - SPI CSN
	  - Input/digital
	  - Chip select pin for SPI communication
	* - D9
	  - PWM 3
	  - Input/PWM
	  - Input PWM for Phase 2
	* - D10
	  - PWM5
	  - Input/PWM
	  - Input PWM for Phase 3
	* - D11
	  - SPI SDI
	  - Input/digital
	  - Serial Data Input from Microcontroller to TLE
	* - D12
	  - SPI SDO
	  - Output/digital
	  - Serial Data Output from TLE to Microcontroller
	* - D13
	  - SPI CLK
	  - Input/digital
	  - SPI Clock
	* - A0
	  - Hall C
	  - Output/digital
	  - Signal of Hallsensor C (from BLDC motor)
	* - A1
	  - Hall B
	  - Output/digital
	  - Signal of Hallsensor B (from BLDC motor)
	* - A2
	  - Hall A
	  - Output/digital
	  - Signal of Hallsensor A (from BLDC motor)
	* - A5
	  - CSO
	  - Output/analog
	  - Output of current sense amplifier

.. _Jumper settings:

Jumper settings
^^^^^^^^^^^^^^^^
For plug & play operation with the provided example code, it's recommended to set the following jumpers:

* 3 jumpers on J2 to connect the RGB LED with the HSS of the TLE9563
* 1 jumper on P3 to enable green status LED, which indicates VCC1 is active
* 1 jumper on P5 to connect board supply voltages
* 1 jumper on P2 to enable software-development mode

DC Motor Shield with TLE9562
----------------------------

Pinout Diagram
^^^^^^^^^^^^^^

.. image:: /img/DC_Shield_TLE9562_Pinout.png

Pin Description
^^^^^^^^^^^^^^^

.. list-table::
	:header-rows: 1

	* - Arduino Pin
	  - Symbol
	  - Type
	  - Function
	* - GND
	  - GND
	  - \-
	  - Ground
	* - D2
	  - INTN
	  - Output/digital
	  - Interrupt output of TLE956x configurable via SPI.
	* - D3
	  - PWM1
	  - Input/PWM
	  - Input PWM for Phase 1
	* - D5
	  - SYNC
	  - Input/digital
	  - Synchronization for wake input
	* - D8
	  - SPI CSN
	  - Input/digital
	  - Chip select pin for SPI communication
	* - D9
	  - PWM 3
	  - Input/PWM
	  - Input PWM for Phase 2
	* - D11
	  - SPI SDI
	  - Input/digital
	  - Serial Data Input from Microcontroller to TLE
	* - D12
	  - SPI SDO
	  - Output/digital
	  - Serial Data Output from TLE to Microcontroller
	* - D13
	  - SPI CLK
	  - Input/digital
	  - SPI Clock
	* - A0
	  - Hall C
	  - Output/digital
	  - Signal of Hallsensor C (from BLDC motor)

Jumper Settings
^^^^^^^^^^^^^^^^
For plug & play operation with the provided example code, it's recommended to set the following jumpers:

* 1 jumper on P3 to enable green status LED, which indicates VCC1 is active
* 1 jumper on P2 to enable software-development mode



.. _`TLE9563-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/
.. _`TLE9562-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/tle9562-3qx/