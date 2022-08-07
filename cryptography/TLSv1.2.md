
### TLS 1.2 Handshake

![RFC TLS 1.2 Handshake](tls/TLSv1.2-assets/tls1.2-handshake.rfc)

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

![Client Hello](tls/TLSv1.2-assets/tls1.2-client-hello)


Taking a look at the `Handshake Protocol: Client Hello`, you will see the data that I previously mentioned that the client would send.

**The TLS Version***
Here the version is specified as TLSv1.2:

![Client TLS Version](tls/TLSv1.2-assets/tls1.2-client-hello.version)

**List of Supported Ciphersuites**
Here is the list of supported ciphersuites (in order of preference). There are a quite a few options here for the different types of algorithms.

![Client Supported Ciphersuites](tls/TLSv1.2-assets/tls1.2-client-hello.cipher-suites)


**Client Random**
I did mention that the client random is 28-bytes. Technically, it is 32-bytes with the first 4 bytes encoding the current timestamp, and the remaining 28 bytes being random.

The Client and Server Random deter replay attacks, which will be explained later.

![Client Random](tls/TLSv1.2-assets/tls1.2-client-hello.random)


**Extensions**

There are quite a few extensions present. "Extensions" are way for TLS clients and servers to communicate additional information while still being compatible with previous versions of TLS. These "Extension" fields are ignored by older TLS versions, but clients/servers with newer TLS versions will processes these.

![Client Extensions](tls/TLSv1.2-assets/tls1.2-client-hello.extensions)

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



#### 2. Server Hello

The server receives the "Client Hello", and prepares the "Server Hello". There are several subsections of this server hello that you will see.

In general:
- The Server stores the Client Random
- The Server generates its own 32-byte "Server Random"
- The Server adds its own certificate to the message
  - This provides the client a way to verify the identity of the user (authenticity)
- The Server selects a cipher suite
  - TLS has particular recommendations such as ECDHE for Key Exchange
- The Server adds a list of key exchange parameters that correspond to that algorithm
- The Server adds a signature for the parameter list which has been signed with the private key
  - This provides the client a way to verify that the certificate has not been modified in transit (integrity).

The Server then sends this "Server Hello" message


##### Wireshark Catpure
The wireshark capture for this is a bit longer than the "Client Hello":

![Server Hello](tls/TLSv1.2-assets/tls1.2-server-hello)

There are four different messages sent here
- Server Hello
- Certificate
- Servery Key Exchange
- Server Hello Done

###### 2.1 Server Hello

![Server Hello](tls/TLSv1.2-assets/tls1.2-server-hello.hello)

The important fields that this Server Hello sends are:
- Server Random
- The TLS Version
- The Session ID associated with the TLS connection
- The selected Ciphersuite

There are a few new extensions here including that you don't need to worry about:
- `application_layer_protocol_negotiation` (ALPN) - which negotiates which protocol should run ontop of TLS without an additional roundtrip


###### 2.2 Certificate

![Server Hello Certificates](tls/TLSv1.2-assets/tls1.2-server-hello.certificates)

In this message, a list of certificates are sent that the client will use to validate the authenticity of the server. The certificates are sent in the order:
- [end-entity certificate, intermediary CA certificate, ..., root CA certificate]

The client will then iterate over these certificates, verifying that they have been signed correctly, and then moving on to the issuer (which is the next certificate in the list). The client stops once they have found a certificate that has been signed by a CA that they trust (**TODO**: not sure how this works, will work on it), otherwise they cannot trust the certificate so the TLS connection will likely be dropped.

As you can see in the wireshark capture's list of certificates:
1. The first cert is associated with the common name "www.google.com"
2. The second cert is associated with the common name "GTS CA 1C3"
    - Taking a look at this name and googling it, you will find that this is an Intermediary CA
3. The last cert is associated with the common name "GTS Root R1"
    - Taking a look at this name aswell, you will find that this is a Root CA

