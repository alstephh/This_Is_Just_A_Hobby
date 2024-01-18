# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.29)


<br><br><br>

# ENUMERATION & USER FLAG
Oh no someone hacked this website :(

![8ed5b871ee77583d822986ad638f887d.png](img/8ed5b871ee77583d822986ad638f887d.png)

I started to enumerate the web service like usual and while my tool were running I got a pretty good hint in the source code

![4fdf2df43a572098dae3baf23469f609.png](img/4fdf2df43a572098dae3baf23469f609.png)

This moved me and use GoBuster with ad-hoc wordlist provided by SecList with a focus on backdoor

![c18143cbaa606197fabc55fa42d103a3.png](img/c18143cbaa606197fabc55fa42d103a3.png)

Gottch'a. the `/smevk.php` have a login form and I try with the weak `admin:admin`. Surprise surprise it worked and we can play with this cool backdoor!

![8c802ce91b0be575651609785ce08bd4.png](img/8c802ce91b0be575651609785ce08bd4.png)

Now I use it to get a reverse shell as `webadmin` and in his home folder I found a pretty interesting text file

![c4d212daaeee40fdc29288293fd977b0.png](img/c4d212daaeee40fdc29288293fd977b0.png)

I spent some time going through the filesystem but the location of this file could be retrieved using `sudo -l`

![f4c65b1eec9c27ce1b76d8bc2da632f2.png](img/f4c65b1eec9c27ce1b76d8bc2da632f2.png)

**<u>luvit</u>** is CLI tool that allows to run LUA script and in this case we can do it as `sysadmin` so I create a LUA script containing just the line `os.execute([[bash]])`, the magic happen when we combine all those elements togheter and we can spawn as `sysadmin`. The user flag is our!

![ba0d1668393f6384f4c1b8eaf9ec3df7.png](img/ba0d1668393f6384f4c1b8eaf9ec3df7.png)



<br><br><br>

# PRIVILEGE ESCALATION

I took a look at the running process and there is a specific one that run every 30 seconds....

![e087e9bc92e94ecafa38f49badfd6dde.png](img/e087e9bc92e94ecafa38f49badfd6dde.png)

Interesting stuff! With some search online I was able to understand the purpose of the motd file, responsible to show the banner message when a user SSH inside the machine. This makes sense because the banner in this machine is not standard

![a4c82ce6ff9e2d875af26770fbd68aa1.png](img/a4c82ce6ff9e2d875af26770fbd68aa1.png)

I went inside `/etc/update-motd.d` and the one responsible for the banner is `00-header` and I changed the content as `cat /root/root.txt` (after all is just a bash script that run as root!) and when I re-login inside the machine the flag would be in front of my face

![cbf2ce2bdbd19a8355bcbd5f42ee18b9.png](img/cbf2ce2bdbd19a8355bcbd5f42ee18b9.png)

Obviously for get persistence we can abuse this file like a simple script that run as root so the possibilities are infinite
