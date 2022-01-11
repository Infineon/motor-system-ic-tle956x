**************
Introduction
**************
Welcome to the Infineon Motor System IC TLE956x library docs!

BLDC Motor Shield with TLE9563
===============================

.. list-table::
	:header-rows: 0

	* - .. image:: img/BLDC_SHIELD_TLE9563.jpg
    		:height: 125
	  - .. image:: img/MOTIX_TLE956x.png
    		:height: 75
	* - `BLDC SHIELD TLE9563-3QX`_
	  - `TLE9563-3QX`_
	* - `BLDC motor shield user manual`_
	  - `TLE9563-3QX Datasheet`_

**Main Features:**
	* support for sensor-less brushless motors using onboard BEMF comparator
	* support for brushless motors with hall-sensor (field weakening range possible)
	* RPM function that keeps a desired RPM speed
	* control onboard RGB LED
	* platform independent C++ architecture
	* various parameters configurable for adaptive gate control (AGC)

DC Motor Shield with TLE9562
============================

.. list-table::
	:header-rows: 0

	* - .. image:: img/DC_SHIELD_TLE9562.jpg
    		:height: 125
	  - .. image:: img/MOTIX_TLE956x.png
    		:height: 75
	* - `DC SHIELD TLE9562-3QX`_
	  - `TLE9562-3QX`_
	* - `DC motor shield user manual`_
	  - `TLE9562-3QX Datasheet`_

**Main Features:**
	* control two DC motors independently of each other
	* control onboard LEDs
	* platform independent C++ architecture
	* various parameters configurable for adaptive gate control (AGC)


.. _`BLDC SHIELD TLE9563-3QX`: https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_tle956x/
.. _`DC SHIELD TLE9562-3QX`: https://www.infineon.com/cms/en/product/evaluation-boards/dc-shield_tle956x/
.. _`TLE9563-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/
.. _`TLE9562-3QX`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/tle9562-3qx/
.. _`TLE9563-3QX Datasheet`: https://www.infineon.com/cms/en/product/power/motor-control-ics/bldc-motor-driver-ics/bldc-motor-system-ics/tle9563-3qx/#!?fileId=5546d4627883d7e00178ca35bade3876
.. _`TLE9562-3QX Datasheet`: https://www.infineon.com/cms/en/product/power/motor-control-ics/brushed-dc-motor-driver-ics/dc-motor-system-ics/tle9562-3qx/#!?fileId=5546d4627883d7e00178ca35b1603873
.. _`BLDC motor shield user manual`: https://www.infineon.com/cms/en/product/evaluation-boards/bldc-shield_tle956x/#!?fileId=5546d46272e49d2a0173240cd6a32199
.. _`DC motor shield user manual`: https://www.infineon.com/cms/en/product/evaluation-boards/dc-shield_tle956x/#!?fileId=5546d46273a5366f0173fb81140a3b77


License
=======

Please find the license file for this library `here <https://github.com/Infineon/motor-system-ic-tle956x/blob/master/LICENSE.md>`_.


.. toctree::
   :maxdepth: 3
   :caption: Home
   :hidden:

   Introduction <self>
   Acronyms <acronyms.rst>
   Related Links <links.rst>

.. toctree::
   :maxdepth: 3
   :caption: Hardware Platforms
   :hidden:

   hardware-platforms
   
.. toctree::
   :maxdepth: 4
   :caption: Library Details
   :hidden:

   Library Architecture <lib-details/library-architecture.rst>
   Porting Guide <lib-details/porting-guide.rst>
   Adaptive Gate Control (AGC) <lib-details/api.rst>

.. toctree::
   :maxdepth: 1
   :caption: Software Frameworks
   :hidden:

   sw-frmwk/arduino/index