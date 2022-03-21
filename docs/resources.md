# Useful Sources

**How it all works**

-   [What happens when you type a URL in the browser and press enter?](https://medium.com/@maneesha.wijesinghe1/what-happens-when-you-type-an-url-in-the-browser-and-press-enter-bb0aa2449c1a)

**Tutorials - Server setup**

-   [Building a simple web serveur in c++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
-   [C++ Web Programming: CGI program](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
-   [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
-   [Server/Location matching](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)
-   [Building the Web: Sockets and Servers for Dummies](https://levelup.gitconnected.com/building-the-web-sockets-and-servers-for-dummies-886d1595a4f8)
-   [How to build a server from stratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

    **Why not select()**

-   [Why is epoll faster than select?](https://stackoverflow.com/questions/17355593/why-is-epoll-faster-than-select/17355702#:~:text=The%20main%20difference%20between%20epoll,duration%20of%20a%20single%20call)
-   [Differences between poll and select](https://stackoverflow.com/questions/970979/what-are-the-differences-between-poll-and-select)
-   [Why should select() be deprecated](https://beesbuzz.biz/code/5739-The-problem-with-select-vs-poll)

**Kqueue**

-   [Kqueue system call](https://man.openbsd.org/OpenBSD-5.1/kqueue.2)
-   [Kqueue: A generic and scalable event notification facility by Jonathan Lemon](https://people.freebsd.org/~jlemon/papers/kqueue.pdf)
-   [Kqueue : Generic Event notification](https://www.slideshare.net/mahendram/kqueue-generic-event-notification) Some slides about kqueue
-   [Writing a Simple TCP Server Using Kqueue](https://dev.to/frosnerd/writing-a-simple-tcp-server-using-kqueue-cah) - nice diagrams, the code is in Go
-   [A TCP server with kqueue](https://dev.to/frevib/a-tcp-server-with-kqueue-527)
-   [Streaming Server Using Kqueue](https://nima101.github.io/kqueue_server)
-   [Interesting kqueue()-based server] http://www.acme.com/software/thttpd/
-   [I/O Multiplexing (select vs. poll vs. epoll/kqueue)](https://nima101.github.io/io_multiplexing)

**Sockets**

-   [Socket programming](https://www.ibm.com/docs/en/i/7.1?topic=communications-socket-programming)
-   [Ports and Sockets](http://www.danzig.jct.ac.il/tcp-ip-lab/ibm-tutorial/3376c210.html)
-   [Difference between a socket and a port](https://softwareengineering.stackexchange.com/questions/171734/difference-between-a-socket-and-a-port)
-   [SO_REUSEADDR option in setsockopt()](https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ)
-   [Difference between socket and websocket](https://stackoverflow.com/questions/62483790/difference-between-the-socket-socketio-and-websockets), also [here](https://stackoverflow.com/questions/4973622/difference-between-socket-and-websocket) and [here](https://stackoverflow.com/questions/16945345/differences-between-tcp-sockets-and-web-sockets-one-more-time)
-   **Nonblocking I/O**:
    -   [Explain Non-Blocking I/O Like I'm Five](https://dev.to/frosnerd/explain-non-blocking-i-o-like-i-m-five-2a5f)
    -   [Dealing With and Getting Around Blocking Sockets](http://dwise1.net/pgm/sockets/blocking.html) - good explanation but they are using select()
    -   [Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm) - more code but also with select()
    -   [Manage a socket flow of events using poll()](https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select)ch)

**Server model: nginx articles**

-   [Inside Nginx architecture](https://www.nginx.com/blog/inside-nginx-how-we-designed-for-performance-scale/)
-   [Nginx Core Architecture](https://devopspoints.com/nginx-nginx-core-architecture.html): with analogies and diagrams
-   [Open Source - NGINX Architecture](https://www.aosabook.org/en/nginx.html): cofounder of NGINX explains it in detail
-   [Understanding Nginx Server and Location Block Selection Algorithms](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)
-   [NGINX Configuration File](http://nginx.org/en/docs/dirindex.html)
-   [Detailed nginx request body reading process](https://programmer.help/blogs/detailed-nginx-request-body-reading-process.html)
-   [Beginner’s Guide to NGINX](http://nginx.org/en/docs/beginners_guide.html)
-   **CGI Doc**

-   [RFC 3875: CGI](https://www.rfc-editor.org/rfc/rfc3875.pdf)
-   [What is a CGI?](https://www.oreilly.com/library/view/cgi-programming-on/9781565921689/04_chapter-01.html)
-   [Best CGI Exemple](https://www.fi.muni.cz/usr/jkucera/tic/tic0305.html): Programmation CGI in C++.
-   [CGI Environmnent Variables](https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI)
-   [Good CGI Howto (and cookies)](http://www.purplepixie.org/cgi/howto.php)
-   [Some examples](https://pub.phyks.me/sdz/sdz/ecrivez-votre-site-web-en-c-avec-la-cgi.html)
-   [CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)
-   [CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)

-   **Nginx Process Request Info**
-   [Phases of Request Processing] (http://nginx.org/en/docs/dev/development_guide.html)

**Tools**

-   [Super Mega Site pour tester les Headers !](https://reqbin.com/)
-   [cURL "--resolve"; curl -H "Host: ..."](https://sodocumentation.net/curl/topic/10565/name-resolve-curl-tricks) (for testing different server names).

### Documentation :

**HTTP 1.1 (standard to follow) :**

[HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

**Other HTTP (legacy / future) :**

[HTTP/1.0 (RFC 1945)](https://www.rfc-editor.org/rfc/rfc1945.html)

[HTTP/2 (RFC 7240)](https://www.rfc-editor.org/rfc/rfc7540.html)

[HTTP/2 : Header Compression (RFC 7241)](https://www.rfc-editor.org/rfc/rfc7541.html)

[FTP (RFC 959)](https://www.rfc-editor.org/rfc/rfc959.html)

**HTTP Header Syntax**

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

**HTTP documentation**

-   [RFC 2616: HTTP 1.1 protocol](https://datatracker.ietf.org/doc/html/rfc2616)
-   [RFC 7230: HTTP/1.1 Message Syntax and Routing](https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf)
-   [List of HTTP status codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
-   [Content-Type](<https://docs.microsoft.com/en-us/previous-versions/exchange-server/exchange-10/ms526508(v=exchg.10)?redirectedfrom=MSDN>)
-   [Content-Type Stack List](https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header)
-   [Content-Type Full List](https://www.iana.org/assignments/media-types/media-types.xhtml)
-   [Tres bon site aussi !](https://developer.mozilla.org/fr/docs/Web/HTTP/Status/)
-   [HTTP messages](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)
-   [Redirections](https://developer.mozilla.org/fr/docs/Web/HTTP/Redirections)

**URI documentation**
-   [URI dedicated RFC 3986](https://datatracker.ietf.org/doc/html/rfc3986)

**Useful RFCs**

-   [RFC editor](https://www.rfc-editor.org/retrieve/): official source for RFCs on the World Wide Web.
-   [RFC 2396: Uniform Resource Identifiers (URI): Generic Syntax](https://datatracker.ietf.org/doc/html/rfc2396): useful definitions of URI, port and host

**Useful C functions**

-   [socket](https://man7.org/linux/man-pages/man2/socket.2.html): creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
-   [listen](https://man7.org/linux/man-pages/man2/listen.2.html): listens for connections on a socket.
-   [poll](https://man7.org/linux/man-pages/man2/poll.2.html): waits for one of a set of file descriptors to become ready to perform I/O.
-   [accept](https://man7.org/linux/man-pages/man2/accept.2.html): accepts a connection on a socket.
-   [recv() return values ](https://stackoverflow.com/questions/17824865/non-blocking-recv-returns-0-when-disconnected/17831323): discussion on when recv() returns 0 when disconnected
-   [Linux System Calls](https://cloudchef.medium.com/linux-system-calls-c2867c7c30c1) - nice diagram about how communication system calls work
-   [Select](https://www.lowtek.com/sockets/select.html): we're not using it but it might be helpful to read about it

### Additional resources:

**Parsers & State Machines**

-   [PARSERS AND STATE MACHINES](https://www.pearsonhighered.com/assets/samplechapter/0/3/2/1/0321112547.pdf)
-   [High Performance Text Parsing Using Finite State Machines (FSM)](https://hackernoon.com/high-performance-text-parsing-using-finite-state-machines-fsm-6d3m33j9)
-   [Finite State Machine Parsing for Internet Protocols](https://ieeexplore.ieee.org/document/6957302)

**Setting up integration tests with Jest**

-   [A step-by-step intro to end-point testing](https://www.freecodecamp.org/news/end-point-testing/)
-   [Beyond API testing with Jest](https://circleci.com/blog/api-testing-with-jest/)

## **Testers**

-   [webserv_tester](https://github.com/acoudert/webserv_tester) by [@acoudert](https://github.com/acoudert)
-   [webserv_tester](https://github.com/fredrikalindh/webserv_tester) by [@fredrikalindh](https://github.com/fredrikalindh)
