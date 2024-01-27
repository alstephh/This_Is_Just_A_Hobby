# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; APACHE (2.4.41)
* **8089** &#8594; SPLUNK (HTTPD)


<br><br><br>

# USER & ROOT FLAG

We have a static website acting as a health solution (makes sense with the machine name lool) on port 80, pretty boring nothin in here obviously Splunk service might help us more!

![b8b16f6e0cb14203e00374a69d6c6f83.png](img/b8b16f6e0cb14203e00374a69d6c6f83.png)

In splunk we have 4 links 

![db3c13a0f74450f4a5be16fdb9675b42.png](img/db3c13a0f74450f4a5be16fdb9675b42.png)

The firt and last one are empty while the 2 in the middle needs credential through HTML Basic Authentication so creds are needed.
To find something usefull was pretty tricky, there is an email on the HTTP page named `info@doctors.htb` and not `info@doctor.htb` so I added `doctors.htb` in my host file and we have something new, a website made with Flask!

![af2e6531b020711af6d9b6ab766e33f4.png](img/af2e6531b020711af6d9b6ab766e33f4.png)

I created a new account (but just after I checked some basic SQL injection) and there is one of my favorite web vulnerability that can be used, **<u>SSTI</u>**.

While I was working on SSTI I noted that every new message post created will be saved in `/post/[ID]` and my first post was saved with ID equals to 2. So I tried to access directly the post with ID 1 and we have discovered a username named admin exist!

![2318686797b2643f372dd7cedc286991.png](img/2318686797b2643f372dd7cedc286991.png)

Other than this no way to make the SSTI work, this is because I run blindly directo to attack forgetting to enumerate this new VHOST and after fixing my stupidity something pop up reallty quickly

![8fcc995569818aed3de5ef105a5691e0.png](img/8fcc995569818aed3de5ef105a5691e0.png)

The `/archive` is somthing I wasn't able to see before and I was pretty dissapointing on wat I found, is just an empty page. What the hell! I look through the page source and there is the title of my last post I created

![9a4edc5096cdb30d9ebdfbbaec1ac4e5.png](img/9a4edc5096cdb30d9ebdfbbaec1ac4e5.png)

![4ddd714fbb5cae168a98fd0ea62ca9e3.png](img/4ddd714fbb5cae168a98fd0ea62ca9e3.png)

So maybe now I can create a new post and instead of checking the contents directly from there (where I have already checked that no SSTI is working) I can use this `/archive` endpoint. I used the title `${{7*7}}` but the archive page tell us something different!

![30085f86015dee7513d4b6e4619caf30.png](img/30085f86015dee7513d4b6e4619caf30.png)

Hell yeah now we can make this SSTI work and with [this](https://book.hacktricks.xyz/pentesting-web/ssti-server-side-template-injection#python) we can get a reverse shell!

![3c7a7577f1564e26bfd5e693e26c0936.png](img/3c7a7577f1564e26bfd5e693e26c0936.png)

Inside the system I found a database file containing a bcrypt hashfor admin locate at `/home/web/blog/flaskblog/site.db`

![748d4cc69417da69e328ac3888822cd1.png](img/748d4cc69417da69e328ac3888822cd1.png)

This type of hash is really slow to crack so probably this is not the way to get the user flag. We get the shell as `web` user and we are inside the **<u>adm</u>** group so we access `/var/log`, this sounds better than wait a huge amount of time to crack a hard hash!
I don't want to spend more than a day to look through all he files stored inside the log so with the power of `grep` I was able to find a possible password used during a reset

![76401f251632d8926a5787c8491bb4e9.png](img/76401f251632d8926a5787c8491bb4e9.png)

I tried to use it on SSH using the otheruser presnet in the home directory called `shaun` but no way to make it work. Makes sense we didn't even touch splunk, maybe here the credentials can work...and did it smoothly!

![27da5b642f4ef4322213b33c1ea595fe.png](img/27da5b642f4ef4322213b33c1ea595fe.png)

While we inside the authenticated section of splunk is just a collection of empty records so I decided to search for some wel known vulnerability that maybe can help us to get a user shell!
With some research I was able to find this cool repository about [SplunkWhisperer2](https://github.com/cnotin/SplunkWhisperer2/blob/master/PySplunkWhisperer2/PySplunkWhisperer2_remote.py) and with this I was able to perform some RCE, I tried at first just pingin my machine

![63ba5feec5540492fa135c72959be93e.png](img/63ba5feec5540492fa135c72959be93e.png)

![9ee111e7bcc56762b361eaa6ad90b16b.png](img/9ee111e7bcc56762b361eaa6ad90b16b.png)

Instad of using just a normal ping I used a simpla bash reverse shell and surprise surprise we get access but as root!

![677c6661c15abd4b5a28b12c91d6dd32.png](img/677c6661c15abd4b5a28b12c91d6dd32.png)
