# PORT SCAN
* **80**&#8594; HTTP (Apache 2.4.18)


<br><br><br>

# ENUMERATION & USER FLAG
Just webapp exploitation path right here, let's get started!

![02d6562564285d19a1bd31a58695aa8f.png](img/02d6562564285d19a1bd31a58695aa8f.png)

The `phpbash` anchor points to a [github repostory owned by the author](https://github.com/Arrexel/phpbash) of theb box containig a PHP reverse shell " to assist in penetration tests where traditional reverse shells are not possible".

Well I tought I have to access some admin panel or find a way to perform RFI but actuallyu to use this webshell was easier than exprected, after some fuzzing I was able to locate the `/dev` directory containing the webshell of the author!

![b7d987da745f8e6012ded663d1b208c5.png](img/b7d987da745f8e6012ded663d1b208c5.png)

LOL! This webshell is able to reach my local machine (test it with `ping`) so I used to get a reverse shell on my local machine

![44a9bbbb51c6639921f5f7862c98736b.png](img/44a9bbbb51c6639921f5f7862c98736b.png)

On `/home/arrexel/user.txt` is where our flag is waiting to be grabbed!

<br><br><br>

# PRIVILEGE ESCALATION

![68f1ab8a16757a347c08b021e973f3a0.png](img/68f1ab8a16757a347c08b021e973f3a0.png)

We can use sudo to impersonate `scriptmanager` user without the password,so I used to spawn a shell as that user. in the root directory we have a uncommon directory called `scripts` which contain a `test.py` and `test.txt` the former is owned by the user we are impersonating and the latter by root. About every minute the `test.txt` results modified and looking at the content of the python file everything is clear

![f590cea21b4874ce8e9333b4736cbd79.png](img/f590cea21b4874ce8e9333b4736cbd79.png)

We can assume that root every minute execute the `test.py` script. We can replace it with a reverse shell uploading from the local machine or injecting directly in the script, I opted for the second choice 

![2f281feac61a67779f170d7311dab0bd.png](img/2f281feac61a67779f170d7311dab0bd.png)

within a minute we see the root shell popout!

![ddb2525ed9feabea456d109295ce0d7e.png](img/ddb2525ed9feabea456d109295ce0d7e.png)
