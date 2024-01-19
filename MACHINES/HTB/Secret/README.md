# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (NGINX 1.18)
* **3000** &#8594; NODE JS (EXPRESS MIDDLEWARE)

<br><br><br>

# ENUMERATION & USER FLAG 
The webpage is quite self-explenatory, to get foothold we have to play with API

![cf686f887255725e44e3b5e335680c0a.png](img/cf686f887255725e44e3b5e335680c0a.png)

We can download the files that are used by the server side and the server itself provide the documentation where we can find some technical details about this <u>API Authentication System</u> : **JWT tokens** and **MongoDB**

I am pretty bad on whatever is involved with Web Pentest so I will start from the files the sites allows us to download, what's interesting is the `.git` folder where I found some interesting logs

![6db2627dbc0ab99d7da34874f7dde3e5.png](img/6db2627dbc0ab99d7da34874f7dde3e5.png)

Cool stuff, actually we already have the `.env` but the token has been censurated. We can use `git show [GIT_COMMIT]` to view the changes and the moral of the story is to never ever give you **.git** away like is nothin'

![9eaba2fb63b2b9a4097615e214aa4ad0.png](img/9eaba2fb63b2b9a4097615e214aa4ad0.png)

Now (with a lot of patience, LOL) I used the API to register a new user 

![8fd9a7be8dcfcca20a5c580f95b04220.png](img/8fd9a7be8dcfcca20a5c580f95b04220.png)

Now I was pretty lost but luckly we can see what the webserver is running and we have collected a good amount of information but is actually not enough for now. The `private.js` file contains some interesting stuff starting from a potential **<u>OS injection</u>** through the `/logs` endpoint 

![dd428606d28802cb6b8cf2cc5ff96f9f.png](img/dd428606d28802cb6b8cf2cc5ff96f9f.png)

Also highlight the fats that only the user `theadmin` can access this feature and here's the issue we have to solve, obviously we can't create this specific user because it alreasy exists

![d0dc3d602c70efac4444222e67ddbbf3.png](img/d0dc3d602c70efac4444222e67ddbbf3.png)

Pretty unlucky but we have the SECRET TOKEN which is used to create valid JWT and in short our job here is to replicate the JWT creation by ourselves, cool! We can use the JWT we get when we have create (and login as) the new user on [jwt.io](https://jwt.io/) changing the username to **theadmin** and using the SECRET TOKEN in the signature field

![21e3db3fcb199a6976ce53e507eb6acc.png](img/21e3db3fcb199a6976ce53e507eb6acc.png)

I can check this using the `/api/priv` endpoint and is valid!

![64a00e1decd40e7d4c2df2177028879b.png](img/64a00e1decd40e7d4c2df2177028879b.png)

We made it now we can move on the `/logs` endpoint and with the code in our hands is a child game

![ce65f6b67f1b162e4e168390a5c888b9.png](img/ce65f6b67f1b162e4e168390a5c888b9.png)

Hell yeah! OS injection work well now let's try to get a reverse shell (my way was to create it locally, use wget on the remote machine to upload it and finally execute the reverse shell from the OS injection) after that the flag is waiting for us

![5d036fbf1fb8cd1b3752dcb5dcb2910a.png](img/5d036fbf1fb8cd1b3752dcb5dcb2910a.png)

<br><br><br>

# PRIVILEGE ESCALATION
We have an interesting file with SUID on `/opt/count` so I tried to run it and the behaviour looks like a summarty of a specifc directory/file

![52c3476f3d2a9cf61ed2d985a00fe7c3.png](img/52c3476f3d2a9cf61ed2d985a00fe7c3.png)

Obviously we can abuse the SUID to set directory or file unaccessible as standard user but there is not much output just some scan summary. We can read the code of the binary located on `code.c` and we have the **PR_SET_DUMPABLE** set to true!

![efb4345713d7d6d4ba13e772d52283cf.png](img/efb4345713d7d6d4ba13e772d52283cf.png)

When a process crash, linux make a core dump and save it on `/var/crash` and usually they report what thet process "were doing" before crash, maybe we can retrieve the flag in this way. We need to crash the process we launch manually and the **<u>SIGSEGV</u>** low level signal can become pretty handy for this. I spawned another reverse shell, execute the process and on the other shell kille with `kill -SIGSEGV [PID]` and we have made the process crash! 

![eaf23afdad0f33b1fe8b18df2cd0ea81.png](img/eaf23afdad0f33b1fe8b18df2cd0ea81.png)

After that in `/var/crash` the core dump file will spawn and we have enough permissions to read and move it wherever we like!
On this `.crash` file we can use `apport-unpack [.crash_file] [destination]` which read the crash file and create a directory as report with different file but the one we need us is `CoreDump`. If we use strings on it we can read the flag!

![87368d1f5901b4960ef29e691f79963f.png](img/87368d1f5901b4960ef29e691f79963f.png)

<br><br>

> Another workaround was through <u>**File Descriptors**</u>, if we look carefully the C code we (should) note that every descriptor opened is not closed and thanks to PR_SET_DUMPABLE are not flushed. this means that we can open the file `/root/root.txt`, put the process in background and visit /proc/[PID]/fd and choose the ID which is linked with the file we want to read. In this manner we bypass the restriction on `/root`!
