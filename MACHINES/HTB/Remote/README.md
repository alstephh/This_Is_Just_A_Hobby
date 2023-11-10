# PORT SCAN 
* **21** &#8594; FTP
* **80** &#8594; HTTP
* **111** &#8594; RPC
* **139 / 445** &#8594; SMB
* **2049** &#8594; NFS
* **5985** &#8594; WinRM


<br><br><br>

# ENUMRATION & USER FLAG & PE
I want to start on HTTP port and take a quick look at the webpage

![e2b21f19fae766e5a0a12c2eea29bbaf.png](img/e2b21f19fae766e5a0a12c2eea29bbaf.png)

At the bottom we can see the name `Umbraco` (same thing on the source code) which is a Open-Source ASP.NET CMS
On `/people` we have a some name and suname which may turn usefull for user enumeration

![f5e8974c33cbc7ce6949097a98a8e7c2.png](img/f5e8974c33cbc7ce6949097a98a8e7c2.png)

With some enumeration I found the login page at `/umbraco/login`

![5a7dc081f6acfb4e9ecbed9a38747edf.png](img/5a7dc081f6acfb4e9ecbed9a38747edf.png)

FTP accept anonymous login but is empty

![12f51ddd240f48c2447c4cbe9e00ee16.png](img/12f51ddd240f48c2447c4cbe9e00ee16.png)

RPC doesn't accept anonymous login, we don't have other option but look at NFS which ahve something ready for us

![66b9414250646f6c659c1f0b67a481ba.png](img/66b9414250646f6c659c1f0b67a481ba.png)

Just mount the NFS locally and start the hunt for something that can be used to gain access somehow

![e898ca705e4e051b81743d2089268e55.png](img/e898ca705e4e051b81743d2089268e55.png)

The file located at `App_Data/Umbraco.sdf` contain a email address and password SHA1 hash

![54b811da231590f376e363f8ff76f69c.png](img/54b811da231590f376e363f8ff76f69c.png)

Cool `admin@htb.local` is our target now we can move on and crack the hash. The plaintext password is `baconandcheese`, just use this credentials and we are inside the management panel

![0f7577be15918e6fd8627e659a59c7d3.png](img/0f7577be15918e6fd8627e659a59c7d3.png)

The version is `7.12.4` and we have a known vulnerability exactly for this version

![3e53c566ae939b8e56de47eeb1e08e2e.png](img/3e53c566ae939b8e56de47eeb1e08e2e.png)

and it work like a beauty <3

![2789496d7d61f1fb84c4aaf9d3544704.png](img/2789496d7d61f1fb84c4aaf9d3544704.png)

Now we can use the `web delivery` module on measploit to gain RCE

![47615c83af57a187cd20fb4ae1ccb364.png](img/47615c83af57a187cd20fb4ae1ccb364.png)

Now I used this [PoC](https://www.exploit-db.com/exploits/46153) and change the payload  in order to execute the powershell and get the meterpeter shell

![2cb8bf85c08fdb5ee1710172e3cd2ae7.png](img/2cb8bf85c08fdb5ee1710172e3cd2ae7.png)

Inside the `Public` directory we have the user flag, we are ready for PE which was really really simple thanks to meterpeter

![69983c7ed0a5e27910f01963670e64d9.png](img/69983c7ed0a5e27910f01963670e64d9.png)

As I learned (books, article, letteraly everyone) talk that when you get a meterpeter always try to use `getsystem` (obv this would not be the case in some situations that will detect us, I know that)

In this case we are abusing of [PrintSpooler](https://itm4n.github.io/printspoofer-abusing-impersonate-privileges/) (we are inside a Windows Server 2019) abusing <u>Token systems</u>.

PrintSpooler is just a tool to manage print job and a `pipe` is a fragment of shared memory used for communication exchange between process (`named pipes` make possible to exchange data even if process are located in different networks, a sort of client/server architecture)

Having the `SeImpersonate` token privileges we can impersonate the `client named pipe security context`, in this case root!

> The same idea can be applied in the named pipe context. If a process creates a pipe, this process will be the pipe owner or the pipe server. When another process connects to this pipe, it will be called the pipe’s client. Once connected, the pipe server can use the pipe client’s privilege level, the client’s security context or the client’s access rights. This is a Windows feature that helps perform activities based on the client’s privileges, since the server may have full access, but the client typically has more limited rights.
>
>This feature can be exploited by creating a pipe server with limited or low privileges and then attempting to connect a much more privileged client to that pipe server. When that happens, the pipe server can abuse the client’s elevated privileges to perform activities based on those access rights. ([resource](https://securityintelligence.com/identifying-named-pipe-impersonation-and-other-malicious-privilege-escalation-techniques/))

In short, we create a (server) pipe with our limited privileges and we convince the system level user to connecta nd execute a shell for us thanks to win32 API (really simplificate and short)
