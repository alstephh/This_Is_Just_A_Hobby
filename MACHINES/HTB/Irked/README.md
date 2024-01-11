# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.10)
* **111** &#8594; RPCBIND
* **6697 / 8067 / 65534** &#8594; UNREALIRC (Admin email djmardov@irked.htb)
* **58385**
* **60239**

<br><br><br>

# ENUMERATION & USER FLAG
Cool port scan, is not usual to see IRC open ports, this sounds fun! This is the port 80 homepage

![234fc5009c01452df342ac6f6a176b9e.png](img/234fc5009c01452df342ac6f6a176b9e.png)

The admin should be really frastruated eheheh let's try to trigger him more. I run th usual web enumeration bruteforcing tool while I investigate on the next services. RPC was not usefull at all, sometimes we can find some NFS with anonymous access but this time is not the case. We can try to connect to the IRC chat, I used `hexchat irc://irked.htb` and now we are inside 

![8b522d2a912c06c97d821b8dc1e50443.png](img/8b522d2a912c06c97d821b8dc1e50443.png)

I have nostalgia for a period that I wasn't able to live, the time where IRC chat where an actual thing sounds fun! By the way returning back to the job, we have a version on thewelcome output `Unreal 3.2.8.1` which is not acutally vulnerable to a vulnerability but [a Trojan Horse was extenally introduced](https://lwn.net/Articles/392201/) in one of the software macro allowing the use of the backdoor for spawning a shell and metasploit have a module just for that!

![aba77823828e15422eb752be1f3513d6.png](img/aba77823828e15422eb752be1f3513d6.png)

We can't access the flag yet so we need to enumerate and the **Unreal** directory sounds a good place to start but I found nothing at all. We have access to read everything (but not the flag) in `/home/djmardov` and in documents there is a "hidden file" called **<u>.backup</u>** and when we look the content...

![9ae786a81a286f7b43c814308af3b2b8.png](img/9ae786a81a286f7b43c814308af3b2b8.png)

Interesting stuff, but this password doesn;t work with the djmardov user....uhm sounds ok as a thing because the "<u>steg</u>" part of the sentence. Stegography are used with image and this means that one the password was actually in front of us in the homepage, we where "watching" the password since the start!
So I downloaded the homepage image and use steghide to extract the information, we have the password and what it reveals is the **<u>djmardov</u>** password

![5b2bf70c1e3f50531c2792a0685ccb0b.png](img/5b2bf70c1e3f50531c2792a0685ccb0b.png)

Now we are free to SSH into the machine and get the user flag!

<br><br><br>

# PRIVILEGE ESCALATION

We have a costum binaries whit SUID bit set!

![b15e71471451196c2f6b85644c304a17.png](img/b15e71471451196c2f6b85644c304a17.png)

`viewuser` sounds an interesting target to put some effort in it

![a6e45e6d149b0cd355069051f46937e8.png](img/a6e45e6d149b0cd355069051f46937e8.png)

At the end of the ouput the binary try to access the `/tmp/listusers` after creating it I relaunch the command but the file is still empty

![1ded02791cf613f9d12e5b1eca631a24.png](img/1ded02791cf613f9d12e5b1eca631a24.png)

I was not sure what was happening behind the curtains so I unleashed the **<u>Ghidra</u>** and luckly was pretty simple and I was able to retrieve the full code.

![e17ec6a832ac9096f8d69ccc8b1250b0.png](img/e17ec6a832ac9096f8d69ccc8b1250b0.png)

Uau, pretty simple now we can create the temporary file as a bash script that just spawn a brand new bash, in this case will run as root thanks the SUID

![dca57cc50b5ee3b393afaa72f69bfdcf.png](img/dca57cc50b5ee3b393afaa72f69bfdcf.png)

PWNED!
