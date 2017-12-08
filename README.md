# Interprocessor-Communication
A client-server interprocess communication system implementation


# How to Run?
### 1. Compile server.cpp and client.cpp by typing
`g++ client.cpp -o client.o -lpthread`
`g++ server.cpp -o server.o -lpthread`

### 2. Connect server by typing
`./server.o`

### 3. Then start multiple clients and enter different
usernames for each (For same usernames program prints
empty strings as expected from us.)

### 4. After all, you can send and receive texts from clients.

// Development environment: Ubuntu 16.04 - ITU SSH servers

Tested in: gcc 4.9.2 & 4.8.5

Everthing Should Work Fine!
