## SNMP


*Simple Network Management Protocol* is an application layer protocol that collects and organizes information on managed devices, configures remote devices and detects network faults on a IP network.

Here are the main componenets of SNMP

*   **SNMP manager** - they are responsible for gathering information on network hosts and infrastructure by sending query requests to hosts that run SNMP agent software. Hosts running SNMP managers are also known as _Network Management Stations **(NMS)**_. Most of the commands defined within the SNMP protocol are designed to be _sent_ by a manager component.
*   **SNMP agent** - this is software installed on a managed host with it's responsibility being to gather information about the local system and then storing / updating the data to the _management information base **(MIB)**_. SNMP agents respond to most of the commands defiend by the protocol.


### Management Information Base (MIB)

The _MIB_ is a standardized database that both managers and agents adhere to. However, while it is standardized, MIB provides flexiblity that allows for vendor specific additions. MIBs are top-down hierachical trees, where each entry or branch is addressed using a _Object Identifier **(OID)**_. Each branch from a parent node has a value assigned to it, that we use with a sequence used to access a particular variable in the MIB database, which we can simply call the _branch path_. Here's an example of an MIB:

![](http://www.opencircuits.com/images/thumb/6/6a/Snmp_mib_oid_tree_example.jpg/900px-Snmp_mib_oid_tree_example.jpg)

If I wanted to access the the mgmt section, I would either use one of the following paths:

*   **Numerical Path** - 1.3.6.1.2
*   **String Path** - iso.org.dod.internet.mgmt

If you want to familiarize yourself with the MIB database, it's structure, and the individual Object Indentifiers (OIDs), you can use [this](https://www.alvestrand.no/objectid/1.3.6.1.2.1.html)

### Protocol Commands

Now that you understand how the Management Information Base (MIB) database works, we're going to see how we use SNMP protocol commands to query the database. SNMP management software uses UDP to communicate with SNMP managed-devices (ie. SNMP agents). SNMP managers will send _Protocol Data Units_, and since the protocol uses UDP, the _**PDU**_ will be _Datagrams_.

*   **GetRequest** - A manager-to-agent request for values of specified Object Identifiers (OIDs), which is answered with a _response_ message sent back to the manager with the data.
*   **SetRequest** - A manager-to-agent request to change/set the value of specified OID variables. A _response_ message is sent back from the agent to the manager with the new current values for the variables that were newly set.
*   **GetNextRequest** - A manager-to-agent request that a manager can use to request the next sequential object in the MIB without having to worry about what OID paths to use in their queries. This command can be used iteratively to traverse the entire MIB database and return all values.
*   **GetBulkRequest** - A manager-to-agent request used for multiple iterations of _GetNextRequest_, and the user will send a _response_ message with multiple variable bindingsd walked from the initial varaible binding/bindings in the request. This command was introduced in SNMPv2
*   **Response** - A agent-to-management response that returns variable bindings and acknowledgments for SNMP command requests such as _GetRequest_, _SetRequest_, _GetNextRequest_, _InformRequest_ etc. If data cannot be returned from a manager-to-agent request, the response contains error fields includeing error-status and error-index fields. Eventhough it is used as a response for both gets and sets, it was originally called _GetResponse_ in SNMPv1.
*   **Trap** - Unlike in other SNMP communication where the manager actively queries the agent for information, _Trap_ PDUs are sent by the agent to the manager without being explicity requested, so are considered _asynchronous notifications_. Trap PDU's are sent by the agent, usually when there is some siginficant event, such as a network fault, that occured on the managed device; standard traps include: _coldStart_, _warmStart_, _linkDown_, _linkUp_, _authenticationFailure_, _egpNeighborLoss_. For more information, CISCO havs a more [in-depth explanation](https://www.cisco.com/c/en/us/support/docs/ip/simple-network-management-protocol-snmp/7244-snmp-trap.html). This helps reduce congestion on the network, as instead of the the SNMP manager constantly polling agents for information, there will only be a response being sent across the network instead of a request and response.
*   **InformRequest** - Introduced in SNMPv2c, this PDU was used for acknowledgement of asynchronous notificiations, ie. acknowledgement of packets sent from a Trap. Since UDP doesn't provide reliable communication, SNMP handles this on the application layer through _InformRequest_, which simply sends an acknowledgement upon receipt.


### Protocol Versions

*   **SNMPv1** - This was the initial implementation of the SNMP protocol which was designed in the 1980s. The original specification came out in 1988 layed out on :
    
    *   [RFC 1065](https://tools.ietf.org/html/rfc1065) - Structure and identification of management information for TCP/IP-based internets
    *   [RFC 1066](https://tools.ietf.org/html/rfc1066) - Management information base for network management of TCP/IP-based internets
    *   [RFC 1067](https://tools.ietf.org/html/rfc1067) - A simple network management protocol
    
    These specification documents were superseded in 1990:
    
    *   [RFC 1155](https://tools.ietf.org/html/rfc1065) - Structure and identification of management information for TCP/IP-based internets
    *   [RFC 1156](https://tools.ietf.org/html/rfc1066) - Management information base for network management of TCP/IP-based internets
    *   [RFC 1157](https://tools.ietf.org/html/rfc1067) - A simple network management protocol
    
This is the most widely deployed SNMP generation used today in TCP/IP networks. This initial standard defined support for SNMP GET, GETNEXT, SET operations, aswell as for asynhronouse notifications called TRAPs. However, SNMPv1 doesn't support newner MIB object types including 64-bit counters. The initial SNMPv1 employed a community-based security model, which consisted of SNMP agent and SNMP manager entities, where manager entities would request management data from agents.
    
Each SNMP community is represented by a _community name_, also known as a _community string_, When managers send requests to a agent to either set MIB variables, or to get MIB variables, they use the community string as a passwords of some sort to authenticate their request. If the agent receives an incorrect community string, it will drop the packet, else it will execute the request sent by the manager.
    
The access control mechanism SNMPv1 provides is very basic, and a community is allowed access to read or write objects in a MIB tree, with most implementations read and write on **all** objects in the MIB, with no restrictions on which parts of the MIB tree can be accessed. SNMPv1 authentication uses _community strings_ as stated above, but this is extremely unsecure as SNMP community strings are quite easy to bruteforce. Furthermore, SNMPv1 sends the community string in plaintext form, so if SNMP packets are intercepted, a adversary can read the packet to extract the community string and then pose as a SNMP manager.
    
*   **SNMPv2** - This version was built to attempt to address the security issues associated with SNMPv1 through use of the _party-based_, _user-based_, and _community based_ security models. However, SNMPv2 failed to successfully address the major security issues present in SNMPv1. SNMPv2 did provide some improvements including the addition of the SNMP command _GETBULK_, which made data retrieval by SNMP managers significantly easier. The security model used for SNMPv2 was intended to be party-based, however, due to the complexity of this security model, variants of SNMPv2 employed user-based and community-based security models:
    *   **SNMPv2c** - also known as _Community-based Simple Network Management Protocol version 2_. This used the same community-based security model that was employed in SNMPv1, with the additions that came with the second iteration of the protocol. Due to it's popularity within the SNMPv2 variants, it is considered the standard SNMPv2 standard. This standard is defined in the documents [RFC 1901](https://tools.ietf.org/html/rfc1901) - [RFC 1908](https://tools.ietf.org/html/rfc1908).
    *   **SNMPv2u** - also known as _User-based Simple Network Management Protocol version 2_. This used a different security model that used user-based security for per-user authentication for SNMP, which actually employed hash functions and symmetric encryption algorithms but was never really popular for some reason. This standard is defined in the documents [RFC 1909](https://tools.ietf.org/html/rfc1909) - [RFC 1910](https://tools.ietf.org/html/rfc1910).
    *   **SNMPv2p** - also known as _Party-based Simple Network Management Protocol version 2_ - This party-based security model was supposedly difficult to understand and hard to implement, so network admins resorted to over versions of SNMPv2, mostly SNMPv2c. However, if you want to learn how the party-based security model works, it is defined in the following documents[](https://tools.ietf.org/html/rfc1910)
  
*   **SNMPv3** - Introduced in 1998, this adopted a _user-based security model **(USM)**_ and enabled the setting of user authentication requirements to one of the below:

*   **NoAuthNoPriv** - connections would not be private and would not be authenticated
*   **AuthNoPriv** - connections would be authenticated, but messages are not private
*   **AuthPriv** - connections are both authenticated and messages are private

SNMPv3 defined several authentication protocols - MD5, SHA and HMAC-SHA2, and it also defined several privacy protocols - CBC\_DES and CFB\_AES\_128, which are all supported in USM.

Furthermore, access control mechanisms were implemented in SNMPv3 to allow employment of restrictions on certain branches that users could access.

### TODO, Finish Knowledge Store
- SNMPv3 engine
- Entity explanations
- Framing SNMP in a security perspective
- Hands on play about with SNMP
