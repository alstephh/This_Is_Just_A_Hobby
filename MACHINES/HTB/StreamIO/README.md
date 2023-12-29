# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; HTTP (IIS 10.0)
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **139/445** &#8594; SMB
* **443** &#8594; HTTPS
* **399/636** &#8594; LDAP
* **3269**
* **5985** &#8594; WINRM

<br><br><br>

# ENUMERATION & USER FLAG
The (AD) domain of the target is `streamio.htb`. As usual I will start from port 80 and 443 (`watchio.htb`), the former is the traditional IIS welcome page while the latter return error 404 but the certificates contains an interesting alt name `watch.streamIO.htb`

![f70a9e93538719992e1a2499f0b7df65.png](img/f70a9e93538719992e1a2499f0b7df65.png)

After appending this to the hosts file we are finally in

![26dac32e8010da42d3e1f647f7bcb687.png](img/26dac32e8010da42d3e1f647f7bcb687.png)

Pretty good number of subdomain so is worth running some subdomain bruteforcing while I enumerate more, I tried with `https://streamio.htb` and we have the actual web service

![fed509d69a36b6b6c2dbc5b02aa3cd2b.png](img/fed509d69a36b6b6c2dbc5b02aa3cd2b.png)

Now we are talking the same language, there is a login page (also a register page but after account credential no way to login, probably is disabled). I strat a simple instance of `SQLMap` and was able to confirm MSSQL as DBMS and found that the username parameter is injectable with a time based heuristic. Since `SQLMap` started to dump the full database I let it work and wait for some credentials to be found (forcing with the `--tables dbo.users` because we don't care about all the films in their database). While is slooowly retrieving data I wanted to take a look at the `watch.streamio.htb`, there should be some others PHP pages to visit

![fa90571a2c3fd4f74a38bca89241bfdb.png](img/fa90571a2c3fd4f74a38bca89241bfdb.png)

Interesting, the `search.php` can be really usefull and faster than the SQLMap execution

![ca04aac87f02bc2c5f3c0b90a1784a79.png](img/ca04aac87f02bc2c5f3c0b90a1784a79.png)

Is actually a well defined search page because if we search for something like `a` every movie title containing the `a` character will be displayed this means we will have a `WHERE` clause of this type `[...] WHERE title LIKE '%a%'` so I tried to inject in the search query the the following `a%' WHERE 1=1 ;--` but they have some sharp security measure right here!

![39af8c6d48d86642a2e8ecf5f14e1150.png](img/39af8c6d48d86642a2e8ecf5f14e1150.png)

Is not an actual block is just a redirect to a costum PHP page so we can go back and retry from scratch, is time to experiment whitout waking up the Goliath. Well probably the `AND` keyword is not well accepted so we can replacing that with `UNION SELECT` and try to guess the number of collumns, I know there is the film **300** and the sequel so I will use it as dummy variable for the test. After a few attempts we have it!

![a2462eda7f460e232605f79581cba8e2.png](img/a2462eda7f460e232605f79581cba8e2.png)

Exactly 2 results like querying the simple `300` string, now that we have the number of collumns we can move further and try something different

![7d0a025311226eff2d0224e8678b531f.png](img/7d0a025311226eff2d0224e8678b531f.png)

Now we can use the few information retrieved from SQLMap and with some manuality, it founds the `dbo.users` table which I assume will contain the collumns `username` and `password` so I made some try and guess what? It worked

![6cef0f241471f40f3e7b7b74d783c087.png](img/6cef0f241471f40f3e7b7b74d783c087.png)
<br>
![909e2cd86cec0993ec0cef1a85890c95.png](img/909e2cd86cec0993ec0cef1a85890c95.png)

Gotch'a we can easily put togheter this 2 results with `concat`

![de25c17633421d3da43cd10422504d07.png](img/de25c17633421d3da43cd10422504d07.png)

Such a beauty! Now hashcat have a juicy file containing a lot of hash ready to be cracked (apparently MD5), thanks to the genius who created such tool we can do it pretty easily 

![1cbcee6de49b65a82841658387ce8a03.png](img/1cbcee6de49b65a82841658387ce8a03.png)

Now I can search every hash and corresponding username but this a good opportunity to use `Hydra` using our costum username and password files

![f42faf474272e042c25ab6ac3923f243.png](img/f42faf474272e042c25ab6ac3923f243.png)

Cool stuff now we have some valid credentials and I was able to login but nothing changed...fear not we just need more enumeration like we did previously and we will discover a `admin.php` page

![9f929c8129b342f8b35cb07153244571.png](img/9f929c8129b342f8b35cb07153244571.png)

The admin panel use a PHP variable to show the different pages

![936fde04849ad766f9a38260be8e7282.png](img/936fde04849ad766f9a38260be8e7282.png)

I am not enough satisfied with the 4 given here, there should probably be something else (also if I assign something on the variable nothing occurs) so again let's FUZZING! We got something

![e440660e7a9fa70ca496fba66fd6d37a.png](img/e440660e7a9fa70ca496fba66fd6d37a.png)

Even if gives a warning message `this option is for developers only` we can still view content through a LFI

![daecd9c7d1277772c6ac32101a35608f.png](img/daecd9c7d1277772c6ac32101a35608f.png)

The first thing I tried was to connect it on my SMB server in order to have hte NTLM hash but was uncrackable even with different wordlists pretty bad :(

![036a43d967b18603a10b8f753f0c4e10.png](img/036a43d967b18603a10b8f753f0c4e10.png)

I was pretty lost here because when I try to access `index.php` raise an error

![89deb47c6924b1cf5fa48701068833b6.png](img/89deb47c6924b1cf5fa48701068833b6.png)

Well the good news is that I learned a good tricks which can be usefull in differents situation, [PHP Wrapper](https://github.com/payloadbox/rfi-lfi-payload-list#lfi--rfi-wrappers-). This feature allows to encode in base64 the resource we exfiltrating so I tried on the `win.ini` file

![e89de47df8c59278e10eb4587cbfc150.png](img/e89de47df8c59278e10eb4587cbfc150.png)

Well I do the same on `index.php` and than decoded it, we have some hardcoded credentials of the database

![492e846ad6ce3340b2a9ae8aaab04e53.png](img/492e846ad6ce3340b2a9ae8aaab04e53.png)

There is no exposed port which host MSSQL service but probably will turn back usefull when we have a step inside the network, I tried other fuzzing at `https://streamio.htb/FUZZ.php` and check each of the results. When was the `master.php`  we had something interesting

![91d3be086ece7441aabb2acd7d7ed71e.png](img/91d3be086ece7441aabb2acd7d7ed71e.png)

![6ba7ff937d9d969b5717370bb727d796.png](img/6ba7ff937d9d969b5717370bb727d796.png)

We are pretty close to the end of the tunnel for the first flag, there is an hidden form which set a POST paramter called `included` if we are lucky enough we can perform a RFI.
At first the plan was not working so I decided to look at the writeup and as always the details matters because I had to add the right `Content-Type` header 

![da7d04d653cb23307f420ac12e18ac45.png](img/da7d04d653cb23307f420ac12e18ac45.png)

![d8feefbc4e89105a7c732917d4cd696f.png](img/d8feefbc4e89105a7c732917d4cd696f.png)

Cool stuff now we are on the right path whatever we are writing inside the PHP will be injected inside an `eval()` but somehow nothing happend. We have used this new trick of PHP wrapper so we can use it as payload in the POST request creating a PHP webshell, encoded in base64 and decoded using the wrapper. With this we can add a GET parameter injecting OS commands

![39e85c780ba50816b73b818e5470904d.png](img/39e85c780ba50816b73b818e5470904d.png)

![4b1700d09bcc443aba32df40171c1bee.png](img/4b1700d09bcc443aba32df40171c1bee.png)

Finally we have a shell! The user we are impersonating doesn't have a directory in the `Users` folder so we have to make some horizontal movement. We can use the SQL credentials we found previously and finally have free access to the database, there is no trace of a SQL client executable but we can use [sqlcmd utility](https://learn.microsoft.com/en-us/sql/tools/sqlcmd/sqlcmd-utility?view=sql-server-ver16&tabs=go%2Clinux&pivots=cs1-bash). After checking the presence of this tool we can now access the database `streamio_backup` and list all the contents of the `Users` table, inside we have the hash of **<u>nikk37</u>** (the only one in the list with a user directory).

![e27a6c80ea269c8bad47fc46d40ea777.png](img/e27a6c80ea269c8bad47fc46d40ea777.png)

Hashcat successfully cracked and extract the password plaintext `get_dem_girls2@yahoo.com`, I used with Evil-WinRM and we have access to the user flag!


<br><br><br>

# PRIVILEGE ESCALATION
From the Evil-WinRM session I launched WinPEAS and it founds some firefox credentials saved in a `.db4` file, finally I have the opportunity to use [LaZagne](https://github.com/AlessandroZ/LaZagne)

![7420595e140f2c290e9b2c11e6d433fe.png](img/7420595e140f2c290e9b2c11e6d433fe.png)

Cool we have a good amount of credentials so i create a list of user and passwords to use with **<u>crackmapexec</u>** on SMB (on WinRM no of that worked) and we have a valide credentials

![e4923501cfb99a050d0b9337d0acf0f1.png](img/e4923501cfb99a050d0b9337d0acf0f1.png)

The SMB shares seems pretty standard maybe BloodHound can help us understand the purpose of this account

![a3c8f28934b584b5d4e3ab5a857f26b5.png](img/a3c8f28934b584b5d4e3ab5a857f26b5.png)

Pretty good the `JDGOD` user own the `CORE STAFF` groups (with **<u>WriteOwner</u>** permission) which can read [LAPS password](https://learn.microsoft.com/en-us/windows-server/identity/laps/laps-overview), so we need at first to add as owner JDgod user and than try to read the LAPS password. I choose to to the first objective in windows and than use pyLAPS to read the password.
Well, for the first objective we can use `Set-DomainObjectOwner`and `Add-DomainObjectAcl` from powerview to make us owner and give full rights on the group before joining (really easy thanks to the help of bloodhound documentation)

![707844c6934e7ed9e5c8958b99a9f938.png](img/707844c6934e7ed9e5c8958b99a9f938.png)

Now is time to use the python script, the password is the administrator one. We have defeated this box!

![44f0be71c3bb3b28364dd3acc315a4e9.png](img/44f0be71c3bb3b28364dd3acc315a4e9.png)
