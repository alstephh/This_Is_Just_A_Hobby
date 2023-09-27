# PRELUDE
This ismy first windows box after a long time, I decide to reset my knowledge in windows and start with simple machine, taking my time and re-learn step-by-step this caothic and really confusing (for me) OS. My objective is to learn AD pentest and learning windows is the basic of the basics

<br><br><br>

# PORT SCAN
* **80** &#8594; HTTP
* **135** &#8594; MSRPC
* **139** &#8594; NETBIOS-SSN
* **443** &#8594; HTTPS
* **445** &#8594; SMB
* **3306** &#8594; MYSQL
* **5000** &#8594; HTTP (403 forbidden)
* **5040** &#8594; UNKNOWN
* **5985** &#8594; WINRM
* **5986** &#8594; WINRM 2.0
* **7680** &#8594; PANDO (?)
* **49664-49670** &#8594; MW RPC

![43f252404e59997d1d2350a1b5206e19.png](img/43f252404e59997d1d2350a1b5206e19.png)

<br>



<br><br><br>

# ENUMERATION
I decide to start enumerate SMB

![ce46b6fa51abde96182bb49c3a8e6e08.png](img/ce46b6fa51abde96182bb49c3a8e6e08.png)

We have a `Windows 10 Pro` machine here, good to know and the SMB have a optional signatures, while I run an hydra dictionary attack on the SMB service in order to (potentially) retrieve some credentials 

<br>

About the webapp this is what we have here

![391e69b44baa8c0c727d6c952197f40b.png](img/391e69b44baa8c0c727d6c952197f40b.png)

And here the `dirb` results

![83e57e99a10f7a8e9585e08e8067b72a.png](img/83e57e99a10f7a8e9585e08e8067b72a.png)

In the `/admin` page we have a simple login page (username:password)

![afead0c5fc12e4b3d263fa6a1e7ed2a6.png](img/afead0c5fc12e4b3d263fa6a1e7ed2a6.png)

Trying some common weak credentials on the admin panel but nothing, I moved on the **<u>HTTPS</u>** part of the sites in order to view the certificate (I have a `403` error)

![2ba3053f369d6dee0fe6a26f0a3360f3.png](img/2ba3053f369d6dee0fe6a26f0a3360f3.png)

Cool, we have a new domain and email address, good to know gonna add the `staging.love.htb` to hosts file and navigate there

![840907f74b72eaef1692d06431c74d1d.png](img/840907f74b72eaef1692d06431c74d1d.png)

Good, even here a login page (weak credentials did not work obv) and if we press on `demo` we can use this free file scanner

![5307fedbf257d9a0815599b6746784ab.png](img/5307fedbf257d9a0815599b6746784ab.png)

Cool, I run a python3 server on my local machine and a test file but no one accessing that when I give the URL, weird! What's fun is when I set `127.0.0.1:80` as URL

![46ee118b5cc6249e8791065de339eee9.png](img/46ee118b5cc6249e8791065de339eee9.png)

COOOOOOOL! How we can use it? We have a **HTTP** port on `5000` that is forbidden for us

![9aaacf34adcfd0b7aa77863c03e49afc.png](img/9aaacf34adcfd0b7aa77863c03e49afc.png)

Let's see if the free file scanner have the permission that I am missings

![de570aa5c76e5c3264508992d430525d.png](img/de570aa5c76e5c3264508992d430525d.png)

Cool stuff! Using the credential we are inside the **<u>Voting System</u>** dashboard as **Neovic Devierte**

![ab5d029e359c540495614f09f581a9fb.png](img/ab5d029e359c540495614f09f581a9fb.png)

In the bottom there is somethin that make me trigger

![08556442bbba7eebc03779862b9f7240.png](img/08556442bbba7eebc03779862b9f7240.png)

WOW, at first I tought the this web-service was costum and actually is not! So I searchd for a vulnerability online an I found this [Authenticated RCE](https://www.exploit-db.com/exploits/49445) 

![ed0c70fe07b7d4e0bdb03912d769fb40.png](img/ed0c70fe07b7d4e0bdb03912d769fb40.png)

Making some changes to the exploit and starting the listener I was able to pop up a shell and get the flag

![8007fceed28bcef86c1a695d17ff51dd.png](img/8007fceed28bcef86c1a695d17ff51dd.png)

<br><br><br>

# PRIVILEGE ESCALATION
Now is the hard part, I'm gonna take my time searching resource online on how to use windows bash to find valuable resources.

I tried some of the commands (like [this ones](https://book.hacktricks.xyz/windows-hardening/basic-cmd-for-pentesters)) but nothing peculiar at least with the basic knowledge I have

<br>

I exported and use **<u>winPEAS</u>** and analyze the output

This is interesting

![7058460ae9abcc8a17c247e314fe0a22.png](img/7058460ae9abcc8a17c247e314fe0a22.png)

**<u>HKCU</u>** (**HKEY_CURRENT_USER**) and **<u>HKLM</u>** (**HKEY_LOCAL_MACHINE**) are Windows Registry


![eb970b66d90cd199663f40172af7df9f.png](img/eb970b66d90cd199663f40172af7df9f.png)

The AlwaysInstallElevated is set to 1 in both the register explained above, this mean we can use msi (Windows installer package file) with `System` privilege

<br><br>

Good I have everything clear and now I will first make some research and search online some examples on how to exploit this vulnerability and than try on my on

Them method is to create a malicious msi with `msfvenom`

![f32c96123f4a4bc57e55cf70db62dffb.png](img/f32c96123f4a4bc57e55cf70db62dffb.png)

Than I upload on the victim the malicious msi and executed it 

![400c71edc6b14963efe8d64fc048afcf.png](img/400c71edc6b14963efe8d64fc048afcf.png)

And on my listener I catch a `System` shell yaoooh!

![6853aeed3d48218c150a5784874d5b84.png](img/6853aeed3d48218c150a5784874d5b84.png)

On the desktop of Administrator User we have the flag

![b2e28cd15b9fea686177a614eb383daf.png](img/b2e28cd15b9fea686177a614eb383daf.png)
