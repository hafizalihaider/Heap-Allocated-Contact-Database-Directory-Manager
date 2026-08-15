# Heap-Allocated Contact Database Directory Manager

A console-based **Contact Database Directory Manager written in C** using dynamic memory allocation, structures, file handling, searching, sorting, editing, and deletion.

The project is designed to demonstrate practical use of **heap-allocated structures and modular C programming** while maintaining contact data persistently in a text file.

## Features

* Add new contacts
* Automatically generate unique contact IDs
* View all stored contacts
* Search contacts by ID
* Edit existing contact information
* Delete contacts
* Sort contacts by ID
* Dynamically expand memory using `realloc()`
* Save contacts to a text file
* Load previously saved contacts when the program starts
* Preserve the next available contact ID after restarting
* Validate menu input and reject invalid values
* Handle memory allocation and file-opening failures

## Contact Information

Each contact contains:

* **ID**
* **Name**
* **Phone Number**
* **Email ID**

The contact is represented using a C structure:

```c
typedef struct {
    char id[50];
    char name[50];
    char phone[20];
    char email[100];
} Contact;
```

## Project Structure

```text
Heap-Allocated-Contact-Database-Directory-Manager/
│
├── src/
│   ├── main.c
│   ├── main.exe
│   └── contacts.txt
│
└── README.md
```

> `contacts.txt` is generated automatically when contacts are saved.

## Core Concepts Demonstrated

### 1. Structures

The `Contact` structure groups related contact information into a single data type.

### 2. Dynamic Memory Allocation

The program initially allocates memory for two contacts using:

```c
malloc()
```

When the allocated memory becomes full, the capacity is doubled using:

```c
realloc()
```

This allows the database to grow dynamically without requiring a fixed maximum number of contacts.

### 3. Pointers

The program uses pointers extensively to modify dynamically allocated memory and maintain the current contact count, capacity, and next contact ID.

For example:

```c
Contact **contacts
```

is used when a function may need to modify the dynamically allocated contact array itself.

### 4. Automatic ID Generation

Each contact receives an automatically generated ID:

```text
CONTACT-001
CONTACT-002
CONTACT-003
...
```

The `next_id` variable ensures that IDs continue increasing during the current program session.

### 5. File Handling

Contacts are saved to:

```text
contacts.txt
```

The program uses:

```c
fopen()
fprintf()
fgets()
fclose()
```

The file is rewritten after contact modifications so that the stored database remains synchronized with the in-memory data.

### 6. Persistent Data

When the program starts, `load_contacts()` reads existing contacts from `contacts.txt`.

The program also identifies the highest existing contact ID and sets the next ID accordingly.

For example, if the file contains:

```text
CONTACT-001
CONTACT-003
CONTACT-004
```

the next generated ID will be:

```text
CONTACT-005
```

### 7. Searching

The `find_contact()` function searches the contact array using the contact ID and returns its array index.

If the contact does not exist:

```c
return -1;
```

### 8. Sorting

Contacts are sorted by ID using a basic **Bubble Sort** implementation.

The comparison is performed using:

```c
strcmp()
```

This keeps the contacts organized before they are displayed or saved.

### 9. Input Validation

The menu does not directly rely on:

```c
scanf("%d", &choice);
```

Instead, `get_menu_choice()` reads the entire input as a string and uses:

```c
strtol()
```

This prevents inputs such as:

```text
abc
7jk
1abc
3.5
-1
+2
0abc
```

from being incorrectly interpreted as valid menu choices.

Only integer values from **0 to 5** are accepted.

## Program Menu

```text
=======================================================
            CONTACT DATABASE DIRECTORY MANAGER
=======================================================

1. Add Contact
2. View All Contacts
3. Search Contact
4. Edit Contact
5. Delete Contact
0. Exit
```

## Add Contact Workflow

When adding a contact:

1. Check whether the allocated array is full.
2. Expand memory if required.
3. Receive contact information from the user.
4. Generate a unique contact ID.
5. Increase the contact count.
6. Sort the contacts.
7. Save the updated database to the file.

## Edit Contact Workflow

The user provides a contact ID.

The program:

1. Searches for the contact.
2. Retrieves its array index.
3. Updates the name, phone number, and email.
4. Sorts the database.
5. Saves the updated database.

The contact ID itself is not changed.

## Delete Contact Workflow

The user provides the contact ID to delete.

The program:

1. Searches for the contact.
2. Finds its array index.
3. Shifts all following contacts one position to the left.
4. Decreases the contact count.
5. Sorts the remaining contacts.
6. Saves the updated database.

## Data File Format

The generated `contacts.txt` file is formatted for readability:

```text
=======================================================
            CONTACT DATABASE DIRECTORY
=======================================================

Contact 1
-------------------------------------------------------
ID            : CONTACT-001
Name          : Ali
Phone Number  : 03264311504
Email ID      : hafizalihaider1942@gmail.com
=======================================================

Contact 2
-------------------------------------------------------
ID            : CONTACT-002
Name          : Example User
Phone Number  : 03001234567
Email ID      : example@gmail.com
=======================================================
```

## Error Handling

The program handles several common failures:

### Memory allocation failure

```c
if (contacts == NULL)
```

and:

```c
if (temp == NULL)
```

prevent the program from continuing when memory allocation fails.

### File opening failure

If `contacts.txt` cannot be opened, the program reports the error instead of attempting to write to an invalid file pointer.

### Invalid menu input

The program repeatedly asks for input until a valid menu number from `0` to `5` is entered.

### Contact not found

Search, edit, and delete operations report:

```text
Contact not found.
```

when the requested ID does not exist.

## Compilation

Using GCC:

```bash
gcc main.c -o main
```

Run:

```bash
./main
```

On Windows PowerShell:

```powershell
.\main.exe
```

## Technologies

* **Language:** C
* **Compiler:** GCC
* **Memory Management:** `malloc()`, `realloc()`, `free()`
* **File Handling:** C standard I/O
* **Data Structure:** Array of structures
* **Sorting Algorithm:** Bubble Sort
* **String Processing:** `strcmp()`, `strcspn()`, `strtol()`, `sscanf()`

## Learning Objectives

This project demonstrates practical understanding of:

* C structures
* Pointers and pointer-to-pointer usage
* Heap memory management
* Dynamic arrays
* String manipulation
* Input validation
* File I/O
* Searching
* Sorting
* Modular function design
* Persistent data storage
* Basic error handling

## Limitations

This is a learning-focused console application rather than a production database system.

Current limitations include:

* Data is stored in a plain-text file rather than a database.
* Contact IDs are generated sequentially.
* Contact fields have fixed maximum string lengths.
* Search is linear, giving **O(n)** search complexity.
* Sorting uses Bubble Sort with **O(n²)** worst-case complexity.
* The program does not perform advanced validation of phone numbers or email addresses.

## Future Improvements

Possible extensions include:

* Search by name, phone number, or email
* Stronger phone and email validation
* Case-insensitive searching
* Faster sorting using `qsort()`
* Binary search after maintaining sorted IDs
* CSV or binary file storage
* Password-protected contact database
* Encryption of stored contact information
* Separate source/header files
* Unit testing
* More robust file corruption handling
* A graphical user interface

## Author

**Muhammad Ali Haider**

## License

This project is intended for educational and learning purposes.
