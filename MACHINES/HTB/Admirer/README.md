# PORT SCAN
* **21** &#8594; FTP
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.25)

<br><br><br>

# ENUMERATION & USER FLAG

![455de5ccbad3034e6bfe353d36a37892.png](img/455de5ccbad3034e6bfe353d36a37892.png)

The webpage is a simple image galley, `/robots.txt` is pretty interesting

![b4689bc7b98b8bd80c96db7cc67e155a.png](img/b4689bc7b98b8bd80c96db7cc67e155a.png)

DirBusting on the `/admin-dir` reveals an interesting file called `/contacts.txt`

![2c5060846a3eedb0105760bf87993a6b.png](img/2c5060846a3eedb0105760bf87993a6b.png)

Cool we have a good set of email (and consecutevely potential username), after a little bit of time I get another more interesting file called `credentials.txt` here things are juicy!

![51077d52f4de91a06cae26ab8858d54a.png](img/51077d52f4de91a06cae26ab8858d54a.png)

Cool, I have no clue of Wordpress (at least on this domain) nor the internal mail login but we can use the creds to login in FTP and downloads whatever we have inside there

![7fe16e33d815931ba1f2b061ec1a5057.png](img/7fe16e33d815931ba1f2b061ec1a5057.png)

Inside the tar archive I have found an upodated version of **<u>credentials.txt</u>** with a bank account creds

![34d00779fe1b73af6e5e7c5131f86e7c.png](img/34d00779fe1b73af6e5e7c5131f86e7c.png)

Other credentials have been found in `index.php`, this time MySQL user

![ec2fd88506c603f69fc77ed556aa5d49.png](img/ec2fd88506c603f69fc77ed556aa5d49.png)

Dig further on this PHP code I have notedthat image are not static but retrieved on the database (dump.sql contains the records) and than putted on the homepage, other credentials can be found inside the **<u>utility-scripts</u>** where a `db_admin.php` is present

![e363332b03d9e6bb8b443ed36d4c4431.png](img/e363332b03d9e6bb8b443ed36d4c4431.png)

 We have a  `admin_tasks.php` file that is used for simple tasks and can be used on the live server
 
 ![30ce58bb25762f1e67b8b27eb4f0b2f3.png](img/30ce58bb25762f1e67b8b27eb4f0b2f3.png)
 
 Makes sense to enumerate this directory nad found something else that maybe is available, if we are lucky enough something here can turn be usefull. Now I have searched "Admirer php" online (I tought was a stupid thing and actually it is, lol) and the first result I get was [Adminer](https://www.adminer.org/) which is "_Database management in a single PHP file_", all makes sense with all the SQL credentials  I have found.
 So while dirusting didn't show results yet I tried to visit `/utility-scripts/adminer.php`, guess what? It worked
 
 > Being stupid sometimes pays more than be logical and technical!
 
 ![7cc1af5dc780590bd7a1e1b6df049cd9.png](img/7cc1af5dc780590bd7a1e1b6df049cd9.png)
 
This specific version is vulnerable to [CVE-2021-43008](https://github.com/p0dalirius/CVE-2021-43008-AdminerRead/blob/master/README.md) that allows attackers to read files on the target directory. Now things became complicated and I tried all the credentials I found but none of them worked on the target host. I was pretty lost so I looked at HTB hint and they suggest to build our own SQL database and used as dummy system just to establish a connection with Adminer. On my Kali machine I have **MariaDB** which is supported by Adminer. I have created a new user for the remote machine and a dummy database to connect to

![9c2f61669e978100f0b3ef224b7da43c.png](img/9c2f61669e978100f0b3ef224b7da43c.png)

This is not enough I have to add a UFW rule to allows incomin connection from the target with `ufw allow from 10.10.10.187 to any port 3306` and a quick change on **bind-address** of `50-server.cnf`

![5f60639d470a83da7e7d8b9f1cd5d133.png](img/5f60639d470a83da7e7d8b9f1cd5d133.png)

Now we can access with the credentials we have created and get access to the **Adminer dashboard**

![3518b584705e4c7ca33a906ff36fc0ef.png](img/3518b584705e4c7ca33a906ff36fc0ef.png)

Gotcch'a, now we can insert inside the dummy table we have created some local files and than retrieve the results with

```{sql}
USE dummyyummy;
LOAD DATA LOCAL INFILE '/var/www/html/index.php' INTO TABLE yummy;
SELECT * FROM yummy;
```

![16725b2e4755eda36c62c713023dd28c.png](img/16725b2e4755eda36c62c713023dd28c.png)

On `index.php` we have credentials and since they are on the live website they probably work with SSH

![3ef41ce867ec300e266cff8718721920.png](img/3ef41ce867ec300e266cff8718721920.png)

First objective has been grabbed, let's move on!

<br><br><br>

# PRIVILEGE ESCALATION
Using `sudo -l` we can execute a bash script that is a **<u>System Administration Menu</u>**

![ce8d3febbfabd69c900a99d3f5c074a2.png](img/ce8d3febbfabd69c900a99d3f5c074a2.png)

We can look at the contents of the bash script and a python script is executed when a backup of the web data is requested

![0a6e205c2f2d4741428f1190de212252.png](img/0a6e205c2f2d4741428f1190de212252.png)

![ca97afdefca0991e1c2215310b4f8e28.png](img/ca97afdefca0991e1c2215310b4f8e28.png)

What I tought at first was to put a symbolic link to the root.txt flag inside `/var/www/html` since it will be used to create an archive but we are not able to navigate iside the directory and no way to append files, good idea but unavailable. What I miss here is the `SETENV` option on **sudo**. I tried at first to abuse the [PYTHONSTARTUP](http://www.witkowskibartosz.com/blog/pythonstartup_what_it_is_and_how_to_use_it.html) variable but works only with interactive python console. Looking at [official python documentation](https://docs.python.org/3/using/cmdline.html#environment-variables) on enviroment variables I found the `PYTHONPATH` that change the pointer to python libraries in a location we decided. The scripts import a library that we can hijack to a malicious one!

I created a new `shutil.py` which set a SUID bit to the bash and specified his path on the variable discovered above. After the execution we can impersonate **root**!

![be259cfa87cfdf4fc55cb145e89547a9.png](img/be259cfa87cfdf4fc55cb145e89547a9.png)
