# PORT SCAN
* **80** &#8594; HTTP (IIS 8.5)
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **1521** &#8594; Oracle TNS Listener (11.2.0.2.0)
* **5985** &#8594; WinRM

<br><br><br>

# USER FLAG & ROOT FLAG (or viceversa this time lol)
The OS is `WIndows Server 2008 R2` and the webpage is just the standard IIS one. The **<u>TNS Listener</u>** is a separate database process that runs locally on the (DB) Server and the purpose is to receive connection from the clients and establish a connection with the DB 

We can try to enumerate and was something new for me, I have started with `SID enumeration` but since we have no access (anonymous not allowed) we get unsuccess, we can move to `SID bruteforce` instead and we have something here

![384cf2557e572eab715e60f83dd156d3.png](img/384cf2557e572eab715e60f83dd156d3.png)

With [ODAT](https://github.com/quentinhardy/odat) we have also one more 

![744dbd9cdb7de2537577c0011f6d0013.png](img/744dbd9cdb7de2537577c0011f6d0013.png)

`XE`, `XEXDB` and `PLSEXTPROC` are valid, The **SID** stand for Service IDentifier and in short is the (Relational) Database Name

Standing on some lectures (like [this one](https://www.blackhat.com/presentations/bh-usa-09/GATES/BHUSA09-Gates-OracleMetasploit-SLIDES.pdf)) the basic methodology is to brute force the credentials, I used `patator` and the hacktricks wordlists

![9d899ccb96640b2753e8abad7d530cda.png](img/9d899ccb96640b2753e8abad7d530cda.png)

`SCOTT:tiger` is a valid account, now we can go on the next steps we can decide different way based on the server configurations, the slide linked above suggest to use the `admin/oracle/post_exploitation/win32exec` metasploit module but we get unlucky because Java is not installed, good to know....

![6a6aafe04e287c268a5adfb8f93b0881.png](img/6a6aafe04e287c268a5adfb8f93b0881.png)

Other 2 methods exist to execute RCE (`scheduler` and `externaltable`) on the server side but for both we don't have enough permissions

![56b6de5f8d775a2d87bd9ef35e9c0cca.png](img/56b6de5f8d775a2d87bd9ef35e9c0cca.png)

We need to make some PE onthe DBMS, sounds great. Searching online we can use `--sysdba` flag on odat to get SYSDBA (sort of root on Oracle) privleges and it worked actually, at least to retrieve file

![b5393c5ed4d889ee16fc29fa7b80f337.png](img/b5393c5ed4d889ee16fc29fa7b80f337.png)

Cool if we can get the root.txt flag means the Oracle services is running with high priv, we need to get a shell
If we can get files this mean we can also uploaded it maybe(?), well we can and we can abuse the IIS website that will host our aspx reverse shell!

![0fbad668d94ce471b21242b6a8516488.png](img/0fbad668d94ce471b21242b6a8516488.png)

This shell is enough to get the user flag plus we already have the root one, mission completed really fun to play with new things like the Oracle DB and tool like `odat` are cool framework for attackers.

In any case we can get SYSTEM shell easily since the user shell we have returned with `SeImpersonatePrivilege` enabled!

> Even if short, this machine took quite a lot of time but that's fine was cool to understand the Oracle internals and all different ways to exploit it
