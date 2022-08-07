## Transport Layer Security (TLS)

TLS is a cryptographic protocol designed to provide secure communication over networks. It is used in VoIP, IM, Email, and HTTPS and other protocols.


TLS aims to provide:
- confidentiality - which means that the data is kept private from unauthroized entities
- authenticity - which means that entity(ies) transacting data have been verified
- integrity - which means that data is trustworthy and has not been modified. This can only happen if the data is authentic, accurate, and reliable.

TLS does this through the use of certificates which we will see soon.


### TLS Handshake (General)

There are two typical ways to initiate a TLS handshake. 
1. Some services such as HTTP, will have a specific port that you can connect to in order to encrypt your connection.
2. Alternatively, some services such as IMAP allow you to upgrade your pre-existing connection by making a protocol-specific request (e.g. `STARTTLS`)

The end goal of the TLS handshake is to provide a **stateful** connection, where higher-level protocols could transfer data securely.

From a 10,000 foot view, the TLS handshake works by the Client and Server using asymmetric cryptography in order to establish a symmetric key. Depending on how this symmetric key was established, this will only last the duration of the session. During this handshake, the client and server agree on various TLS parameters to establish connection security
