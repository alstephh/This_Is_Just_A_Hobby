# PORT SCAN
* **80** &#8594; HTTP (IIS 6.0)

<br><br><br>

# ENUMERATION & USER FLAG
Given the name of the box I expected a lot of unpatched/outdated software or other CVE but don't jump the conclusion too early and try to enumerate as much as we can

The nmap scan return the output of some NSE on port 80 and we have so,ething called WebDAV, let's take a look!

![696aae03129c232257190660fa94d665.png](img/696aae03129c232257190660fa94d665.png)

Interesting, first time I see something like this. I discover 2 new tools for this the first one is `davtest` which is a sort of machine gun trying all possible method and extension available to file upload saved in a dedicated directory

![d19e237e96991dca708f31dddaff830b.png](img/d19e237e96991dca708f31dddaff830b.png)

Pretty cool here's the results

![259e8b7055c287158f49ef9692345f23.png](img/259e8b7055c287158f49ef9692345f23.png)

Also try for simple shell upload and what's cool is we execute them as 2 different users

![1682af2703297f199e11128c3426a1ee.png](img/1682af2703297f199e11128c3426a1ee.png)

![aca70e61760f0146bba49abb3ba14ef6.png](img/aca70e61760f0146bba49abb3ba14ef6.png)

Before going on I also tried `cadaver` which is a CLI client and allows attacker to manually put files whitout thisbrute-force approach

![6e61f5ad0d783cd43ce11c593688a40c.png](img/6e61f5ad0d783cd43ce11c593688a40c.png)

Now I want to proceed on the webshell I have has `nt authority\network`, as I have discovered in previous machine get a sevice account can returned on a easier privlege escalation also we have **<u>SeImpersonatePrivilege</u>**

![2e6944349b46206629f73956a10316f0.png](img/2e6944349b46206629f73956a10316f0.png)

For a better Quality-Of-Life I used the metasploit module called `exploit/windows/iis/iis_webdav_upload_asp` to get a meterpeter shell

![8cb24c899dca57363764c9eeec2abf5c.png](img/8cb24c899dca57363764c9eeec2abf5c.png)

I can't find the flag because there is no direcotry called `Users` and I think is because we are dealing with a `Windows Server 2003`, pretty outdated good news for us

BTW the path for users directory is `C:\Documents and Settings`

![27d8af9bacd7785099aadd0e7557879b.png](img/27d8af9bacd7785099aadd0e7557879b.png)

We don't have access as Lakis user and we don't have powershell so actually not totally good news. This is when metasploit come handy but this will be covered in the privilege escalation software

<br><br><br>

# PRIVILEGE ESCALATION
Serching the windows version I returned with more than 1 privilege escalation software so let's find out if metasploit have something ready for us in order to gain full permissions

![b3b214bc259d9bf1b0d7690eff38839f.png](img/b3b214bc259d9bf1b0d7690eff38839f.png)

We have more than one method but the `MS14-070` is the one that appear many many times so I am going with this. This exploit is specifically with the version of the target and craft a input buffer abusing the `tcpip.sys` driver controlling the code flow through a pointer allows for privileged command execution

![d824d717dfecec2a1b28f88d4b518a03.png](img/d824d717dfecec2a1b28f88d4b518a03.png)

Moral of the story : if you need to use webdav at least put credentials and plz don't use outdated windows machine!
