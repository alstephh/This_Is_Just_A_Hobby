# PORT SCAN
* **21** &#8594; FTP
* **23** &#8594; TELNET
* **80** &#8594; HTTP


<br><br><br>

# ENUMERATION & USER FLAG
The port 80 is prettty naked we just have an image with **<u>LON-MC6</u>** as title

![83b6d1c814ab2da14331a1d0825409d3.png](img/83b6d1c814ab2da14331a1d0825409d3.png)

I searched the image with google but nothing related just somne wirteup ofthis machine. I run another instance of nmap but trying to enumerate telnet and apparently we are dealing with a windows XP OS

![87e8310caea71b1046dec07fba178d40.png](img/87e8310caea71b1046dec07fba178d40.png)

Telnet require <u>username</u> and <u>password</u> so I can't go further is a good idea to make a try on FTP that accept anonymous login.

We have downloaded 2 directories `Engineer` and `Backup`. The first one contain a zip file which need to be decrypted with a password 

![ebfe5a4a572e7c1aa11c7eb1576536dd.png](img/ebfe5a4a572e7c1aa11c7eb1576536dd.png)

In the latter we have a `ackup.dbm` file, I run strings and I have found something that look like a password

![a0d3eaaafd57e1a0d140851e165bf095.png](img/a0d3eaaafd57e1a0d140851e165bf095.png)

Cool, I used on the zipped file and it worked!
Inside the archive there was the `Access Control.pst`, the `pst` stand for **<u>Personal Storage Table</u>** and is Microsoft proprietary format which is used for Calendars, Messages and other stuff used by the Microsoft Suite

`lspst` is used to list the data inside this file format

![cf22470c24395d3fe2c4cf7fa805ed20.png](img/cf22470c24395d3fe2c4cf7fa805ed20.png)

That's cool! now we can use `readpst` to translate the pst in `mbox` file format which is human-readable

![e13b6a8e82b1a3e1a9ab491dfb2d36c7.png](img/e13b6a8e82b1a3e1a9ab491dfb2d36c7.png)

We have credentials now and the flag is ready for us

![75e60437e2650125848db45e60c240ae.png](img/75e60437e2650125848db45e60c240ae.png)


<br><br><br>

# PRIVILEGE ESCALATION
Inside the security user folder we hae an "hidden" directory called `.yawcam` which is a software called **<u>Yet Another Webcam Software</u>**

![3bc148161157ce1413ec58e2fde98a09.png](img/3bc148161157ce1413ec58e2fde98a09.png)

I enumerate as much as I can but actually nothing usefull but fortunately we have apretty outdate Microsoft Server machine 

![526637fbe14b52984a3f5b11c5df3dec.png](img/526637fbe14b52984a3f5b11c5df3dec.png)

WIth some simple CMD command I am able to discover that Admnistrator have an active sessions and his credentials are stored inside the box

![b8168b94117c8c347056d0ea139ab4ca.png](img/b8168b94117c8c347056d0ea139ab4ca.png)

We can do different things like using mimikatz to retriev the credentials or just us `runas` to retrieve the root flag. I havejust created a powershell reverse shell saved locally, run a python http server and use the following line to retrieve and execute it

![fe9697ac30e88418f4198a77c4e7c195.png](img/fe9697ac30e88418f4198a77c4e7c195.png)

Than just wait a few seconds (telnet is really not performant as ssh) and the Administrator shell will pop up!

![fc7bcaa2547652800e7d63b3f91b1eb5.png](img/fc7bcaa2547652800e7d63b3f91b1eb5.png)

> The image on the HTTP server was bugging me what was the point of that? Checking the machine information and other writeup I quote "[...]highlights how machines associated with the physical security of an environment may not themselves be secure[...]"
