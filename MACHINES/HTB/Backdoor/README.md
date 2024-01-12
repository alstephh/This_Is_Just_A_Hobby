# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.41)
* **1337** &#8594; UNKNOWN

<br><br><br>

# ENUMERATION & USER FLAG
Port 80 is running `Wordpress 5.8.1`, good sign

![766fd55db40d4acc0ddc235c41f9f459.png](img/766fd55db40d4acc0ddc235c41f9f459.png)

![20b760c5e5477652e2765a6a417eec5e.png](img/20b760c5e5477652e2765a6a417eec5e.png)

Enumeration found nothing but a plugin called **<u>Ebook-Download</u>** inside `/wp-contents/plugin`. ExploitDB have an [exploit](https://www.exploit-db.com/exploits/39575) just for this specific version (**1.1** that can be checked on the readme.txt inside the plugin directory). As you can see we are able to download file on the target

![7ab18ee93c80808aa9f646d6524a925f.png](img/7ab18ee93c80808aa9f646d6524a925f.png)

Now we can use the `/proc` filesystem directory to get some info about what is running on the target side. I have already tried to do some cache poisoning on the `apache log file` and `/proc/self/environ` but apparently we have no permission to access that files. Look at this example

![b8f9a9b4e9a27f3862bc912640bf8917.png](img/b8f9a9b4e9a27f3862bc912640bf8917.png)

At first I have just echoed the PID of the current shell, than we are spawning a shell and we are visiting the PID directory inside proc, the cmdline file is interesting. It contains what actually the process is running which is the bash!
The idea now is to bruteforce the PID on that specific path and maybe find something similar that can help us to get inside.

To accomplish this I used the **<u>Burp intruder</u>** and `842` looks pretty interesting...

![81a648d3bd8c558e6b8d1ce8493c633f.png](img/81a648d3bd8c558e6b8d1ce8493c633f.png)

Finally I know what is running on port <u>1337</u> and [this](https://book.hacktricks.xyz/network-services-pentesting/pentesting-remote-gdbserver#exploitation) appears to be the way to get a reverse shell now

First we create a **<u>msfvenom</u>** payload with the `PrependFork=true` (essential for the attack) as `elf` file and we extend a gdeb session with the target GDB port as debugger

![68a644bc1a0396ba6da59092f5b8a2a8.png](img/68a644bc1a0396ba6da59092f5b8a2a8.png)

now we just nee to put the local file remotely and than point to it as executable

![dd13ae14d761edf15b4849a09e430f6a.png](img/dd13ae14d761edf15b4849a09e430f6a.png)

As you can se a child process has been created and is our reverse shell!

![0bfb439b7681403e0ceb87559a43360b.png](img/0bfb439b7681403e0ceb87559a43360b.png)

Finally we can grab the suer flag now!

<br><br><br>

# PRIVILEGE ESCALATION
No `sudo` without password and no SUID so is time to enumerate, usually `/var/www/html/wp-config.php` sounds cool when wordpress is running

![6204791622b65551033058f1c4fc1c01.png](img/6204791622b65551033058f1c4fc1c01.png)

Cool mysql credentials that is pretty cool and inside I found a password for the admin user 

![b7dbc2bab830e7593d22d1b2541bced4.png](img/b7dbc2bab830e7593d22d1b2541bced4.png)

phpass take quite a good amount of time to crack so meanwhile hashcat is running we can go through and check something else. We have an interesting process run by root involving `screen`

![c4286da2b6564b1e4b74252065965fe6.png](img/c4286da2b6564b1e4b74252065965fe6.png)

Interesting stuff, we need to do a sort of hijacking (similar to TMUX) but I don't have access to others screen sessions and [here I found a cool answer](https://unix.stackexchange.com/questions/219851/how-to-access-screen-created-by-other-users) that maybe can help me to get over it

![6c65592a1543010a2a17b5b5ded5fe07.png](img/6c65592a1543010a2a17b5b5ded5fe07.png)

I don't know how the file is configuredinside root but we know that run in deteached mode (`-dmS`) and we can use the `-x root/root` flag to attach on that session and luckly it worked. We are impersonating admn right now! (yeah the admin hash was a hole in the water ehehe)

![e5e9139fba13e59704d24ee422f6d267.png](img/e5e9139fba13e59704d24ee422f6d267.png)
