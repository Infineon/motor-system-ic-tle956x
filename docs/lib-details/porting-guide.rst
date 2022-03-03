Porting Guide 
==============

Porting the library to a new software development framework and hardware
platform entails the implementation of the corresponding ADC, GPIO and Timer
PAL classes. In the following sections, some additional explanations and
hints are provided:

Framework PAL Implementation
----------------------------

Implement the abstract PAL interface for you framework. The **ADC
class**, **GPIO class** and **Timer class** are mandatory.

The Doxygen comments on the *“src/pal/adc.hpp”*, *“src/pal/gpio.hpp”*
and *“src/pal/timer.hpp”* describe the required behavior of each function
of the PAL Interface.

Consider the existing framework implementations as reference examples
for you design: *“/src/framework/sample_fmwk/pal”*. Some of the
functions are optional depending on your framework and intended usage of
the library.

That is the case of *init()* and *deinit()*, which take care of the
hardware peripherals init/deinitialization. If this is done in your main
application (or somewhere else outside the library), there is no need of
delegating such initialization to the High-side Switch library. The definition
of these functions can just be a return with the success return code.

Framework API Wrapper
---------------------

The framework API wrapper implementation is optional, it is meant to
ease the usage. Mostly the main help is to avoid the creation of the
ADC, GPIO and Timer object instances for the developer.

To illustrate this approach, it is easier to evaluate a concrete
implementation of the Arduino wrapper. For example have a look in *“src/corelib/DCM-control.cpp”*:

.. code-block:: C

	pwmA->ADCWrite(_DutyCycle);
	pwmB->ADCWrite(_DutyCycle);

is wrapped for Arduino like this:

.. code-block:: C

	analogWrite(ARDUINO_UNO.PWM_U, _DutyCycle);
	analogWrite(ARDUINO_UNO.PWM_V, _DutyCycle);

using this instantiation in *“src/DCM-control-ino.cpp”*:

.. code-block:: C
	
	DCMcontrol::pwmA = new ADCIno(ARDUINO_UNO.PWM_U);
	DCMcontrol::pwmB = new ADCIno(ARDUINO_UNO.PWM_V);

	DCMcontrol::timer = new TimerIno();

where the pin configuration is stored it the ARDUINO_UNO struct in *"src/framework/arduino/wrapper"*.

While it does not seems to simplify much in number of arguments, an
Arduino developer can simply pass the pin number as argument, and
does not need to deal with the (probably unknown) GPIO classes,
neither specify further GPIO configuration as the mode (input,
output, pull-up..), positive/negative logic, etc in the core library.

For each ecosystem and framework, any other criteria can be chosen,
hopefully matching as well its code conventions, implementation
principles and paradigms.

