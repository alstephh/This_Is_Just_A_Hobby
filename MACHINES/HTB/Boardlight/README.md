# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.41)


<br><br><br>

# ENUMERATION & USER FLAG

A really simple, traditional website on port 80

![f8565e3c517fcf6e1fba6a5326aa5295.png](img/f8565e3c517fcf6e1fba6a5326aa5295.png)

All the buttons redirect on this homepage (even the `contact us` section). No subdomain and dirbusting returned nothing usefull, uhm...
you can see on the very bottom on the footer that the real domain of the machine is **<u>board.htb</u>** so I re-tried subdoamin enumeration with this domain, this time we have something!

![895e45e4ad1fd0f4d50e7a5f37a3e137.png](img/895e45e4ad1fd0f4d50e7a5f37a3e137.png)

Into `crm.board.htb` we have **<u>Dolibarr</u>** (v17.0.0) instance. The credentials **admin:admin** worekd smoothly!

![a2a68f3534dda8f9a12c922987398d7b.png](img/a2a68f3534dda8f9a12c922987398d7b.png)

Here we are inside with Admin role but we with restricted access (Dolibarr have also SuperAdmin role above the Admin one), the only thing that we can do is creating and view a new website 

![9d96ef757f27e947d4fc151513e07f45.png](img/9d96ef757f27e947d4fc151513e07f45.png)

My idea was to inject sme PHP code but gets rejected because we have a lack of permissions but following this [guide](https://www.swascan.com/security-advisory-dolibarr-17-0-0/) I found there is a way to bypass this just making some difference to the php word.

![cb8729bae5c2cb8cba14e4a8fe6b813a.png](img/cb8729bae5c2cb8cba14e4a8fe6b813a.png)

Things like "Php", "PhP", ecc.. bypass the check, here below a result of a simple `whoami` command through PHP using this small trick

![08496e65467471d3f8e3301032a56cf2.png](img/08496e65467471d3f8e3301032a56cf2.png)

Last thing was just to inject a PHP reverse shell and finally we have a foothold.

![246b274d345ebf2eaf73e96dd1ffeed8.png](img/246b274d345ebf2eaf73e96dd1ffeed8.png)

I was seaerching for configuration files to get some password, I found the one with the DB that permitted me the access to Dollibar users hashes  

![bbb337de742043e57251a0a7be959ca4.png](img/bbb337de742043e57251a0a7be959ca4.png)

The hash of **dolibarr** user is with b2crypt so pretty long so while I was cracking I tried to access as the only user inside the machine called **<u>larissa</u>** using the DB password. Luckly it worked!

![342dfcb6bee7153c03ae926d3cc78afa.png](img/342dfcb6bee7153c03ae926d3cc78afa.png)

Now I just grabbed the flag and move on PE

![8b06fd54711364505e60a711342785d8.png](img/8b06fd54711364505e60a711342785d8.png)

<br><br><br>

# PRIVILEGE ESCALATION

Looking inside this machine I found that this specific binary `/usr/lib/x86_64-linux-gnu/enlightenment/utils/enlightenment_sys` hace the SUID bit set! The name is a hint looking back at the machine title and checking [this reference](https://securityonline.info/cve-2022-37706-privilege-escalation-flaw-in-enlightenment-desktop-for-linux/) contains also a PoC that we can use to abuse this vulnerable binary

![38228e710b65e9ba8e8b5063cadfe8d6.png](img/38228e710b65e9ba8e8b5063cadfe8d6.png)
