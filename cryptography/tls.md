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

### TLS Handshake (More depth)

#### 1. Negotiating TLS parameters

The TLS protocol is quite customizable which means that its behavior may vary depending on the negotiated TLS parameters. The most important part of the TLS parameters are:
1. The version of TLS used (e.g 1.2 or 1.3)
2. The cipher suite used

During TLS Handshake, there are numerous types of algorithms used including:
  - Authentication / Digital Signature Algorithms - determines how entities will verify the authenticity of the other entity
  - Key Exchange Algorithm - establishes how the symmetric key will be securely established
  - Symmetric Encryption Algorithm - determines how the exchanged key is used to encrypt the data (e.g. AES, Blowfish)
  - Hashing / MAC Algorithms - determines how entities will verify the integrity of the data that will be transferred

There are a lot of avaiable algorithms for each of these types. However, it is unlikely that every client and server will support the same algorithms. Therefore during the TLS handshake it's neccessary to establish which ciphersuites are supported by both, and then select the most secure one. A ciphersuite will typically be represented as a string in the format:
- `TLS_{HANDHSAKE}_{ASYMMETRIC_ENCRYPTION}_WITH_{SYMMETRIC_ENCRYTPION}_{HASHING_ALGORITHM}`

And an example of a ciphersuite string is `TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256` where:
- `TLS` defines what the ciphersuite will be applied to
- `ECDHE` defines that the key exchange will be Eliptic Curve Diffie Hellman Exchange
- `RSA` defines what the public key algorithm is (to verify the certificate and potentially help in exchange)
- `AES_128_GCM` defines that the symmetric key algorithm will be 128-bit Key AES encryption using the GCM cipher mode
- `SHA256` defines the hashing algorithm used to verify the integrity of the data

Similarly, the client and server are not guaranteed to support the same TLS versions, therefore the client and server communicate to determine the latest version that they both support. It is important to note that new TLS versions deprecate insecure algorithms or introduce secure new ones. Therefore, whether the ciphersuite is supported is dependent on the supported TLS versions.

This negotiation can be implemented in a single round trip where:
1. The Client tells the Server what TLS versions and Ciphersuites it supports
2. The Server selects the most secure option based on the options that are supported by both the client and server (this happens for TLS version and Ciphersuite) and sends the selected choice back to the client

**NOTE**: If the Server and Client have no shared parameters that both of them support, than the TLS handshake is dropped


#### 2. Verifying authenticity of Server (and Client)

At this point, the client and server have established the TLS version and ciphersuite for the TLS connection. However we still need to verify whether the server we are communicating with (over a TCP connection) is really who we think they are. The Server sends an X.509 certificate (we'll explore X.509 certificates soon) which contains information that allows the client to verify that the Server is really who they say they are.

##### X.509 Certificate Basics

X509 certificates (also referred to as TLS/SSL certificates) are used to verify the authenticity of an entity (e.g. the server).
The certificate is structured as follows:
- Certificate
  - Version Number
  - Serial Number
  - Signature Algorithm ID
  - Issuer Name
  - Validity Period
    - Not Before
    - Not After
  - Subject Name
  - Public Key Info
    - Public Key Algorithm
    - Public Key
  - Issuer Identifier
  - Self Identifier
  - Extensions
    - ...
- Certificate Signature Algorithm
- Certificate Signature


#### 3. Symmetric Key Exchange

At this point, the TLS version, ciphersuite, signature algorithm, and algorithm parameters have been selected. A selected Key Exchange algorithm is negotierated in the previous step, including associated parameters that are negotiated.

What is done next is for the Client and Server is dependent on the Key Exchange Algorithm used. If it is DHE, then there are public and private values that are generated, which will be communicated to the endpoint of a entity's TLS connection. In reality, ECDHE is strongly recommended to be used by TLS.


#### Afterthroughts

THe above setps provided a general workflow of how TLS works, section by section. However, in reality, these steps are interleaved and some messages are combined together in order to reduce the number of trips over the network to speed up the handshake. Taking a look at the TLSv1.2 and TLSv1.3 walkthroughs will better help you understand how the handshakes are structured.



### TLS 1.2 Handshake

![RFC TLS 1.2 Handshake](tls/tls1.2-handshake.rfc)

\* Indicates optional or situation-dependent messages that are not always sent.

**NOTE**: To help avoid pipeline stalls, ChangeCipherSpec is anindependent TLS protocol content type, and is not actually a TLS handshake message.

**NOTE**: A Good concise resource - https://cabulous.medium.com/tls-1-2-andtls-1-3-handshake-walkthrough-4cfd0a798164. The message flow for a full handshake from the RFC is as follows:




#### 1. Client Hello

The client starts by sending a "Client Hello" message which includes:
- The TLS version
- The list of supported cipher suites
- A 28-byte random number called "Client-Random"


##### Wireshark Catpure

The wireshark catpure for the "Client Hello" is as follows:

![Client Hello](tls/tls1.2-client-hello)


Taking a look at the `Handshake Protocol: Client Hello`, you will see the data that I previously mentioned that the client would send.

**The TLS Version***
Here the version is specified as TLSv1.2:

![Client TLS Version](tls/tls1.2-client-hello.version)

**List of Supported Ciphersuites**
Here is the list of supported ciphersuites (in order of preference). There are a quite a few options here for the different types of algorithms.

![Client Supported Ciphersuites](tls/tls1.2-client-hello.cipher-suites)


**Client Random**
I did mention that the client random is 28-bytes. Technically, it is 32-bytes with the first 4 bytes encoding the current timestamp, and the remaining 28 bytes being random.

The Client and Server Random deter replay attacks, which will be explained later.

![Client Random](tls/tls1.2-client-hello.random)


**Extensions**

There are quite a few extensions present. "Extensions" are way for TLS clients and servers to communicate additional information while still being compatible with previous versions of TLS. These "Extension" fields are ignored by older TLS versions, but clients/servers with newer TLS versions will processes these.

![Client Extensions](tls/tls1.2-client-hello.extensions)

Some of these extensions specify additional TLS parameters or algorithms:
- `extended_then_mac`
- `signature_algorithms`

Some of these are parameters for specific cryptographic algorithms
- `ec_point_formats`
- `supported_groups`

And some of these just help with routing
- `server_name` (provides a host name in case multiple hosts on a single IP)

If the Extension is standard, you should be able to read about it in the TLS RFC:
- https://www.rfc-editor.org/rfc/rfc5246

