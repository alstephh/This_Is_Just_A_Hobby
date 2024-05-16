# PORT SCAN
* **80** &#38594; HTTP (IIS 10.0)

<br><br><br>

# ENUMERATION & USER FLAG
The webpage is static and without outlinks or interaction at all

![bd49ed15256f54c0691704721035b0d8.png](img/bd49ed15256f54c0691704721035b0d8.png)

With `ffuf` I was able to discover the `dev.pov.htb` subdomain which is pretty pretty interesting and redirect to a default path `/portfolio/default.aspx`

![46429593b5a9ab14dabaf429aa522458.png](img/46429593b5a9ab14dabaf429aa522458.png)

This is the first time I saw something like that, we can download the CV of he author (shoutout to the box creator) and can be a good hints of the technology we can find inside

![64f7a96cd6f51f254d1eeeb82ea7029b.png](img/64f7a96cd6f51f254d1eeeb82ea7029b.png)

I can try to see if we can redirect the download to somehing on my local machine and use responder to get the hash. After changing the `file` parameter the hash pop up on responder interface 

![86f74d80adddc162031d7292bb250201.png](img/86f74d80adddc162031d7292bb250201.png)

![785fdd8e64c256e16d1f1ea73213a016.png](img/785fdd8e64c256e16d1f1ea73213a016.png)

There is no way to crack the hash but we have a username, cool enough! Playing a little more with this function I was able to perform some **<u>LFI</u>**!

![a0b03e962a64ceb6d93cc02d4df4cbc9.png](img/a0b03e962a64ceb6d93cc02d4df4cbc9.png)

With this method I was able to retrieve the content of `..\web.config`

![f894b4921ab0c9d9495110f1e5eace2f.png](img/f894b4921ab0c9d9495110f1e5eace2f.png)

Cool stuff is that we have decryptio, validation key and use SHA1 so maybe we can exploit the **<u>VIEWSTATE</u>** parameter (makes sense, Point-of-View....viewstate, bingo!). thanks to the [HackTricks blog](https://book.hacktricks.xyz/pentesting-web/deserialization/exploiting-__viewstate-knowing-the-secret) I was able to create the malicious viewstate to gain RCE. First of all I have targeted the `/contact.aspx` page using that as `--path=/portfolio/contact.aspx` argument of **<u>ysoserial</u>**, for the rest we just need to specify the 2 keys and alghorithm using for decoding/validation. We have all we need the command I used is a powershell command returned from `web delivery module` of metasploit

![26ecf251c5710d2fa575ff2b7b48fbb7.png](img/26ecf251c5710d2fa575ff2b7b48fbb7.png)

Just replace this **VIEWSTATE** with the one on the web request and we shoudl be fine!

![4d7cdedaf0001bbce41ee2cc3c01b9fa.png](img/4d7cdedaf0001bbce41ee2cc3c01b9fa.png)

After that the meterpeter shell will popup as the user we have previously identified but no flag with this user. It didn't take much before I identify a xml file located at `/Documents/connection.xml` containing something I am pretty familiar with (all the windows machine I have done returned usefull!)

![4e93a7f02c40ceb01fbe6255f7bdf967.png](img/4e93a7f02c40ceb01fbe6255f7bdf967.png)

This are a predefined PS Credential whih we can "import" through a default PowerShell command `import-Clixml -Path connection.xml` and after that we can execute command with the usual `Invoke-Command -ComputerName localhost -ScriptBlock {}`. When I used the powershell command to spawn another meterpeter shell it worked and we are free to grab the user flag!

![0df9473738cdfce25695bd9a4271e495.png](img/0df9473738cdfce25695bd9a4271e495.png)


<br><br><br>

# PRIVILEGE ESCALATION

We need to enable the `SeDebugPrivilege` in order to migrate our session in a high privilege process environment and root the box, so I used [RunasCs](https://github.com/antonioCoco/RunasCs/releases) to spawn a meterpeter shell where the privilege we have targeted is now enabled

![b7bb66506539bcaba50413df44547928.png](img/b7bb66506539bcaba50413df44547928.png)

![fd909592db17188a12f9869a0250c494.png](img/fd909592db17188a12f9869a0250c494.png)

With this privilege we can now migrate on every process we like, a common choice is `winlogon.exe` which always run as system! 

![f12a94e005abed378fc9dfa712c7003f.png](img/f12a94e005abed378fc9dfa712c7003f.png)




