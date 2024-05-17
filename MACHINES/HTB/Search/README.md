# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; HTTP
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **389 / 636** &#8594; LDAP
* **443** &#8594; HTTPS
* **445** &#8594; SMB

<br><br><br>

# ENUMERATION & USER FLAG
What caught my eye here start from the NMAP scan because there is no WINRM port (usually available when the machine is a domain controller) so probably we would get inside the machine only with Administrator credentials through PSEXEC. But this is too early to think about this stuff so let's move on the website.

![6a0e28cf4631c06b26cf2d6faa7829e9.png](img/6a0e28cf4631c06b26cf2d6faa7829e9.png)

Pretty cool website, the machine is rated as hard but we are in AD enviroment a truf that I feel a (little bit) more comfortable. What can become usefull is the name and surname of the company team, we can play a little bit with Kerberos to check exisisting username

![39716d73bd416f541a6ad8be220d8dc7.png](img/39716d73bd416f541a6ad8be220d8dc7.png)

I used [usernamer](https://github.com/jseidl/usernamer) to generate a wordlist of potential username given a file with name and surname on every line. Now we can use one of my favorite tricks (I think this works good in CTF but is not ideal in a real engagement, too noisy) let's force ASREP roasting on the username list and check the ones that return `UF_DONT_REQUIRE_PREAUTH`

![63e2dd91f9d2894cd36084fe7e1df7d4.png](img/63e2dd91f9d2894cd36084fe7e1df7d4.png)

You see? Cool ah? Now we have something, it would be greate if one of them have PREAUTH enabled but that's fine is a good start. Ok this is a pretty stupid thing honestly (this type of things are the one that I hare of CTF) but look closely on one image of a carousel...this where we have a potential username and password 

![a651dcb9c9695ea23bd849c6a5653546.png](img/a651dcb9c9695ea23bd849c6a5653546.png)

Bruh. We can look the good side of it we have a username and a potential password. I have updated the username list and try some password spraying

![9392a6bc620bd5e22155eef7bbc387b3.png](img/9392a6bc620bd5e22155eef7bbc387b3.png)

Coooooool stuff! now we have access to a valid account and we can explore a little bit more ehehehe. Let's start from the SMB share

![b02c8b2d759beb41ef0a67de84af792d.png](img/b02c8b2d759beb41ef0a67de84af792d.png)

Also let's collect the LDAP info with `ldapdomaindump`, I took a look and we have a huuuuge list of user, computers and groups.

![2c3cffa7302cbde0aeed1edfedd23506.png](img/2c3cffa7302cbde0aeed1edfedd23506.png)

I was so happy to have an user controlled by me that I have jumped directly to possible Kerberoasting and surprise surprise we have it!

![55567d8261e482e02ce1bda81585b047.png](img/55567d8261e482e02ce1bda81585b047.png)

Tell me no more the powerfull cat is ready to roar!

![12c1d80c44a24a7165b0c24874f2b0cb.png](img/12c1d80c44a24a7165b0c24874f2b0cb.png)

![c4c2e899aa72148594f956bea23f0aec.png](img/c4c2e899aa72148594f956bea23f0aec.png)

Nice we have a second user available. I get inside the **<u>CertEnroll SMB share</u>** and is just a fodler with boring crt files. I want ot have access on the `HelpDesk SMB` share instead so makes sense to collect all the user in that group thanks the power of grep files retrieve from LDAP

![272f30e79cf95d5c03e9bef71ac78a91.png](img/272f30e79cf95d5c03e9bef71ac78a91.png)

Now back to password spraying!

![74817f96c9c649b986605dc7cd1f6535.png](img/74817f96c9c649b986605dc7cd1f6535.png)

Oh yeah, Active Directory ILY <3

![176ed82849afe5b67c69081406cc6fed.png](img/176ed82849afe5b67c69081406cc6fed.png)

AHAHAHAHAHAHAHA no way! Well at least we have a new user but omg what a pain, come on! :(
Well we got something else, the share `RedirectedFolder$` is just a link to the home folders, using the credentials of **Edgar.Jacobs** I get inside his home directory and the Desktop have something cool!

![f4b329ec3ee8cba9bc7b0b094ba92e9b.png](img/f4b329ec3ee8cba9bc7b0b094ba92e9b.png)

We can take a look at it on a online viewer, something is strange....

![d83f9df68651b96576e1fb4d73ba39c6.png](img/d83f9df68651b96576e1fb4d73ba39c6.png)

Look closely, the column `C` is missing. That's bad probably is protected from an optional password which is displayed only on Excel (or LibreOffice), the online viewer simply don't ask for it and simply hide it. Go and look at other writeup and make a laugh on how much simple this was compared to the intended way lol (but really usefull to understand how to bypass that).

![867c8587621be53084940321c6395fac.png](img/867c8587621be53084940321c6395fac.png)

Gottch'a we have a new list and passwords let's see if they are still working

![22401c60b89e65da77d3817b4c2e6029.png](img/22401c60b89e65da77d3817b4c2e6029.png)

Good morning Sierra I'm going to take your account for a while see ya! I get back to the `RedirectedFolders` share and we have our flag, finally the first half of the reward!

![a329c617e8ab001aaeb8c681b97db844.png](img/a329c617e8ab001aaeb8c681b97db844.png)


<br><br><br>

# PRIVILEGE ESCALATION
Now is time to get inside the machine, no need to rest let's go. We have a backup folder with inside a PFX and a p12 file

![110acd4aba1c3f6bc86ae65ea8caf55a.png](img/110acd4aba1c3f6bc86ae65ea8caf55a.png)

Trying to export on firefox required a password, uauauuau time to use pfx2john and crack it. Didn't require too much before retrieving the plaintext

![c79e31e5bf244131008782482aafdbb1.png](img/c79e31e5bf244131008782482aafdbb1.png)

![15c3f12c9c6a78f901d4b4a5493c3913.png](img/15c3f12c9c6a78f901d4b4a5493c3913.png)

I forgot to say that during the dirbusting process the path `/staff` has been found, probably now with this certificate we can acces it and look what we have


![8e4c3129910cce914e9cdd498f635e23.png](img/8e4c3129910cce914e9cdd498f635e23.png)

Using the credentials don't work but probably because we need to specify the computer name. LDAP info now are usefull as hell I need to find the FQDN of the domain controller

![efdc35fb2cb1b4717ef67a9b4866c0d1.png](img/efdc35fb2cb1b4717ef67a9b4866c0d1.png)

Boom easy, gottch'a

![d00e4f867372ce0bcd3b8a1907b4db99.png](img/d00e4f867372ce0bcd3b8a1907b4db99.png)

I cannot spawn a meterpeter shell with the web delivery module but that's fine we would use this not so beauty interface. I think is time to feed bloodHound and start analyzing permissions from there, let's see where we can get.

First thing first we have a non-standard user part of the administrator group

![1f74e4c71f0c89d66c1579cfd7a06730.png](img/1f74e4c71f0c89d66c1579cfd7a06730.png)

How we can reach this bad guy right here? Well let's ask to our favorite AD sniffer dog

![5f5260fd93069760965fcba0dd6528a0.png](img/5f5260fd93069760965fcba0dd6528a0.png)

Hell yeah, I know how to abuse this! Simply talking we can read the **<u>GMSA</u>** password, the **<u>Group Managed Service Account</u>** members are accounts that don't need human interaction and permits to other object to read their password in order to execute service/program with their privilege on remote machine (for instance running the same service but on different hosts, a sort of benefic crackmapexec). Since we have the possibility to read it means we have control of the account as well.

![628125f63e2db007f6c954958fdece6c.png](img/628125f63e2db007f6c954958fdece6c.png)

We are in AD so the hash is fine for our objective, `BIR-ADFS-GMSA$` have **GenericAll** permissions to the Administrator user. We can change the password with `pth-net` and than is officially game over!

![f6d0b37f772f553aeec96e8dbd419207.png](img/f6d0b37f772f553aeec96e8dbd419207.png)

Oh yeah the web-powershell was useless because I prefer playing with my sweet Kali machine but a good refresh on how to use certificate hurts just the ones that are not going to do it ;)
