SNMP
----

_Simple Network Management Protocol_ is an application layer protocol that collects and organizes information on managed devices, configures remote devices and detects network faults on a IP network.

Here are the main componenets of SNMP

*   **SNMP manager** - they are responsible for gathering information on network hosts and infrastructure by sending query requests to hosts that run SNMP agent software. Hosts running SNMP managers are also known as _Network Management Stations **(NMS)**_. Most of the commands defined within the SNMP protocol are designed to be _sent_ by a manager component.
*   **SNMP agent** - this is software installed on a managed host with it's responsibility being to gather information about the local system and then storing / updating the data to the _management information base **(MIB)**_. SNMP agents respond to most of the commands defiend by the protocol.

