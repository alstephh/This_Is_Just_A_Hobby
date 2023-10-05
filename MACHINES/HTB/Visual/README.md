# PORT SCAN
* **80** &#8594; Apache HTTP 2.4.56 (Win64)

<br><br><br>

# USER FLAG
Uau, really poor port scan so we don't have other options than visiting the webapp

![3a03488d88e9c0acefa562a0623cf8c6.png](img/3a03488d88e9c0acefa562a0623cf8c6.png)

Interesting stuff, interesting stuff. So the first thing sound weird to me is the fact that I have to give a Git Repo at the webpage, is widely known that the boxes are not connected to the internet in any way, so I need something like **Gitea** to selfhost a Git repo by myself

So apparently (this is my assumption) the webapp recover the repo and build the .NET/C# file and compile it for us giving back the DLL or executable, if we can find a way to execute a powershell command and get a reverse shell we would take the user flag 

So  I first used `dotnet` to create a project with a simple Hello World programm inside

![c04b1ec06f4d4c01f65cc01b5b5acc00.png](img/c04b1ec06f4d4c01f65cc01b5b5acc00.png)

![7f10b3de3e3ff82a3ec5ad474dcbfa03.png](img/7f10b3de3e3ff82a3ec5ad474dcbfa03.png)

Following [this article](https://github.com/MicrosoftDocs/visualstudio-docs/blob/main/docs/ide/reference/build-events-page-project-designer-csharp.md) I was able to discover how to inject some

![ab229494879112ac47ccb3993aec79c1.png](img/ab229494879112ac47ccb3993aec79c1.png)

Which I changed to this 

![a9c8d8cc25afc5b61032aff37c6925e5.png](img/a9c8d8cc25afc5b61032aff37c6925e5.png)

Than I have just set up a docker container running gitlab, I take some configurations json files on existing GitHub repository of VS build (fllowing the error returned through the webapp) and than give the repository link to the webapp while netcat is running.

After few seconds I have my user shell and I am free to take the flag (finally!)

![c25edce7f46756517060f995522a3182.png](img/c25edce7f46756517060f995522a3182.png)

<br><br><br>

# LATERAL MOVEMENT & PRIVILEGE ESCALATION

Let's take a look at the groups we are part of

![1f625cdd5be18730c7804c6d2542d837.png](img/1f625cdd5be18730c7804c6d2542d837.png)

Plus we don't have enough privilege so we probably have to make some lateral movement. During the experience with this machine and some discussion with other HTB players I have acknowledge that **<u>in Windows Service Account are usually more powerfull than standard user</u>** (`NT AUTHORITY\System` is like being root in linux enviroment), running something like a web server service as local service is a misconfiguration (like running it on a user with `sudo NOPASSWD` in linux) and this is the case

<br><br>

So first of all I have checked all the service running in the machine

![628ce9dffd20f1def6cdd5b4cd68a0c5.png](img/628ce9dffd20f1def6cdd5b4cd68a0c5.png)

What's catch my eye is obviously the **Apache Web Server**, so let's pick information about it

![6f82b2b12807e442ae87f1069809278f.png](img/6f82b2b12807e442ae87f1069809278f.png)

Cool stuff so we have the directory where Apache is hosting the webapp so let's see if we have enough permission to abuse it and take controll of `NT AUTHORITY\Local Service` 

![bbbd7e53df1b6ad94356e0a1a01c3001.png](img/bbbd7e53df1b6ad94356e0a1a01c3001.png)

Oh yeah we got it, we can upload a simple pp reverse shell and access it through the browser to check everything is smooth. Here we go

![a7bdd898a300ecd636e8bebcf8cbe5b0.png](img/a7bdd898a300ecd636e8bebcf8cbe5b0.png)

Let's bring a reverse shell

![b2fc8069f23d2275f334b4fa11d716fa.png](img/b2fc8069f23d2275f334b4fa11d716fa.png)

<br>

Gotch'a cool cool stuff, researching how to abuse this type of user I have encountered [FullPowers.exe](https://github.com/itm4n/FullPowers), this executable is powerfull wehn have access as service account (database or web server for instance), it create a scheduled task and the process created (by the Task Scheduler) have all the default privileges set. In this way we can have more space to get root 

<br>

Than I used to get a reverse shell with the same user but with wider privileges

![be8f7976da13da820752abbb19161fe2.png](img/be8f7976da13da820752abbb19161fe2.png)

![6294fe43c0e60e8d7ebf5b22bb2fc058.png](img/6294fe43c0e60e8d7ebf5b22bb2fc058.png)

Look how beautiful it is now, we have the overpowered **<u>SeImpersonatePrivilege</u>** token set and with a quick web search [GodPotato](https://github.com/BeichenDream/GodPotato) is what it fit in this scenario

GodPotato is the overpowered and versatile version of the [Potato Privilege Escalation series](https://jlajara.gitlab.io/Potatoes_Windows_Privesc), this version exploit some defect on RPCSS (service that must be openend by the system account) dealing with oxid makin it possible to run on every WindowsOS (that's why is the GOD)

![75366fbeb97474ddefc6df46f19fd0ce.png](img/75366fbeb97474ddefc6df46f19fd0ce.png)

It works! Now I just set he command to read the root flag and I have successfully rooted this windows box but most important learn a cool Privilege Escalation technique!
