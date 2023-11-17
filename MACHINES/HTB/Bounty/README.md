# PORT SCAN
* **80** &#8594; http (IIS)


# ENUMERATION & USER FLAG
LMAO this is the homepage

![d4035113a53192de4b88889917d2367b.png](img/d4035113a53192de4b88889917d2367b.png)

wtf, let's what we can do here. The server is `Microsoft IIS 7.5` and with little enumeration I found `/aspnet_client` and `/uploadedfiles` but return error 403 when try to access them

![b471c93498986112f3d570fd3bb6515b.png](img/b471c93498986112f3d570fd3bb6515b.png)

Internet suggest me to use the `auxiliary/scanner/http/iis_shortname_scanner` metasploit module in order to return potential files/direcotries

![28887ea0e1d3d68c270a487e9bb631a3.png](img/28887ea0e1d3d68c270a487e9bb631a3.png)

`/transfer.aspx` allows to upload files

![4dd8b1d73a515dfa507ff29dbe629878.png](img/4dd8b1d73a515dfa507ff29dbe629878.png)

Than I quickly discovered files are stored in `/uploadedfiles` and use my cat logo as example

![3b224c5a8a1dd780cfd84bd11ef96257.png](img/3b224c5a8a1dd780cfd84bd11ef96257.png)

Gottch'a, PHP files are not accepted and same for ASPX but **<u>config files are accepted</u>**

This article was very formative on how to run ASP code trough .config files and I used the first example to check if is working trying to executing this simple code `Response.write(1+2)`

It worked! now we can try to get a reverse shell abusing the `config` file like it follow

![8e575fa0725e3d546877af25286cffe8.png](img/8e575fa0725e3d546877af25286cffe8.png)

After few seconds I can enjoy my user shell!

![05537b8c10d5f5564c70d5a393ed6c2d.png](img/05537b8c10d5f5564c70d5a393ed6c2d.png)


<br><br><br>

# PRIVILEGE ESCALATION

We are inside as `merlin` user and we have the SeImpersonatePrivilege

![cb09890b514acdc349ace0452b4069bf.png](img/cb09890b514acdc349ace0452b4069bf.png)

We can use a Potato attack and I decided to use it through metasploit (I am trying to getmore familiare with it) which include the module `exploit/windows/local/ms16_075_reflection_juicy`.

Obviously I need to retrieve a meterpeter sessions in order to trigger the privilege escalation and is something pretty easy to achieve thanks to module `web delivery`. After triggering the meterpeter shell we are free to good and get out `nt authority\system` permission

![fcdadcfa03ad88977d6795d0a9f5a538.png](img/fcdadcfa03ad88977d6795d0a9f5a538.png)

Really normal machine a good amount of web, I learned about **<u>malicious config files</u>** and a pretty cool enumeration method for IIS
