# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (nginx/1.18.0)
**3000** &#8594; ppp

<br><br><br>

# ENUMERATION & FOOTHOLD

Port 3000 with service **PPP** is something new for me, **<u>Point-to-Point Protocol</u>** (PPP) work at level 2 and is used for serial communication establishment and management.

![235b73521667cbeeacee310b8f08d718.png](img/235b73521667cbeeacee310b8f08d718.png)

This is the website and is acting like a Google Drive clone, we have some names of the team that can be turned to potential target but nothing more. We can also **login** or **register** to the webservice.

But before that if we press on HOME we can hav something interesting

![48ab2fd1a4f89b0cbf8850bd07ef4f1c.png](img/48ab2fd1a4f89b0cbf8850bd07ef4f1c.png)

To access it we need an account so I created one in order to access to this file and we have just a welcome message

![f48bd405f7e03ec50efbbbf97c763b75.png](img/f48bd405f7e03ec50efbbbf97c763b75.png)

This is the upload page,I have played a lot with it trying different extension and common bypass but none works 

![a0d0e5d921b538340ede06c35d4f8e61.png](img/a0d0e5d921b538340ede06c35d4f8e61.png)

<br>

So the options here are various :
1) Try to upload something like a php rev shell, but seems something really hard to achieve since the webapp accept only MIME text file
2)  Somehow get access of another group and check some sensitive file or directly access file that I am not able to see right now
3)  Find a way to get access as admin user

So I decide to focus on **[2]**, when we access a document we send a reqeust like `/[document_id]/GetFileDetail` and we are returned with a HTML page with title `getFileDetail id [document_id]`. I created a quick and dirty python script to bruteforce some integer and if we are lucky enough we can access to some files (even if I was pretty sure nothing usefull will come as output was worth a least a try)


```{python3}
#!/usr/bin/python3

import requests
keyword = "getFileDetail id"


for inc_int in range(1, 10000):  
    url = f"http://drive.htb/{inc_int}/getFileDetail/"

    cookies = {"csrftoken": "TbyPOTOHNU43f4XQGWr8l6yEhh3cjQXq","sessionid": "iz9sohj9auvz6apff0pbpqdbntlsoqjd"}


    response = requests.get(url, cookies=cookies)
    if keyword in response.text:
        print("SUCCES "+ url)
```

Unlucky we have returned back just the welcome file and 3 files that I have personally uploaded nothing interesting, rip!

![f27f3f84e6b102dcb282c8f1ae8c0d9c.png](img/f27f3f84e6b102dcb282c8f1ae8c0d9c.png)

NULL BYTE injection doesn't work, same thing for the contact-us page which appear to be just a rabbit-hole

![571fcda938bc440dceecc1121f9c97b0.png](img/571fcda938bc440dceecc1121f9c97b0.png)

<br><br>

Fine, let's take a few steps back
We are dealing with a store file services, this is just my assumption but probably what the webapp do is to create a directory named with the user account and here is where file are stored. My assumption has been confirmed when I try a path traversal on the filename

![8d01a961b89819629a6cf179d991a527.png](img/8d01a961b89819629a6cf179d991a527.png)

Cool, we can also have a list of file that we can "Reserve" (block) at `/blockFile` sending a request as `/[document_id]/block`

![208c3490b2d0c4c42f56461396156e33.png](img/208c3490b2d0c4c42f56461396156e33.png)

So I can do the same fuzzing (this time I used wufzz) on the document ID and we have some results (I have deleted previous uploaded documents)

![e494281a52bd06d4702a4a6bf6cf4e86.png](img/e494281a52bd06d4702a4a6bf6cf4e86.png)

Cool stuff, now in UnReserve section we have some more interesing files

![6a78dca0bac88509ea3eaae7347df709.png](img/6a78dca0bac88509ea3eaae7347df709.png)

Cool now we can unreserve this cool files but I can't access it at the moment (not present on the show file list). 

But what about groups? I created a groups called **Admin** and when I go to edit the group `/47/editGroup` we have list of users 

![abf4047362e00cf38a44fcc24589685b.png](img/abf4047362e00cf38a44fcc24589685b.png)

There is nothing more we can do actually. There is something that I missed, **I have not check the URL where wfuz returned 200** (I get angry with myself but fine we can now see the results)

![fedd9d959ca01f4edc55fb9cea745b20.png](img/fedd9d959ca01f4edc55fb9cea745b20.png)

![155c5c56ce999159c705cdc88e7ac34c.png](img/155c5c56ce999159c705cdc88e7ac34c.png)

![e40a0503aa64d58ddf2bdc37cf0608ac.png](img/e40a0503aa64d58ddf2bdc37cf0608ac.png)

