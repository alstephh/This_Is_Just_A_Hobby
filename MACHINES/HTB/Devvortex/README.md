# PORT SCAN 
* **22** &#8594; SSH
* **80** &#8594; HTTP (NGINX 1.18)

<br><br><br>

# ENUMERATION & USER FLAG
The webpage is a static brochure for the website development company, littl enumeration and we found a subdomain `dev.devvortex.htb` where
we can access the `README.txt file`

![f3edb5249825f03c9094a37b87c3a4c1.png](img/f3edb5249825f03c9094a37b87c3a4c1.png)

And the `robots.txt`

![5eb3bfe6d5f50644dbef91a323dddc7e.png](img/5eb3bfe6d5f50644dbef91a323dddc7e.png)

WIth more enumeration thanks to `droopescan` I was able to retrieve the exact versions of Joomla which is the 4.2.6 which is vulnerable to [CVE-2023-23752](https://www.exploit-db.com/exploits/51334) but we don't need a exploit to run to get the info we need, simply this version of Joomla have a information disclousure at `/api/index.php/v1/config/apllication?public=true` 

![10167ac72d11929a3ca892403afec73e.png](img/10167ac72d11929a3ca892403afec73e.png)

We have the lewis account password, cool we can use it on the joomla login form at `/administrator` and we have access as **SuperUser**

![b35c242718a834fd927d3fe730159c13.png](img/b35c242718a834fd927d3fe730159c13.png)

Now the path is pretty straightforward, we go in configuration and select the template to target (we have just **cassiopeia** in the box), we select a php files to edit like **<u>error.php</u>** and put our reverse shell, easy and smooth we have our shell as `ww-data`.

Inside the directory of the webapp we have the `configuration.php` files that doesn't contain credentials for SSH but have some info about the DB...

![eef3fac76c2f02c1346b6da18dd893a7.png](img/eef3fac76c2f02c1346b6da18dd893a7.png)

We can get what we need easily

![1e3cc2df918d96247ea2b5e7fb4bbe8b.png](img/1e3cc2df918d96247ea2b5e7fb4bbe8b.png)

We have just `logan` home directory so I get the hash (b2crypt) and used hascat to retrieve the plaintext, we have success in a few second

![b78f8a21a28a994a989263d912da97de.png](img/b78f8a21a28a994a989263d912da97de.png)

We can access on SSH and take the user flag!

<br><br><br>

# PRIVILEGE ESCALATION
We can run `SUDO` on the following command

![231da45b542cb4fbb8d3a140047861b0.png](img/231da45b542cb4fbb8d3a140047861b0.png)

Apport use less as standard pager, this means we can spawn a shell within less as root, now I need to understand how to make it works

![f6ffc067af59353556ebf0b60d4a2489.png](img/f6ffc067af59353556ebf0b60d4a2489.png)

Cool we can choose whatever command we want and use `less`

![fd1be4ed8f975966b1ceb721897bedb6.png](img/fd1be4ed8f975966b1ceb721897bedb6.png)

Now we can view the report with `V` and we are inside less with root priv, just a simple `!/bin/bash` and we are done!

![07a873c98c561d09a306e1d146b76850.png](img/07a873c98c561d09a306e1d146b76850.png)

