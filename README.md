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


-![alt text](http://i.imgur.com/SbV4fyv.png "Diagram of embeded device")

### Backend

Data is exchanged through the use of a RESTful web service built using the Django web framework; written in Python. The API is built to unify the communication between the  backend system and the embedded device and frontend interface.
The embedded device sends a POST request to the web server containing the user ID and a product UPC code. A record is generated in the database. Then a response is built, generally, it contains the product name, price, and category. The information contained in the response is then displayed on the device LCD display as visual feedback for the user.
When the web service tries to match the UPC code against the records in the database, and a record is not found, a request is sent to third party services, which is used to retrieve and cache relevant information about a product.
Accessing the database is done via an Object-relational mapping (ORM) to ease the workflow and use a higher layer of abstraction of the database models.

-![alt text](http://i.imgur.com/2ggIXIV.png "Backend diagram")


### Frontend

Users can access and manage the information recollected by the embedded device by using a web application.
The web application communicates to the backend through the RESTful API, where a user can easily outperform the actions the device is able to do. The user can perform actions such as add or remove a product, modify the quantities, or order the products directly from the supermarket. 
The web application is accessible from mobile devices or computers, allowing the user to access the service while in a supermarket, even several users can manage the grocery list in real time making it possible to add products on the go.

-![alt text](http://i.imgur.com/YhtY6pL.png "Web Application")


## Future Work

There are a plethora of possible future improvements for the platform. Once hundreds of users are using the solution, it will be possible to take advantage of the information collected; statistics about what products are bought, their geographic areas, frequency of purchase, among other. Information that provides. With this data the platform will be able to recommend products to users, based on their previous purchases.
RFID will eventually replace barcodes for grocery tagging, therefore creating new ways of interaction. Products no longer will have to be manually scanned, but they could just be disposed and be automatically scanned, rendering the process effortless.
