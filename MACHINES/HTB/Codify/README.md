# PORT SCAN
* **22** &#8594;  SHH
* **80** &#8594; HTTP (Apache)

<br><br><br>

# ENUMERATION & USER FLAG

![4eb00d41a2c47e8b9c258096e6472bf8.png](img/4eb00d41a2c47e8b9c258096e6472bf8.png)

Thi is the homepage of the website and is just a interpeter for node JS in a sandbox (at least is what is written here).

Before press the `"Try it now"` I want to check the <u>**limitations**</u> page 

![c0ff222822567c8dddd570353cb62a8f.png](img/c0ff222822567c8dddd570353cb62a8f.png)

Ok, we have a defined set of modules that can be used on the sandbox now is time to abuse this and try to bypass the `child_process` and `fs` limitation

In `/about` we have some information about the code editor

![8a632f55713b6809b02c6f05c9e58364.png](img/8a632f55713b6809b02c6f05c9e58364.png)

especially the **<u>vm2</u>** version which is `3.9.16` and, good thing for us, vulnerable to [CVE-2023-29199](https://nvd.nist.gov/vuln/detail/CVE-2023-29199). This vulnerability allows to escape the sandbox and if we accomplish that we will be able to execute the reverse shell using `child_process` or `fs`

Here we have the [PoC](https://gist.github.com/leesh3288/f05730165799bf56d70391f3d9ea187c) but let's be a little be superior than the avarage script kiddie and try to understand what we are abusing here.

In this case the vm2 sandbox doesn't properly manage exceptions causing leak of unsanitized host exceptions that are used to escape the sandbox and run arbitary code <u>**without limitation**</u> .
Simple and smooth, we can force an error, catch the exception and run our code

![5a1dcf1126dc8e439d356764c8242ba9.png](img/5a1dcf1126dc8e439d356764c8242ba9.png)

The function `stack()` repeatedly call itself causing and error and we run the whole code in a VM2 instance and exploit the vulnerability injecting our no-constraint code obviously to get a reverse shell

![d05b8f2d3621633ccddc4e56507e5f84.png](img/d05b8f2d3621633ccddc4e56507e5f84.png)

We have access as `svc` user so we need some horizzontal move to get the user flag that is probably inside the `joshua` home folder.

Inside `/var/www/contact` we have the <u>**tickets.db**</u> file which is used for helpdesk support ticket (lol I found no good way to explain that), the file contain an encrypted password

![c483ecad80079460504fdf28bb8478a0.png](img/c483ecad80079460504fdf28bb8478a0.png)

The password appear to be encrypted through `bcrypt` (`-m 3200` with hashcat)

![96ef1278049a3b328fc06a1df4e035e1.png](img/96ef1278049a3b328fc06a1df4e035e1.png)

After the cracking we know that joshua is a nickelodeon fan

![20eda56b71a68d547fdf1fed0022f58b.png](img/20eda56b71a68d547fdf1fed0022f58b.png)

Now we can move on an grab the flag

![8cb452369efb63039981b213ddcf4936.png](img/8cb452369efb63039981b213ddcf4936.png)

<br><br><br>

# PRIVILEGE ESCALATION
This sounds easy, with `sudo -l` we have something

![e7fefcc24af59a8ce84c9f46161173e6.png](img/e7fefcc24af59a8ce84c9f46161173e6.png)

We can't modify the scipt but we can take a look inside it

![841cc358b5e11293d40a296da30e0a2f.png](img/841cc358b5e11293d40a296da30e0a2f.png)

This is a simple SQL backup which can lead us to get the root password, the simple sh script doesn't seems to have some vulnerability or something to abuse so the only way is to brute force the root password but how to check when we are on the good path in the bruteforce?

Well actually we have a vulnerability which was something new for me so very very cool check this out.

Look closely at the conditional where the the password is checked

![77787ce98d8a45004bef7c0b02322989.png](img/77787ce98d8a45004bef7c0b02322989.png)

Is within **<u>double-bracket</u>** and not single bracket and bash script treat the conditional inside differently than single bracket ([this resource](https://tldp.org/LDP/abs/html/comparison-ops.html) was really usefull)

![7000d9ef4156c06d5f6baf24aa15a02f.png](img/7000d9ef4156c06d5f6baf24aa15a02f.png)

Double bracket conditional is more "powerfull" becasue support logical operator like `&&`, `||` and most importantly `*`. Like the example above we can check if the password start with some text, for instance if retrun true to `[[ PASSWORD == a* ]]` means the password start with `a`, smooth and clear.

If we insert just `*` as password we get true as output and makes sense so we can iterate this behaviour to bruteforce the root password with a small script.

Lesson of today, be very carefull when you create conditional with bash script, please.

![dad995553e7c27da68410555280a1b19.png](img/dad995553e7c27da68410555280a1b19.png)

Beautiful let's try this password to get root privileges

![aa932c857da0530759903d8467d8d4ba.png](img/aa932c857da0530759903d8467d8d4ba.png)

GOTCH'A!
