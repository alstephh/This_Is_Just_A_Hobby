# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; HTTP
* **88** &#8594; KERBEROS
* **135** &#8594; RPC MAPPER
* **139** &#8594; SMB over NBT
* **389** &#8594; LDAP
* **445** &#8594; SMB over TCP
* **593** &#8594; RPC
* **3268** &#8594; LDAP
* **5985** &#8594; WinRM
 
 <br><br><br>
 
 # USER FLAG
 The box is a domain controller on the `EGOTISTICAL-BANK.LOCAL` domain, starting from the HTTP port we have a static site without user interaction (no form, no uploads, no nothing). In the `/about` page we have the team employee names
 
 ![d49734dd275a6873b9c3ccd60d73ff0d.png](img/d49734dd275a6873b9c3ccd60d73ff0d.png)
 
 This name nad surname can quickly become a list of potential valid username, I found [this usefull script](https://github.com/florianges/UsernameGenerator) using a text file with name and surname will output a text file with common combination used for username creation
 
 ![991195b02949168ccd8552192adaaa4d.png](img/991195b02949168ccd8552192adaaa4d.png)
 
 Now I was able to perform ASREP roasting using impaket script `GetNPUsers`, the `fsmith` user is vulnerable and we have 2 different ASREP password hash 
 
![9ccb7b04ceac5237988b3da30d7ec4be.png](img/9ccb7b04ceac5237988b3da30d7ec4be.png)

Both return the same plaintext password (hashcat)

![25cfcc9be40c1332eb348541349af376.png](img/25cfcc9be40c1332eb348541349af376.png)

Now we can login through WinRM and get the flag

![b378c30bc2851ac6e3b8b0c7dc219096.png](img/b378c30bc2851ac6e3b8b0c7dc219096.png)

<br><br><br>

# PRIVILEGE ESCALATION
Inside the machine we can know that `svc_loanmanager` use AutoLogon and we can extract teh dault password

![40764297d3bbde6e6a9fd488d3a7f215.png](img/40764297d3bbde6e6a9fd488d3a7f215.png)

Cool, a service user can become really usefull if can get controll of it. There is no user called `svc_loanmanager` but we have one pretty close to it `svc_loanmgr`, we can try this password on this username on Evil-WinRM to get access

![6f9f7cb2045069b10d9e83e9ba9218f9.png](img/6f9f7cb2045069b10d9e83e9ba9218f9.png)

Now I just extractd teh info for bloodhound and look at the shortest path to get controll of the domain

![bd8c8ff941ca92987e0ab7a5ef162fe1.png](img/bd8c8ff941ca92987e0ab7a5ef162fe1.png)

This mean we can do a DCSync attack using `secretsdump.py`

![9cdd6b330d06e4c6eea0839d88e3b2b6.png](img/9cdd6b330d06e4c6eea0839d88e3b2b6.png)

Cool we have the administrator hash we can try and use it on evil-WinRM to get a shell

![e5a279b9b74b1dfcc30a94672160eec8.png](img/e5a279b9b74b1dfcc30a94672160eec8.png)

Grab the flag and take the reward!
