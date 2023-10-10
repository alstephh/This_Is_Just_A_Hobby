# PORT SCAN
* **22** &#8594; SHH
* **80** &#8594; HTTP (Werkzeug/2.1.2 - Python3)

<br><br><br>

# ENUMERATION & USER FLAG

![6e84ffd1a204630a8a134e29b82c603f.png](img/6e84ffd1a204630a8a134e29b82c603f.png)

The webpage run Werkzeug and, like flask, have a debug console but this time is blocked with a pin on **/console**

![41ccf8a21965b7d211e135092701d077.png](img/41ccf8a21965b7d211e135092701d077.png)

Usually to get access to the debug console protected by a PIN we need a LFI and read the init python file where the code is stored

We have an uploa feature at **/upcloud**

![b6bc2f566055ac9b6b4e8e697859b0b6.png](img/b6bc2f566055ac9b6b4e8e697859b0b6.png)

After the upload the file is stored in the **/upload** directory

![96be4bf824575be0841d496515ebab5a.png](img/96be4bf824575be0841d496515ebab5a.png)

On the "`Try Upload Section`" we can upload the source code and inside we have a `.git` folder (bad bad bad)

We have 2 branch <u>public</u> and <u>dev</u>, obviously switch into the **dev branch** and check with some dif the logs and we have some credentials

![0bda3c0b5069adaf6b27ab3a6f636556.png](img/0bda3c0b5069adaf6b27ab3a6f636556.png)

They don't work with ssh but good to know it in casewe have login form of some sort

Returning back at the Upload feature, I found a way to read file 

![dc7a777c06109cb59771f0f981c42adc.png](img/dc7a777c06109cb59771f0f981c42adc.png)

And with some perseverance we have the path for the **init file of wekzeug**

![c27a2240271f7cb3d3a5a84299565c3c.png](img/c27a2240271f7cb3d3a5a84299565c3c.png)

What we need now to reverse the pin is : 

1) `/proc/self/cgroup`
2) `/proc/sys/kernel/random/boot_id`
3) `/sys/class/net/eth0/address` &#8594; convert it in decimal (chec the devices using `/proc/net/arp`)

With this variable set we can use some script that can be found online (for instance [here](https://www.daehee.com/werkzeug-console-pin-exploit/)), cool we have access to the debug console 

![8751fab24fea5fc01f36e28c69758c11.png](img/8751fab24fea5fc01f36e28c69758c11.png)

![9f5e0676cc523dfa4d3429c9c679294e.png](img/9f5e0676cc523dfa4d3429c9c679294e.png)

Probably the webapp is running inside docker wince we are root user, let's get the shell

![0c1cb3f9041dbcf6446c8f3472556b9d.png](img/0c1cb3f9041dbcf6446c8f3472556b9d.png)

We are inside a docker environment as previously assumed (`.deckerenv` placeholder file)

Usin IP Route we have the gateway

![64dd7b02380fa30ea73298ac446895c7.png](img/64dd7b02380fa30ea73298ac446895c7.png)

The port 3000 (of `172.17.0.1`, the victim outside the container) can be accessed so I will sue chisel to port forwarding and check what is running. An instance of **<u>Gitea</u>** is running nd we can use the credentials find before to get access. We have a rsa key pair that we can use to gain access trhough SSH

![244af3740c160fb6422113a2e4886962.png](img/244af3740c160fb6422113a2e4886962.png)

![19db7dff840dac0e528b09a102571f35.png](img/19db7dff840dac0e528b09a102571f35.png)

Gottch'a

<br><br><br>

# PRIVILEGE ESCALATION
There is a cron job running calling this file 

![91795367bbfffc770749b5d07e2bea6b.png](img/91795367bbfffc770749b5d07e2bea6b.png)

This script just makes a backup of the `dev01` user home directory

![1180c017fc28331a37211d0d72c51795.png](img/1180c017fc28331a37211d0d72c51795.png)

We can use [git hooks](https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks) to abuse it, specifically I have changed the `pre-commit.sample` to this 

![1c3bbc201d07ebb2f45078590680a813.png](img/1c3bbc201d07ebb2f45078590680a813.png)

Than just renamed `pre-commit` and before the commit the script will be executedas root. Let's wait a few seconds and we have access to our flag (we could do same thing with the private ssh key to gin persistence)

![da418c316462a15c4c1d28a6c3c6fb36.png](img/da418c316462a15c4c1d28a6c3c6fb36.png)
