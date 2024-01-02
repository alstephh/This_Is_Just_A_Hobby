# PORT SCAN
* **22** &#8594; SSH
* **25** &#8594; SMTP
* **80** &#8594; APACHE (HTTP 2.2.3)
* **110** &#8594; POP3 (CYRUS 2.3.7)
* **111** &#8594; RPC
* **143** &#8594; IMAP (CYRUS 2.3.7)
* **443** &#8594; HTTPS (APACHE 2.2.3)
* **993** &#8594; SSL
* **3306** &#8594; MYSQL
* **4559** &#8594; HYLAFAX (4.3.10)
* **5038** &#8594; ASTERISK CALL MANAGER (1.1)
* **10000** &#8594; MINISERV (1.570)

<br><br><br>

# ENUMERATION & USER FLAG
Uau a lot of services running, pretty overwhelming but this is just a good thing, we have a lot of place to get info from. Is also possible that a more than one way is possible to compromise this box.

We start simple from HTTP service which redirect to HTTPS and a strange error pop up...

![31fba8b145ce7115456aa3d1c6c73418.png](img/31fba8b145ce7115456aa3d1c6c73418.png)

It appears that the SSL version used by the server is lower than 1.2 the minimum required by firefox (I also tried Brave and Chrome with no different outcomes), same thng happen when we try to access port 10000 which ask to use the HTTPS protocol rather the standard one

![9e2fe8824804ac855f22ea1e72c7af97.png](img/9e2fe8824804ac855f22ea1e72c7af97.png)

We can easily put the minimum version of TLS supported on FireFox going to `about:config` and set `security.tls.version.min` to **1**. Now we can access the services provided on the box! On port 10000 we have a `webmin` login form

![8cb0d4d7c2d65ee7201f2a7484fb4e74.png](img/8cb0d4d7c2d65ee7201f2a7484fb4e74.png)

While on standard HTPPS port we have an `Elastix` login page

![5e81f6cce63bbf39210880788a9b542c.png](img/5e81f6cce63bbf39210880788a9b542c.png)

This specific service is vulnerable to a [RCE vulnerability](https://www.exploit-db.com/exploits/18650),  I used this [well done PoC](https://github.com/k4miyo/FreePBX-Elastix-RCE-exploit) to get a reverse shell

![13853a570cd619d6cb6d713103caab83.png](img/13853a570cd619d6cb6d713103caab83.png)

> I saw the different methods to gain foothold of the machine and this one is the only one where you spawn as non-root user this is why the other paths are not present here


<br><br><br>

# PRIVILEGE ESCALATION
Even for the PE section seems to be multiple ways to root the machine, especially with `sudo -l`

![5ac928f479a4cced70a447bc107139e3.png](img/5ac928f479a4cced70a447bc107139e3.png)

nmap is always a good choice thanks the **interactive mode**

![4f691f87b527af55bd04c9e83a2b2fe1.png](img/4f691f87b527af55bd04c9e83a2b2fe1.png)

Also `chmod` is a dangerous binary to give SUDO permission without password....

![c0246c77f442bd1a3f209956595debcd.png](img/c0246c77f442bd1a3f209956595debcd.png)

Really easy machine, piece of cake!
