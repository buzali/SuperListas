SuperListas
=============

SuperListas is a consumer electronics device and web platform that improves the grocery shopping experience and automate the creation of shopping lists. The idea is simple, when you run out of a product, you scan it using the device and it automatically sends the information to the server and creates a shopping list that you can manage online through a computer or smartphone.

## Introduction

Making grocery lists with regularity has been an arduous task, especially for people who plan their grocery shopping mentally, and then create a handwritten list.  This is generally exhausting, particularly to housewives, househusbands and people with full-time jobs.
The constant growth and development of e-commerce and mobile applications has improved the shopping methods of those who have taken advantage of these technologies. Supermarkets have exploited these resources by offering their customers the possibility to order products via web pages or mobile apps. Buyers will receive their products shortly after in the convenience of their doorstep, saving time and effort. 
To take advantage of this new tendency and to solve the main need, a product was developed. This integrated system consists of two parts: an electronic device capable of barcode scanning and WiFi connection, as well as a web platform designed to display a list consisting of the previously scanned products and the possibility of ordering through it. 

## System Architecture

The system comprises of three main modules: the device that scans and transmits the UPC codes, the backend system that processes and stores the data and the frontend interface that lets the user manage and view their grocery lists.

### Embedded Device

The powerful, yet energy efficient microcontroller, Microchip PIC24FJ256GA106 was utilized as the main controller of the embedded device. The microcontroller is connected to the barcode scanner LV1000 that reads the UPC and EAN codes and transmits them via RS232 protocol to the microcontroller. To interface the system with the server, the microcontroller connects via Wi-Fi (IEEE 802.11b) utilizing Microchip’s module, MRF24WB0MA. An LCD display and a rotary encoder are used to interface with the user and display information. In order to store device settings and preferences, an EEPROM memory is also included. To further expand the capabilities of this device, external I/O and USB interfaces were integrated.


-![alt text](http://i.imgur.com/Lk5wWZK.png "Diagram of embeded device")

