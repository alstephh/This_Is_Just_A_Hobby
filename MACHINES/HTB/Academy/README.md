# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.41)
* **33060** &#8594; MYSQL(x)

<br><br><br>

# ENUMERATION & USER FLAG
The website is a clone of the HTB academy and there is not much we can do if not create a new accounts

![8271eb83b1aefc7ec0abdfcae35a63f4.png](img/8271eb83b1aefc7ec0abdfcae35a63f4.png)

After the login there is not much we can interact with, pretty bad so I decided to make some dirbusting/vhost/subdomain enumeration nd while all of this is running try to see what happen behind the curtains when we register/login since are the only thing we can interract with. Interesting the fact that there is an "hidden" field in the POST request named `roleid`

![24983b5c213996ed302b1ef9a62e3d79.png](img/24983b5c213996ed302b1ef9a62e3d79.png)

By default is setted to 0 so I create a new user but with role ID equals to 1, nothing changed when we access the academy dashboard but `dirb` found the `/admin.php` page. Wtih my default user I can't access it but with the user with role ID equals to 1 we can log successfully. This is what I have found

![0941565095cdf1ac2c64ecd0283b0b56.png](img/0941565095cdf1ac2c64ecd0283b0b56.png)

This subdomain looks pretty caothic lol

![03794f11f949837e3107962d5bd90d55.png](img/03794f11f949837e3107962d5bd90d55.png)

We have something interesting here 

![7adb78203fd6cb608d5f62b25df9157b.png](img/7adb78203fd6cb608d5f62b25df9157b.png)

I tried the credentialsboth with mysqlx and ssh but no low hanging fruit here but we know that is running Laravel just analyzing the errors we have here (debug mode) what's interesting is that we have the `APP_KEY` and usually is used to decrypt cookies but we don't have any of that. I can't find the version of laravel but I know the machine has been created in 2020 so I will keep in scope all vulnerability lower than this date.

Metasploit have 2 exploit and 1 of them is in scope called `laravel_token_unserialize_exec`

![f2722598198eebcb644de83b219b3529.png](img/f2722598198eebcb644de83b219b3529.png)

We just have to set the usual options and the APP_KEY to get RCE

![8d7a100f532a758c7b4a2a5615191e39.png](img/8d7a100f532a758c7b4a2a5615191e39.png)

We need to get access with a valid user to grab the flag but this machine have multiple user so we need to understand which is valid and which not.
Inside `/var/www/html/academy` I found the `.env` file where I found a password

![260840cf7a3e5aa655833cc4dad0b93c.png](img/260840cf7a3e5aa655833cc4dad0b93c.png)

I collected all the username in a file and use hydra to check if there is a user with this password

![679f37e4efc285e9008ad45d321bf6eb.png](img/679f37e4efc285e9008ad45d321bf6eb.png)

Now we can login and grab the user flag!

<br><br><br>

# PRIVILEGE ESCALATION

Nothing that we can do with this user but we are inside `adm` group, pretty good that I never worked with ths group before bcause I learned something new. As part of this group we can perform some log auditing and the fastet way is to use `aureport --tty` which can lead to some credentias exfiltration

![1d389b041c94392ced66c89146eacb44.png](img/1d389b041c94392ced66c89146eacb44.png)

Here we can see all the inputs of a TTy sessions owned by **<u>mrb3n</u>** user and we exfiltrate the password, I used on SSH and see if there is something more we can do to gain root permissions.
We are in the right way here because we can run composer as `root`

![0fc86c7c02440fd2688958aca2b97fd9.png](img/0fc86c7c02440fd2688958aca2b97fd9.png)

Using GTFO bins I found the way to exploit this permissions and gain access as root!

![fa50f87648fa637aacc3fd3631bcf23a.png](img/fa50f87648fa637aacc3fd3631bcf23a.png)
