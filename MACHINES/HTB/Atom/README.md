# PORT SCAN
* **80** &#8594; HTTP (Apache 2.4.46)
* **135** &#8594; MSRPC
* **443** &#8594; HTTPS (Apache 2.4.46)
* **445** &#8594; SMB
* **6378** &#8594; REDIS 


<br><br><br>

# ENUMERATION & USER FLAG
Pretty heterogeneous machine here, I like it!

The port 80 and 443 are the same (certificate doesn't have nothing important for us)

![fab8778e97f49a6a8777f4d0ab81c509.png](img/fab8778e97f49a6a8777f4d0ab81c509.png)

Well I have downloaded the file which is nothing less than an executable that we can run with `wine` and well was not pretty usefull

![79411592b18f5fe641446c52bae8550d.png](img/79411592b18f5fe641446c52bae8550d.png)

**<u>Error in auto-updater</u>** uhm interesting time to reverse the executable with `Ghidra` but actually nothing really clear (at least with my poor knowledge lol)

At least, SMB will give use some satisfaction back 

![a6de97a52957b34a7607034abdf2661c.png](img/a6de97a52957b34a7607034abdf2661c.png)

Inside we have a PDF with interesting info about the software telling that the current version is just a client with no other implementation plus give us this step-by-step guide on how the product release works

![e29440cad77b26ead587c41058892247.png](img/e29440cad77b26ead587c41058892247.png)

Cool stuff becuase the SMB share have 3 distinct (empty) **client folders**

![cd728f4834def929cc3d72583bef9cc6.png](img/cd728f4834def929cc3d72583bef9cc6.png)

I am starting to see how to start an attack here but what we mean with "update"? That's the point now so re-read the PDF and we have something

![a876b721b95c85eaa702c40c2d3dd4c5.png](img/a876b721b95c85eaa702c40c2d3dd4c5.png)

Atom...Electron...all makes sense now and we can start research things like [this](https://blog.doyensec.com/2020/02/24/electron-updater-update-signature-bypass.html) which would allow RCE on the target, we gotta it!

So first of all we haveour answer to what is intended as "update" which is a combo of `yml + actual exe file`.

Now what's wrong with electron is how the yml is parsed allowing attackers to bypass signatures and at the same time execute costum executable, all that we need

1) Create a reverse shell exe file with msfvenomen
2) Create a costum signature with 

![a67e415472801352d31168caf5dffc05.png](img/a67e415472801352d31168caf5dffc05.png)

3) Create the malicious yml file changing the signature with the new one and the executable/path like it follows

![7a55f093553aac1c3659d0388dbe310f.png](img/7a55f093553aac1c3659d0388dbe310f.png)

4) Use SMB to put this 2 files (remember the name of the exe file should be changed)

![0d125b4908919b8e605f864e39a7ff32.png](img/0d125b4908919b8e605f864e39a7ff32.png)

After few minutes I have received my shell back (I have lost time trying with a meterpeter shell but after going for the simplest one everything was way more smooth)

![19d1110d7da03e33de9f0e4950e9c77c.png](img/19d1110d7da03e33de9f0e4950e9c77c.png)

Uau cool, we have our first flag!


<br><br><br>

# PRIVILEGE ESCALATION
No special privileges to save the day this time (thanks god I was tired of the amount of boxes with `SeImpersonatePrivilege`), what we have is inside the **Downloads** folder which contains **<u>node_modules</u>** and **<u>PortableKanban</u>**.

The latter is a Calendar Organizer and [ExploitDB have a PoC](https://www.exploit-db.com/exploits/49409) to extract password given a `.pk3` file, actaully we have a file like that but with an additional `.lock` extension

![02339018cf58f2c981174b3de6d5552d.png](img/02339018cf58f2c981174b3de6d5552d.png)

![c0b4955c55c494a2b3af7bbccd7888f5.png](img/c0b4955c55c494a2b3af7bbccd7888f5.png)

We also have a `PortableKanban.cfg` file which is a JSON blob but, with an online JSON Beautify, I was able to understand his connection with the Redis server in order to send automatic notification via email

![717cee643fe67691d42f0f19d4c2c693.png](img/717cee643fe67691d42f0f19d4c2c693.png)

The encryption is useless for us but checking the redis configuration file we have the plaintext password

![ad6cbfdebdea76144808c188f5ca008a.png](img/ad6cbfdebdea76144808c188f5ca008a.png)

Now we can use the credentials to unlock the redis realm to us and we ca read an interesting KEY

![f2302c0d1d75dbfa1c40b905c06bd1c7.png](img/f2302c0d1d75dbfa1c40b905c06bd1c7.png)

At first I tought I was blocked again but with a quick look at writeups this is exactly the password we can retrieve with the ExploitDB script, the IV and the key are the ones hardcoded so cool stuff.

So I just changed the PoC so can decrypt an hardcoded value and we have what we deserve, the Administrator password

![3e8cc27ba67adfde5ec00018fc1a068f.png](img/3e8cc27ba67adfde5ec00018fc1a068f.png)

![d0e48d7a08ed08408658bc7b4c0a74f1.png](img/d0e48d7a08ed08408658bc7b4c0a74f1.png)

We are good to go with `PSEXEC`

![0ba18ad780ce28a03e461e04309ebecc.png](img/0ba18ad780ce28a03e461e04309ebecc.png)
