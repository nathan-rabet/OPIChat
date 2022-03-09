# OPIChat

[![Test suite](https://github.com/nathan-rabet/OPIChat/actions/workflows/unit_testing.yml/badge.svg)](https://github.com/nathan-rabet/OPIChat/actions/workflows/unit_testing.yml)

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

#### Actions to perform

No action is required for the server

### Client

The client executable is called `opichat_client` and is located in the `build/` folder.

To start the client, you must run the following command:

```bash
    ./opichat_client <ip> <port>
```

Where `<ip>` is the IP address of the server and `<port>` is the port where the server is listening to.

#### Actions to perform

When the client launchs, you will deal with the command prompt.

```text
Command:
```

At this step, you must enter a valid command name (as described in the [protocol](PROTOCOL.md)).

If you enter a valid command, you will have one or both of these 2 following prompts:

**Parameters**
At this step, the client program valued that you entered a command which requires parameters.

```text
Parameters:
```

To enter a parameter as a key/value pair, you must type:

```text
<key>=<value>
```

Where `<key>` and `<value>` are respectively the key and the value of the parameter.
Then press <kbd>Enter</kbd>.

> The parameters prompt will always asks you key/value pairs forever until you press <kbd>Enter</kbd> and nothing else.

**Payload**
At this step, you must enter the content of the payload. Unless *Parameters*, no specific format is applied here.

```text
Payload:
```

> Some command (like `SEND-DM` or `BROADCAST`) will loop on the payload prompt forever. To exit the payload prompt, type `/quit`.

### More information

If you are interested in the architecture of the project, check [ARCH.md](ARCH.md).

Also, if you want a detailled documentation about the communication protocol, read [PROTOCOL.md](PROTOCOL.md).
