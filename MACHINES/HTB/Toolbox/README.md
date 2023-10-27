# PORT SCAN
* **21** &#8594; FTP (FileZilla)
* **22** &#8594; SSH
* **135** &#8594; Microsoft RPC
* **139** &#8594; NetBios
* **443** &#8594; HTPS (Apache / Debian)
* **445** &#8594; SMB (signing enabled but not required)
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
This box is a windows machine but the port scan report Debian distribution, plus the FTP port accept anonymous login and inside we have `docker-toolbox.exe`

![f5e5ac8860ab307ff620de87eec9a4c9.png](img/f5e5ac8860ab307ff620de87eec9a4c9.png)

Docker-Toolbox is just the installer and automatic setup for docker for older version of Mac and Windows (in this case appears to be **Windows 7**)

![3a88f3d5d26da1faf003942356eee99f.png](img/3a88f3d5d26da1faf003942356eee99f.png)

<br>

About the HTTPS page, it act like a maritime logistics company (make sense with docker machine theme)

![0f03aaea1d9c6e1e2b58e8eece229cab.png](img/0f03aaea1d9c6e1e2b58e8eece229cab.png)

The home page is just the traditional static page with no user interaction but if I look closer at the NMAP scan we have a clue of a subdomain

![3e0a1231b598483d9c37d149001483ad.png](img/3e0a1231b598483d9c37d149001483ad.png)

This can also be discovered manually looking t the certificate 

![a42783681dc627d7de3f9289352855cd.png](img/a42783681dc627d7de3f9289352855cd.png)

We have a simple login form, I tried with the traditional `admin` as username and `' OR 1=1;--` as password.

I was not able to bypass it but we have some cool error that we can see in the background of the template

![833e93119b583204355d57182f335092.png](img/833e93119b583204355d57182f335092.png)

So I set the sql string on username and we are in

![2442f9512bd27a3692b0eeb55b483048.png](img/2442f9512bd27a3692b0eeb55b483048.png)

Actually inside there is nothing usefull but we can use the `--os-shell` on SQL map to get command execution 

![80e18ab03b1f3a8449bf66d187b0a1d8.png](img/80e18ab03b1f3a8449bf66d187b0a1d8.png)

Cool stuff now we can inject a reverse shell 

![ce6539f4bd4316ffa83808654e147615.png](img/ce6539f4bd4316ffa83808654e147615.png)


In `/var/lib/postgresql` we have the user flag

![c0fd80958230592d4d43f478a5c70412.png](img/c0fd80958230592d4d43f478a5c70412.png)

<br><br><br>

# PRIVILEGE ESCALATION

Inside `/` we have the `.dockerenv` file and the home directory have a user called **<u>tony</u>** but we already have the user flag.

Now we should run for root privileges, I was pretty lost than with some research I have discovered that [boot2docker](https://hub.docker.com/r/boot2docker/boot2docker/) usually run togheter with the docker image and the default credentials are 

![74ca37b8620ded2eaf8c25b3ffd09c89.png](img/74ca37b8620ded2eaf8c25b3ffd09c89.png)

Docker was designed to run on Linux system but toolbox is for Windows and Mac, **<u>boot2docker</u>** is a (light) Linux distribution (officially deprecated and unmaintained) that is created in order to run docker. 

![5e51b49b6e9dc294d0da86791b6567e4.png](img/5e51b49b6e9dc294d0da86791b6567e4.png)

So both the container and boot2docker deamon run inside virtualbox so somehow have to communicate, what's bad is not have binaries like `ping` or `pingsweep` so we have to guess the IP address

![d7a3f823ec7b1c128708968573cae5e4.png](img/d7a3f823ec7b1c128708968573cae5e4.png)

In addition to the **loopback** interface we have `eth0` with IP `172.17.0.2`

![17755899ae049a9c097f737efe578997.png](img/17755899ae049a9c097f737efe578997.png)

We have to guess here, knowing that boot2docker is created BEFORE the docker container make sense tha the boot2docker IP is `172.17.0.1`

![f939d05ef854848d771d178b7d2e0867.png](img/f939d05ef854848d771d178b7d2e0867.png)

We are inside cool and WTF the `sudo -l` output is overpowered, probably becuase inside the boot2docker doesn't require as password since is only used as support for the containers

![ae04ba807ac998d44e333c60ad8d3d95.png](img/ae04ba807ac998d44e333c60ad8d3d95.png)

There is no flag here but in `/` we have a unexpected directory called **c**

![d6d328d477b894c4bc9c2e679cc16b2f.png](img/d6d328d477b894c4bc9c2e679cc16b2f.png)

This is the `C` folder of the Windows host and in the Administrator's Desktop directory we got the flag

![1ff6fed180797c59f818bddb2cdc1d19.png](img/1ff6fed180797c59f818bddb2cdc1d19.png)

(There is also the .ssh folder with the private keys of administrator for persistence)
