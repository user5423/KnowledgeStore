### NetBIOS

NetBIOS, or _Network Basic Input/Output System_ is a layer 5 session protocol on the OSI model which enables applications on different hosts to communicate with each other over a Local Area Network. In modern networks, NetBIOS runs over TCP/IP using the _NetBIOS over TCP/IP **(NBT)**_ protocol, however, previous NetBIOS implementations ran on other layer 3 network protocols such as IPX, SPX or even the upper layer 2 data link protocol LLC.

NetBIOS provides three distinct services:

*   **Name Service** - Before applications can initiate sesions with other applications running on hosts of the same network, the application needs to register a NetBIOS name using the name service. NetBIOS names have 16 ASCII characters with the 16th character reserved as a NetBIOS suffix, which is used to tell other applications what type or resource or services a system has to offer. This can describe a record type such as host record, master browser record, or domain controller record. NetBIOS names are exclusively alphanumerical, and since Windows 2000, NetBIOS names have to comply with DNS name restrictions and rules. NetBIOS names are often the same as the host's hostname, truncated at 15 characters, although this isn't always the case.

    
    <br>In order to connect to a host on a TCP/IP network via it's NetBIOS name, its neccessary to resolve the name to an IP address. In modern-day IP networks, NetBIOS name resolution is either done by broadcasting, or a **WINS** server (a NetBIOS Name server). For networks that don't use WINS servers for NetBIOS name resolution, the Windows LMHOSTS file provides a NetBIOS name resolution method as a fall back.

    <br> The operations that NetBIOS name services provides include:
    
    *   **Add Name** - register a NetBIOS name
    *   **Add Group Name** - registers a NetBIOS "group" name
    *   **Delete Name** - un-registers a NetBIOS name or group name
    *   **Find Name** - looks up a NetBIOS name on the network

    <br>It's important to note that NetBIOS name resolution isn't supported by Microsoft for IPv6.
    <br>
    
*   **Datagram distribution service** - transport of PDU's are connectionless which means that it is up to the Application to implement error-checking and recovery. For NetBIOS over TCP/IP (NTB), this runs on UDP port 138

    The operations that NetBIOS datagram distribution provides include:

    *  **Send Datagram** - sends a datagram to a remote NetBIOS name
    *  **Send Broadcast** Datagram - sends a datagram to all remote NetBIOS names on the network
    *  **Receive Datagram** - waits for a packet to arrive from a Send Datagram operation
    *  **Receive Brodcast Datagram** - waits for a packet to arrive from a Send Brodcast Datagram
    <br>

*   **Session service** - transport of PDU's are connection-oriented, and so messages can span mulitple packets without the application requiring to provide error checking and recovery. For NetBIOS over TCP/IP (NTB), this runs on TCP port 139. 
    
    <br> 
    The operations that NetBIOS session service provides include:
    
    *   **Call** - opens a session to a remote NetBIOS name
    *   **Listen** - listens for attempts by a remote NetBIOS name to initiate a session
    *   **Hang up** - closes the sesion
    *   **Send** - sends a packet to the remote NetBIOS host on the other side of the session
    *   **Send No Ack** - sends a packet to the remote NetBIOS, but doesn't require a acknowledgemnt packet to be returned
    *   **Received** - waits for a packet to arrive from a Send by the remote NetBIOS host on the other side of the session

The original NetBIOS was developed in the 1980s intended for small newtorks. However applications using legacy NetBIOS don't scale well as these run NetBIOS over NetBIOS frames (NBF). However, the more modern version of NetBIOS runs over TCP/IP which is able to scale applications to be run in large TCP/IP networks including the internet, this protocol is known as (NBT).


####  TODO: Finish off KnowledgeStore
- Add diagrams and show the interactions
- Add hands on (e.g. on the wire)
- Add security perspective
