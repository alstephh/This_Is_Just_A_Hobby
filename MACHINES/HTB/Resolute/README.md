# PORT SCAN
* **53** &#8594; DNS
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **389** &#8594; LDAP
* **593** &#8594; MSRPC (HTTP)
* **3268** &#8594; AD LDAP
* **5985** &#8594; WinRM
* **9389** &#8594; .NET MESSAGE FRAMING


<br><br><br>

# ENUMERATION & USER FLAG
The box is an Active Directory Domain Controller on the **<u>megabank.local</u>** domain, the OS is `Windows Server 2016 Standard 6.3`. This version is pretty outdated and I remind of the `ZeroLogon` vulnerability a really powerfull attack on outdated windows server. I run [this checker](https://github.com/SecuraBV/CVE-2020-1472) in order to be sure if is vulneable or not and the output is good to go

![2a891df09d7880438e4187b65946645a.png](img/2a891df09d7880438e4187b65946645a.png)

This is cool but I prefer to enumerato more before try to actively exploit this vulnerability

SMB is not accessible by anonymous so I need to move on RPC which accept anonymous, we can easily retrieve a <u>list of users</u>

![560854e021f015c9889a84504f359bd9.png](img/560854e021f015c9889a84504f359bd9.png)

I quickly make a username-as-password brute force attempt with crackmapexec but none woked. Same thing can be done with <u>groups</u>

![aa9ebf4d8098b772593a6c7e43f93c60.png](img/aa9ebf4d8098b772593a6c7e43f93c60.png)

We have more than something here, `DnsUpdateProxy` may probably lead to `dnsAdmin`, `Cloneable Domain` Controllers may lead to `DC sync` and so on but for now these are just assumptions.

> <u>**LET'S TAKE A BREAK HERE PLZ!**</u>
> After some enumeration i decided to perform the zerologon exploit to reset the machine account to an empty string through the metasploit module
> ![b8f4ba2ad3ad246dbfe8a16d6d3bc1c0.png](img/b8f4ba2ad3ad246dbfe8a16d6d3bc1c0.png)
> Now I can easily perform a DCSync and dump all the hashes I want, Administrator included
> ![80fc3bdef4f3289e15309d42cabdbd2b.png](img/80fc3bdef4f3289e15309d42cabdbd2b.png)
> Now I can just login through WinRM and pwn the box in a blink of an eye!
> ![450cd59c38404f5b8d272b2cfe46dbd4.png](img/450cd59c38404f5b8d272b2cfe46dbd4.png)
> Everything cool but sound weird to me since we are talking of a medium machine, so I just take a quick look at some writeups and obviously there was an intended path to get the box. That being said I grab the 2 flags and submitted them but I want to train myself to do something more that just run a couple of POCs so I will go through the walkthrough (cool words-game lol) without relying on ZeroLogon despite the fact I finally used this well known vulnerability

Is a good practice to take a look at users description in order to gain powerfull information or hardcoded password, `marko` description fit the case perfectly 

![849007451143a1b20ccb5e3f708f4720.png](img/849007451143a1b20ccb5e3f708f4720.png)

It doesn't directly works with the `marko` user but with crackmapexec we can check if another user use the same password

![c0d8860b79f3c734638e0e8f457140f8.png](img/c0d8860b79f3c734638e0e8f457140f8.png)

Dear <u>**melanie**</u>, thanks to allow me to get inside the domain contoller and retrieve the user flag. Whish you the best :)

![a9ea9a4b6c174e31b4dc2c98cdf42194.png](img/a9ea9a4b6c174e31b4dc2c98cdf42194.png)

<br><br><br>

# PRIVILEGE ESCALATION
`Melanie` user doesn't take part of particular special groups 

![0f01ee49ed0de543eb26f45338137d81.png](img/0f01ee49ed0de543eb26f45338137d81.png)

In the root directory we have a directory called `PSTranscripts` which may contains some lof of powershell

![b6369b4bc9e1464f4f23aead8524767e.png](img/b6369b4bc9e1464f4f23aead8524767e.png)

I was not able to check the content inside the directory but I hve searched online and adding `-recurse` at `gci -force` we would retrieve the contents

![a2e91e7d7416471287eb6d1bb6e0bd0e.png](img/a2e91e7d7416471287eb6d1bb6e0bd0e.png)

The file contains logs file of `ryan` user

![e3339e6f677330a7667e5425455be1f3.png](img/e3339e6f677330a7667e5425455be1f3.png)

Let's see if can get a password

![2dbf6e483ccf8eef166c642d43816073.png](img/2dbf6e483ccf8eef166c642d43816073.png)

With the credentials we have we can login on resolute with a more privileges than before

![6e3ec7e4e39e9eebcf9ea98daf0bd457.png](img/6e3ec7e4e39e9eebcf9ea98daf0bd457.png)

Inside the Desktop we have a txt file called `note.txt`

![25da604a8b6a32ecdf5614243bb989ea.png](img/25da604a8b6a32ecdf5614243bb989ea.png)

UHMMMMM, we are part of `Contractors` and with the power of powershell (powerview and sharphound are blocked by Windows Defender) we can see it's part of `DnsAdmin`

![9cfabb240a5e82ac2fa6c09b68ec5aa3.png](img/9cfabb240a5e82ac2fa6c09b68ec5aa3.png)

(same thing can be achieved with `whoami /groups`)

Cool stuff we can exploit this permissions we can force the Domain Controller DNS service to run a specified DLL (which will run as `nt authority/system`)

<br>

1) Create a costum DLL that will give us back a Shell with msfvenom and put it in a SAMBA server running locally

![879ee421e8118fc75a28ec9738b84b20.png](img/879ee421e8118fc75a28ec9738b84b20.png)

<br>

2) Set the DNS service to load the DLL

![24177bcbe2beab8ec7e423d47113bcd3.png](img/24177bcbe2beab8ec7e423d47113bcd3.png)

<br>

3) Restart the DNS service

![1217bff9fd25f9e40b9a8e743cf7a024.png](img/1217bff9fd25f9e40b9a8e743cf7a024.png)

![08dd990a0dfc98ffd98e10d251fb5593.png](img/08dd990a0dfc98ffd98e10d251fb5593.png)

<br><br>

Now just wait a few seconds an netcat will popout for us

![32d889de4b55437af88d1be823791cb6.png](img/32d889de4b55437af88d1be823791cb6.png)

Gottch'a!

DNSAdmin abuse is something that luckly I have done already in a project for a UNI course that I am attending so good thing to refresh this things
