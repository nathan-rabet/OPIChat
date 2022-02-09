# OPIChat

*OPIChat* is a simple IRC-like chat software. It is written in C and aims to be a simple, secured and easy to use. The project includes a client and a server.

## Installation and compilation

1. Make sure to have the following dependencies:
   - (GNU) make
   - gcc

2. Clone the repository

3. Build the project using the `make` command.

4. Both of client and server are in the `build/` folder.

## Usage

### Server

The server executable is called `opichat_server` and is located in the `build/` folder.

To start the server, you must run the following command:

```bash
    ./opichat_server <ip> <port>
```

Where `<ip>` is the IP address of the server and <`port`> is the port where clients will connect to.

> Note that you can use `localhost` as a loopback `<ip>` address to start the server.

### Client

The client executable is called `opichat_client` and is located in the `build/` folder.

To start the client, you must run the following command:

```bash
    ./opichat_client <ip> <port>
```

Where `<ip>` is the IP address of the server and `<port>` is the port where the server is listening to.

### More information

If you are interested in the architecture of the project, check [ARCH.md](ARCH.md).

Also, if you want a detailled documentation about the communication protocol, read [PROTOCOL.md](PROTOCOL.md).
