# PORT SCAN
* **80** &#8594; HTTP (IIS 10.0)
* **135** &#8594; MSRPC
* **139/445** &#8594; SMB
* **8080** &#8594; HTTP (Apache Tomcat 8.5.37)

<br><br><br>

# ENUMERATION & USER FLAG
This is a pretty cool ports configuration, port 80 host the default IIS homepage so will be pretty useless at least for now.
Port `8080` is more juicy

![f1078c2e8645536583f9a3c82cd5a5b5.png](img/f1078c2e8645536583f9a3c82cd5a5b5.png)

I tried some VHOST and directory busting but nothing usefull at all, luckly SMB accept anonymous access!

![f92aa76ca4475a284fd386d511cdf536.png](img/f92aa76ca4475a284fd386d511cdf536.png)

I have spend a few minutes on the `Users` durectory (with the **tree** command was pretty easy to have an overview) but nothing usefull has been found, the `BatShare` (shoutout to batman lol) is our only hope. Inside we have `appserver.zip` which include 2 file

* **<u>IMPORTANT.txt</u>**
<bt>
![e340f875bd27c6de024beeab20e84e96.png](img/e340f875bd27c6de024beeab20e84e96.png)

* **<u>backup.img</u>** which is a LUKS () encrypted file
<br>
![b5753c6f6c598b1bdfbbe1586d690bb9.png](img/b5753c6f6c598b1bdfbbe1586d690bb9.png)

