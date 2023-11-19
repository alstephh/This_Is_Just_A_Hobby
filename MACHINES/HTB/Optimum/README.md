# PORT SCAN
* **80** &#8594; HTP (HttpFileServer 2.3)

<br><br><br>

# ENUMERATION & USER FLAG

![7cec1c4f10b60c00fde96786c5002902.png](img/7cec1c4f10b60c00fde96786c5002902.png)

This is the website we get when visiting Optimum website running **<U>HttpFileServer</U> (HFS)** service designed for file sharing. The version is `2.3` and we have a public exploit for this

![bfe32912921a8833a058de8784773400.png](img/bfe32912921a8833a058de8784773400.png)

Really cool we can grab this flag really fast running this exploit which trigger an invalid pointer write access through some HTTP header abuse. The command would be a powershell reverse shell command obviously and catched by our netcat listener

![bc06cc76db4004f65584a252b069f8e4.png](img/bc06cc76db4004f65584a252b069f8e4.png)

Grab this piece of cake flag and move on

<br><br><br>

# PRIVILEGE ESCALATION
The box is hosting a `Windows Server 2012 R2 Standard (6.3.9600)` with some enumeration we have credentials inside AutoLogon

![c7e7325532408d124e4cd9d98ea9e48a.png](img/c7e7325532408d124e4cd9d98ea9e48a.png)

In addition the machine is vulnerable to `MS15-032`, a lack of sanitization of standard handles in windows secondary logon service through a crafted webpage through internet explorer

Mwtasploit have a script ready for us, after spawning a meterpeter session I was free to run the exploit

![65ee0e28bd141d947f24c67f90681e7c.png](img/65ee0e28bd141d947f24c67f90681e7c.png)

Really easy!
