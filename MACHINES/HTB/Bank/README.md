# PORT SCAN

* **22** &#8594; SSH
* **53** &#8594; DNS (ISC BIND 9.9.5)
* **80** &#8594; HTTP (APACHE 2.4.7)


<br><br><br>

# PRIVILEGE ESCALATION

This bank is just waitig to be robbed, I am the thief!
Port 80 is a login form

![0b67d1399bd2cbb41c2b9759b3ed4d28.png](img/0b67d1399bd2cbb41c2b9759b3ed4d28.png)

It can be all here, in fact `ffuf` found the **<u>cryptix</u>** subdomain which just have a default Apache2 page

![9e34bce277fcc7f2bcbc73bfe69361a0.png](img/9e34bce277fcc7f2bcbc73bfe69361a0.png)

The directory busting on the base domain found `/balance-transfer` with a huge number of file listed

![b944e9d6743f3c301c9a5c016d77159d.png](img/b944e9d6743f3c301c9a5c016d77159d.png)

This is a random file that I have picked

![afd5af2feb15a89e6fefb6d64956ec6a.png](img/afd5af2feb15a89e6fefb6d64956ec6a.png)

I looked closely at the size of each file and there was 1 really low respect to others

![2d0e65b53ed78f79e49f0abb2a1b2751.png](img/2d0e65b53ed78f79e49f0abb2a1b2751.png)

The motivation of why is extremely low is because is not encrypted at all and have some hardcoded credentials

![caf9c09f25744ef62c7f519368c8553b.png](img/caf9c09f25744ef62c7f519368c8553b.png)

Obviously they are the key to open the chris bank account

![7b702ba280906af20f6a2331c641355e.png](img/7b702ba280906af20f6a2331c641355e.png)

We have a `Support` section inside the account dashboard which permits to attach a file on a ticket system which will be posted and give a link to to view the attachment (`/uploads/[file_name])` . The bank system is not a total dumb, it permitts only image file to be uploaded so we have to bypass it and if we are successfull we will achieve RCE through a PHP webshell!

The first thing I tried was **Poison Null Bye Inject** but somehow it did not worked as is suppossed to also different methods like the one listed [here](https://null-byte.wonderhowto.com/how-to/bypass-file-upload-restrictions-web-apps-get-shell-0323454/) and on HackTricks but the answer was in front of us (actually I don't like this CTF stuff I don't feel their usefull enough but remember HTB BOXES ARE A GAME!) because on the source code there is a interesting comment...

![836f9b2955636f8d54061f834e96df5b.png](img/836f9b2955636f8d54061f834e96df5b.png)

Oh god....Well we can achieve RCE throgh this extension

![30bf45fe544b66031935566730286171.png](img/30bf45fe544b66031935566730286171.png)

Good, I used the **Ivan Sincek** PHP reverse shell to have a stable shell on my netcat listener. After that we can take the first wave of money we found in this bank :)

![670e6ac2391b2e0e43e990b0ef66df65.png](img/670e6ac2391b2e0e43e990b0ef66df65.png)

<br><br><br>

# PRIVILEGE ESCALATION
We aregreedy and we are here to steal, we don't have enough money yet. WE WANT TO HOLE THE BIG SAFEBOX!

This was pretty quick and a piece of cake after all. I searched for some SUID binaries and found  `/var/htb/bin/emergency`. I was not sure of the behaviour of the binary....so I jut ran it and we get a shell as root!

![af5ac8f3b6fc1babfff4a481568c097d.png](img/af5ac8f3b6fc1babfff4a481568c097d.png)

> Yeah very disappointing and not challenging, maybe is the message the box wants to communicate is that bank are (strongly) understimating computer security
