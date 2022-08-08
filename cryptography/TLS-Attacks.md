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

##### SSL Strip 1

The SSL Strip works as follows:
1. Client connects to the HTTP Server (Port 80)
2. The Server will then reply with a 302 response to redirect to the HTTP Service
    a. The MitM will stop this redirect, and continue forcing all resource requests to use HTTP
3. The Client communicates using HTTP

There are a couple of problems with this:
- The HTTP Service may be programmed to not service any requests to a particular resource (and only the HTTPS Service is allowed)

##### HTTP Strict Transport Security (Mitigation)

The HSTS mechanism allows web services to declare themselves as only accessible via secure connections. It was designed to protect against sslstrip by forcing a secure connection or the connection would be terminated.

The mechanism works as follows:
1. The Client makes an HTTP Request to the Server
2. The Server responds with a 301/302 Redirect 
3. The Client then connects to the HTTPS Service
4. The Server responds with a HSTS header that looks like:
  - `Strict-Transport-Security: max-age=31536000; includeSubDomains; preload`
  - This header defines the parameters of how the Host should be treated by the User Agent

What this mechanism does is it states that the Host will only accept HTTPS Requests. Therefore, if we try to perform SSL Strip 1, then the Server will continually perform a 301 redirect before you are able to access any resources. The SSL Strip 1 attack no longer works on it

However, this mitigation is still vulnerable by itself, and a different SSL Strip attack can still be performed

More can be found here on HSTS: 
- https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Strict-Transport-Security#preloading_strict_transport_security


##### SSL Strip 2

The SSL Strip works as follows:
1. Client connects to the HTTP Server (Port 80)
2. The Server will then reply with a 302 Response to redirect to the HTTPS Service
    a. The MitM will intercept this and setup the secure HTTPS connection with the Server
    b. The MitM will then return the new content to the Client in plaintext
3. The Client continues to communicate with the MitM in plain-text, and the MitM will forward this using its encrypted connection to the HTTPS Server

###### HTTPS Everywhere / HTTPnowhere (Mitigation)

The main mitigation for this is for the user agent to enforce usage of the HTTPS protocol from the first request sent to the web service. This generally defeats the above SSL Strip 2

The MitM will not be able to downgrade the connection without the client noticing
- If the MitM tries to reroute the TCP segments to the Server's Port 80, then the TLS negotiation will fail, and the connection will be terminated
- If the MitM tries to impersonate the server at the TLS level, then it will likely fail as the MitM will require the Server's private key during the Key Exchange, so the TLS negotation will fail, and the connection will be terminated



#### Notes / Afterthoughts

We can build a SSL Strip 2 using the `StreamInterceptor` library once that has been built


#### Excellent Resources:

https://www.secplicity.org/2019/11/05/hsts-a-trivial-response-to-sslstrip/
https://www.rfc-editor.org/rfc/rfc6797.html


