.. _arduino-compatible-kits:

Arduino Compatible Kits
=======================

TODO: content needs to be updated

This library is designed for multiple platforms with Arduino Uno compatible headers and different SDKs. The following hardware platforms are compatible and tested:

.. list-table::
    :header-rows: 1

    * - Hardware Platform
      - Type
      - SDK
      - File Marker
      - Checked
    * - Arduino
      - Uno
      - Arduino IDE or PlatformIO
      - \*.ino
      - yes
    * - Arduino
      - Mega 2560
      - Arduino IDE or PlatformIO
      - \*.ino
      - no
    * - Infineon XMC
      - XMC1100 Boot Kit
      - Arduino IDE or PlatformIO
      - \*.ino
      - no
    * - Infineon XMC
      - XMC4700 Relax Kit
      - Arduino IDE or PlatformIO
      - \*.ino
      - no
    
Other MCU platforms which have an Arduino port like the `XMC for Arduino`_ may not work as high frequency PWM is required (30kHz) that is not available on all boards. 

.. _`XMC for Arduino`: https://github.com/Infineon/XMC-for-Arduino

Supported MCU Platforms
-----------------------

Verified MCU Platforms
""""""""""""""""""""""

The library examples have been built and successfully executed on the following platforms:

* `XMC1100 Boot Kit`_
* `XMC4700 Relax Kit`_
* `Arduino Uno Rev3`_
* `Arduino MKR1000`_

.. _`XMC1100 Boot Kit`: https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc11_boot_001/#ispnTab1
.. _`XMC4700 Relax Kit`: https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_5v_ad_v1/
.. _`Arduino Uno Rev3`: https://store.arduino.cc/arduino-uno-rev3
.. _`Arduino MKR1000`: https://store.arduino.cc/arduino-mkr1000-wifi-with-headers-mounted

Other MCU platforms
"""""""""""""""""""