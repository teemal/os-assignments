// R Jesse Chaney
//

#pragma once

#ifndef SOCKET_HDR_H
# define SOCKET_HDR_H

# define MAXLINE 1024
// The size of the buffer used to transfer data from/to the server. This makes
// it easy for me to display how much data have been move from/to the server.
// We want this value to be larger than sizeof(cmd_t)

# define SERV_PORT 10001
// The default port on which the server will try to listen for incomming
// client connections. You may/will need to make use of the command line option
// to alter this when you are testing your code.

# define CMD_LEN 7
# define NAME_LEN 75

#ifndef TRUE
# define TRUE 1
#endif // TRUE
#ifndef FALSE
# define FALSE 0
#endif // FALSE

// The following structure can be used when a client connects to the server. A client
// would fill out the structure as something like:
//   cmd:  get
//   name: zeroes5k.dat
// The client would connect to the server and write the structure to the socket.
// The server would then write the data from the zeroes5k.dat file back to the
// client over the socket connection with the client. The client would create and
// store the file in its directory. Once the data transfer is complete, the server
// will close the socket, making it easy for the client to know when the transfer
// of a file is compelte.
//
// Another example, 
//   cmd:  put
//   name: zeroes5m_Test1.dat
// The client would connect to the server and write the structure to the socket.
// The client would write the data from the zeroes5m_Test1.dat file to the server,
// which would create and store the file in its directory. Once the data transfer
// is complete, the client will close the socket, making it easy for the server to
// know when the transfer is compelte.
//
// Last example:
//   cmd:  dir
// the server use the popen() command to send a listing of it current directory back
// to the client.

typedef struct cmd_s {
    char cmd[CMD_LEN];
    // The command that a client sends to the server.

    char name[NAME_LEN];
    // The file name that a client sends to or fetches from the server.

    // I use this structure in the server process to provide data to the
    // threads that manage the connections with the clients.
    int sock;
    // The socket returned from the accept() call in the server when a new
    // client connects to the server.

    unsigned long tcount;
    // I assign an integer value to each thread that is created when a client
    // connects to the server. I can use this to help track file
    // transfer progress as data are moved from/to the server to/from clients.
} cmd_t;

// Fetch a file from the server to the client.
# define CMD_GET "get"

// Copy a file from the client to the server.
# define CMD_PUT "put"

// Return a directory listing from the server's working
// directory.
# define CMD_DIR  "dir"

# define CLIENT_OPTIONS "i:p:c:uvh"
// The command line options for the client are:
//    i: identify the ip address where the server is running.
//       This should an IPv4 style address.
//    p: The port on which the server process is listening.
//    c: The command the client wishes to send to the server.
//       The choices are
//           get: fetch each file from the server to the client
//           put: copy each file from the client to the server
//           dir: fetch a directory listing from the server.
//                The server can only return the directory listing
//                where it was started.
//                Make use of the popen() command.
//    u: add a 1000 microsecond delay after each read/write on
//       the socket connection to the server. Adding the delay
//       makes it easier to see how the server is managing multiple
//       concurrent connections.
//       This is optional for the client, but required for the server.
//    v: Display a dot for each 1k bytes read from or written to the
//       server. This will allow yout to see how the server is
//       handeling multiple client connections.
//       This is not an optional command line option for the client,
//       your client must accept and perform this action.
//    h: the usual helpful message option.
//       This command line option is optional.

# define SERVER_OPTIONS "p:uvh"
// The command line options for the server are:
//    p: The port on which the server process is listening.
//    u: add a 1000 microsecond delay after each read/write on
//       the socket connection to a client. Adding the delay
//       makes it easier to see how the server is managing multiple
//       concurrent connections.
//       This is optional for the client, but required for the server.
//    v: The usual verbose processing option.
//       This command line option is optional for the server.
//       Though optional, this is a very helpful option. I display
//       a decimal for each 1k of data transfered. Each connection
//       is given a unique integer, starting at 1 and going up.
//    h: the usual helpful message option.
//       This command line option is optional.


// This is the ls command you can use with a call of popen() to
// return the directory listing on the server, from its current
// working directory. There is not a method in this to allow the
// server to change directories.
# define CMD_DIR_POPEN    "ls -lFABh --group-directories-first"

#endif // SOCKET_HDR_H
