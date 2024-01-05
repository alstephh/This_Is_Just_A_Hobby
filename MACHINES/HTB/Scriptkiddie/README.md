# PORT SCAN
* **22** &#8594; SSH
* **5000** &#8594; HTTP (Werkzeug 0.16.1)


<br><br><br>

# ENUMERATION & USER FLAG
Finally a box made for me LOL, obviously we have to look through port 5000 which is a really website for kiddies

![1295b5a2a45f769791e163cd9cba2643.png](img/1295b5a2a45f769791e163cd9cba2643.png)

The nmap scan can reach my local machine

![43ba702ebba55a51aa13ac6a0511a1f3.png](img/43ba702ebba55a51aa13ac6a0511a1f3.png)

By the way they are simple forms which are (probably) used as parameter so I tried some OS injection but this kid is not too much noob as expected

![9bcb82b54f50093f939c09e81583cbfb.png](img/9bcb82b54f50093f939c09e81583cbfb.png)

Interesting stuff OS injection are not possible so we need to focus on the only webtool which allows us to upload a file, `msfvenom`. So I searched for some vulnerability on msfvenom which include a template file to be executed, the response was [CVE-2020-7384](https://nvd.nist.gov/vuln/detail/CVE-2020-7384). Thanks to a crafted APK we can execute remote command if msfvenom use it as template and this [PoC](https://github.com/nikhil1232/CVE-2020-7384) is well done for this purpose

![b5757e9da197a7c9a7d161540321ebe3.png](img/b5757e9da197a7c9a7d161540321ebe3.png)

Now we just need to upload and set whatever ip you want after a few seconds the reverse shell will pop up! User flag is in the home directory of **kid** user

![4ad475faeabef421eb02d537611a45ff.png](img/4ad475faeabef421eb02d537611a45ff.png)

<br><br><br>

# PRIVILEGE ESCALATION
We have a `logs` directory in the home folder which name of file inside is pretty interesting

![3ee54b079521bc5da572aa5ed79e652c.png](img/3ee54b079521bc5da572aa5ed79e652c.png)

"hackers" ah? maybe the error we had before can now be usefull! The log file is filled with the soruce ip when the error `Stop hacking me - we'll hack you back` that occur when a non alphanumeric character is present in one of the parameter (the content of `app.py` is pretty usefull here)

![dc8782272e978bbb2571d3ee9f966438.png](img/dc8782272e978bbb2571d3ee9f966438.png)

We have another folde in the home directory called `pwn` which inside have a bash script that use the ip inside the log and scan it with nmap and then delete the records inside it (this is why when we try to read the file is empty). I also check with TCPDUMP and is actually scanning so there will be a cronjob or watchdog that trigger the script

![c8f8ced85b372c8c230ab7766b55d183.png](img/c8f8ced85b372c8c230ab7766b55d183.png)

We can write inside the file `hackers` so I injected this to perform OS injection

![85cec463a010b2e5c6ccd05b7d3c5ddb.png](img/85cec463a010b2e5c6ccd05b7d3c5ddb.png)

It was not working because thescript will cut the first 3 whitespace (`-f3-`) and treat them as single strings so we can use something little different

![8f959b65c182a1f366bb1ef8459f4501.png](img/8f959b65c182a1f366bb1ef8459f4501.png)

![0987e66b614893943b9c8f42584e232c.png](img/0987e66b614893943b9c8f42584e232c.png)

Now we can try to pop another shell but this time as **<u>pwn</u>**

![dfa507860d2af64ad1d2f3c5a310cbfe.png](img/dfa507860d2af64ad1d2f3c5a310cbfe.png)

Now that we are in as **pwn** we can execute as root the metasploit framework!

![6d72851f4bf085b5870ccaa18ced1f03.png](img/6d72851f4bf085b5870ccaa18ced1f03.png)

this is a pretty good thing, through `msfconsole` we can exevute OS command and with this permission we will do it as root. Easy!

![3c3d8dac3422b847af7e22608793bf6c.png](img/3c3d8dac3422b847af7e22608793bf6c.png)
