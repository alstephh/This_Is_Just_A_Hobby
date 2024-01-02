# PORT SCAN
* **80** &#8594; HTTP ()
* **2222** &#8594; SSH


<br><br><br>

# ENUMERATION & USER FLAG

Interesting choice to move the SSH service to another port (2222) by the way is pretty useless without credentilas so let\s take a look at port 80

![57387049d2441a7af530ed19526a2249.png](img/57387049d2441a7af530ed19526a2249.png)

"Don't bug me"? uhm, I think I will soon or later. This is just a static page and the webserver doesn\t seems to support PHP so we need a strong enumeration before proceed to out attack
Dirbuster found the `/cgi-bin` this special directory is **Common Gateway Interface** and allows to communicate directly with the hosting server with usually the [allow-script set to true](https://stackoverflow.com/questions/1913894/cgi-bin-directory-contents-what-else-can-be-stored-there-apart-from-the-cgi-sc) it makes sense to force enumeration on specific file format like Python,Php and bash script

![a5511e91570881bc7abf5a0baebc9479.png](img/a5511e91570881bc7abf5a0baebc9479.png)

![3f080cea1056de6e9457aa5a10cc8a2c.png](img/3f080cea1056de6e9457aa5a10cc8a2c.png)

Cool stuff! I had to take a look at the official walkthrough because I was pretty lost, it appears the machine name is a hint for [ShellShock](https://github.com/opsxcq/exploit-CVE-2014-6271)

![711bbb7e66fc8b99d5801ed0150cc6e6.png](img/711bbb7e66fc8b99d5801ed0150cc6e6.png)

Cool now we can perform RCE and spawn a user shell! I had some issue doing it manually but with searchsploit we can retrieve the module **<u>34900</u>**

![67765c4da6f98945ac05f5f7e17fc861.png](img/67765c4da6f98945ac05f5f7e17fc861.png)

Good, we have access to the user flag now!

<br><br><br>

# PRIVILEGE ESCALATION

This looks pretty simple, with `sudo -l` we can execute as root without password the perl binary!

![0ae891d03bbcc62ede7386a21df62736.png](img/0ae891d03bbcc62ede7386a21df62736.png)

We can execute perl code with the `-e` flag and spawn a high level shell!

![8fccc206e1ca5e98a155785904fdc609.png](img/8fccc206e1ca5e98a155785904fdc609.png)

We made it!
