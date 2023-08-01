# PORT SCAN

* **22** [ssh]
* **80** [http] &#8594; redirect to `http://searcher.htb`

<br><br>

# ENUMERATION & FOOTHOLD

![1.png](img/1.png)

In the bootom the website told us is made with flask and all the research are made with [Searchor 2.4.0](https://github.com/ArjunSharda/Searchor) which is vulnerable to [RCE](https://security.snyk.io/vuln/SNYK-PYTHON-SEARCHOR-3166303) thanks to a unsafe implementation of `eval()`

So sounds great starting from this and try to access our first foothold using this [PoC](https://github.com/V35HR4J/Searchor-2.4.1-RCE/blob/main/exploit.py) and try a few commands actually it works!

![2.png](img/2.png)

I used to retrieve immediately the user flag

![3.png](img/3.png)

We can try to get a revere shell (the PoC suggest to use a base64 encode in order to avoid character that can break the attack flow)

I tried with different payload but I wasn't able to get a reverse shell in this way so I changed the PoC and I found this [one](https://github.com/jonnyzar/POC-Searchor-2.4.2) where we can get a reverse shell injecting in the ` What do you want to search for` section the python code for the shell

![4.png](img/4.png)

<br><br>

# PRIVILEGE ESCALATION
We have no interesting stuff inside this machine but looking at the network information we have something interestings

![5.png](img/5.png)

I add the `gitea.searcher.htb` in the **/etc/hosts** file and visited the site which run `Gitea`

![6.png](img/6.png)

This a self-hosted version of git and unless some user we have no repository inside

![7.png](:/6952aaa03b304e29855c31c72e

![9.png](img/9.png)

caa041)

Also an `API` section

![8.png](img/8.png)

Inside the directory `/var/www/app` we have **.git** directory were we can find credentials of **Cody** user

![9.png](img/9-1.png)

And we can login inside **Gitea** 

![10.png](img/10.png)

The only repositroy we can access is the python code which run the previous webapp (and we can not some commit made by the administrator)

The same password can be used by the local user which we get teh reverse shell, so we can try and check the **sudo permission**

![11.png](img/11.png)

So we can just use python3 and run a specific script with this usage

![12.png](img/12.png)

The interesting stuff here is the `docker-inspect` command which need 2 parameters : the format and the name of the container

![13.png](img/13.png)

I check online what type of format can be used ([documentation](https://docs.docker.com/engine/reference/commandline/inspect/)) and I decide for `{{json .Config}}`. This leak some credentials

![14.png](img/14.png)

With this credential we can acces the **MySql** database and we can retrieve the hash (and the algorithm) for every user

![15.png](img/15.png)

![16.png](img/16.png)

I can't figure out the way to crack this password yet and since we had to reuse a password once I tried to use this credentials as `administrator` user on Gitea.

I was right and we can see the code of the python script on Gitea.


The logic vulnerability inolved is on the **full-checkup** option which use **./full-checkup**. Is a relative path not a absolute 

![17.png](img/17.png)

I went on **/tmp** and create the script with the same name that will add teh SUID at `/bin/bash` after that we just need to run **/bin/bash -p** and we rooted the box!

![18.png](img/18.png)

 