I have found a really well done [blog post about LUKS Cracking](https://irq5.io/2014/11/19/bruteforcing-luks-volumes-explained/?source=post_page-----832d5543101f--------------------------------) it explains very well how this encryption method is done and how to perfrorm bruteforcing on it.
The file header is enough to retrieve the plaintext password

![dfbc046729a4c221f14add32734c4b1a.png](img/dfbc046729a4c221f14add32734c4b1a.png)

Now we can use hashcat with the `-m 14600` and `-w 3` (suggested by hashcat itself to set host memory to 3MB), it will take a lot even with my **GTX 1080** so I will make a break while the cat will work on it. 

Ok maybe is taking too much time and the estimated time is 4 days so I don't want to wait all this time, I look around the web and discover the tool `bruteforce-luks` with a 20 cuncurrent threads but again is still not enough. 
Since we have a lot of reference about Batman I have searched online if there is a pre-built wordlist on this theme but no results so we can still use rockyou.txt but reducing the wordlist grepping words like **batman**, **alfred** and **joker**, much much much better now!

![552babe7994e6c6e225bd2db78c41f58.png](img/552babe7994e6c6e225bd2db78c41f58.png)

Cool now we can access the `backup.img` file and mount it on our local machine

![eaff856655abcc5daa15c0e07d7b4482.png](img/eaff856655abcc5daa15c0e07d7b4482.png)

![1c2fc73a3dd56b3ce215007c6473319b.png](img/1c2fc73a3dd56b3ce215007c6473319b.png)

Finally we have access to the Wayne's secret...which doesn't contains nothing if not some default configuration tomcat files and a PDF with all the dialogue of batman begis (really, WTF). The only information we have in our pocket is the password for now.

I have took a look around the webapp and we have something that I have never seen, at `/userSubscribe.faces` we have a form using to send the email and signup to the subscription program

![11634380e75e1ff9e708cbf8b7b812d2.png](img/11634380e75e1ff9e708cbf8b7b812d2.png)

A quick search on duckduckgo taught me that `.faces` pages means they are using **<u>JavaServer Faces</u>** a technology which use java to create Web Pages with reusable components, cool something new!

[HackTricks](https://book.hacktricks.xyz/pentesting-web/deserialization/java-jsf-viewstate-.faces-deserialization) was one of the first results searching on how this type of pages can be attacked and **<u>deserialization</u>** is the most common. JSF implement sessions with **<u>ViewState</u>** and usually is stored in a hidden input field

![dfa0a58571b82408855afe0dfa4cb8ff.png](img/dfa0a58571b82408855afe0dfa4cb8ff.png)

Exist 2 variance of JSF `MyFaces` and `Mojarra`, is important to detect which implementations is the server hosting because in case of the former the ViewState is encrypted by default with a password. Well maybe the tomcat files can turn really usefull now especially the file `web.xml.bak` which tell us the JSF implementation running is **<u>MyFaces</u>** with plaintext password inside the XML tag

![f570ba38a799a4826f42c88bfe94fb18.png](img/f570ba38a799a4826f42c88bfe94fb18.png)

That's cool, HackTricks provide a script that will use the exfiltrated password to crypt the payload, using that return that the naive plaintext password found was base64 encoded (I am pretty stupid sometimes ehehe) so just a simple decoding is fine to take the plaintext password

![caf037bfe508704ea1734e1dc573dbf4.png](img/caf037bfe508704ea1734e1dc573dbf4.png)

Reeally good, we have the first component needed to attack now we need to create a payload and when you say Java deserialization I scream [ysoserial](https://github.com/frohoff/ysoserial), the gadget that HackTricks suggest to start with is the `common collection` one

![a0aa2f0afc70c0cdcee19a994cab3231.png](img/a0aa2f0afc70c0cdcee19a994cab3231.png)

As you can see I am saving the output of **ysoserial** in a file called `pay1` which will be used as input of the script and write the crypto version of it in `pay2` which can be used when intereceptin the request when we try to subscribe at `/userSubscribe.faces` cahnging the value of ViewState. I tried to fetch a file hosted throug an HTTP server on my local machine and we achieve RCE finally!

![7b7cc04e998550e69e5baf01d23341db.png](img/7b7cc04e998550e69e5baf01d23341db.png)

I have used this methof to upload and execute a reverse shell through netcat and we have access as `alfred` which his home directory contains the user flag

![b2489840fa8614e5ecad6eb348e07164.png](img/b2489840fa8614e5ecad6eb348e07164.png)

<br><br><br>

# PRIVILEGE ESCALATION


Now that we are inside the batcave let's see if we can find a crack to leverage more and escalate the privileges. Since we have a `batman` user we can try to get his credentials, In the downloads directory we have a `backup.zip` which I have tried to download it on my local machine through SMB but there is a policy restricting the procedure 

![37f621d6673293608670b2ea1a008dd5.png](img/37f621d6673293608670b2ea1a008dd5.png)

Hope is not lost there are a lot of ways to download files from a remote machine and the fact that we can't use driectly the SMB share will not demoralize me at all. The error highlight the problem, the policies doesnt accept unauthentcated SMB access but we can set a username and password and check some documentation online on how to autenticate on sMB share from windows

![fa9f06d9d0e1cfa87a6cc8b46c4bc8ad.png](img/fa9f06d9d0e1cfa87a6cc8b46c4bc8ad.png)

![391dd0c55961d192ebfb180d7e02f1bb.png](img/391dd0c55961d192ebfb180d7e02f1bb.png)

Cool, good stuff! Unzipping the file we know that it contains a OST (Outlook Offline Storage) file 

![3d3f48dd6ed3d6ccb087191eb4a5aa9e.png](img/3d3f48dd6ed3d6ccb087191eb4a5aa9e.png)

With a little research we have a cool set of tools under `pst-utils` which can help us to read and investigate this type of files

![91da498726b2875599eb3a585bdce2cd.png](img/91da498726b2875599eb3a585bdce2cd.png)

The tool found a Drafts file (and save it in `Drafts.mbox`), maybe we will extract it and get something right there . I have discovered **<u>MUTT</u>** a CLI email viewer. I used `mutt -f Drafts.mbox`, than I selected the only email presented

![67703b1efdbe4486c569eba0b8e2cbe1.png](img/67703b1efdbe4486c569eba0b8e2cbe1.png)

The email is just an attachment and the destination is to `batman`, v can press `v` to view the list of attachments and select the only one available

![28867969a302db6826328990a657db9c.png](img/28867969a302db6826328990a657db9c.png)

Cool this screenshoot is valuable we can try that password to execute a reverse shell as `batman`

![aa2b9edce76cfcd50d19e08bde7181a1.png](img/aa2b9edce76cfcd50d19e08bde7181a1.png)

Credentials work, now  we just need to call a reverse shell and we finally become a superhero!

![1e8493b8ab7b57dd33fb49c8732956c4.png](img/1e8493b8ab7b57dd33fb49c8732956c4.png)

The box is pwned becasue this user is part of Administrator group and we can now retrieve the root flag!

> I am pretty happy about the skills I have gained in 1/2 month, with this done I have done all easy and medium box of windows. I am more ken with this OS and I can navigate pretty easily without been lost and confused with the powershell. Pretty good if I think I start hating attack this OS and now looking back all the progresses I made in a (relative) short amount of time. I can now take the BatCar and drive to my next adventure!
