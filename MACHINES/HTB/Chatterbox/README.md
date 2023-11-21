# PORT SCAN
* **135** &#8594; msrpc
* **139 / 445** &#8594; SMB
* **9255 / 9256** &#8594; A-chat

<br><br><br>

# ENUMERATION & USER FLAG
SMB seems pretty unbreachable but we have the 2 `A-CHAT` ports that are pretty uncommon, this is our way in for sure!

![47f73f8254939e0c287a02f3f7eea43f.png](img/47f73f8254939e0c287a02f3f7eea43f.png)

Prtty cool ah? I have checked and the latest version is the 0.150 and guess waht? is vulnerable to stack overflow 

![b9928c842ea6366871aa14cd22846281.png](img/b9928c842ea6366871aa14cd22846281.png)

What's cool here is that we need to inject a msfvenom shellcode but for some reason a direct reverse shell don't work we can only execute remote command. I create a oneline reverse shell and hosted in a loca http server and create a shell code that download and execute the script in order to retrieve a shell

![784a30470e1b73f836ca0085aaed48da.png](img/784a30470e1b73f836ca0085aaed48da.png)

Easy and smooth we got a shell and user flag (strange this should be a medium lol)

![44f3ad248b6ea554b1483461cc3faaff.png](img/44f3ad248b6ea554b1483461cc3faaff.png)

<br><br><br>

# PRIVILEGE ESCALATION

Inside `Program FIles` there is `DVD Maker` directory corresponding to the well known software, we have to find the vulnerable version in order to check some well known exploit but actually nothing works so after some enumeration I found out that the `Administrator` directory cannot be read but we have full permissions on it 

![074d1a59eba7f252ad2515a28303bc3d.png](img/074d1a59eba7f252ad2515a28303bc3d.png)

Simple and direct, we can change permissions (we have full permissions on it) in order to read file inside 

![51a7635469431e2af06687b7caac4f1e.png](img/51a7635469431e2af06687b7caac4f1e.png)

More an easy than a medium one, lol
