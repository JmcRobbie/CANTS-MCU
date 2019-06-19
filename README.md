# CAN-TS server example for MCU

This project provides reference implementation of server side CAN-TS protocol, suitable for memory constrained microcontrollers.

## CANT-TS overview

The use of a CAN bus on-board a satellite brings many advantages to the design and later the operation of the satellite. It provides a robust and higher data rates communication bus. It allows multiple masters on the same bus, which allows for more flexibility in connected satellite subsystems together. It features a well-developed error handling methodology, allowing the whole bus to be alerted to any faults that might occur. Most importantly, it features bus access arbitration, meaning that messages with a higher priority will always be sent before messages with lower priority.

CAN-TS is a communication protocol and device services specification for embedded systems used in space applications. In terms of the OSI model, CAN-TS implements the layers above and including the network layer. The CAN-TS protocol defines an addressing scheme, several memory efficient communication transfers type to support TM/TC, bulk data transfer (multi-message transfers) to and from device, unsolicited TM, time synchronization across devices and defines redundancy mechanism. The lower level protocol implementing the data link and physical layers is CAN, although devices using some other means of communication (such as, RS-422/485, SPI, I2C, Ethernet, EtherCAT) can also implement the CAN-TS. CAN-TS provides memory efficient communication stack for embedded application, while still assures high level of services between devices.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

In order to build and run this project, you need to obtain [PicoSky Evaluation Board](https://www.skylabs.si/portfolio-item/picosky-evaluation-board-sky-9213) and PicoSkyWARE (included in PicoSky Evaluation Board package). Project can be open with Eclipse IDE for C/C++ Developers.

Following instructions assume that build environment is prepared according to instructions found in PicoSkyWARE package. User should also familiarize himself/herself with basic development workflow which is explained in tutorials that come together with PicoSky Evaluation Board.

### Building

Project can be build by selecting Project -> Build All in Eclipse menu.

### Uploading

Firmware can be uploaded and run by selecting Run -> Debug in Eclipse menu or pressing F11.

### Communicating with the board

Demo PC application, which is able to communicate with the board, will be uploaded later.

### Supported telecommands, telemetry and block transfers in this example

Following telecommand channel is supported:

| Channel | Data length | Description |
| :---: | :---: | :--- |
| 0 | 1 | Set LEDs on port 5 |

Following telemetry channel is supported:

| Channel | Data length | Description |
| :---: | :---: | :--- |
| 0 | 1 | Status of LEDs on port 5 |

Block transfer application layer simulates simple memory device 256B in size.

| Address | Size | Description |
| :---: | :---: | :--- |
| 0 | 256 | Memory, which can be written and read with Set and Get Block transfers, respectively

## Documentation

Project documentation can be build with doxygen with configuration file provided in doc folder.

Following external documents are available:

| Name| Type | Version | Date |
| :--- | :---: | :---: | :---: |
| [CAN-TS protocol](https://support.skylabs.si/public/CAN-TS_protocol_v1.4.pdf) | pdf | 1.4 | 06/2019 |
| [CAN-TS overview presentation](https://support.skylabs.si/public/CAN-TS_protocol_presentation_v1.0.pdf) | pdf | 1.0 | 06/2019 |

## License

This project is licensed under the 3-clause BSD License - see the [LICENSE.txt](LICENSE.txt) file for details.