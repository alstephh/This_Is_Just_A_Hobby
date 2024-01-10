# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (NGINX 1.14.2)
* **8065** &#8594; MATTERMOST


<br><br><br>

# ENUMERATION & USER FLAG
MatterMost service need some credentials or more info to build a costum wordlist for password/usernames.
The HTTP page is made by a single page with info about the purpose of MM.

![df6c6c3c8a5a42abd0b6c871117f2fe1.png](img/df6c6c3c8a5a42abd0b6c871117f2fe1.png)

Well the helpdesk is under `helpdesk.delivery.htb` so I add it to hosts file and visited it. Is a subdomain powered by [OSTicket](https://osticket.com/) and I use [this script](https://github.com/Legoclones/pentesting-osTicket/blob/main/ostVersionScanner.py) to get exfiltrate the version of the software

![b28572d6b93f95c7509ba36468fd0315.png](img/b28572d6b93f95c7509ba36468fd0315.png)

At first I tought to abuse a SQL injection that affects this 2 specific version but was not working so since we don't need to be authenticated to request ticket I started creating a new one, hopefully we should have a email address

![d1340833aea9bd71ae9696537cb5144d.png](img/d1340833aea9bd71ae9696537cb5144d.png)

![5ff9cefc36cac6ae3309a98564f49445.png](img/5ff9cefc36cac6ae3309a98564f49445.png)

Gotcha, now we can use the new email address to create a new account and an email address need to be verified but if we go to the ticket status (we need to enter the email address used for the ticket request and Ticket ID) the email body is sent directly on the ticket status

![a64177acc9920ef6e9ec12aefd121aec.png](img/a64177acc9920ef6e9ec12aefd121aec.png)

Now we are to login on the mattermost server! Thiss "attack" is called [Ticket Trick](https://medium.com/intigriti/how-i-hacked-hundreds-of-companies-through-their-helpdesk-b7680ddc2d4c) and the main issue is that we don't have to verify email address to see the ticket queue/status resulting on verification email appended on the issue list. Pretty powerfull and is not something that can be fixed (at least, for now) and affect a good number of companies according to the creator of the attack.

Having access to development team chat is not a good thing for victims thing like credentials can be found, like in this case

![222b26c3bf929010e84b6c2078cb347d.png](img/222b26c3bf929010e84b6c2078cb347d.png)

We can use this credential into SSH and get the user flag!

![59cea61c2c4f9b9cc838c8556b388e4d.png](img/59cea61c2c4f9b9cc838c8556b388e4d.png)

<br><br><br>

# PRIVILEGE ESCALATION
The user we have access to is not inside the sudoers group so no way to use `sudo` but another user that is not root have terminal access : **mattermost**

Since I am not sure 100% that we need to pivot on the mattermost user to get root is worth trying to lookup at `/opt/mattermost` directory for some configuration file or similar (if nothing is found here the next target is `/var/www/html`)
Pretty easy, we have credentials of **MySQL**

![b0f681fd59d46d349eb2a693ae389d0b.png](img/b0f681fd59d46d349eb2a693ae389d0b.png)

Useless to say that there wasa table called `Users` where root hash is stored

![83cba843ab49fb8ab97b0439dbf8fffb.png](img/83cba843ab49fb8ab97b0439dbf8fffb.png)

Now we have a good hint on the MM chat, we can use a rule file with hashcat on the string `PleasSubscribe!`. This is something that should be done on this specific hash because is **bcrypt** which is really really slow to crack especially with a long passwordlist. I tried with the [sparcflow file-rule](https://github.com/sparcflow/HackLikeALegend/blob/master/corporate.rule) on the base word tha we know and after a few seconds we have our plaintext

![806f223454f290955e516f3e8bc69087.png](img/806f223454f290955e516f3e8bc69087.png)

We can use it with `su` and have access to root flag!
