# PORT SCAN
* **7680** &#8594; pando-pub / Windows Update Delivery Optimization (?)
* **8080** &#8594; HTTP

<br><br><br>

# ENUMERATION & USER FLAG

![7877a1428b8c259976ce0125432333a6.png](img/7877a1428b8c259976ce0125432333a6.png)

This is the homepage, the other panels are useless and give information about the facilities and services given about the gyms.

But here we go, on `/contact.php` we have info about the software used

![53d3a0fdec25203ea4b367ff92869c66.png](img/53d3a0fdec25203ea4b367ff92869c66.png)

Using searchsploit we know this is vulnerable to differens exploits

![6148d9f389d5f4dc15185f3cc2fb61a0.png](img/6148d9f389d5f4dc15185f3cc2fb61a0.png)

Cool I will start using the **<u>Unauthenticated Remote Code Execution</u>**

![90e58bbe3c3657cb6b714b5c281b4e9c.png](img/90e58bbe3c3657cb6b714b5c281b4e9c.png)

The script bypass the image upload filter and allows an unauthenticated user to upload a malicious php file

![ec3298ab94703d0805f2fe9b4bfeac5f.png](img/ec3298ab94703d0805f2fe9b4bfeac5f.png)

Really cool, really easy but the shell is really unstable so I upload netcat and use the multi handler to catch the a stable shell

![dd9b8cb7a5189c871740406c449cdfcc.png](img/dd9b8cb7a5189c871740406c449cdfcc.png)

Let's grab the flag!

<br><br><br>

# PRIVILEGE ESCALATION

Inside the `/Downloads` folder we have a `CloudMe_1112.exe`, I have executed and no output return back to us, so let's see what is happening behind the curtains

Searching the name of the exe I have discovered it is vulnerable to [stack overflow](https://www.exploit-db.com/exploits/48389) (here a interesting [blog article](https://bufferoverflows.net/practical-exploitation-part-1-cloudme-sync-1-11-2-bufferoverflow-seh/) on how it was discovered) plus it listen on port `8888` by default

![c881b7195ed0c44a7197c28328f45cc6.png](img/c881b7195ed0c44a7197c28328f45cc6.png)

Ok cool, first of all let's use chisel to make a tunneling so we are able to connect on the CloudMe application

<br>

On the attacker machine

![b7256a7408dc3533435c9b21d697da94.png](img/b7256a7408dc3533435c9b21d697da94.png)

On the victim

![cd4a447d6a5e9280b7208c9e026607f0.png](img/cd4a447d6a5e9280b7208c9e026607f0.png)

Now I can download the exploit on exploit-db and makes the right changes, especially the payload

![1d00a880e594bbbfbd3711d70deaac71.png](img/1d00a880e594bbbfbd3711d70deaac71.png)

Than just launch the scripts et voila' we are `Administrator` in a blink of an eye

![d3a42362c1347c7fef8e95380b313cbb.png](img/d3a42362c1347c7fef8e95380b313cbb.png)
