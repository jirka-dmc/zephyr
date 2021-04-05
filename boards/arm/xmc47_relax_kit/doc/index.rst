.. _xmc47_relax_kit:

INFINEON XMC47-RELAX-KIT
########################

Overview
********

The XMC4700 Relax Kit is designed to evaluate the capabilities of the XMC4700
Microcontroller. It is based on High performance ARM Cortex-M4F which can run
up to 144MHz.

.. image:: ./xmc47_relax_kit.jpeg
   :width: 720px
   :align: center
   :alt: XMC47-RELAX-KIT

Features:
=========

* ARM Cortex-M4F XMC4700
* 32 Mbit Quad-SPI Flash
* 4 x SPI-Master, 3x I2C, 3 x I2S, 3 x UART, 2 x CAN, 17 x ADC
* 2 pin header x1 and x2 with 80 pins
* Two buttons and two LEDs for user interaction
* Detachable on-board debugger (second XMC4700) with Segger J-Link

Details on the Relax Kit development board can be found in the `Relax Kit User Manual`_.

Supported Features
==================

* The on-board 12-MHz crystal allows the device to run at its maximum operating speed of 120MHz.

The Relax Kit development board configuration supports the following hardware features:

+-----------+------------+-----------------------+
| Interface | Controller | Driver/Component      |
+===========+============+=======================+
| NVIC      | on-chip    | nested vectored       |
|           |            | interrupt controller  |
+-----------+------------+-----------------------+
| SYSTICK   | on-chip    | system clock          |
+-----------+------------+-----------------------+
| UART      | on-chip    | serial port           |
+-----------+------------+-----------------------+

More details about the supported peripherals are available in `XMC4700 TRM`_
Other hardware features are not currently supported by the Zephyr kernel.

Building and Flashing
*********************
Flashing
========

Here is an example for the :ref:`hello_world` application.

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :board: xmc47_relax_kit
   :goals: flash

Debugging
=========

Here is an example for the :ref:`hello_world` application.

.. zephyr-app-commands::
   :zephyr-app: samples/hello_world
   :board: xmc47_relax_kit
   :goals: debug

Step through the application in your debugger.

References
**********

.. _Relax Kit User Manual:
   https://www.infineon.com/dgdl/Infineon-Board_User_Manual_XMC4700_XMC4800_Relax_Kit_Series-UM-v01_02-EN.pdf?fileId=5546d46250cc1fdf01513f8e052d07fc

.. _XMC4700 TRM:
   https://www.infineon.com/dgdl/Infineon-ReferenceManual_XMC4700_XMC4800-UM-v01_03-EN.pdf?fileId=5546d462518ffd850151904eb90c0044
