### IMAP

IMAP is a protocol used by Email clients to manage stored emails from a mail server. With IMAP, clients provide users with the functionality to perform numerous email management operations including:
- Creation, Renaming, and Deletion of Mailboxes (mailboxes are a destination to where emails are delivered, and are identified by Email addresses)
- Searching for messages and removing messages
- Parsing messages as per RFC 5322, 2045, 2231

Protocols such as IMAP and POP do **not** handle the mail delivery, which is instead handled by seperate delivery protocols such as SMTP (Simple Message Transfer Protocol). Typically a user will use a client that supports IMAP such as Mozilla Thunderbird, which then communicate with the remote IMAP server to perform the user's wishes.

There are quite a few advantages over POP:
- Keeping Connection Alive - The POP protocl typically only stay connected to download messages from the webserver. However, IMAP typically keeps the connection alive, which allows for real-time retrieval of mail and updates to the user interface by the user-client (e.g. thunderbird)
- Simulatenous Client Usage - The IMAP protocol allows and handles simultaneous access by multiple clients unlike POP which requires only one connected client at a time.
- Multiple Mailboxes - IMAP4 Clients are able to create, rename, and delete mailboxes aswell as access control to allow for restrictions or sharing of mailboxes.
- Server Side Searches - IMAP4 Protocol provides a mechanism to perform server-side searching instead of the IMAP Client having to constantly redownload messages and perform searching locally
- Extension Mechanism - IMAP4 defines an explicit mechanism that allows developers to extend it, (POP now has this)

There are also a few disadvantages:
- The IMAP RFC is insufficiently strict and allows for behavior that negative impacts its usefulness
- If not implemented carefully, Server-side searches can consume large amount of resources
- IMAP4 Clients need to maintain a TCP connection in order to be notified of a new mail arrival. This connection is shared by data and control mechanisms which increases its complexity

IMAP typically runs on two different ports:
- TCP Port 993 - IMAP over TLS
- TCP Port 143 - IMAP (plaintext)

Connecting to the IMAP server running on TCP Port 993 will initiate a TLS connection before any IMAP packets can be sent over the connection. Although connecting with TCP Port 143 results in a plaintext connection, you can upgrade the connection to a TLS connection by issuing the IMAP command `STARTTLS`. However, unlike directly connecting to Port 993, this method of using TLS is susceptible to downgrade attacks, unless some mechanism similar to HSTS is implemented (which can still be bypassed by a better implemented MitM attack).

[Information from Wikipedia](https://en.wikipedia.org/wiki/Internet_Message_Access_Protocol)