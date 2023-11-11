# PORT SCAN
* **80** &#8594; HTTP (IIS)
* **135** &#8594; MSRPC
* **445** &#8594; SMB
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
The webpage is a (beautiful) login pagess

![2bce4ab8befd8395730e4a7c4285d0bd.png](img/2bce4ab8befd8395730e4a7c4285d0bd.png)

No worries we can `login as guest` 

![36203f037ae8e43601e722939a1f15b1.png](img/36203f037ae8e43601e722939a1f15b1.png)

We have an issues thread started from user `Hazard` who also attached a file. The file is the configuration file of a cisco router containing a password hash

![6881c1f4e45137736d70ff5cf0349abc.png](img/6881c1f4e45137736d70ff5cf0349abc.png)

The password obtained is a **type 5** (in cisco) which is a simple MD5 hash, hashcat ready to crack this and take a plaintext password

![5c19ffe9c986e1e17f5beac752a6893e.png](img/5c19ffe9c986e1e17f5beac752a6893e.png)

I try to use this password with the username `Hazard` on WinRM but no access, maybe we need more user enumeration

I used the impacket script `lookupSID` to get some users and we have a cool list now 

![2ee491af760acdebfaccc9c499c2e740.png](img/2ee491af760acdebfaccc9c499c2e740.png)

Using crackmapexec we have no other match if not the same user we  already have control off

Checking the Cisco File we have some **<u>type 7 passwords</u>** both `rout3r` and `admin`. The **<u>type 7</u>**  has been cracked in 1995 and are easy reversible even for a [online tool](https://www.networkers-online.com/tools/cisco-type7-password-decrypt/) like this. The 2 passwords are `Q4)sJu\Y8qz*A3?d` and 
`$uperP@ssword` is time for a second run of CME.

![c970d6140e0859735032b1bc42738c03.png](img/c970d6140e0859735032b1bc42738c03.png)

Hello Chase, thanks for your account which have access to winRM and on the desktop rely the user flag :)

![a8fedc94955eb6622b609ba8d1800811.png](img/a8fedc94955eb6622b609ba8d1800811.png)

<br><br><br>

# PRIVILEGE ESCALATION
With my surprise we have `Mozilla Firefox` inside the `Program Files` directory, cool we are dealing with a widely used browser here

I spend some time surfing the web searching for something cool and I have encountered [this medium article](https://kylemistele.medium.com/stealing-saved-browser-passwords-your-new-favorite-post-exploitation-technique-c5e72c86159a) which cover how to retrieve firefox saved passwords, I followed the steps and retrieve the `key4.db` but is not working somehow.
I found a new tool (which I will add to my arsenal the only downise is its not lightweight at all) called [LaZagne](https://github.com/AlessandroZ/LaZagne/tree/master) which retrieve all password that it can using different technique. I get no credentials here BUT I was able to understand that the firefox password database was empty (in short no passwords are stored). 

![d805ab0901e2fc82dcb31653481d97f0.png](img/d805ab0901e2fc82dcb31653481d97f0.png)

We have no options but to perfrom a memory dump, first let's see if firefox is running while we are playing with the box

![3121328a3b1b3ee89f5b43445d1ad918.png](img/3121328a3b1b3ee89f5b43445d1ad918.png)

Cool stuff now we can use `ProcDump` to get a dump (lol) and use it on the PID of firefox (with `-ma` to retrieve all data in memory and metadata) retrieved with `Get-Process`. Than uploaded locally and just a `strings+grep` combo was enough to retrieve the password for a user with `admin@support.htb` email.

![bd30c5c18fd2ca76099475b826ff6efb.png](img/bd30c5c18fd2ca76099475b826ff6efb.png)

We know is the same password of the Administrator user = box pwned!

![a572ed7d4724568b9b9cd29e18c1f0fa.png](img/a572ed7d4724568b9b9cd29e18c1f0fa.png)
