### TLS Security

This takes a look at the most common/known security attacks against TLS

An RFC was put together that summarises known TLS attacks which can be found here:
- https://www.rfc-editor.org/rfc/rfc7457.html

Although the RFC was created in 2015, it is pretty concise and explains the general attacks that SSL/TLS was facing at the time (including historically). Mitigiations have been put in place, and new vulnerabilities have been devisied in a constant back-and-forth cycle between defender and attacker. However, this should give you a bit more understanding of the lay of the land

#### TLS/SSL Strip

I previously mentioned in the TLS Basics article that there were two ways to initiate a TLS connection:
1. Directly connect to a server port where TLS is running on (e.g. HTTPS on Port 443)
2. Upgrade a pre-existing plain-text connection to use TLS (e.g. HTTP response redirects client to HTTPS, IMAP client sends a STARTTLS command)

SSL Strip focuses a subset of the second scenario. In the past, it was more prevelant where clients would communicate with the server using an unsecure protocol. Some services would try to enforce encrypted versions of the protocol on certain resources, and this was done often time using a redirection to the endpoint that the secure version of the protocol was listening on.

The general workflow for HTTPS SSLStrip was this:
1. Client would connect to the HTTP Server (port 80)
2. The Server would then reply with a 302 Response (Redirection)
3. The Client would then connect to the HTTPS Server (port 443)
4. The Client would then continue accessing resources via this service

However, this workflow is susceptible when there is a MitM. If you considered a MitM then the flow would look like this:


Client <-----------> MitM <--------------> Server

**TODO**: Add graph

The SSL Strip works as follows:
1. Client connects to the HTTP Server (Port 80)
2. The Server will then reply with a 302 Response to redirect to the HTTPS Service
    a. The MitM will intercept this and setup the secure HTTPS connection with the Server
    b. The MitM will then return the new content to the Client in plaintext
3. The Client continues to communicate with the MitM in plain-text, and the MitM will forward this using its encrypted connection to the HTTPS Server

##### Mitigations

###### HTTP Strict Transport Security (HSTS)


###### HTTPS Everywhere / HTTPnowhere



