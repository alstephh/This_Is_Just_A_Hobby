# PORT SCAN	
* **22** &#8594: SSH
* **80** &#8594; HTTP (nginx 1.14)

<br><br><br>

# ENUMERATION & USER FLAG
The homepage is a simple presentation apge but we have a link that redirect to `images.late.htb`

![9366c850fa200b003f237ec152b82713.png](img/9366c850fa200b003f237ec152b82713.png)

Cool, what we have is a web tool that translate the image to text, and is running with flask according to the website heading

![2f59c94c5563aacef0328959de9b9809.png](img/2f59c94c5563aacef0328959de9b9809.png)

I try some different stuff, the function of the machine is really interesting for a CTF box, I think there should be a way to make a SSTI somehow and somewhere

In my first project at university (Data Science) I have encountered `tessereact` and probably is the same thing running in the background and giving us results (is just my assumption)

Sending multiple request unlocked me the vision on how the file is stored (already tried path traversal but doesn't work, same thing on SSTI the input is sanitized enough to stop me to use the file name as abuse tool)

![c0cbf9dfe12aac58c1e44145dbbf0a7f.png](img/c0cbf9dfe12aac58c1e44145dbbf0a7f.png)

Well, I do something that in my childish play will be cool af if it works I take this image and check the output

![abuse2.png](img/abuse2.png)

OMG, let's se the results

![e043313af78d91bce339b32ed99539aa.png](img/e043313af78d91bce339b32ed99539aa.png)

Makes sense, I would try with some more payload to check the consistency and work of this cool stuff. The only issue here is that is really hard for whatever is translating the image to text to understand the difference between `_` and `__`.

After some tial and error (and ask to AI chatbot to create a script to create clear image with a text provided) I was able to get a shell (I will not go through all the process of SSTI as I have explained like 4 time, is something often present in HTB I usually follow this [guide](https://0xedeon-pentest-notes.deno.dev/exploit/web/framework/python/flask-jinja2-pentesting/))

![aa6221b9992f39ea505d37c54a94d79e.png](img/aa6221b9992f39ea505d37c54a94d79e.png)

Just grab the flag and let's move on

<br><br><br>

# PRIVILEGE ESCALATON

We have an obvious attack vector here

![884d0d3ff19ab0efbfc62e0e58eba1ad.png](img/884d0d3ff19ab0efbfc62e0e58eba1ad.png)

We have this script inside 

![5e3cf2d8b76a132e208ecd189faa2f53.png](img/5e3cf2d8b76a132e208ecd189faa2f53.png)

Is using `sendmail 8.10.1` and apparently is vulnerable to a CVE but we have permission to add data inside the script, ez play here

The script is running as root and is active whenever we access with ssh

![444897e39c5dc6109d7a52322e7bffa5.png](img/444897e39c5dc6109d7a52322e7bffa5.png)

Now I can grab the ssh key of the user and try to access on victim through port 22, if everything is cool we will get a SUID on bash 

![226b441e5adc9b8260603a2f7ecac630.png](img/226b441e5adc9b8260603a2f7ecac630.png)

Gotch'a
