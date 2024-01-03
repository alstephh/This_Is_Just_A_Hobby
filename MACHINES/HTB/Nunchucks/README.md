# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (NGINX 1.18)
* **443** &#8594; HTTPS (NGINX 1.18)
* 
<br><br><br>

# ENUMERATION & USER FLAG

![ec614fa4dc7ee9b7c7cf3c153c0e0b1b.png](img/ec614fa4dc7ee9b7c7cf3c153c0e0b1b.png)

We have a HTTPS website which use **Express** as web framework (implemented with NodeJS) and the machine name can be a hint, in HackTricks we can found under the SSTI section the [NUNJUCKS](https://book.hacktricks.xyz/pentesting-web/ssti-server-side-template-injection#nunjucks) template injection. Pretty interesting! 

Signup and signin form are disabled and we need to find some dynamic page which use a input to create the page. At `/privacy` we have a Privacy Policy which idk if can turn usefull or not. Same thing when we access the `/terms`.

Apparently here is nothing interesting so I will run some subdomain/vhost enumeration

![3e1e2b3dbd2999b7af26d50584d85c1f.png](img/3e1e2b3dbd2999b7af26d50584d85c1f.png)

Cool we have a new target

![f568b91498525bd01be4694f48b570a6.png](img/f568b91498525bd01be4694f48b570a6.png)

We can try to inject on the email form on the bottom and guess what? Is vulnerable!

![5cb106103a5ce949859762d63c39b9b4.png](img/5cb106103a5ce949859762d63c39b9b4.png)

Interesting, RCE now can be easily performed thanks to hacktricks notes

![5a4c6bfc2015feb7763b46ad2fef62ad.png](img/5a4c6bfc2015feb7763b46ad2fef62ad.png)

And with that the reverse shell is served!

![427106c09050697a013bc6c1af87e6ce.png](img/427106c09050697a013bc6c1af87e6ce.png)

The first flag is inside the home directory of `david` user

<br><br><br>

# PRIVILEGE ESCALATION
On this machine `PERL` have **<u>CAP_SETUID</u>** set

![72062851f160b04b41579fe6a1a74e3a.png](img/72062851f160b04b41579fe6a1a74e3a.png)

Capabilities are speciall attribute which can be allocated to allow specific privileges that are reserved for high-level user, interesting stuff are that even if `whoami` work correctly if we spawn a bash or read the root flag we mantain the user permissions...

![9ea2c6dfbabfd803b160d942b9020c34.png](img/9ea2c6dfbabfd803b160d942b9020c34.png)

This requires a bit of knowleadge, the **<u>AppArmor Kernel Enhancement</u>** can confine programs and binaries restricting their use through **program profile**. We can enumerate the profiles on `/etc/apparmor.d`

![f8127032c07bb91d2a89c016e9b32d47.png](img/f8127032c07bb91d2a89c016e9b32d47.png)

![9ce6dd3c367b690895124de698ffd41a.png](img/9ce6dd3c367b690895124de698ffd41a.png)

As we can see in the <u>perl profile</u> we have no access in the `/root` directory but we can modify the `/opt/backup.pl` which automatically set the capabilities for us (so we can just fire it up adn we are running as `root`)

![4e532925e966995264bb5c32ff709178.png](img/4e532925e966995264bb5c32ff709178.png)

As normal user we can't modify it I tried to modify using the GTFO one liner but was harder than expected, always HackTricks is here to save the day becuase in the AppArmor section have some notes about `sheabang bypass` we can create our costum perl script with high capabilities and just execute after setting the `+x permissions`

![2e100342ee18249db51d0a159063bd2e.png](img/2e100342ee18249db51d0a159063bd2e.png)
