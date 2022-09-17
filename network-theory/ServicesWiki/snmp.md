## SNMP


*Simple Network Management Protocol* is an application layer protocol that collects and organizes information on managed devices, configures remote devices and detects network faults on a IP network.

Here are the main componenets of SNMP

*   **SNMP manager** - they are responsible for gathering information on network hosts and infrastructure by sending query requests to hosts that run SNMP agent software. Hosts running SNMP managers are also known as _Network Management Stations **(NMS)**_. Most of the commands defined within the SNMP protocol are designed to be _sent_ by a manager component.
*   **SNMP agent** - this is software installed on a managed host with it's responsibility being to gather information about the local system and then storing / updating the data to the _management information base **(MIB)**_. SNMP agents respond to most of the commands defiend by the protocol.


### Management Information Base (MIB)

The _MIB_ is a standardized database that both managers and agents adhere to. However, while it is standardized, MIB provides flexiblity that allows for vendor specific additions. MIBs are top-down hierachical trees, where each entry or branch is addressed using a _Object Identifier **(OID)**_. Each branch from a parent node has a value assinged to it, that we use with a sequence used to access a particular variable in the MIB database, which we can simply call the _branch path_. Here's an example of an MIB:

![](http://www.opencircuits.com/images/thumb/6/6a/Snmp_mib_oid_tree_example.jpg/900px-Snmp_mib_oid_tree_example.jpg)

If I wanted to access the the mgmt section, I would either use one of the following paths:

*   **Numerical Path** - 1.3.6.1.2
*   **String Path** - iso.org.dod.internet.mgmt

If you want to familiarize yourself with the MIB database and it's structure and the individual Object Indentifiers (OIDs), you can use [this](https://www.alvestrand.no/objectid/1.3.6.1.2.1.html)

### Protocol Commands

Now that you understand how the Management Information Base (MIB) database works, we're going to see how we use SNMP protocl commands to query the database. SNMP management software uses UDP to communicate with SNMP managed-devices (ie. SNMP agents). SNMP managers will send _Protocol Data Units_, and since the protocol uses UDP, the _**PDU**_ will be _Datagrams_.

*   **GetRequest** - A manager-to-agent request for values of specified Object Identifiers (OIDs), which is answered with a _response_ message sent back to the manager with the data.
*   **SetRequest** - A manager-to-agent request to change/set the value of specified OID variables. A _response_ message is sent back from the agent to the manager with the new current values for the variables that were newly set.
*   **GetNextRequest** - A manager-to-agent request that a manager can use to request the next sequential object in the MIB without having to worry about what OID paths to use in their queries. This command can be used iteratively to traverse the entire MIB database and return all values.
*   **GetBulkRequest** - A manager-to-agent request used for multiple iterations of _GetNextRequest_, and the user will send a _response_ message with multiple variable bindingsd walked from the initial varaible binding/bindings in the request. This command was introduced in SNMPv2
*   **Response** - A agent-to-management response that returns variable bindings and acknowledgments for SNMP command requests such as _GetRequest_, _SetRequest_, _GetNextRequest_, _InformRequest_ etc. If data cannot be returned from a manager-to-agent request, the response contains error fields includeing error-status and error-index fields. Eventhough it is used as a response for both gets and sets, it was originally called _GetResponse_ in SNMPv1.
*   **Trap** - Unlike in other SNMP communication where the manager actively queries the agent for information, _Trap_ PDUs are sent by the agent to the manager without being explicity requested, so are considered _asynchronous notifications_. Trap PDU's are sent by the agent, usually when there is some siginficant event, such as a network fault, that occured on the managed device; standard traps include: _coldStart_, _warmStart_, _linkDown_, _linkUp_, _authenticationFailure_, _egpNeighborLoss_. For more information, CISCO havs a more [in-depth explanation](https://www.cisco.com/c/en/us/support/docs/ip/simple-network-management-protocol-snmp/7244-snmp-trap.html). This helps reduce congestion on the network, as instead of the the SNMP manager constantly polling agents for information, there will only be a response being sent across the network instead of a request and response.
*   **InformRequest** - Introduced in SNMPv2c, this PDU was used for acknowledgement of asynchronous notificiations, ie. acknowledgement of packets sent from a Trap. Since UDP doesn't provide reliable communication, SNMP handles this on the application layer through _InformRequest_, which simply sends an acknowledgement upon receipt.

