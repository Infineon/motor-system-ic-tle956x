
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
For plug & play operation with the provided example code, it's recommended to set the default jumpers:

.. list-table::
	:header-rows: 1
	
	* - Jumper
	  - Function
	  - Default
	* - J1
	  - In and output of the high speed CAN transceiver
	  - None (only in-/output)
	* - J2
	  - Connect each High-Side-Switch of the TLE9563 with a color of the RGB LED
	  - **set 3 Jumpers**
	* - P1
	  - Connect RESET of Arduino with RSTN of TLE5963
	  - **set**
	* - P2
	  - Connect the INTN pin of TLE9563 with a pulldown to GND. This enables software development mode.
	  - **set**
	* - P3
	  - Connect green LED with VCC1 of TLE9563 indicating chip is powered on
	  - **set**
	* - P4
	  - Connector for hall-sensor
	  - None (only input)
	* - P5
	  - Connect VIN of Arduino with 5V regulator on TLE9563 shield
	  - **set**

For more information refer to the `BLDC motor shield user manual`_ and `TLE9563-3QX Datasheet`_.


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

Jumper Settings
^^^^^^^^^^^^^^^^
For plug & play operation with the provided example code, it's recommended to set the default jumpers:

.. list-table::
	:header-rows: 1
	
	* - Jumper
	  - Function
	  - Default
	* - J1
	  - Connect D0 and D1 with LIN transceiver
	  - None
	* - J3
	  - In and output of the high speed CAN transceiver
	  - None (only in-/output)
	* - P1
	  - Connect RESET of Arduino with RSTN of TLE5962
	  - **set**
	* - P2
	  - Connect the INTN pin of TLE9562 with a pulldown to GND. This enables software development mode.
	  - **set**
	* - P3
	  - Connect green LED with VCC1 of TLE9562 indicating chip is powered on
	  - **set**
	* - P4
	  - Output of remaining High-Side-Switches HSS1 and HSS2.
	  - None (only output)
	* - P5
	  - Connect VIN of Arduino with 5V regulator on TLE9562 shield
	  - **set**
	* - P6
	  - Connect the PWM1 pin of TLE9562 with a pulldown to GND. This enables CRC.
	  - None

For more information refer to the `DC motor shield user manual`_ and `TLE9562-3QX Datasheet`_.



.. _`TLE9563-3QX Datasheet`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/#!?fileId=5546d4627883d7e00178ca35bade3876
.. _`TLE9562-3QX Datasheet`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/tle9562-3qx/#!?fileId=5546d4627883d7e00178ca35b1603873
.. _`BLDC motor shield user manual`: https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_tle956x/#!?fileId=5546d46272e49d2a0173240cd6a32199
.. _`DC motor shield user manual`: https://www.infineon.com/cms/en/product/evaluation-boards/dc-shield_tle956x/#!?fileId=5546d46273a5366f0173fb81140a3b77