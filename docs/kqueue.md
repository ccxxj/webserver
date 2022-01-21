KQUEUE explained
In the past, before kqueue(2)1/ kevent(2)2, when we wanted to track events related to a socket, or if a file descriptor changed, 
we used a polling mechanism to test for readiness/events (maybe using select(2) or poll(2)). 
This polling techniques were inefficient and got replaced by the more efficient kqueue API. 
Instead of continually polling, we can now be notified about events by the kernel. 
Also, mechanisms like select(2) and poll(2) were restricted to work with file descriptors. 
kqueue increases the range of operating systems events to monitor not only for file descriptors but also elements like signals, timers, network devices, etc.


So if it is inefficient to poll for events, and sometimes we can't obtain the event directly. How can we know if an event occurred?

We know that all the system calls always notify the kernel, so it would be the perfect place to get the notifications from.

That is the idea behind kqueue. We create a kernel event queue, where we show our interest to the kernel for specific events, 
and when the kernel has some information about the events we are interested in it'll push it to our queue.

The process is as follows:

1. Create a kernel event queue using the system call kqueue this returns a file descriptor. We can access our kernel queue using that file descriptor.
```
    int sock_kqueue = kqueue(); //creates a new kernel event queue and returns a descriptor.
    if (sock_kqueue < 0) {
        std::cout << "Error creating kqueue. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
```
2. We set up our kqueue to register our interest in specific events by using the system call kevent.
 kevent() is used to register events with the queue, and return any	pending events	to the user.
```
    if (kevent(sock_kqueue, kev, 1, NULL, 0, NULL) < 0) {
        perror("kevent");
        exit(1);
    }
```
    But before doing it we will be using the macro EV_SET to initialize our kev structure:
    ```
    EV_SET(&kev[0], listening_sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    ```
    It will be basically  needed to create the kevent structure that sets up our kqueue to listen for notifications. 
    How we would be filling it:
    - Here we are using the file descriptor _listening_sockfd_ that we received as a parameter.
      We want to be notified when the socket (represented by the file descriptor) has data to be read. 
    - We also want to add the following actions EV_ADD and EV_ENABLE, this means add this event to our kqueue and allow the event to be returned when triggered. 
    - The flags, data, and udata we are not going to use so we set them to 0 or nil on each case.

 

3. Again we use kevent but this time to listen for the events to be triggered.
```
    int new_events = kevent(sock_kqueue, NULL, 0, event, 1, NULL);
```

We wait for any event on our sock_kqueue to be triggered and then we read the data that is populated in the event structure
After we are sure that the socket has data we call _recv_ to read the data. 
Then we start the loop again and call kevent that will block and wait for another event to be triggered. We could have also added a timer as a time out.


*** SYSTEM CALL KEVENT***

```
   int kevent(int	kq, const struct kevent	*changelist, int nchanges,
    struct	kevent *eventlist, int nevents,
    const struct timespec *timeout);
```
   *** changelist *** is a pointer to an array of kevent
    structures, as defined in <sys/event.h>.  All changes contained in	the
    changelist	are applied before any pending events are read from the	queue.
    nchanges gives the	size of	changelist.  eventlist is a pointer to an ar-
    ray of kevent structures.	nevents	determines the size of eventlist.  If
    timeout is	a non-NULL pointer, it specifies a maximum interval to wait
    for an event, which will be interpreted as	a struct timespec.  If timeout
    is	a NULL pointer,	kevent() waits indefinitely.  To effect	a poll,	the
    timeout argument should be	non-NULL, pointing to a	zero-valued timespec
    structure.	 The same array	may be used for	the changelist and eventlist.

*** THE STRUCTURE KEVENT ***
```
    public struct kevent {

    public var ident: UInt /* identifier for this event */

    public var filter: Int16 /* filter for event */

    public var flags: UInt16 /* general flags */

    public var fflags: UInt32 /* filter-specific flags */

    public var data: Int /* filter-specific data */

    public var udata: UnsafeMutableRawPointer! /* opaque user data identifier */

    public init()

    public init(ident: UInt, filter: Int16, flags: UInt16, fflags: UInt32, data: Int, udata: UnsafeMutableRawPointer!)
}
```
_Remember that the structure can be populated by you, or by the kevent system call. 
So some of the fields make sense only when written by the triggered event._

***ident:*** this property is used to identify our event. 
For example, it could be a file descriptor for a file, or a file descriptor for a socket, or a specific signal, etcetera. 
In combination with the filter property, we can determine what the ident property points to.
For example, if the filter is EVFILT_SIGNAL , the ident property will point to a specific system signal.
***filter:*** identifies the filter to be used for this event.
- Next you'll see a few examples for this field, to see the system supported filters, check the man page for kevent4
    - EVFILT_READ - triggers when the file descriptor is ready for reading.
    - EVFILT_WRITE - triggers when the file descriptor is available for writing.
***flags:*** this property indicates the actions for this event.
- Next are a few examples for this field, to see the system supported flags, check the man page for kevent5
    - EV_ENABLE - allows the kevent to return the event if the event is triggered.
    - EV_EOF - some filters set this flag to indicated End-Of-File conditions.
*** fflags:*** filter-specific flags.
- For example, for the filter EVFILT_VNODE, we could have the following filter-specific flags: (for a full list of filter-specific flags6)
    - NOTE_ATTRIB - triggers when the file descriptor had its attributes changed.
    - NOTE_WRITE - triggers when there was a write event on the file descriptor.
*** data:*** filter-specific data. This field can be written by kevent when the event is triggered.
*** udata:*** Opaque user-defined value passed through the kernel unchanged.                     

The sources used:
https://rderik.com/blog/using-kernel-queues-kqueue-notifications-in-swift/
