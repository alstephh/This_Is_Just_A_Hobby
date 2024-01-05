# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.6)


<br><br><br>

# ENUMERATION & USER FLAG

The nmap output highlights that port 80 is using **<u>Drupal 7.56</u>** (`/CHANGELOG.txt` can give us same information)

![eaeee1b315c6ef4f06e5b99ece7e7414.png](img/eaeee1b315c6ef4f06e5b99ece7e7414.png)

There was a few pages more but no helpfull at all (like `/web.config` and `/robots.txt`) so we can try to find if the drupal version which is pretty hold (the current as the time I am writing this is 10). Pretty quickly I found the [CVE-2018-7600](https://nvd.nist.gov/vuln/detail/CVE-2018-7600) a vulnerability that can be exploited through basic drupal modules which are not sanitazing the input with a RCE as outcome (this is the [PoC](https://github.com/pimps/CVE-2018-7600) I used)

![a6483648fab7fd60cf4a69a418102ccb.png](img/a6483648fab7fd60cf4a69a418102ccb.png)

Cool Stuff but I was not able to retrieve a reverse shell, probably is not stable enough or idk what else happened. A run of searchsploit with `drupal 7` as parameter and we have the right solution which is the suggestion of the box title **<u>DRUPALGEDDON2</u>** (LOOKS LIKE A MOVIE TITLE LOL). Metasploit have a module just for that and I wanna stay in my comfort zone so this time msfconsole will save the day!

![4cadf56e1d9be40312fbc373f87582fe.png](img/4cadf56e1d9be40312fbc373f87582fe.png)

The webuser is not able to reach the `/home` directory (not even spawn a stable shell, that's why I was not able to pop up a reverse shell) so we need some horizontal movement. Pretty easy we can take a look at drupal configuration file and credentials will spawn in a blink of an eye

![8dd107118d00e7f9fbfa9c9956d1f00d.png](img/8dd107118d00e7f9fbfa9c9956d1f00d.png)

Now with this pretty bad "shell" we can use the credentials to exfiltrate all the rows in the `users` table

![3ec88dfbba516ea3733f041025f22ffc.png](img/3ec88dfbba516ea3733f041025f22ffc.png)

Now hashcat can help us to retrieve the plaintext

![85e6ba8b7e8795319b7626c339313a06.png](img/85e6ba8b7e8795319b7626c339313a06.png)

...and we have access to the user flag using the credentials to SSH into the machine, finally we have a stable shell!

![340a51ce9a72f997346cc2246322a7df.png](img/340a51ce9a72f997346cc2246322a7df.png)

<br><br><br>

# PRIVILEGE ESCALATION
Ouch, we can run as sudo `snap install`

![4f3da534c51927668f3d5f82e2491fce.png](img/4f3da534c51927668f3d5f82e2491fce.png)

We can PE through a malicious crafted snap package thanks to **<u>fpm</u>** and the **install hook** of snap packages where we can inject the command we want to inject

![7ad61bddafb506ea46c516f9fcceca74.png](img/7ad61bddafb506ea46c516f9fcceca74.png)

Cool now we can uploaded on the target amchine and execute the `snap install`

![07bbf88ebe8175253cda10ef59785e89.png](img/07bbf88ebe8175253cda10ef59785e89.png)

Ok cool is working but can't change the permission of the `bash` binary because is in read-only. I copied a the binary on the home folder cross the fingers...

![cdad162bb1adcb53d22e26d4b3657e27.png](img/cdad162bb1adcb53d22e26d4b3657e27.png)

Uauuu amazing root!

![3bc59ece0e77ab1e782aded855ade619.png](img/3bc59ece0e77ab1e782aded855ade619.png)
