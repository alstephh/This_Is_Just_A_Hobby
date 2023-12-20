# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (nginx 1.18)


<br><br><br>

# ENUMERATION & USER FLAG

![65dd7c671f4aa574d14bb87d8052ff9d.png](img/65dd7c671f4aa574d14bb87d8052ff9d.png)

The webpage is really simple but have an `/admin/login` page

![c780cdf070400112dd2e7e2c8a6fe895.png](img/c780cdf070400112dd2e7e2c8a6fe895.png)

Looking at the source code we have version `4.4.14` which is vulnerable to [CVE-2023-41892](https://nvd.nist.gov/vuln/detail/CVE-2023-41892)

![c1540bc580d49d38c5342b6707b7cc28.png](img/c1540bc580d49d38c5342b6707b7cc28.png)

Following this [small blog post](https://threatprotect.qualys.com/2023/09/25/craft-cms-remote-code-execution-vulnerability-cve-2023-41892/) I was able to replicate what they did

![8ca267552c7627f947f65a9dcbda8e14.png](img/8ca267552c7627f947f65a9dcbda8e14.png)

Cool now I need to find a way to leverage this simple PoC to something usefull and [this other PoC](https://gist.github.com/to016/b796ca3275fa11b5ab9594b1522f7226) is the one that allowed me to perform foothold keeping attention at what the creator told us

> This POC is depending on writing webshell, so <u>finding a suitable folder with writable permission is necessary</u>.

I changed the location of the path where the shell will be created to `/cpresources` and we hve a shell as `www-data`

![f265f51ec809e085f118807732d1d1c2.png](img/f265f51ec809e085f118807732d1d1c2.png)

Now with a run of linpeas I found some **credentials for the mysql** server

![3a689e1568d93e4945e82ff30840e03d.png](img/3a689e1568d93e4945e82ff30840e03d.png)

Inside there was the table `users` which contains the bcrypt `password hash for matthew`, this type of hash require a lot of time to crack but was worth to try while I was searching around.

Makes sense to take a look at `/html/craft` maybe we have a simpler hash for the matthew password and inside `/html/craft/storage/backups` we have a zip file containing a sql file

![d6506aa38db0372cb0b1bd9e415d4431.png](img/d6506aa38db0372cb0b1bd9e415d4431.png)

Inside we have our reward!

![bc58aaaa7bfa2f51f11f2e39ecd92a87.png](img/bc58aaaa7bfa2f51f11f2e39ecd92a87.png)

![de9c7fafb5968a34d89d227c11d554bd.png](img/de9c7fafb5968a34d89d227c11d554bd.png)

With this we have access through SSH and the user flag!

<br><br><br>

# PRIVILEGE ESCALATION

The previous linpeas output highlighted the port 8080 open on the target so with chisel we can forwarded to attacker machine 

![2e6d5759a5a9aa7c540abe46bdf4632b.png](img/2e6d5759a5a9aa7c540abe46bdf4632b.png)

[ZoneMinder](https://zoneminder.com/) is a open source surveillance software system and now we know why the machine is called like that so we are in the right path

We need some credentials to get inside and after that we can use the metsploit module `exploit/unix/webapp/zoneminder_lang_exec` exploiting [CVE-2022-29806](https://nvd.nist.gov/vuln/detail/CVE-2022-29806) but since there is no way to get credentials (I have searched everywhere butno hash, hardcoded or default credentials) but let's not lose hope too quickly because the version of zoneminder (which is the `1.36.9` you can check it at `/usr/share/zoneminder/db`) is vulnerable to an [unauthenticated RCE](https://nvd.nist.gov/vuln/detail/CVE-2023-26035) through snapshot which have missing authorization.

By the way, metasploit have a fresh module for this vulnerability called `unix/webapp/zoneminder_snapshots`, pointing this to the localhost port forward will spawn a shell as **<u>zoneminder</u>** user

![7d226e535e7fd59d3bdef18170a6e04d.png](img/7d226e535e7fd59d3bdef18170a6e04d.png)

This user have a pretty strange sudo permissions

![556a45bb3eb046d9e07cc0cd943f2e92.png](img/556a45bb3eb046d9e07cc0cd943f2e92.png)

Now I take some time to check every perl script and I come with this but was not what I expected....

![d4af4b4cb48a79f1c05aa7a509974607.png](img/d4af4b4cb48a79f1c05aa7a509974607.png)

uhm unlucky this was my bad because the nested command for instance `$(whoami)` will take the permission of the main bash command and even if we launch something with sude the owner of the nested command will be the low priv user, cool BUT we can add a layer to this OS injection.

the `--user` parameter is going to be used as `-u` parameter of the **mysql** command, obviously with `system()` function that will run under `root`. So if we use the nested command as strings instead of directly execute as low priv and then use it as parameter for the sql login we will do the opposite pasing our malicious parameter as a string!

![79e69aeaba2e3eacd9e42734abe56b9e.png](img/79e69aeaba2e3eacd9e42734abe56b9e.png)

Yeah it worked! even if we have no output from the new shell we get it as `root`, so we can create and execute it with this method. the results will be our wanted high privileged shell! 

![470b632dde4de21426cb838d69bb962b.png](img/470b632dde4de21426cb838d69bb962b.png)
