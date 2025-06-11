# Online Library Management System

The Online Library Management System (OLMS) is built using system calls to perform diverse tasks such as managing processes, handling files, implementing file locking, managing multithreading, and facilitating interprocess communication. Socket programming is employed to establish client-server communication, enabling concurrent access to the library database by multiple clients.

## Server:

Implemented in library_server.c, the server handles multiple clients simultaneously using multi-threading. Each client request includes a specific Request Number, enabling the server to identify and perform the requested operation.

Mutex-based locking ensures safe access to files and resources, preventing clashes when multiple clients access the same resource.

## Client:

Implemented in library_client.c, the client provides a menu-based interface for both members and admins. Depending on the selected operation, a request is generated (beginning with a Request Number) and sent to the server via a socket.

## admin_login.txt

Stores the login credentials for the admin.

## logins.txt

Stores the login details for the members of the library.

## books.txt

Stores the details of the books in the library in the form of (BookID, Book Name, Author, Number of Copies).

## borrowed_books.txt

Stores the details of the borrowed books and the memebrs who borrowed them: (BookID, Username)

