# MyScooterRentals

# Overview
Welcome to the Scooter Rental Application! This application is designed to efficiently manage scooter rentals using the Qt framework for the user interface. It provides functionalities for customers, employees, and administrators to interact with the system.

<img width="398" alt="Screenshot 2024-08-09 at 3 31 53 PM" src="https://github.com/user-attachments/assets/1d7dc8e0-be55-4964-a659-1c9c61acb0a9">





https://github.com/user-attachments/assets/f5cd2ed1-1fab-4d0a-a76e-56031d9a47d5






## Development Process

The development process began by designing the application architecture. Using Qt’s modular approach, you created different windows for login, registration, and scooter management. Each window was linked to specific .cpp and .h files that handled the logic behind button actions, input validation, and database communication. For instance, when a user logs in or rents a scooter, their actions on the interface trigger C++ functions connected to Qt’s signals and slots system — a mechanism that allows user interface events (like button clicks) to call specific functions in the backend code.

The next step was connecting the application to the SQLite database, which serves as the data storage system. Instead of using a remote server, SQLite stores all information locally in a .db file called database_q.db. This made the project lightweight and portable while still supporting standard SQL operations. Within this database, you defined tables such as Users, Scooters, and Rentals.

- The Users table stores login credentials, user types, and personal information.

- The Scooters table tracks each scooter’s status (available, in use, under maintenance), ID, and location.

- The Rentals table records active and completed rentals, including start times, end times, and the user associated with each rental.

## Use of SQL

SQL played a central role in handling all database operations. When a user signs up, the program executes an INSERT query to add the new account into the Users table. During login, the system runs a SELECT query to check if the username and password exist. When a scooter is rented, an UPDATE query changes its status to “in use,” and an INSERT statement adds a new record into the Rentals table. When a scooter is returned, another UPDATE query marks it as available again and stores the return timestamp.

These SQL queries are executed through Qt’s built-in QSqlDatabase and QSqlQuery classes, which provide an easy way to run commands and handle results directly from C++ code. Each query result is processed and displayed on the interface in real time, keeping the user interface synchronized with the database content.

## Qt Desktop Functionality

The Qt framework was responsible for creating the graphical user interface (GUI) and managing interaction between the user and the program. Qt Designer, part of the framework, allowed you to visually design each window by dragging and dropping widgets such as text boxes, labels, tables, and buttons. These interface components are saved as .ui files, which are automatically converted into C++ code during compilation.

Qt also handles user interaction through its signal-slot system. When a user performs an action — for example, clicking a “Rent” button — the button emits a signal that is connected to a slot, which is a function in your C++ code. That slot function might check scooter availability, run an SQL query to update the rental record, and then refresh the interface to show the scooter as “rented.” This system makes Qt applications responsive and event-driven.

## Integration and Results

By combining Qt and SQLite, the project achieved a fully integrated desktop system capable of managing scooter rentals from a single interface. Users can log in, view available scooters, rent them, and return them, while administrators can monitor all activity in the database. The interaction between the front end (Qt interface) and back end (SQL database) occurs seamlessly, allowing data changes to be reflected instantly in the GUI. The application demonstrates key software engineering principles such as modularity, user data validation, real-time updates, and persistent storage.
