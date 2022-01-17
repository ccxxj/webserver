https://www.rfc-editor.org/search/rfc_search_detail.php?title=HTTP&page=All 7230 - 7235

# Useful Sources?

## **Sources**

- [Beej's Guide to Network Programming](http://beej.us/guide/bgnet/)
- [Server/Location matching](https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms)
- [Configuration File](http://nginx.org/en/docs/dirindex.html)

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

[Non-blocking I/O](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm)

### **CGI**

[CGI : Getting Started](http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm)

[CGI 1.1 Documentation](http://www.wijata.com/cgi/cgispec.html#4.0)

**Tutorials**

- [Building a simple web serveur in c++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
- [C++ Web Programming: CGI program](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)
- [HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [Manage a socket flow of events using poll()](https://www.ibm.com/docs/en/i/7.2?topic=designs-using-poll-instead-select)
- [C++ programming applied to network](https://bousk.developpez.com/cours/reseau-c++/) (in French)
- [Network programming](https://beej.us/guide/bgnet/)

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

**CGI Doc**

- [RFC 3875: CGI](https://www.rfc-editor.org/rfc/rfc3875.pdf)
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

Sockets

A Beginner's Guide to WebSockets > [https://www.youtube.com/watch?v=8ARodQ4Wlf4&ab_channel=freeCodeCamp.org](https://www.youtube.com/watch?v=8ARodQ4Wlf4&ab_channel=freeCodeCamp.org)

[http://dwise1.net/pgm/sockets/blocking.html](http://dwise1.net/pgm/sockets/blocking.html)

[https://www.ibm.com/docs/en/i/7.1?topic=communications-socket-programming](https://www.ibm.com/docs/en/i/7.1?topic=communications-socket-programming)

## **Testers**

- [webserv_tester](https://github.com/acoudert/webserv_tester) by [@acoudert](https://github.com/acoudert)
- [webserv_tester](https://github.com/fredrikalindh/webserv_tester) by [@fredrikalindh](https://github.com/fredrikalindh)
