# PORT SCAN
* **53** &#8594; DNS
* **88** &#8594; Kerberos Key Distribution Center
* **135** &#8594; Microsoft RPC
* **139** &#8594; Netbios-SSN
* **389** &#8594; LDAP
* **445** &#8594; SMB
* **464** &#8594; KPASSWD
* **593** &#8594; Microsoft RPC over HTTP
* **636** &#8594; LDAP over SSL/TSL
* **3268** &#8594; LDAP catalog for AD
* **3269** &#8594; LDAP catalog for AD over SSL
* **5985** &#8594; UPnP 

<br><br><br>

# ENUMERATION & USER FLAG
Now I am starting to understand (a minimum) of Windows Pentest so first thing first **<u>SMB</u>**

![dab945f824119fdbd61accc467c7ba88.png](img/dab945f824119fdbd61accc467c7ba88.png)

Obviously let's check the `support-tools` directory

![8436f95318667fd10d119877312699c6.png](img/8436f95318667fd10d119877312699c6.png)

Wow pretty neat directory here, almost all the the executable/files are well known software but UserInfo.exe.zip is not. Let's download and extract it

We can know that we have a **<u>.NET binary</u>**

![8dd799344802668b841b5922f7e00b0d.png](img/8dd799344802668b841b5922f7e00b0d.png)

Running the `strings` command we have something that seems and ID and a Windows Path

![edc4403a2f7a37d312d8a2a311479794.png](img/edc4403a2f7a37d312d8a2a311479794.png)

There should be more juice within this binary so I discovered **<u>ILSpy</u>** to reverse the `UserInfo.exe`

<br><br>

First of all the ID previously found appear to be a GUID

![8aec6540cf6c424d65970536b147f706.png](img/8aec6540cf6c424d65970536b147f706.png)

In `LdapQuery()` we can have some info about the user that appears to be used to login in **<u>LDAP</u>**

![89b1e820eafafd269b17c0e5844bd4aa.png](img/89b1e820eafafd269b17c0e5844bd4aa.png)

But here's the prize, a encripted password in the protected class with the key, PERFECT!

![9bef818870a12c1b4a61f62a1aa24b23.png](img/9bef818870a12c1b4a61f62a1aa24b23.png)

I used ChatGPT to explain me the behaviour of the code

![25411a0f05f909714f4a012bbbaa25a1.png](img/25411a0f05f909714f4a012bbbaa25a1.png)

<br>

***
Just a little digression about using IA, I don't think using chatGPT (or whatever GPT available) is cheating especially in this type of scenario, it gives valuable information with a quick response so I can save time and work on the box itself. Obiously the information are really generic (especially in complex topics like reverse engineering) and we don't have to abuse the results pretending they are absolute truth. Make your own research and deep the task yourself we don't have overstimate the IA but not be left apart in the industry etiher, is a cool thing to understand how to use new tools/software especially in the security field!
***

<br>

So we have a really simple and weak encryption, I tell to my IA friend to recreate this code in python so I can run locally and check the results 

![340d4fdad6cfe83be5bcec15f6e1ae3d.png](img/340d4fdad6cfe83be5bcec15f6e1ae3d.png)

Cool stuff let's see if is right with `ldapsearch` and analyze the output aiming for something like a password or similar. There is a field that usually (bad trained) sysadmin use to put sensitive info thinking that without a sysadmin privilege nobody can read it, I'm talking about `info` field. Well we can read it :P

![a13b2625d2a0b11c46b564e0e9627051.png](img/a13b2625d2a0b11c46b564e0e9627051.png)

We have access through **<u>WinRM</u>**

![c6f08ba8afda75e745cee21903cdadd5.png](img/c6f08ba8afda75e745cee21903cdadd5.png)

Grab the flag and move on the next step

<br><br><br>

# PRIVILEGE ESCALATION
As `support` user we are part of an interesting group

![e13d93bb17b7beef36cee432340083cc.png](img/e13d93bb17b7beef36cee432340083cc.png)

That's cool, now let's check some of Active Directory information

![9478a49aa19009bf26a97d0cbde36a13.png](img/9478a49aa19009bf26a97d0cbde36a13.png)

We now know that we the group found above is a `Security` group 

![9776d5f7d3d6e17001bc4a59f3e1aecf.png](img/9776d5f7d3d6e17001bc4a59f3e1aecf.png)

Is time to use my favorite pentest tool for Active Directory, **<u>Bloodhund</u>** so we can see what type of privilege we have on the Domain Controller

![c4a47a65f79612fd69aa90b3f1ae38bd.png](img/c4a47a65f79612fd69aa90b3f1ae38bd.png)

We have a `GenericAll` privilige on the DC of `support.htb`, this mean **<u>Resource Based Constraint Delegation Attack</u>**

Here's the step-by-step approach

<br>

### 1) **<u>Create a new Computer Object</u>** 
I used impacket because I am not that good in powershell yet :(

![f081b84621dbdbca2fd911a6b14bdf5a.png](img/f081b84621dbdbca2fd911a6b14bdf5a.png)

We can check the existence of this new object directly on the victim machine

![82a954beaf4e541a1a88b68d542d8b60.png](img/82a954beaf4e541a1a88b68d542d8b60.png)

<br>

### 2) CONFIGURING <u>RBCD</u>

We need to set the `msDS-AllowedToActOnBehalfOfOtherIdentity` attribute to a computer in our controll (malpc). In this way Kerberos will be responsible to authenticate our account and later we can check some SPN that are available to abuse

![f42c19cf9102b6aa1df2d44e74b21689.png](img/f42c19cf9102b6aa1df2d44e74b21689.png)

With the `PrincipalsAllowedToDelegateToAccount` attribute set we are setting `msds-
allowedtoactonbehalfofotheridentity` too

Let's check the results

![515d6f001660751e2b10b51da94eff84.png](img/515d6f001660751e2b10b51da94eff84.png)

And on the DC side the SecurityIdentifier is the same as the SID of the malpc

![6d553a9fbdcb26efe0475c35d845b458.png](img/6d553a9fbdcb26efe0475c35d845b458.png)

<br>

### 3) S4U ATTACK
Service-for-user (S4U) attack abuse the Kerberos delegation (especially unconstrained delegation!) to generate a ticket of Administrator

First thing first let's obtain the password hash of `malpc`, using [Rubeus](https://github.com/GhostPack/Rubeus)

![dd0fbed877834be741fc57425b439e4f.png](img/dd0fbed877834be741fc57425b439e4f.png)

Using the `rc4_hmac` we can get the ticket

![7875abfe70d864dd3722cc4887a5891f.png](img/7875abfe70d864dd3722cc4887a5891f.png)

The last ticket (which impersonate) the **Administrator** account is what we need

![d39f47e6fcb525fa60f1e07169d807f4.png](img/d39f47e6fcb525fa60f1e07169d807f4.png)

I saved the ticket locally and decode it in a new ticket file and use a metasploit module to convert in **<u>ccache</u> format** 

![6f6e501014415e523fd4088477ca9664.png](img/6f6e501014415e523fd4088477ca9664.png)

Now we get a shell using **PsExec** with the `KRB5CCNAME` variable set to our final ticket

![34e47c6877bc297dfdcd64b1892094da.png](img/34e47c6877bc297dfdcd64b1892094da.png)

Cool stuff and cool box, I admit I used the writeup to get priv esc but I learned this cool vulnerability abuse, really usefull and formative!

We get the root flag and we are done, I will check some videos about this type of abuse to have a better understanding even in different scenario
