# PORT SCAN
* **53** &#8594; DNS
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **389 / 636** &#8594; LDAP
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
Cool, Active Directory here! I started with some RID bruteforce in order to retrieve a list of potential users

![7acd6ea4fd5621fd100ebfae350088ab.png](img/7acd6ea4fd5621fd100ebfae350088ab.png)

Also LDAP accept anonymous user and we can use it to retrieve more specific information about users like the descriptions or other metadata.

We have a legacy password for `r.thompson` user

![17be2b6a5853a2b2040fab6296d6c5e3.png](img/17be2b6a5853a2b2040fab6296d6c5e3.png)

I used the password on crackmapexec to be sure this user or someone else use this password decode from the base64 format which gave us the plaintext `rY4n5eva`

![1b8c9c5f10d0880c16f168a3974f1eb6.png](img/1b8c9c5f10d0880c16f168a3974f1eb6.png)

Gottch'a, inside SMB folder we have this cool email ready to be read 

![b2f5f6fc5de404ce13d25b3c887ab021.png](img/b2f5f6fc5de404ce13d25b3c887ab021.png)

But that's not everything we also have a `VNC registry` of **s.smith**

![40f5a4221dcba15c43724901c9d47ef6.png](img/40f5a4221dcba15c43724901c9d47ef6.png)

Cool we have a password and [here](https://github.com/billchaison/VNCDecrypt) is given a one liner to decrypt it

![c1871bd922700fa2978cb513fed36014.png](img/c1871bd922700fa2978cb513fed36014.png)

Cool stuff, we have the password of `s.smith` and work with WinRM, that's mean the user flag is ours!

<br><br><br>

# PRIVILEGE ESCALATION
Actually there is nothing usefull inside the machine but on SMB we can access `NETLOGON` directory which is used as script a **Logon Script**

![7a9ed79af2359eb2d259844fd908c57c.png](img/7a9ed79af2359eb2d259844fd908c57c.png)

![f98fad4893a9664599da441d61fae890.png](img/f98fad4893a9664599da441d61fae890.png)

We have something about a `.network` object which is actually what the email and is stored inside `AUDIT$` and the s.smith user have access to it where inside we have the `RunAudit.bat`

![5ab3c46f939394c54199702346e9a690.png](img/5ab3c46f939394c54199702346e9a690.png)

Cool, inside the db file we have a base64 encoded password for user `ArkSvc`


![b596ccf464afa83ef39b9c681079f6cb.png](img/b596ccf464afa83ef39b9c681079f6cb.png)

But we are unlucky because the password doesn't seems right for now

![8121e6bd5492698f2c7dcd3f79bc8690.png](img/8121e6bd5492698f2c7dcd3f79bc8690.png)

I think we need to investigate more the `CascAudit.exe` which is compiled with .NET
After decompiling the .exe file wecan cleary see the hardcoded key used for encrpytion

![a9b477f94cdc5fe2ed1872ef93b18eda.png](img/a9b477f94cdc5fe2ed1872ef93b18eda.png)

Look up at the DLL we have a simple (thanks to the hardcoded IV) decryption algorithm

![4705571eaa4c996f56d83b4d806062f2.png](img/4705571eaa4c996f56d83b4d806062f2.png)

Just using a .NET online decompiler is enough to get the plaintext password for the user 

![003fb5c4fc6d40dcd1981854c5c73005.png](img/003fb5c4fc6d40dcd1981854c5c73005.png)


Now we are inside as `ArkSvc` and look here, is cool, we are part of a new group called **<u>AD Recycle Bin</u>** and remember the email found before? It was talking about a temporary account that after the use is going to be deleted, as part of this group we can read deleted objects using a simple one liner

```Get-ADObject -filter 'isDeleted -eq $true' -includeDeletedObjects -Properties *```

and yes sir, we have the password of `TempAdmin`

![8bd12c60abbd7dc07372ae58a70c77d7.png](img/8bd12c60abbd7dc07372ae58a70c77d7.png)

At first I tought it was in plaintext but in reality was decoded with base64 and the real password is `baCT3r1aN00dles` which fit for the Administrator user, we rooted it!
