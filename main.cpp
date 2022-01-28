#include <sys/socket.h> // for socket
#include <netinet/in.h> // for sockaddr_in struct
#include <sys/event.h> // for kqueue and kevent
#include <sys/errno.h>
#include <unistd.h> // for close
// #include <iostream>
#include <string>

#include "request/ParseRequest.hpp"

 void handle_events(int listening_sockfd) {
    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    int connection_sock_fd;
    // Prepare the kqueue.
    int sock_kqueue = kqueue(); //creates a new kernel event queue and returns a descriptor.
    if (sock_kqueue < 0) {
        std::cout << "Error creating kqueue. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    struct kevent kev[10], event[10]; // kernel event

    // Create event 'filter', these are the events we want to monitor.
    // Here we want to monitor: listening_sockfd, for the events: EVFILT_READ 
    // (when there is data to be read on the socket), and perform the following
    // actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue 
    // and enable it).
    // so, preparing an event:
    EV_SET(&kev[0], listening_sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0); // is a macro which is provided for ease of initializing a kevent structure. 
    // This will asically create the kevent structure that sets up our kqueue to listen for notifications

    // We set up our kqueue to register our interest in specific events by using the system call kevent.
    if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
        perror("kevent");
        exit(1);
    }
    // Actual event loop.
    while(true){
        // Check for new events, but do not register new events with
        // the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
        // Only handle 1 new event per iteration in the loop; 5th
        // argument is 1.
        //TODO: addding timeout as the last parameter?
        int new_events = kevent(sock_kqueue, NULL, 0, event, 1, NULL);
        if (new_events == -1){
            perror("kevent");
            exit(1);
        }
        for (int i = 0; new_events > i; i++){
            int event_fd = event[i].ident;
            // When the client disconnects an EOF is sent. By closing the file
            // descriptor the event is automatically removed from the kqueue.
            if (event[i].flags & EV_EOF) {
                std::cout << "The client has disconnected." << std::endl;
                close(event_fd);
                std::cout << "FD " << event_fd << " is closed." << std::endl;
            }
            // If the new event's file descriptor is the same as the listening
            // socket's file descriptor, we are sure that a new client wants 
            // to connect to our socket.
            else if (event_fd == listening_sockfd) {
                // Incoming socket connection on the listening socket.
                // Create a new socket for the actual connection to client.
                connection_sock_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
                if (connection_sock_fd < 0 )
                {
                    perror("Accept socket error");
                }
                // Put this new socket connection also as a 'filter' event
                // to watch in kqueue, so we can now watch for events on this
                // new socket.
                EV_SET(kev, connection_sock_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0)
                {
                    perror("kevent error");
                }
            }
            else if (event[i].filter & EVFILT_READ) {
                



                // ! added:
                std::string result = "";
                ParseRequest parse_request;






                char buf[1024];
                size_t bytes_read = recv(event_fd, buf, sizeof(buf), 0);
                if (bytes_read > 0) {

                     // ! added:
                     parse_request.read_HTTP_request(buf, result);

                    printf("read %zu bytes\n", bytes_read);




                }

                // Send a message to the connection
                std::string response = "I do exist!\n";
                send(connection_sock_fd, response.c_str(), response.size(), 0);
                std::cout << "---------Existence confirmed.-----------------------" << std::endl;
            }
        }
    }
}

//  on the server side
int setup_listening_socket() {
    int listening_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (listening_sockfd < 0) {
            std::cout << "Socket failed. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "---------Socket created.-----------------------" << std::endl;
        // When retrieving a socket option, or setting it, you specify the option name as well as the level. When level = SOL_SOCKET, the item will be searched for in the socket itself.
        int value = 1;
        // SO_REUSEADDR Reports whether the rules used in validating addresses supplied to bind() should allow reuse of local addresses,
        // if this is supported by the protocol. More explanation in the docs/resoures/#Sockets
        if (setsockopt(listening_sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
            std::cout << "Setting SO_REUSEADDR failed. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
        // TODO: adding other socket options like TCP_DEFER_ACCEPT?

        sockaddr_in server_addr;
        const int PORT = 8080;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(PORT); // htons is necessary to convert a number to network byte order
        int server_addr_len = sizeof(server_addr);
        if (bind(listening_sockfd, (struct sockaddr*)&server_addr, server_addr_len) < 0) {
            std::cout << "Failed to bind to port " << PORT << ". errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
        // Start listening:
        if (listen(listening_sockfd, 5) < 0) {
            std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

     return listening_sockfd;
}

int main(void) {
    int listening_sockfd = setup_listening_socket();
    handle_events(listening_sockfd); //with kqueue
    close(listening_sockfd);     // TODO:: do we need to close the connection?
}