Although, I'd love to breakdown the certificates, I'll leave that for a knowledge store on PKI

**TODO**: Add a knowledge store on PKI and insert link here

###### 2.3 Server Key Exchange

![Server Key Exchange](tls/TLSv1.2-assets/tls1.2-server-hello.server-key-exchange)

This contains the parameters of the Key Exchange. In this case, the selection algorithm is EDHCE and we have the parameters listed for it including:
- curve type
- named curve
- public key

Aswell as the signature of this parameter list (to ensure integrity):
- Signature
- Signature Algorithm


###### 2.4 Client Cerficiate Request (optional)

**TODO**: Add a section here. This isn't as common, so I'll need to run an openssl server that enforces this (and then trace the traffic)

###### 2.5 Server Hello Done

This acts as a delimiter to tell the client that there are no more Server messages to be sent for now. This can be seen by the lack of fields or valuable:

![Server Hello Done](tls/TLSv1.2-assets/tls1.2-server-hello.done)




##### 3 Client Key Exchange

![Client Key Exchange](tls/TLSv1.2-assets/tls1.2-client-key-exchange)

The Client then performs:
- Verifies the server's identity with the server certificate
- Verfies the key exchange algorithm parameters using the corresponding signature
- The pre-master secret is generated using:
    - The Server Key Exchange parameters (step 2)
    - The Client Key Exchange parameters (step3)
- The Master Secret is then generated by:
    - ```
      master_secret = PRF(pre_master_secret, "master secret",
                    ClientHello.random + ServerHello.random)
                    [0..47];
      ```
    - The PRF is a Pseudo-Random-Function that is defined by the TLS version
    - The values used include:
      - Client Random
      - Server Random
      - Pre-master Secret
- Keys are then generated of the Master Secret for Bulk Encryption
    - The master secret isn't used directly for "seurity reasons" to avoid it being reused. However, shouldn't the master secret be generated on a per-session basis - I would have though tthat would be good enough. **TODO**: Need a bit more understanding on this
- The Pre-master key is encrypted using the server's private key and then sent to the server

###### 3.1 Client Key Exchange

![Client Key Exchange](tls/TLSv1.2-assets/tls1.2-client-key-exchange.client-key-exchange)

Here the pre-master key is encrypted using the server's private key. This is seen in the filed `Pubkey`


###### 3.2 Change Cipher Spec

![Client Change Cipher Spec](tls/TLSv1.2-assets/tls1.2-client-change-cipher-spec)

This message states that the Client will now send all future messages using the bulk encryption method and key that was previously negotiated and generated.


###### 3.3 Encrypted Handshake Message

![Client Change Cipher Spec](tls/TLSv1.2-assets/tls1.2-client-encrypted-handshake)

A value is generated by hashing of all the previously exchanged messages. This value is then encrypted using the previously generated bulk encryption key, and then sent to the server.



##### 4 Server Acknowledgement

![Server Acknowledgment](tls/TLSv1.2-assets/tls1.2-server-acknowledgement)

"Server Acknolwedgement" is not a term official to the RFC, however, I think it describes well what happens. The Server will receive the previous messages in Step 3 and the TLS Server will update it's state accordingly (in the handshake)

###### 4.1 Change Cipher Spec

![Server Change Cipher Spec](tls/TLSv1.2-assets/tls1.2-server-change-cipher-spec)

This message states that the Server will now send all future messages using the bulk encryption method and key that was previously negotiated and generated. The record layer now changes its state to use symmetric key encryption


###### 4.2 Encrypted Handshake Message

![Server Encrypted Handshake Message](tls/TLSv1.2-assets/tls1.2-server-encrypted-handshake-message)

Just like the Client, the Server generates a hash of the previously exchanged message and encrypts it. The Server then sends this message to the Client. Once the client has decrypted this message and verified it, the Handshake is complete.

##### 5 Application Data

The Handshake is complete, and the client and server can now communicate with each other.

