# PORT SCAN
* **21** &#8594; FTP
* **80** &#8594; HTTP (IIS 8.5)
* **135** &#8594; RPC
* **139 / 445** &#8594; SMB
* **5985** &#8594; WinRM


<br><br><br>

# ENUMERATION & USER FLAG
From the Nmap scan, the OS appears to be `Windows Server 2008 R2` and look what we have in the webapp

![18d1aa9d9a8876f1a351c013527a6d9c.png](img/18d1aa9d9a8876f1a351c013527a6d9c.png)

HackTheBox clone, cool. No anonymous access both in FTP and SMB but we known thanks to **Wappalyzer** that is runing `ASP.NET`.

BY the way after all the enumeration that I can th only tought is to bruteforce and after some attempts somehow the admin page just flash before return back on the login page whe try to access `index.html`, strange and I am starting to think that we don't need to get acess, at least for now.
I have made 2 relevant discovery after some time

* We can access the `/files/password.txt` file

![2f2c83a3c4def3ee82952a78087813bd.png](img/2f2c83a3c4def3ee82952a78087813bd.png)

* We can play with the **API which give token** back to the user (look on the left we have the MD5 hash of the `admin` word)

![621d82299dbd116894ad2c99bbd70481.png](img/621d82299dbd116894ad2c99bbd70481.png)

This last one is pretty interesting while the first actually give me nothing at all, the box is called JSON and this API is actually using JSON. We have another API which do the opposite given the `OAuth2` will return back the user information

![194d5012943a562890462e0238955c42.png](img/194d5012943a562890462e0238955c42.png)

This last one is pretty ineresting, is a deserialization so maybe we can exploit that with `ysoserial` (I used [this version](https://github.com/pwntester/ysoserial.net) according to multiple writeups is the best choice)

> Note that we can delete the OAuth2 and the API ill still work because what is checking is the <u>**Bearer**</u> 

Now I can jump on windows and try to create a **deserialization payload**

![1b0855c556fc2577767535376023558f.png](img/1b0855c556fc2577767535376023558f.png)

Cool now I've just base encoded and changed the **Bearer token** and give a try, if it works we would receive a connection on the dummy SMB share we have created. We gotta it!

![9d08d8e29d98d6eec8f47dfd4aeeacfc.png](img/9d08d8e29d98d6eec8f47dfd4aeeacfc.png)

Than I have ust used a **powershell reverse shell** (base64 encoded version) as payload and we get the shell!

![2b292c58e6648f00d39f681ae9c125bf.png](img/2b292c58e6648f00d39f681ae9c125bf.png)

This user is enough for the user flag

<br><br><br>

# PRIVILEGE ESCALATION
Now the first thing to check are obviously groups and privileges and woth to notewe have `SeImpersonatePrivilege` enabled 

![d68459142c4185bd2bcd1c08f2cb2d3d.png](img/d68459142c4185bd2bcd1c08f2cb2d3d.png)

Good but I want to enumerate further because we have something missing, the <u>`FTP`</u> protocol

Inside `Program Files` we have the **<u>Sync2Ftp</u>** program, inside we have hte configuration file with some base64 credentials

![12dc228630076052cfab89d772d7816a.png](img/12dc228630076052cfab89d772d7816a.png)

But actually credentials seems to be encrypted somehow

![00c802a61ed28ee0ca959a6ef0168c40.png](img/00c802a61ed28ee0ca959a6ef0168c40.png)

So what I can do here is to upload the exe file and chek with the `file` command

![543fbf1d877002a29f64e47ca9839dbd.png](img/543fbf1d877002a29f64e47ca9839dbd.png)

Cool is a .NET assembly is time for **<u>dnSpy decompilation</u>** and here we are we have the code for the Decryption

![f332e5ea9e739a81a8eb17298d6cb806.png](img/f332e5ea9e739a81a8eb17298d6cb806.png)

For a faster resoultution thanks to bard I was able to create a quick and dirty script in python that will decrypt both username and password since we have the key in cleartext is just a time of a prompt (another way can be to use dotnetfiddle.com or directly debug the executable)

![76c5dddf095259e8291867ec8c081a27.png](img/76c5dddf095259e8291867ec8c081a27.png)

With this credentials we can access to the FTP which have full access to the disk, root flag included!
