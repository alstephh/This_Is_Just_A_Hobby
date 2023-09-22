# PORT SCAN
* **22** &#8594; SSH
* **53** &#8594; DNS (ISC BIND)
* **80** &#8594; HTTP (NGINX)

<br><br><br>

# ENUMERATION & USER FLAG

The webpage is a simple form which is not activated

![1.jpeg](img/1.jpeg)

Making some consistent enumeration, I found something new in **<u>DNS Transfer Zone</u>**

![2.png](img/2.png)

Cool we have a new domain to checks, at  `root.trick.htb` we have the same version of the website of the original domain....interesting but not cool at all :(

In `preprod-payroll.trick.htb` we got something new, finally!

![3.jpeg](img/3.jpeg)

Ok cool, I run a hydra and dirb instance while I was trying some simple SQL injection and with the parameter **username** equals to `' OR 1=1#` and whatever password we can get access inside.

<br>

#
Looking at the walktrhough after the SQL injection I found that the title of the site is  `Admin | Employee's Payroll Management System`, **<u>Payroll Management System</u>** is an [exisisting management software](https://www.adp.com/resources/articles-and-insights/articles/p/payroll-management-system.aspx) (this is what was slowing me, I tought was just a random name) and searching for some known exploit we have a [SQL vulnerability](https://nvd.nist.gov/vuln/detail/CVE-2022-28468), is cool to discover something simple by yourself and than checking that someone already discovered that
#

<br>

We are in as **Administrator** account, good let's see what we have here ready to be discovered

In `Attendance` we have a name and the time record 

![4.png](img/4.png)

`Employ` give us more info about our guy john

![5.png](img/5.png)

I user something interesting, we have just a user called `Administrator` and username is `Enemigosss`, when we click edit we have hte password field and with a simple tik tok trick we are able to change the type from password to plain-text

![6.png](img/6.png)

Try to use this set of credentials through ssh but no access at all, no good

`Payroll List`, wow we can see how much john gain as salary

![7.png](img/7.png)

Here I was pretty lost nothing interesting we can do about it and yeah...I saw the writeup but I learned that sqlmap can be used not only to test adn retrieve database contents

<br>

We already know that the username is injectable so we are using that in the `-p` flag and we can use the `--privileges` to check what privileges the DB account have in  this case **<u>FILE</u>**

![8.png](img/8.png)


Now we can rerun sql map with `--file-read=/etc/passwd` to retrieve the file which is saved locally and we can check the contents

![9.png](img/9.png)

We gotta it, we can check the contents in the nginx dirctory and retrieve another host if present especially in `/etc/nginx/sites-
enabled/default`. We have a new subdomain, gotcha!

![10.png](img/10.png)

Let's add it to host file (`preprod-marketing.trick.htb`) and navigate there

![.jpeg](img/Untitled.jpeg)

Ok, enumeration again nothing new lol

In the `about` page we some employes names which is cool

![11.jpeg](img/11.jpeg)

What's interesting here is the use of the **?page** parameter

![12.png](img/12.png)

So the **?page** parameter is used to select the file to show in the webpage, maybe we can make a **<u>LFI</u>** pretty easy with the `....//` trick

![13.png](img/13.png)

In [this article](https://medium.com/@Aptive/local-file-inclusion-lfi-web-application-penetration-testing-cc9dc8dd3601) I found a cool way to use the email to make a reverse shell, we know that the user `micheal` exist looking at the `/etc/passwd` (I checked through the LFI).

<br>

This is the commands I used to upload the php reverse shell

![14.png](img/14.png)

Than just use the **LFI** to activate the reverse shell

![15.png](img/15.png)

<br><br><br>

# PRIVILEGE ESCALATION
Ok cool stuff now we don't need to make a lot of enumeration, `sudo -l` was enough

![16.png](img/16.png)

Checking the id we know we are inside the security group

![17.png](img/17.png)

Checking some configuration file inside `/etc/fail2ban` look we have the `action.d` directory that is own by the security group

![18.png](img/18.png)

So it is writable by us, eheheehhe cool let's use [this article](https://juggernaut-sec.com/fail2ban-lpe/) to get what I want!

<br>

1) We can't write in the fles but with this TRICK we can make it happen, in details we need to edit the `iptables-multiport.conf` file

![19.png](img/19.png)

2) We write inside the `iptables-multiport.conf` file changing the actionban to whatever we like (I directly copied the `root.txt` file and give me the permission to read it, originally use iptables to blacklist the malicious address)

![20.png](img/20.png)

3) We need to get ban, try to login in ssh missing the password and after few attempts the command we decide to use will be triggered, enjoy!

![21.png](img/21.png)

