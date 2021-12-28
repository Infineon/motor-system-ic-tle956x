
Hardware Platforms
==================

BLDC Motor Shield with TLE9563
-------------------------------

Pinout Diagram
^^^^^^^^^^^^^^^

.. image:: /img/BLDC_Shield_TLE9563_Pinout.png
    :height: 500

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


DC Motor Shield with TLE9562
----------------------------

Pinout Diagram
^^^^^^^^^^^^^^

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
    * - D5
      - TP COMP 1
      - Output/digital
      - Input to check state of comperator 1, can be measured at tespoint TP_COMP1.
    * - D6
      - IN - BTS500xx
      - Input/PWM
      - Input of BTS500xx chip to switch on / off.
    * - D9
      - LED2
      - Input/PWM
      - Input to shield LED no. 2.
    * - D19
      - LED1
      - Input/PWM
      - Input to shield LED no. 1.
    * - A0
      - S2
      - Output/digital
      - Handling of user input. External Key can be connected, low active.
    * - A2
      - IS
      - Output/analog
      - Measuring of current IS.
    * - A3
      - TP 5V
      - Output/analog
      - Measuring availability of 5V.
    * - A5
      - VBAT
      - Output/analog
      - Measuring of VBAT via voltage divider.

* `TLE9563-3QX`_
* `TLE9562-3QX`_

.. _`TLE9563-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/
.. _`TLE9562-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/tle9562-3qx/
.. _`BTS50010-1TAD`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/power-profet-automotive-smart-high-side-switch/bts50010-1tad/
.. _`BTS50015-1TAD`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/power-profet-automotive-smart-high-side-switch/bts50015-1tad/
.. _`BTS7002-1EPP`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/profet-plus-2-12v-automotive-smart-high-side-switch/bts7002-1epp/?redirId=103258
.. _`BTS7004-1EPP`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/profet-plus-2-12v-automotive-smart-high-side-switch/bts7004-1epp/
.. _`BTS7006-1EPP`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/profet-plus-2-12v-automotive-smart-high-side-switch/bts7006-1epp/
.. _`BTS7008-1EPP`: https://www.infineon.com/cms/en/product/power/smart-low-side-high-side-switches/high-side-switches/profet-plus-2-12v-automotive-smart-high-side-switch/bts7008-1epp/
.. _`24V Protected Switch Shield with BTT6030-2EKA and BTT6020-1EKA`: https://www.infineon.com/cms/en/product/evaluation-boards/24v_shield_btt6030/
.. _`12V Protected Switch Shield with BTS50010-1TAD`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts50010-1tad/
.. _`12V Protected Switch Shield with BTS50015-1TAD`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts50015-1tad/
.. _`SHIELD_BTS7002-1EPP`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts7002-1epp/
.. _`SHIELD_BTS7004-1EPP`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts7004-1epp/
.. _`SHIELD_BTS7006-1EPP`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts7006-1epp/
.. _`SHIELD_BTS7008-1EPP`: https://www.infineon.com/cms/en/product/evaluation-boards/shield_bts7008-1epp/