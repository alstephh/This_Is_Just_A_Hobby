# PORT SCAN
* **22** &#8594;SSH
* **139 / 445** &#8594; SAMBA
* **1800** &#8594; HTTP (NODEJS)
* **9999** &#8594; HTTP (NGINX)


<br><br><br>

# ENUMERATION & USER FLAG
Pretty strange port mapping, on port **9999** we have a standard nginx page

![d37e27d58f033b500fadc1309ac62d32.png](img/d37e27d58f033b500fadc1309ac62d32.png)

I tried to visit port `1880` but return "Server Not Found" error but using curl I am able to see the code source, pretty bad stuff but maybe SMB can give us something....

![11b261d653a38414d927e55c1d71fabb.png](img/11b261d653a38414d927e55c1d71fabb.png)

Even here in this protocl, nothing, cool is time to web enumeration!  
The first thinf I found is `/admin` with a simple login form with only 3 attempts available (just reload the page will reset the attempts), here I tried some simple SQL injection but apparently is not the case also no way to do some bruteforcing so I waited for some other results from the dirbusting.
`/test` is a simple PHPINFO page and `/dev` is not accessible (probably we will be redirected there after the login form). Just a lil bit of patience and `/backup` will popup

![10794f750c70991b93ac2b3d70e9c418.png](img/10794f750c70991b93ac2b3d70e9c418.png)

The credentials are not working, probably the password has been changed but the username sems to be valid. About `/backup/loop` and `/loop` we receive error **403** so again it can maybe suggest we can access it only after crednetials have been found. I have slammed my head on my wall for how much stupid was this. I checked the source code and the login form is linked to the `login.js` script which well, is not that secure. Look at this LOL!

![b5de6cf246850bdd54348dba4373f450.png](img/b5de6cf246850bdd54348dba4373f450.png)

After the login I was understood how much ironic this machine creator is

![1d42fc7b89049ce5c5134f8721fefdad.png](img/1d42fc7b89049ce5c5134f8721fefdad.png)

Doesn't seems a random string probably is a peculiar encoder in play, with some research **<u>Ook</u>** is the encoder used and with this [online tool](https://www.splitbrain.org/services/ook) we can retrieve the plaintext

![9c4ac7eb6fc45b81dcde7453bc4ed000.png](img/9c4ac7eb6fc45b81dcde7453bc4ed000.png)

Now I went on the path specified on the obfuscated string and we have another encoded string

![5ad980251d73f8ad79429d7eb3e83559.png](img/5ad980251d73f8ad79429d7eb3e83559.png)

![01f36d321c2ac71dd948673f5019f62d.png](img/01f36d321c2ac71dd948673f5019f62d.png)

We can barely see what this base64 "contains" but it seems is a base64 of a file, my (little) experience suggest to try with `.zip` but saving in a file without extension and use `file`

![95dde40b56bb3615c15e5afcb903c102.png](img/95dde40b56bb3615c15e5afcb903c102.png)

Gottch'a, if we want to extract the file we need a password and is a child-play with **john the ripper utilities**

![59a465d9b308de0ee0dff940a3d3573f.png](img/59a465d9b308de0ee0dff940a3d3573f.png)

Very very fun, another encoding string this time is in HEX, which is a base64 which is....ehm idk what is

![11411ad27c4c8da8c4312877616916e4.png](img/11411ad27c4c8da8c4312877616916e4.png)

Here I had to watch the writeup, this is **<u>brainfuck</u>** string and we can use the same tool used on previous encoded string to get the plaintext

![7de6ee146e85596db7c2de54ad896f99.png](img/7de6ee146e85596db7c2de54ad896f99.png)

Uhmmm actually not fun lol. I have returned back to enumeration and try to dirbusting `/loop` and `/dev`, in the latter I have found the path `/dev/backup` path with a juicy information

![bd27580bc855aeaf24abfb5ef96b2642.png](img/bd27580bc855aeaf24abfb5ef96b2642.png)

![7a019bd9d761bb2c48c11b690fbf6616.png](img/7a019bd9d761bb2c48c11b690fbf6616.png)

We have a good number of password to try and `admin:idkwhatispass` worked, now we are inside the application and RCE is easy to establish.
The version of **PlaySMS is 1.4** and thanks to a lack of input sanitiztion using `import.php` we can execute OS commands, [this PoC](https://github.com/jasperla/CVE-2017-9101) summarize everything in a python script

![fa798670755a903684daed745ad18546.png](img/fa798670755a903684daed745ad18546.png)

Now we can spawn a reverse shell and get the user flag!

<br><br><br>

# PRIVILEGE ESCALATION

I have grabbed the credentials stored in `/var/www/html/playsms/config.php` and used to login into mysql searching for some other password but nothing new has been found but we have an interesting SUID binary

![a59cc85e5228331934f929a06ca5d4b1.png](img/a59cc85e5228331934f929a06ca5d4b1.png)

The name is pretty self-explainatory so I have uploaded on my machine and made a quick blind test on what this binary does

![b0a60eaa391b31604ab8b0b889c81ed9.png](img/b0a60eaa391b31604ab8b0b889c81ed9.png)

Now I used the `checksec` function of **GDB** to have the type of security measures in play

![087717c2fe680860f90c095f2ab07c16.png](img/087717c2fe680860f90c095f2ab07c16.png)

Cool the stack is not executable so a **<u>ret2libc</u>** is the objective (I have checked that ASLR is disabled on the target machine) so now we can move on and find the offset to reach the Stack pointer

![4b5b4c9fadfab7760de16703d6c63a93.png](img/4b5b4c9fadfab7760de16703d6c63a93.png)

The offset is 52, next step is to identify the `libc address` on the target system, `ldd` can accomplished that easily

![9ceec0a48f808c208379f7fa5be584c5.png](img/9ceec0a48f808c208379f7fa5be584c5.png)

This is the base address that will be added to our **<u>gadgets</u>**, about that we need `system`, `sh` and `exit` (the last one was not obvious at the start after looking at few writeup I understand what I was missing). For `system` and `exit` a `readelf -s` in combination with `grep` on the libc is enough. For `sh` we need to run strings and grep as well

![27c4daba8eeb57c37a1931544e6e539b.png](img/27c4daba8eeb57c37a1931544e6e539b.png)

With all of these relative address we have to add the base address of the libc library, the payload is pretty easy `OFFSET + SYSTEM + EXIT + SH`

![46a116e253cd946540372074412798a6.png](img/46a116e253cd946540372074412798a6.png)
