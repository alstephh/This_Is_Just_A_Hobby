# PORT SCAN
* **80** &#8594; HTTP (IIS 10.0)
* **135** &#8594; MSRPC
* **445** &#8594; SMB
* **50000** &#8594; Jetty

<br><br><br>

# ENUMERATION & USER FLAG
Jump directly into the website and we have something really funny, a search bunny that whatever the input is redirect me to an image with a error message

![ba54c01fd49184e6b497a04e2a195a0c.png](img/ba54c01fd49184e6b497a04e2a195a0c.png)

![9283de1e6adb091f017407628badd8de.png](img/9283de1e6adb091f017407628badd8de.png)

Well actually this is usefull we have some versions of the software and .NET, uhm cool. Reviewing the source code we are not sending any request and the value of the textfield is actually not used (but required) and when we click on search we are directly redirect to `/error.html` so we will just take this picture as information.

The SMB is unbrachable without a credentials sets so we are left with the Jetty port. Jetty is a Web Server based on Java and on this port we don't have much just an 404 error

![0ad6acedbb5d3b188856278cd76e7a38.png](img/0ad6acedbb5d3b188856278cd76e7a38.png)

THat link jusr redirect us on the [Jetty official website](https://eclipse.dev/jetty/) but give use the version is running

While I was enumerating I tought was fine to run a scan hunting for shortnames in IIS and we have some cool results

![8cab1e9dcea24040bd2ea86827950eb9.png](img/8cab1e9dcea24040bd2ea86827950eb9.png)

Error and index have been already visited but we have the `/ask-je(?).jpg` shortname, I just google **Jeeves** (same thing can be done searching the logo on the search bar I suppose) and I get as first results [askjeevees official site](https://www.askjeeves.net/) (now called ask.com)

Running some bruteforcing (depending on the tool you would need different wordlists) I have found on port 50000 the directory `/askjeeves` which contains the dashboard of **<u>Jenkins Dashboard</u>**

![052f05213a16a0c5b8c32fde390dae4b.png](img/052f05213a16a0c5b8c32fde390dae4b.png)

Metasploit allow us to get usefull information about the Jenkins service 

![57b82afa0186c8599dac5894b5b99a10.png](img/57b82afa0186c8599dac5894b5b99a10.png)

We have a lot of access as anonymous, the most interesting that can also turn to be our way inside the network is the `Script Console`, nothing usefull than that! Let's make a malicious script and intercept that shell with nc

![684ed936cdd40cb3d0c1de38db62ac6a.png](img/684ed936cdd40cb3d0c1de38db62ac6a.png)

Than I get a meterpeter shell with web delivery and we are ready to go and catch the user flag!

<br><br><br>

# PRIVILEGE ESCALATION
Really boring, again the base user have `SeImpersonatePrivilege` enabled so I run juicy and get access as **SYSTEM**

![e9f2d1d6acfceb74065ed355f6c4103e.png](img/e9f2d1d6acfceb74065ed355f6c4103e.png)

We have the fl...WAIT A SECOND!

![f5b7ece420330a62bcb98e7f2a92ffb6.png](img/f5b7ece420330a62bcb98e7f2a92ffb6.png)

UHM, probably we have to access the SMB file share. First we need to retrieve the NTLM hash, we have 2 ways to do it thanks the fact we are root

1) `hashdump` command on meterpeter 
![f941401c3337c71f69e6dba21cbf7bb9.png](img/f941401c3337c71f69e6dba21cbf7bb9.png)

2) `kiwi` meterpeter module using `lsa_dump_sam`
![8ed03602bd0471790d11ae473f745030.png](img/8ed03602bd0471790d11ae473f745030.png)

Here the check that both works as well 

![1bf27792c70d19230ecc556094101625.png](img/1bf27792c70d19230ecc556094101625.png)

Cool, this seems a standard SMB

![43b383653103bdb58cd5127f2053d26c.png](img/43b383653103bdb58cd5127f2053d26c.png)

Actually after login I decide to move on `C` directory sharin and going inside the Administrator directory. Why on SMB, because I know a way to read alternative data stream with `allinfo`

![85559c6082e105adf7d6b04eed5921b4.png](img/85559c6082e105adf7d6b04eed5921b4.png)

![181b98aaf7d4f34ed541d89f74fae232.png](img/181b98aaf7d4f34ed541d89f74fae232.png)

WE GOTTA IT but I fell stupid lol, I know this way to read different data channels but there was a way to do it directly on the command prompt with `dir /R`

![71672f15582b43df6d4afff7ea9572c2.png](img/71672f15582b43df6d4afff7ea9572c2.png)

Well was not the intended way but I am happy of two things

1) I learned a new (small) thing
2) I used my knowledge to get over an obstacle in the way seems wrong but I get myself to get hash and access the SMB, not the easiest and efficent way but the important is pwn the machine not how you pwn the machine ;(
