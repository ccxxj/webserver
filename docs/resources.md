# Useful Sources?

## **Sources**

- [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
- [Server/Location matching](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)
- [Configuration File](http://nginx.org/en/docs/dirindex.html)
- [Building the Web: Sockets and Servers for Dummies](https://levelup.gitconnected.com/building-the-web-sockets-and-servers-for-dummies-886d1595a4f8)

### **Server setup**

[How to build a simple HTTP server](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

[C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)

### **HTTP 1.1 (standard to follow) :**

[HTTP/1.1 (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616.html)

[HTTP/1.1 : Message Syntax and Routing (RFC 7230)](https://www.rfc-editor.org/rfc/rfc7230.html)

[HTTP/1.1 : Semantics and Content (RFC 7231)](https://www.rfc-editor.org/rfc/rfc7231.html)

[HTTP/1.1 : Conditional Requests (RFC 7232)](https://www.rfc-editor.org/rfc/rfc7232.html)

[HTTP/1.1 : Range Requests (RFC 7233)](https://www.rfc-editor.org/rfc/rfc7233.html)

[HTTP/1.1 : Caching (RFC 7234)](https://www.rfc-editor.org/rfc/rfc7234.html)

[HTTP/1.1 : Authentication (RFC 7235)](https://www.rfc-editor.org/rfc/rfc7235.html)

### **Other HTTP (legacy / future) :**

[HTTP/1.0 (RFC 1945)](https://www.rfc-editor.org/rfc/rfc1945.html)

[HTTP/2 (RFC 7240)](https://www.rfc-editor.org/rfc/rfc7540.html)

[HTTP/2 : Header Compression (RFC 7241)](https://www.rfc-editor.org/rfc/rfc7541.html)

[FTP (RFC 959)](https://www.rfc-editor.org/rfc/rfc959.html)

### **HTTP Header Syntax**

[HTTP Request Methods](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol#Request_methods)

[HTTP Status Codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

[HTTP Header Break Style](https://stackoverflow.com/questions/5757290/http-header-line-break-style)

### **Select and non-blocking**

[Select](https://www.lowtek.com/sockets/select.html)



### **Why epoll() but not select()**
- [epoll man page](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [Why is epoll faster than select?](https://stackoverflow.com/questions/17355593/why-is-epoll-faster-than-select/17355702#:~:text=The%20main%20difference%20between%20epoll,duration%20of%20a%20single%20call) and an article about  [differences between poll and select](https://stackoverflow.com/questions/970979/what-are-the-differences-between-poll-and-select)  and this source about  [why should select() be deprecated](https://beesbuzz.biz/code/5739-The-problem-with-select-vs-poll)
- [Detailed explanation of epoll of linux high concurrency network programming](https://blog.csdn.net/mengzuchao/article/details/81294554) - use google translate 
- **Examples of programming a socket with epoll:**
  - [Single thread TCP server by using linux's epoll facility](http://swingseagull.github.io/2016/11/08/epoll-sample/)
  - [socket programming and epoll model in Linux](https://www.fatalerrors.org/a/socket-programming-and-epoll-model-in-linux.html)

### **CGI**

[CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)

**Tutorials**

- [Building a simple web serveur in c++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
- [C++ Web Programming: CGI program](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [Manage a socket flow of events using poll()](https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select)ch)

**HTTP documentation**

- [RFC 2616: HTTP 1.1 protocol](https://datatracker.ietf.org/doc/html/rfc2616)
- [RFC 7230: HTTP/1.1 Message Syntax and Routing](https://www.rfc-editor.org/rfc/pdfrfc/rfc7230.txt.pdf)
- [List of HTTP status codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)
- [Content-Type](https://docs.microsoft.com/en-us/previous-versions/exchange-server/exchange-10/ms526508(v=exchg.10)?redirectedfrom=MSDN)
- [Content-Type Stack List](https://stackoverflow.com/questions/23714383/what-are-all-the-possible-values-for-http-content-type-header)
- [Content-Type Full List](https://www.iana.org/assignments/media-types/media-types.xhtml)
- [Tres bon site aussi !](https://developer.mozilla.org/fr/docs/Web/HTTP/Status/)
- [HTTP messages](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages)
- [Redirections](https://developer.mozilla.org/fr/docs/Web/HTTP/Redirections)

**Useful RFCs**

- [RFC editor](https://www.rfc-editor.org/retrieve/): official source for RFCs on the World Wide Web.
- [RFC 2396: Uniform Resource Identifiers (URI): Generic Syntax](https://datatracker.ietf.org/doc/html/rfc2396): useful definitions of URI, port and host

**Useful C functions**

- [socket](https://man7.org/linux/man-pages/man2/socket.2.html): creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
- [listen](https://man7.org/linux/man-pages/man2/listen.2.html): listens for connections on a socket.
- [poll](https://man7.org/linux/man-pages/man2/poll.2.html): waits for one of a set of file descriptors to become ready to perform I/O.
- [accept](https://man7.org/linux/man-pages/man2/accept.2.html): accepts a connection on a socket.
- [Select](https://www.lowtek.com/sockets/select.html): we're not using it but it might be helpful to read about it

**CGI Doc**

- [RFC 3875: CGI](https://www.rfc-editor.org/rfc/rfc3875.pdf)
- [What is a CGI?](https://www.oreilly.com/library/view/cgi-programming-on/9781565921689/04_chapter-01.html)
- [Best CGI Exemple](https://www.fi.muni.cz/usr/jkucera/tic/tic0305.html): Programmation CGI in C++.
- [CGI Environmnent Variables](https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI)
- [Good CGI Howto (and cookies)](http://www.purplepixie.org/cgi/howto.php)
- [Some exemples](https://pub.phyks.me/sdz/sdz/ecrivez-votre-site-web-en-c-avec-la-cgi.html)

**Tools**

- [Super Mega Site pour tester les Headers !](https://reqbin.com/)
- [cURL "--resolve"; curl -H "Host: ..."](https://sodocumentation.net/curl/topic/10565/name-resolve-curl-tricks) (for testing different server names).

**Server model: nginx documentation**

- [Inside Nginx architecture](https://www.nginx.com/blog/inside-nginx-how-we-designed-for-performance-scale/)
- [Understanding Nginx Server and Location Block Selection Algorithms](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)

**Sockets**
- [SO_REUSEADDR option in setsockopt()](https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ)
- [Ports and Sockets](http://www.danzig.jct.ac.il/tcp-ip-lab/ibm-tutorial/3376c210.html)
- [Difference between a socket and a port](https://softwareengineering.stackexchange.com/questions/171734/difference-between-a-socket-and-a-port)

- [Socket programming](https://www.ibm.com/docs/en/i/7.1?topic=communications-socket-programming)

- [Difference between socket and websocket](https://stackoverflow.com/questions/62483790/difference-between-the-socket-socketio-and-websockets), also [here](https://stackoverflow.com/questions/4973622/difference-between-socket-and-websocket) and [here](https://stackoverflow.com/questions/16945345/differences-between-tcp-sockets-and-web-sockets-one-more-time)
- **Nonblocking I/O**:
- [Dealing With and Getting Around Blocking Sockets](http://dwise1.net/pgm/sockets/blocking.html) - good explanation but they are using select()
- [Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm) - more code but also with select()
- [Blocking I/O, Nonblocking I/O, And Epoll](https://eklitzke.org/blocking-io-nonblocking-io-and-epoll) -more information about edge-triggered polling in the epoll system call



## **Testers**

- [webserv_tester](https://github.com/acoudert/webserv_tester) by [@acoudert](https://github.com/acoudert)
- [webserv_tester](https://github.com/fredrikalindh/webserv_tester) by [@fredrikalindh](https://github.com/fredrikalindh)
