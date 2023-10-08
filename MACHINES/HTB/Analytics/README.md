# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (nginx 1.18.0)

<br><br><br>

# ENUMERATION & USER FLAG

![6cd9bda70021b813971fec1f0d473061.png](img/6cd9bda70021b813971fec1f0d473061.png)

This is the webpage and I am gonna start to enumerate that to find a crak in the wall since no other protocols are available

This is a static page with nothing usefull but when I press to `login` we are redirecto to `data.analytical.htb`. I add it to the host files and check what we have here, this is a **<u>metabase</u>** login page

![fcab786eaaee6e6a6088a02ed3aa9874.png](img/fcab786eaaee6e6a6088a02ed3aa9874.png)

MetaBase is vulnerable to [CVE-2023-38646](https://nvd.nist.gov/vuln/detail/CVE-2023-38646) which is an unauthenticated RCE, cool!

I used this PoC to test the exploit and is a success

![0090a5be07179a439e4baa07557e5a04.png](img/0090a5be07179a439e4baa07557e5a04.png)

![969ae61a2c0ba0d1fdbcaf548dcadda0.png](img/969ae61a2c0ba0d1fdbcaf548dcadda0.png)

Just run a reverse shell and enjoy our exploit!

![febe084c089083b871d8f0008cdca350.png](img/febe084c089083b871d8f0008cdca350.png)

![3717a9686fa73f1472ec2bc7752552e7.png](img/3717a9686fa73f1472ec2bc7752552e7.png)

Inside we have no user flag, seems bad but I found why in a few minutes. We are inside a docker enviroment as we can see on the root directory we the placeholder file for docker containers

![ffe729dd8d45d0709af662fb886aa6ad.png](img/ffe729dd8d45d0709af662fb886aa6ad.png)

No need to escape just take a look at the enviroment variables and we have some credentials that can be used on the SSH ports

![2fcd5ff4f589b8d12f8067769cd254aa.png](img/2fcd5ff4f589b8d12f8067769cd254aa.png)

Now we are inside the machine and we can get our flag

![1524033b36a37546d1ad5ee3af202168.png](img/1524033b36a37546d1ad5ee3af202168.png)

<br><br><br>

# PRIVILEGE ESCALATION
Linpeas and further enumeration return me nothing usefull so I decide to getsome information about some kernel exploit

![fb3e1e249979374c5e2a1f6346d7e879.png](img/fb3e1e249979374c5e2a1f6346d7e879.png)

After some research (and a waste of time trying useless CVE) the right vulnerability to exploit is [CVE-2023-2640](https://github.com/luanoliveira350/GameOverlayFS), just change the id command with a bash spawn and we are good to go!

Really easy machine!
