# UDP Server and Client Application in C++ for windows using MySQL database

This is a windows console application to send data using UDP port and receives data from that port. Also store the received data to MySQL database.  
This project is mainly for beginners to networking also for those who is facing problem creating connection using UDP port for sending/receiving data.
 
Language Used: C/C++  
IDE: Visual Studio 2019  
Database system: MySQL Server 8.0 

## Table of Contents

- [Project Setup](#project-setup)
- [Usage](#usage)
- [Features](#features)
- [How to run](#how-to-run)
- [Authors](#authors)
- [Contact Information](#contact-information)
- [Project Snapshot](#project-snapshot)

## Project Setup

Prerequisites: Install Visual Studio, Install MySQL Server  

Setup Procedure:
1. Download the repository and open both project/sln (UDP_server and UDP_client) in Visual Studio.
  
2. To store data in MySQL database-  
Create a Database named as userInfoDb and Create a table as userInfo in userInfoDb.

You can use this command given here for this task.
```bash
CREATE DATABASE userInfoDb;

USE userInfoDb;

-- Create the UserInfo table
CREATE TABLE IF NOT EXISTS UserInfo (
  id INT NOT NULL AUTO_INCREMENT,
  user_id INT DEFAULT NULL,
  first_name VARCHAR(80) DEFAULT NULL,
  last_name VARCHAR(80) DEFAULT NULL,
  email VARCHAR(80) DEFAULT NULL,
  subscription_time DATETIME DEFAULT NULL,
  last_entry_time DATETIME DEFAULT NULL,
  view_time BIGINT DEFAULT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Show table userInfoDb
select * from userInfoDB;
```
**N.B: If you get an error like "WS2_32.lib" is not found or something like that than copy the "WS2_32.lib" from this repository under "UDP_Server_and_Client_with_MySQL_in_C++\dependencies" and paste it to "C:\Windows" directory on your computer. Hopefully, this will solve the error.  
If any other errors occurs you can ask me.

## Usage
It's a basic level project to send data to server from client using User Datagram Protocol(UDP). It the basis to establish a connection using UDP ports. Mainly for learning purpose.

There are many applications using UDP ports though UDP does not ensure reliable connection. This protocol is best for simple query response protocols like DNS service and as it is faster that TCP so it is suitable for Online games, media streaming.

## Features
1. Client: Send multiple packets of data via UPD port (UDP port 4400 used in this project)
NB: You can use any UDP port from 1024 to 65535
2. Server: Receives data from UDP port (UDP port: 4400).
3. Store the received data in MySQL
4. Before storing checks the email validation for the received email from the client.
   Criteria for email validation: email should have "@" symbol, email ends with ".com", 
   email should have contain a "test string" defined by the server.

      For example the text string "gmail" is used to validate the email in this project.
 
                         "xyz@gmail.com" -> Valid 
		                 "x@text.com" -> Invalid 
		                 "abcgmail@.com" -> Valid
		                 "validate.com" -> Invalid
     The email validation is not for proper email validation. Just considered some criteria for implementing a validation process for data to be insert in the MySQL database.  

## How to run 
Step-1: Open the "UDP_server.sln" file in Visual Studio under "networking/UDP_server_and Client_with_MySQL/UDP_server" and run it. As it is the server do not close it [keep it running]. 

Step-2: run "UDP-client.sln" file in Visual Studio under "networking/UDP_server_and Client_with_MySQL/UDP_client" and run it. You can run the client code multiple time while running the server only once at the start, the server will keep receiving data until it is not closed. 

Congratulations. Now you can send data between UDP port.

## Authors  
- [@Shihabul-Shuvo](https://github.com/Shihabul-Shuvo)

## Contact Information  
- [shihabul900@gmail.com](shihabul900@gmail.com)
- [Portfolio](https://shihabul-shuvo.github.io/Portfolio-shihabul/)

## Project Snapshot 
![Server started](https://github.com/Shihabul-Shuvo/Networking/blob/main/UDP_server_and_Client_with_MySQL/Snapshots/server_started.png)

Server Started  
![Client data send](https://github.com/Shihabul-Shuvo/Networking/blob/main/UDP_server_and_Client_with_MySQL/Snapshots/client_data_send.png)  
Client Sending Data to Server  
![Data received from client](https://github.com/Shihabul-Shuvo/Networking/blob/main/UDP_server_and_Client_with_MySQL/Snapshots/data_received_n_stored.png)  
Server Receivied Data and Storeed in the Database  
![Data received but invalid email](https://github.com/Shihabul-Shuvo/Networking/blob/main/UDP_server_and_Client_with_MySQL/Snapshots/data_received_with_invalid_email.png)  
Server Received Data with Invalid Email  
![Data receivec but sql error](https://github.com/Shihabul-Shuvo/Networking/blob/main/UDP_server_and_Client_with_MySQL/Snapshots/data_received_with_sql_error.png)  
Server Received Data with SQL Error  
