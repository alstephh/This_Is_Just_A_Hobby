# PORT SCAN
* **21** &#8594; FTP
* **80** &#8594; HTTP (Paessler PRTG bandwidth monitor)
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB

<br><br><br>

# ENUMERATION & USER FLAG
The FTP protocol accept anonymous login so I want to start from here 

![4d4d4e1ad75e801b72e1b1ad3d88eaf1.png](img/4d4d4e1ad75e801b72e1b1ad3d88eaf1.png)

The only user present on the box is the Administrator

![950632500f20a1db288715d089679902.png](img/950632500f20a1db288715d089679902.png)

Inside the `Public` folder we have the user flag which can be downloaded and submitted, LOL (I've noted that given the same difficult level Windows boxes are easier than linux one, probably because dealing with windows may be confusing at first but this is letteraly too much). Well we have the first flag in a blink of an eye!

<br><br><br>

# PRIVILEGE ESCALATION
The web interface host the **<u>PRTG NETWORK MONITOR</u>** software (tried default credentials but no catch, unlucky)

![e20e81c45313e40a27845842cf3999c4.png](img/e20e81c45313e40a27845842cf3999c4.png)

Appear to have multiple known vulnerability, I am interested in the RCE obviousy but we need to find the version and the credentials to authenticate with

![01f9669ef5bd87ffe467b8bd61e4c2f1.png](img/01f9669ef5bd87ffe467b8bd61e4c2f1.png)

About the version can be found at the bottom (I missed at first)

![b3af8c4ca9f340039b3ca40d868052e7.png](img/b3af8c4ca9f340039b3ca40d868052e7.png)

So no RCE here (for now) but we can use [this PoC](https://github.com/titanssystems/PRTG-Exploit/tree/main) to perform some information disclosure exploit in order to gain more information

![6d770816c33ecdc9189d7fb7cc5ddcd7.png](img/6d770816c33ecdc9189d7fb7cc5ddcd7.png)

Nothing interesting, rip

On the same results page I get [this exploit](https://github.com/A1vinSmith/CVE-2018-9276) which allows to get a reverse shell for our version but we need to get the credentials so let's visit FTP again and check something for us. Was not that hard because we have access to the whole WIndows Filesystem (obviously not the high privilege directories) and the file `/ProgramData/Paessler/PRTG Network Monitor/PRTG Configuration.old.bak` contains the password for the default user

![7d9b0e04f4e412cd2dd948f2f18ba4d4.png](img/7d9b0e04f4e412cd2dd948f2f18ba4d4.png)

Now we can use the exploit to get access on the box but the credentials are wrong. Looking at the password we note the year 2018 is used so I checked when the machine was published on HTB and use it instead of the one on the old configuration file. In this case with the password `PrTg@dmin2019` everything is smooth

![5a69db96569f8450fa7403b1b9e1f3c5.png](img/5a69db96569f8450fa7403b1b9e1f3c5.png)

![3e04f0739013c2b10254add0052ec760.png](img/3e04f0739013c2b10254add0052ec760.png)
