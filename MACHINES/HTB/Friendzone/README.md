# PORT SCAN
* **21** &#8594; FTP
* **22**  &#8594; SSH
* **53**  &#8594; DNS (BIND 9.11.3)
* **80**  &#8594; HTTP (APACHE 2.4.29)
* **139 / 445**  &#8594; SAMBA
* **443**  &#8594; HTTPS

<br><br><br>

# ENUMERATION & USER FLAG
The  HTTP page is pretty simple and static

![885bb6a4b6a025a68741f8f60fe2c171.png](img/885bb6a4b6a025a68741f8f60fe2c171.png)

Pretty strange services right here, not want to be involved to be honest lol. The HTTPS page returned a not found error but checking the certificate we have something a possible domain that end with `.red`

![90c06f2b5f782e87d73bf61665f93f29.png](img/90c06f2b5f782e87d73bf61665f93f29.png)

After adding the record to the host file we can access the HTTPS website!

![c42d55573ad773f83846413a5bfdbe92.png](img/c42d55573ad773f83846413a5bfdbe92.png)

Now I fired up some web enumeration and while they are cooking I checked the other protocols, FTP is closed to anonymous access but SAMBA have something for people without credentials like me!

![49c053c37eb82f57b4645fd915beaad3.png](img/49c053c37eb82f57b4645fd915beaad3.png)

The only file that I have found was `/general/creds.txt`, the other directories where empty or need credentials

![f97187fdaa7d6268e9883ad10aeb07ec.png](img/f97187fdaa7d6268e9883ad10aeb07ec.png)

I tought the "admin thing" was the path portal that can be found on `/admin` discovered with GoBuster but was an empty directory with no file inside it, pretty bad but maybe we can use this credentials something else. Bruteforcing subdomains return nothin so I played a little with the DNS service and requesting AXFR records we have a new VHOSTs

![eb7fad180829c898061dfabb28074a31.png](img/eb7fad180829c898061dfabb28074a31.png)

We have a good amount of domains to add at the host file, on **administrator1** subdomain we have a panel where we can try to use our credentials

![fb4183adaf99ee0eedfbfc84649c90b9.png](img/fb4183adaf99ee0eedfbfc84649c90b9.png)

After the login we are redirected on a dashboard with a simple PHP code that allows to view images specifying them on a GET variable, this sounds pretty goof

![d70b19b0c4d5330aa8aafa7f6dd21522.png](img/d70b19b0c4d5330aa8aafa7f6dd21522.png)

The image requested would be used inside a HTML img tag

![7fa5ae849497ef85b083f2ca0a2d127f.png](img/7fa5ae849497ef85b083f2ca0a2d127f.png)

Pretty interesting stuff, the static path where the img is extracted is on `/images` and through the `uploads.friendzone.red` domain we are able to upload image files 

![1978c9fe265a9727135746601f207af6.png](img/1978c9fe265a9727135746601f207af6.png)

Somehow the uploaded files are not stored in `/images`, pretty bad. If we look closely to the example GET parameters we have there is also the **pagename** variable that is set to **timestamp** as default. Converting the timestamp returned the current time, pretty strange. Given the name of the GET variable I visited `/timestamp.php` which is simply the same output we have on the dashboard page, this mean a LFI is possible!

![0280af1c5aa9afa8bd21ba3b2d7df911.png](img/0280af1c5aa9afa8bd21ba3b2d7df911.png)

To leverage this we can return back to SMB where we can access 2 folders : 1 where we found the credentials file and `Development` which is empty but allowed an anonymous user to upload some files, BINGO! On the first screenshoot of SAMBA you can see that the location of folder seems to be `/etc/[folder_name]`. Is everything we need to get a reverse shell, I created the malicious PHP file, uploaded on Development SAMBA directory and than access it through the LFI assigning `/etc/Development/re.php` to the **pagename** after that the reverse shell will popup!

![53122835bddfa42b8ff114fd16a3d2f4.png](img/53122835bddfa42b8ff114fd16a3d2f4.png)

Now we are free to grab the user flag!


<br><br><br>

# PRIVILEGE ESCALATION
Find some credentials for SSH was not an hard task, inisde `/var/www/mysql_data.conf` we have a pair of credentials of a system user, always check web related directories ehehe

![2cc6e68943a4c6bb7aa0bc4c4005f27d.png](img/2cc6e68943a4c6bb7aa0bc4c4005f27d.png)

Enumeration on the system revealed not many information as I wanted so is worth to run **<u>pspy</u>** and see if some cron job is running behind the curtains, this apper to be the case

![2a4ba21c09678cce2a26be5195845081.png](img/2a4ba21c09678cce2a26be5195845081.png)

![55bfddeb987e5097a187cc72ebb36f83.png](img/55bfddeb987e5097a187cc72ebb36f83.png)

There is not much here and we have no permission to modify it but we have the `import os` line that can turn usefull. Runnin `/usr/bin/python -v` I was able to retrieve the path of the OS library

![e654b55ed4040a7842510ed3c9690b7b.png](img/e654b55ed4040a7842510ed3c9690b7b.png)

What's interesting is that we can modify it, if there is a method to execute a command when the python library is imported would be great! This technique exist and is called **<u>Python library Hijacking</u>**. In python a library is simply a set of function definitions and when imported the whole code is executed so during runtime that functions can be used. this everything we need to know, we can add a line directly inside the library (`os.py`) that add a SUID bit to the `/bin/bash` binary. I used `system("chmod 4777 /bin/bash")` and when the script would be executed again the magic happens!

![53a51116eb2cc517b83bba0c4e2aebb6.png](img/53a51116eb2cc517b83bba0c4e2aebb6.png)

We are out of the friendzone I think...or we simply owned it? 
