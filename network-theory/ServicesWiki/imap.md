### IMAP

IMAP is a protocol used by Email clients to manage stored emails from a mail server. With IMAP, clients provide users with the functionality to perform numerous email management operations including:
- Creation, Renaming, and Deletion of Mailboxes (mailboxes are a destination to where emails are delivered, and are identified by Email addresses)
- Searching for messages and removing messages
- Parsing messages as per RFC 5322, 2045, 2231

Protocols such as IMAP and POP do **not** handle the mail delivery, which is instead handled by seperate delivery protocols such as SMTP (Simple Message Transfer Protocol). Typically a user will use a client that supports IMAP such as Mozilla Thunderbird, which then communicate with the remote IMAP server to perform the user's wishes.

There are quite a few advantages over POP:
- **Keeping Connection Alive** - The POP protocl typically only stay connected to download messages from the webserver. However, IMAP typically keeps the connection alive, which allows for real-time retrieval of mail and updates to the user interface by the user-client (e.g. thunderbird)
- **Simulatenous Client Usage** - The IMAP protocol allows and handles simultaneous access by multiple clients unlike POP which requires only one connected client at a time.
- **Multiple Mailboxes** - IMAP4 Clients are able to create, rename, and delete mailboxes aswell as access control to allow for restrictions or sharing of mailboxes.
- **Server Side Searches** - IMAP4 Protocol provides a mechanism to perform server-side searching instead of the IMAP Client having to constantly redownload messages and perform searching locally
- **Extension Mechanism** - IMAP4 defines an explicit mechanism that allows developers to extend it, (POP now has this)

There are also a few disadvantages:
- The IMAP RFC is insufficiently strict and allows for behavior that negative impacts its usefulness
- If not implemented carefully, Server-side searches can consume large amount of resources
- IMAP4 Clients need to maintain a TCP connection in order to be notified of a new mail arrival. This connection is shared by data and control mechanisms which increases its complexity

IMAP typically runs on two different ports:
- TCP Port 993 - IMAP over TLS
- TCP Port 143 - IMAP (plaintext)

Connecting to the IMAP server running on TCP Port 993 will initiate a TLS connection before any IMAP packets can be sent over the connection. Although connecting with TCP Port 143 results in a plaintext connection, you can upgrade the connection to a TLS connection by issuing the IMAP command `STARTTLS`. However, unlike directly connecting to Port 993, this method of using TLS is susceptible to downgrade attacks, unless some mechanism similar to HSTS is implemented (which can still be bypassed by a better implemented MitM attack).

[Information from Wikipedia](https://en.wikipedia.org/wiki/Internet_Message_Access_Protocol)



### Deeper Dive

Let's now take a deeper dive into IMAP4 and how it works. We'll only be focusing on IMAP4 revisions:
- [RFC 3501](https://www.rfc-editor.org/rfc/rfc3501) - IMAP4rev1 (current standard)
- [RFC 9051](https://datatracker.ietf.org/doc/html/draft-ietf-extra-imap4rev2) - IMAP4rev2 (waiting for approval)


#### Protocol Overview

IMAP4 protocol assumes a reliable data stream is provided such as TCP. When TCP is used, an IMAP4 Server listens on port 143 (plaintext port) or port 993 (implicit TLS)


#### Command Structure

A typical IMAP connection will involve several operations:
1. The client establishes a network connection (e.g. TCP, [TLS]) to the IMAP server
2. The IMAP server responds with an initial greeting
3. The IMAP client and server will then have several interactions
    - These client/server interactions typically consist of a client command, server data, and a server completion response

##### Complete Commands

A client **command** initiates an operation. Client commands have the format

`<TAG> <SP> <CMD> (<SP> <ARG>)+ <CRLF>`

An example command is the following:

`a001 login mrc secret \r\n`

Let's break what each of the above entities means:


**CRLF**
Interactions performed by IMAP clients and servers are in the form of lines, (which are strings that end with a CRLF (`\r\n`)). The CRLF is used to delimit the end of a request or response in IMAP.

**SPACE**
It is a syntax error to send commands with
1. missing spacing
2. extraneous spacing - (eventhough this would look the same to a human, the IMAP protocol is strict on whitespace)

**COMMAND and ARGUMENT**
This doesn't really require an explanation. In order to perform an operation, you need to use the corresponding command and arguments in your IMAP request to perform it.

**TAG**
Every client command is prefixed with an uniquely generated identifier that is called a **tag**, (which is an alphanumeric string such as `A001`). The IMAP protocol permits several concurrent commands to be sent from the client to the server on the **same** connection. There is no guarantee that the commands are responded to in the same sequence in which they were sent to the IMAP server. Since order cannot be guaranteed, one way the client is able to determine which command a response corresponds with is by using the **tag** identifier to match them up. 

Tag definition has been changed from IMAP4rev1 to IMAP4rev2 - the client "SHOULD" generate a unique tag for every command, but a Server must accept tag reuse. **NOTE:** I'm not sure what new functionality has caused relaxation on the uniqueness of the tag for commands.

##### Command Responses

...

##### Incomplete Commands

There are two scenarios where a line (ie. string delimited by CRLF) that was sent to the server is NOT a command:
1. The command argument is quoted with an octect count (TODO: example??)
2. The command arguments require feedback before the rest of the command can be executed (which will be performed using data from a future line). An example of this is the `AUTHENTICATE` command.

In either scenario, the server will respond to a client with a "command continuation request" asking the client to continue with the incomplete command. This response from the server is prefixed with the token: `+`

However, if the server detects an error with the client, then it sends a tagged "BAD complete response" where the response tag corresponds to the request tag.


TODO: Examples of 1 and 2 (incomplete commands)


###### What about concurrency?

The above is all fine, but the IMAP protocol allows for concurrent processing of requests. While performing a incomplete command, it is possible that the client receives a response for a **different** in-progress command.

RFC 3501 states *"In all cases, the client MUST send a complete command (including receiving all command continuation request responses and command continuations for the command) before initiating a new command."*

This provides a few properties

1. There is no ambiguity in what the server receives
- The Server Protocol Receiver processes command requests sequentially. This is important for commands that require multiple interactions as the client CANNOT send a different request during the multistep command, so there is no ambiguity. If the server doesn't receive the expected
2. There is some ambiguity in responses that start with the token `*`. The server may send responses that are not 1) command continuation requests or 2) complete command responses, and it is possible for intermediate responses (i.e. those that start with `*`) to be interleaved. (I haven't seen anything in RFC 3501 that suggests otherwise). This doesn't seem like a big issue anyways. I believe that the Client Protocol Receivers in this case should be focusing on "command completion" responses as this is the final state of the request. These response are always tagged so there's no ambiguity in which request the reply corresponds to)


