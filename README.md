DESCRIPTION:
    The Bank File Management System is a console-based application developed in C that simulates core banking operations using file handling techniques. The system stores account information in a binary file and maintains a transaction history in a text file.
    This project is designed to demonstrate practical usage of structures, file operations, and modular programming in C, making it ideal for academic and learning purposes.


OBJECTIVES:
    To implement real-world banking operations using C.
    To understand binary file manipulation.
    To demonstrate persistent data storage.
    To apply structured and modular programming concepts.


FUNCTIONALITIES:
Deposit Operation
     Allows users to add money to an existing account
     Updates the balance directly in the binary file

Withdrawal Operation
    Enables secure withdrawal with balance validation
    Prevents overdrawing

Transaction Logging
    Records every transaction in transactions.txt
    Includes timestamp using system time

Mini Statement
    Displays transaction history for a specific account
    Filters relevant entries from the log file


CORE CONCEPTS USED:
    1. Structures (struct) for account data
    2. File Handling:
        Binary Files (rb+)
        Text Files (r, a)
    3. Pointers and file positioning (fseek, fread, fwrite)
    4. Time functions (time.h)
    5. String handling (string.h)


PROGRAM WORKFLOW:
    1. Program opens the binary file (credit.dat)
    2. Displays menu options to the user
    3. Based on input:
        Performs deposit or withdrawal
        Updates file records
        Logs transaction
    4. Mini statement reads from log file
    5. Loop continues until user exits


POSSIBLE IMPROVEMENTS:
    Add user authentication (PIN/password)
    Convert into a web-based banking system
    Generate detailed account reports
