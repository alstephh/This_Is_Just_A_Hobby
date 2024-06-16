# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (Apache 2.4.41)

<br><br><br>

# ENUMERATION & USER FLAG
Simple scan, just a web server, no excuse, no mercy! This webserver start with a login page that needs only a simplepassword...uhmmm

![b104fc20108ef14165cbf022719934d6.png](img/b104fc20108ef14165cbf022719934d6.png)

Before doing some weird stuf I wanted to analyze the request with Burp, while the request looks pretty normal the response (actually the source code of the site lol) highlight some interesting logic behind the authentication

![5a959e555b0093bda7491fe91dd68150.png](img/5a959e555b0093bda7491fe91dd68150.png)

The `/api/login` is actually responsible for the authentication and we can directly interact with it without changing the GET parameter

![b7529735725d176b36ee5790aaf2e133.png](img/b7529735725d176b36ee5790aaf2e133.png)

If we manage to get a positive results here we would be able to ge access to the data behind the login form for sure...but how? Actually have no iea so I have just played with this request. After some fix and tune when I swapped from GET to POST I get some juicy info about the web technology used

![f4a8c2d91bb094afbbf8e74994f9635f.png](img/f4a8c2d91bb094afbbf8e74994f9635f.png)

Gotch'a it's **<u>Laravel</u>**! The post is interesting let's see if we can sent the same GET request but with something in the body.

![f4c358638507f40da3db4450c6114eb3.png](img/f4c358638507f40da3db4450c6114eb3.png)

This is interesting, now I am pretty lost but with some research I can manage it. I was pretty sure some `type smuggling` was needed to approach this first part but how...I HAVE IT!

Let's re-take the first image about the logic code behind the authentication

![80fb5c8da59eae18cd63334eaf76ba24.png](img/80fb5c8da59eae18cd63334eaf76ba24.png)

You can clearly see that what the form will send to `/api/login` something similar to the JSON we arenow able to deliver

```
{
	"password":"aabbcc"
}
```

So what if we just force the value of password to...**TRUE**?

![378610bc0e7fd2ec9bcc3671ec0c5e88.png](img/378610bc0e7fd2ec9bcc3671ec0c5e88.png)

Damn! We have it!

![2c0b772ba25bf38ca2605568e295cce2.png](img/2c0b772ba25bf38ca2605568e295cce2.png)

(We have the user.txt but I will conclude the section only when I have a shell into the machine)

I downloaded the zip file but bviously require a password to extract the zip file but unlucky using john2zip was not enough ((

![d82693b13a9043601b5581012ff4aca4.png](img/d82693b13a9043601b5581012ff4aca4.png)

Using the `file` command on the zip file I was able to clear the version is 2.0, pretty outdate and vulnarable to **<u>[known plaintext attack](https://code.whatever.social/exchange/crypto/questions/19716/breaking-zip-2-0-encryption-without-password)</u>**. I found this [tool](https://github.com/Aloxaf/rbkcrack) that is designed specifically for this vulnerability, let's see how it works!

We need some common plaintext in order to retrieve the keys to decrypt everything. At first I tried (since some common bytes are needed) to use the first line of every ssh private key (`-----BEGIN OPENSSH PRIVATE KEY-----`) but I was not able to get anything back. Now the solution I found is not that hard but we just need to analyze the file a little bit with `bkcrack -L`

![2159c95025c646e8b4fd01195280eed9.png](img/2159c95025c646e8b4fd01195280eed9.png)

Now look closely to `.profile` file and the CRC, usually this type of file into a home directory is rarely touched so I cloned my local version of it (untouched), zip it and reuse the tool to compare the CRC, it match we have a gap!

![f5a81a577953a561d62e986747002d47.png](img/f5a81a577953a561d62e986747002d47.png)

Now is pretty easy to going on and extract the keys!

![b556a5b991d4b2631eb9d897252a7db4.png](img/b556a5b991d4b2631eb9d897252a7db4.png)

With the keys we can retrieve a decrypted version of the zip file 

![4821764f1fe58590a9aa78e51c1e1647.png](img/4821764f1fe58590a9aa78e51c1e1647.png)

....and now we have the ssh private key!

![44228a15aef535be7c5d15131c40d6c6.png](img/44228a15aef535be7c5d15131c40d6c6.png)

In the public key I can find the name of the owner of the home folder 

![97cda12e0a12f43ce3db1a48dea4974b.png](img/97cda12e0a12f43ce3db1a48dea4974b.png)

Now time to get in!

![3f8279f0249854fcc9de2d5b28a3c720.png](img/3f8279f0249854fcc9de2d5b28a3c720.png)

<br><br><br>

# PRIVILEGE ESCALATION

There is an uncommn directory in the root of the filesystem called `srv` where inside is located the webroot of the webapp in `/srv/prod/public`!

![8313aad2de1b9698cd0b3b6bf5f0740d.png](img/8313aad2de1b9698cd0b3b6bf5f0740d.png)

My idea here was to find the password of the very first login page but hey really a pain with all this files right? The answer is and will alwasy be.....GREP EVERYTHING!
I usually use this shorthand `grep -arin -o -E '(\w+\W+){0,5}password(\W+\w+){0,5}'` and I was able to filter a good amount of files, removing the ones inside standard documentation folder I finally found what I wanted. Inside `app/Http/Controllers/AuthController.php` I get my prize!

![74a4f5b2f5f3892ba8fd34fd703cb82b.png](img/74a4f5b2f5f3892ba8fd34fd703cb82b.png)

This passwrod was enough to spawn a root bash with `su`

![efa478bb2a204787a02c517d5e73cf06.png](img/efa478bb2a204787a02c517d5e73cf06.png)

Got it!
