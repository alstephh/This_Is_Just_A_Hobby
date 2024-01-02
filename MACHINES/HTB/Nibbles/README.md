# PORT SCAN

* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.18)

<br><br><br>

# ENUMERATION & USER FLAG

We have a grateful message in the homepage of port 80!

![bf8953664d73dbac823a8ddc9bedd4e7.png](img/bf8953664d73dbac823a8ddc9bedd4e7.png)

The source code have a commenttelling us to visit `/nibblenlog` because there is nothing in the homepage, LOL

![3846d91ca6b6f8fb0841da272f4d6f12.png](img/3846d91ca6b6f8fb0841da272f4d6f12.png)

The blog is made with [nibbleblog](https://github.com/dignajar/nibbleblog), if we enumerate this part we have better results than the homepage

![6e39f8856ecbfb7e36a1daf0309ea544.png](img/6e39f8856ecbfb7e36a1daf0309ea544.png)

at `/admin.php` we have the login form while at `/content/private/users.xml` exfiltrate the **admin** user

![b7b25d7444fa1079447316da8c739518.png](img/b7b25d7444fa1079447316da8c739518.png)

So I tried some weak credentials and `admin:nibbles` work smoothly, now we are inside the dashboard of nibbleblog

![518cd70304e726bae8d0e840ce045bc9.png](img/518cd70304e726bae8d0e840ce045bc9.png)

I need to find the version of nibbleblog and in the settings section can be found at the bottom, the `4.0.3` which is vulnerable to [CVE-2015-6967](https://github.com/FredBrave/CVE-2015-6967) a file upload vulnerability which can be used for a php reverse shell!	

I used the PoC but can easily performed manually uploading whatever extension you want (in this case php) through the **<u>MyImage</u>** plugin which existence can be checked with a little enumeration on the XML files

![bec545bff045778e1e630eaa141831a6.png](img/bec545bff045778e1e630eaa141831a6.png)

This is enough to exfiltrate the first flag of the day!

<br><br><br>

# PRIVILEGE ESCALATION

Inside the `nibbler` directory we have a zip file which contains a `monitor.sh` script moreover this script can be run as **root**

![acd273ded71cbe93f9464bc31e539a03.png](img/acd273ded71cbe93f9464bc31e539a03.png)

Well pretty easy, I just add at the end of the script the command bash to spawn a shell as root!


![8c2750e666f72749d858f67ddcb2effa.png](img/8c2750e666f72749d858f67ddcb2effa.png)

Now after the execution we have our high privilege shell!

![994020c9210ba11ff7cf383fc070c176.png](img/994020c9210ba11ff7cf383fc070c176.png)
