.. _arduino-compatible-kits:

Arduino Compatible Kits
=======================

This library is designed for multiple platforms with Arduino Uno compatible headers and different SDKs. The following hardware platforms are compatible and tested:

.. list-table::
    :header-rows: 1

    * - Hardware Platform
      - Type
      - SDK
      - File Marker
      - Checked
    * - Arduino
      - `Uno Rev3`_
      - Arduino IDE
      - \*.ino
      - yes
    
Other MCU platforms which have an Arduino port may not work as high frequency PWM (30kHz) is required on pins 3, 9 and 10 what is not available on all boards.
Second limitation is the SPI bound to pins 11 (MOSI), 12 (MISO) and 13 (SCK) what is only available on the UNO and XMC boards. However if you wire it manually, the shield might work on more MCUs.

`XMC for Arduino`_

.. _`XMC for Arduino`: https://github.com/Infineon/XMC-for-Arduino


.. _`XMC1100 Boot Kit`: https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc11_boot_001/#ispnTab1
.. _`XMC4700 Relax Kit`: https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_5v_ad_v1/
.. _`Uno Rev3`: https://store.arduino.cc/arduino-uno-rev3
.. _`Arduino MKR1000`: https://store.arduino.cc/arduino-mkr1000-wifi-with-headers-mounted
