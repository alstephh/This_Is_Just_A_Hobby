# PORT SCAN

* **22** &#8594; SHH
* **80** &#8594; HTTP (APACHE 2.4.37)
* **443** &#8594; HTTPS (APACHE 2.4.37)


<br><br><br>

# ENUMERATION & USER FLAG

The HTTP service is a **CentOS** test page (same thing on port 443)

![9215ef1ff4b456b4c8ccc35ce7288649.png](img/9215ef1ff4b456b4c8ccc35ce7288649.png)

Some standard web enumeration return nothing valuable so I decide to look at the response when accessing the webpage and 1 header have a sub domain in it

![b701d8b96141e9adec74172e8dd86a6f.png](img/b701d8b96141e9adec74172e8dd86a6f.png)

After adding this to the hosts file we have a better looking website made with WordPress

![1d3d2b0c41a8398ed1683d0a012fbae0.png](img/1d3d2b0c41a8398ed1683d0a012fbae0.png)

Looking at the source doe is enough to find the wordpress version

![f4a51c3225c854e10338246be56843b8.png](img/f4a51c3225c854e10338246be56843b8.png)

This version is vulnerable to [CVE-2019-17671](https://www.exploit-db.com/exploits/47690) where we can view unauthenticated posts

![89151e0c348866f4a989e861b592ecf1.png](img/89151e0c348866f4a989e861b592ecf1.png)

Hell yeah! Wordpress is always full of surpise, now we have a new host and a "secret" link we can access to register to a `rocket.chat` instance and inside there is a bot called **<u>recyclops</u>**

![eb5508a07241f2fbb2459045dfb4b41c.png](img/eb5508a07241f2fbb2459045dfb4b41c.png)

Hell yeah! Interesting

![f2a0f246ab70109bbfdf76e64ed01a1c.png](img/f2a0f246ab70109bbfdf76e64ed01a1c.png)

Now let's see if we can perform some OS injection or something similar

![304950788cbce732c4c01313433b7ff5.png](img/304950788cbce732c4c01313433b7ff5.png)

The bot to be implemented use [hubot](https://github.com/RocketChat/hubot-rocketchat) (you can see listing the files on the home directory), according to the documentation the configuration file is `.env` so now we can use the command `file` of the bot to retrieve the configuration file and exfiltrate the password

![a6090d3fcd104fe1da6dff96554d247c.png](img/a6090d3fcd104fe1da6dff96554d247c.png)

We try to use this password to SSH as `dwight` user and get the first flag

![02c7153adbaa4aa1d54daace96856b6d.png](img/02c7153adbaa4aa1d54daace96856b6d.png)

<br><br><br>

# PRIVILEGE ESCALATION
Pretty clean machine just a cronjob that restart the bot during system reboot, the sudo version is a bad signal `1.8.29` vulnerable to [CVE-2021-3560](https://github.blog/2021-06-10-privilege-escalation-polkit-root-on-linux-with-bug/#history) which abuse **<u>polkit</u>**  (use it for dialog box running in the background). Without going too much in details we can trick polkit bypassing credentials request for **<u>D-BUS request</u>** (IPC mechanism) and this [PoC](https://github.com/secnigma/CVE-2021-3560-Polkit-Privilege-Esclation) abuse it create a new local administrator (the basic user we have access is not in sudoers) it can require multiple tries because the time-based nature of the attack

![6314de434a4fdce941bcc549e6c1ccd8.png](img/6314de434a4fdce941bcc549e6c1ccd8.png)

Gotch'a!
