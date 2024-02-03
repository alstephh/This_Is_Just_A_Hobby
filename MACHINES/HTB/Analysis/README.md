# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; Microsoft HTTPAPI httpds 2.0
* **88 / 464** &#8594; KERBEROS
* **135 / 593** &#8594; MSRPC
* **139 /  445** &#8594; SMB
* **389 / 636** &#8594; LDAP
* **3306** &#8594; MySQL
* **5985 / 47001** &#8594; WinRM
* **9389** &#8594; AD RECYCLE BIN
* **33060** &#8594; MYSQLX

<br><br><br>

# ENUMERATION & USER FLAG
The box looks like an Active Directory Domain Controller (`dc-analysis.htb`) of the `analysis.htb` domain. Port 80 is pretty strange have the same version of WinRM port, never saw something like that before. By the way the website run on IIS 10.0 and is pretty empty...

![a20d44ae53cc0f20f5c6ef6d9355c346.png](img/a20d44ae53cc0f20f5c6ef6d9355c346.png)

I make some enumeration on the subdomains and `internal.analysis.htb` looks valid but when we access it gives error 403. Even if we cannot access it I was able to perform some dirbusting and I found some path like `/employees` and `/users`, on the latter I recursevely perfrom directory busting and found a interesting PHP file

![d41320c485babf6b4b0d0b8dfa14ed82.png](img/d41320c485babf6b4b0d0b8dfa14ed82.png)

It requiered a parameter to work and with some intuition I was able to retrieve a user

![ff016c53070d0d74c92ac8512b3e755d.png](img/ff016c53070d0d74c92ac8512b3e755d.png)

To be sure this user exist in the AD I user `GetNPUsers`, et voila' we have a valid username now!

![4d5bce779a091742d68ff625224bb3d0.png](img/4d5bce779a091742d68ff625224bb3d0.png)

I am not sure why we can access this part of the website but is worth to check if `/employees` have something too and actually `/login.php` exist

![47d1da73f212413b2c9582c0990d3167.png](img/47d1da73f212413b2c9582c0990d3167.png)

Now, let's jump back at `/users/list.php`, believe me ornot but just with my intuition I use **name** as parameter and `*` to retrieve all the possibile records like SQLi but is not SQL (I ty some injection) but LDAP. I tought about that becuase return the same information as Domain Controller user list .
We can try some LDAP injection but how? Well with `ffuf` I was able to recover another parameter other than name called **<u>description</u>** so I try to perform the injection like this `?name=*)(description=9*`

![3f64e5f4486b6a92f29dd48599da15ce.png](img/3f64e5f4486b6a92f29dd48599da15ce.png)

This is cool it means that the description start with `9` so I can iterate this process with a python script 

![083110e1b09a45c4aa595e2447f13bdf.png](img/083110e1b09a45c4aa595e2447f13bdf.png)

What we have back is the string `97NTtl*4QP96Bv`, this sounds like a pasword so I used directly on the dashboard found before

![68ee58fd6fd3e809f41ef81a170d3386.png](img/68ee58fd6fd3e809f41ef81a170d3386.png)

Here we have a list of user so I create a user list (traditional thing like first letter of name and surname and blablabla) and I tried the password found checking if someone reused it! 

![aa83879659ebb31977cb88e6fb48b95e.png](img/aa83879659ebb31977cb88e6fb48b95e.png)

Yeah, nice thing but SMB is empty, we can't login with WinRM and other things (kerberoast,ASREProasting,ecc...) don't work but on RPC I get an exahustive list of user

![7de3af9bbefe9422cb077b520fc830c9.png](img/7de3af9bbefe9422cb077b520fc830c9.png)

Inside the dashboard we can upload file through the appropiate section

![afad1cdcae843a809a86754ec6ea731b.png](img/afad1cdcae843a809a86754ec6ea731b.png)

Here I uploaded the Ivan Sincek reverse shell and voi'la

![e76eb53f5675b2804abde9223fc97b9a.png](img/e76eb53f5675b2804abde9223fc97b9a.png)

In this shell I run **winpeas** and found AutoLogon password of `jdoe`

![eddbfdde465c728e384d477dd6730249.png](img/eddbfdde465c728e384d477dd6730249.png)

Cool stuff now we can use this credentials on EvilWinRM and get the first flag!


<br><br><br>

# PRIVILEGE ESCALATION
The `jdoe` user have no special permission but we can note there is Snort installed in hte system


![78157ed0f3c4e8909834a5346a01996d.png](img/78157ed0f3c4e8909834a5346a01996d.png)

The version of Snort is `2.9.7` and is vulnerable to [DLL HIJACKING](https://packetstormsecurity.com/files/138915/).
With some trial and error I was able to find the gap where this vulnerability rely, in `/snort_dynamicpreprocessor` if we add a DLL called `sf_engine.dll` (made with **msfvenom**) after a few seconds we will receive a shell back as SYTEM

![ee01eb79e0f155c7643a4c71dadb70f7.png](img/ee01eb79e0f155c7643a4c71dadb70f7.png)

![5991366eb665c363b29efdabcfc11ae2.png](img/5991366eb665c363b29efdabcfc11ae2.png)