We have a set of credentials and we can try on SSH. 

Finally we are in

![84a7979c21d05792d65ada762c2f0979.png](img/84a7979c21d05792d65ada762c2f0979.png)

<br><br><br>

# USER FLAG
The machine is running gitea on port 3000, I make a tunnel with SSH and try with the the credentials already available but no access. I decide to check which users are available on gitea and instead of `martin` we should use `martinCruz` (we also have some email addresses)

![7637d840fe55ba24ea05a892b3bbf0fb.png](img/7637d840fe55ba24ea05a892b3bbf0fb.png)

The only repository available is the source code of the webapp

![d53f913bde4196df89af2bad7cadf952.png](img/d53f913bde4196df89af2bad7cadf952.png)

Inside we have the `db_backup.sh` cited in the files retrieved before on the webapp, what's cool here is there is an hardcoded password

![7cb47a240d74f49d2b71dc4c348bc721.png](img/7cb47a240d74f49d2b71dc4c348bc721.png)

I was able to crack the tom password (extracting the dbbackups) and I was able to SSH as `Tom` user, where is home folder store the user flag

![5ea51c69f26e7fbdda96ad73cc795acf.png](img/5ea51c69f26e7fbdda96ad73cc795acf.png)

<br><br><br>

# PRIVILEGE ESCALATION
Inside the home folder we have a cool file that can be used

![23ed21caa23ac8d467e228e816b00838.png](img/23ed21caa23ac8d467e228e816b00838.png)

Cool, binary exploitation let's upload it and use **Ghidra**, we have a huge binary right here!

Luckly we have some hardcoded credentials. 

![1618c4cd0c0f3db94a350007a11564ad.png](img/1618c4cd0c0f3db94a350007a11564ad.png)

![3fef2ab88e5ca9f23c3a0abf9af47126.png](img/3fef2ab88e5ca9f23c3a0abf9af47126.png)

Let's use it to unlock the binary

![439c7a3524a33236c3f1559182f63b48.png](img/439c7a3524a33236c3f1559182f63b48.png)

The options number 5 allows to insert a usernam as input, interesting probably is working with database and SQLi is possibile

![f6a99d6703efcb3bf852729ad386bc91.png](img/f6a99d6703efcb3bf852729ad386bc91.png)

Looking back at Ghidra, the options 5 is calling the `activate_user_account()` function and we can clearly see the SQL statement

![c6d794fb239b658efc05c73bd27eb2b1.png](img/c6d794fb239b658efc05c73bd27eb2b1.png)

Obviously will not be that simple (we are adealing with an hard machine after all), look closely there is `sanitize_string()` function

![6c5e313c35dc276f3a850a6726054ca0.png](img/6c5e313c35dc276f3a850a6726054ca0.png)

First thing first, the string injected will be struncated at 35 char and every special character removed (`\{/|';`, `[SPACE]` and `[NULL]`)

![9547db64462f52c57f153700c70cc312.png](img/9547db64462f52c57f153700c70cc312.png)

<br><br><br>

Summarize what we need to get user, first to execute some commands I am gonna use the `edit()` function in sqlite3 and we can gain RCE like this `edit("","sh -c whoami")`

We can use tab to get rid of the [SPACE] sanitization

![35134f20adb57a20a8ec70bac845eddb.png](img/35134f20adb57a20a8ec70bac845eddb.png)

So I have tried something like this but is not working

![ca9a75bbdd2f0f806d69cd1dde3e57b7.png](img/ca9a75bbdd2f0f806d69cd1dde3e57b7.png)

Probablly some issue with PATH, somehow sqlite cannot call without full path whatever command we need.

I copied in `./w` the `nano` tool so we can use the escape to get a shell and use the `char()` SQL statement to bypass the sanitization but still now working

![6c5e3c35088453fdbef98ff87a187d16.png](img/6c5e3c35088453fdbef98ff87a187d16.png)

After some trying and error (I also tried with VIM and use the `/tmp` folder with no results at all) I decide to change attack vector and use the `load_extension`

I created a `.so` with the following C code

```{c}
#include <stdlib.h>
#include <unistd.h>

void _init() {
    setuid(0);
    setgid(0);
    system("/usr/bin/chmod +s /bin/bash");
}
```

Than I found this cool method to inject and load our code, using the `+` and close the quotes in this case we don't need comments, this is probably why it wasn't working the comment was destroyng the `;` char. Plus no TAB needed

![88caaefa4d1409e56db2b14827d120dc.png](img/88caaefa4d1409e56db2b14827d120dc.png)

We got what we want, the SUID on the bash binary

![6af0f470214841ec742c72bd0d4db504.png](img/6af0f470214841ec742c72bd0d4db504.png)

