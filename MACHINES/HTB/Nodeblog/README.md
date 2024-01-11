# PORT SCAN
* **22** &#8594; SSH
* **5000** &#8594; NODE.JS (Express Middleware)

<br><br><br>

# ENUMERATION & USER FLAG
The NODE.JS service i a simpl blog 

![adf2d8064a13a8a568d12db25e39ac46.png](img/adf2d8064a13a8a568d12db25e39ac46.png)

I am pretty confused on what to do because it lacks of contents, pretty bad stuff here. Well let's focus on the login form first of all `admin` is a valid account because otherwise will return invalid username error, that's great!

I tried some SQLi and the SQLMAP instance but none return nothing so probably they are using something else like NoSQL. The request is sent with POST so we need to change the content type to `application/json` if we want to inject.

Luckly I have already mess up with some NoSQL database so I know a little on how to check if there is a injection or not. This is the case!

![f5c16d81343eec535cdf1b7a8c4a9d0e.png](img/f5c16d81343eec535cdf1b7a8c4a9d0e.png)

As you can see we have an authentication cookie thismeans we can access as admin in this way

![ec51c1f257f4ead82637f34937b7c597.png](img/ec51c1f257f4ead82637f34937b7c597.png)

On the `UPLOAD` section I tried to upload a (what I think it was) innocent image file but actually is not what the server was expecting

![3fd93942221194c49a6d9af85625d768.png](img/3fd93942221194c49a6d9af85625d768.png)

XML....interesting, this means we can try to perform some XXE but I want to understand what type of schema is the server expecting (because I pick a random XML in the web and was not working lol), just see the source code and everything will be clear

![1d1bf79f887774fb9b0d04faea706c26.png](img/1d1bf79f887774fb9b0d04faea706c26.png)

After that we can create a simple test XML and te XML will be read and use it for creating a new article, makes sense

![86a514ee1b4fa940aede00c87ef6d9b6.png](img/86a514ee1b4fa940aede00c87ef6d9b6.png)

We can now perform the XXE Injection!

![c7137a678fc07f31e3c0716da950645a.png](img/c7137a678fc07f31e3c0716da950645a.png)

Pretty useless the password file so I try to enumerate a little bit (and actually with the help of walkthrough) I was able to retrieve the `server.js` file commonly used for code that run the whole app.
What's interesting here is the `serialize` and `unserialize` on the cookie

![3cd69342b6d1e6a357c17af7ed0e18f0.png](img/3cd69342b6d1e6a357c17af7ed0e18f0.png)

Taking a closer look to the cookie we have something interesting

![517503ce75f8e798ce7d15da21a9b59f.png](img/517503ce75f8e798ce7d15da21a9b59f.png)

Now we found a way, we can change the JSON cookie in something else in order to perform RCE and than encoded in URL and use it as cookie. If everything work well we should be able to perfrom the RCE (I followed [HackTricks](https://book.hacktricks.xyz/pentesting-web/deserialization#nodejs) guide as always is gold!)

With this payload I was able to ping my machine

![71ef792d1ee44a9fb872be949edb4410.png](img/71ef792d1ee44a9fb872be949edb4410.png)

![8cd654ddc123720f36bf1e342a6e23ca.png](img/8cd654ddc123720f36bf1e342a6e23ca.png)

Now that we have RCE I used to download and execute a reverse shell I had saved on my local machine, now we havea feet inside the target! The user flag is there waiting for us!

![439db46b60a1bb3164456e44bbddf574.png](img/439db46b60a1bb3164456e44bbddf574.png)

<br><br><br>

# PRIVILEGE ESCALATION
We don't have much inside the directory and we can't run `sudo -l` without a password so is time to find something usefull. I saw in the previous LFI through XE that the DBMS used is MongoDB so is worth trying to check it out.

The configuration file is located at `/etc/mongodb.conf` and we know is running on port **27017**

![76f4dde6588134851ac8a032cde5a7e1.png](img/76f4dde6588134851ac8a032cde5a7e1.png)

We can connect whitout a password and we found the admin cleartext password!

![55b4563313a108dc15be318abc01421f.png](img/55b4563313a108dc15be318abc01421f.png)

Now we can see what we can run with sudo and actually everything can be executed as root, bash included

![40f06d1f3226164fb55335f998019a35.png](img/40f06d1f3226164fb55335f998019a35.png)

Root flag is ready to be grabbed!
