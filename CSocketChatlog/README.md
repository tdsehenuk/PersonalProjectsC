# C Socket Chat App

This is a basic TCP client-server chat program written in C using Winsock. The app allows one client to connect to the server and exchange messages interactively.

## 🛠 Features

- Simple socket communication using Winsock
- Real-time chat between client and server
- Handles basic disconnects and errors

## ✅ Current Status

- [x] One-on-one client-server communication over TCP

## 🚧 Planned Features (for tomorrow)

- [] Support multiple clients using `select()`, `WSAEventSelect`, or multithreading
- [] Log messages to a `.txt` file on both server and client
- [] Add timestamps to messages using `time.h`
- [] Display user names like `[alice]` before each message

## 🔧 How to Run

1. Compile both client and server code with a C compiler that supports Winsock (e.g., MinGW or MSVC).
2. Start the server first.
3. Launch the client and connect to the server.

- gcc .\client.c -o .\client.exe -lws2_32
- gcc .\server.c -o .\server.exe -lws2_32
- .\server.exe
- .\client.exe
