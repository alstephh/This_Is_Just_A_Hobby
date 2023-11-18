# PORT SCAN
* **135** &#8594; MSRPC
* **8500** &#8594; Cold Fusion MX Server



<br><br><br>

# ENUMERATION & USER FLAG
Before starting out I want to note this machine have a really low reviews stars (w.6 out of 5), the reviews are complaint about how much unstable this box is so let's take into account that some exploit or scan can return some inconsistent results.

Well the port 8500 is the most interesting because is strictly related to the box name : **<u>Cold Fusion</u>** which deceloped by Adobe and described as it follows

> Adobe ColdFusion is a battle-proven and high-performing application server that makes web development easy for every coder.
Bring your vision of a scalable, game-changing and reliable product to life.

I look at the official website and this service is all about versatility, thri-party integration and cloud using **<u>ColdFusion Markup Language</u> (CFML)**

I try to access on RPC client but failed so I took a look directly at port 8500

![d645d0afd527078b19026c58c76b2792.png](img/d645d0afd527078b19026c58c76b2792.png)

I read a few articles about pentest this service and explored the webapp I encountered the administrator login panel which tell us the version of coldfusion

![a98f7480890a43ee7bfa0b31adda3e2b.png](img/a98f7480890a43ee7bfa0b31adda3e2b.png)

Lokking at [this page](https://pentest.mxhx.org/04-webapps/coldfusion) I have been able to extract the password hash of the admin user through this LFI

`/CFIDE/administrator/enter.cfm?locale=../../../../../../../../../../ColdFusion8/lib/password.properties%00en`

![c115390327fc2657117142d0a5ade068.png](img/c115390327fc2657117142d0a5ade068.png)

Cool I used john and retrieve it easily (we are talking of a SHA1 password)

![697ab2ca5b21a43dae1e86b69e8d5b2d.png](img/697ab2ca5b21a43dae1e86b69e8d5b2d.png)

Great, we are in!

![db34e5e4886fa59db80a5cd60d3a6bb6.png](img/db34e5e4886fa59db80a5cd60d3a6bb6.png)

To get access we can abuse the `Scheduled Task` feature to upload a remote file (that will be a `JSP` reverse shell)

![d03237b3ac24c4a42a57241e0573ea93.png](img/d03237b3ac24c4a42a57241e0573ea93.png)

Than just run the scheduled task

![94e8feadf22c3339259aec77c2d0fe30.png](img/94e8feadf22c3339259aec77c2d0fe30.png)

Just visit the file in order to trigger it, the user flag is owned

![5d763339ac4a3ce4a3c01972a1659ca9.png](img/5d763339ac4a3ce4a3c01972a1659ca9.png)


<br><br><br>

# PRIVILEGE ESCALATION
As usual we have `SeImpersonatePrivilege` enabled and we are inside a Windows Server 2008 this means there are probably a lot of exploit that can be run in order to get **SYSTEM**.

![85ce4c38466f1c7c91b5bdbf8e787b34.png](img/85ce4c38466f1c7c91b5bdbf8e787b34.png)

We have our old friend `reflection` and `reflection juicy` so I will rely on the latter, effective and smooth!

![4b5c60b27e656ac1c33ecb37fd9e2868.png](img/4b5c60b27e656ac1c33ecb37fd9e2868.png)

> Windows easy machine can be really boring after a while but since I started as a total ignorant of this OS now everything start to compose togheter and I now what to check, where to check and what to do at least for obvious privilege escalation flaws, which is not much I know that but is something I new I have embraced and learned, cool stuff!
