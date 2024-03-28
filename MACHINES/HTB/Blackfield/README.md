# PORT SCAN
* **53** &#8594; DNS
* **88** &#8594; KERBEROS
* **135** &#8594; RPC
* **389** &#8594; LDAP
* **445** &#8594; SMB
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
A simple Domain Controller with nothing web-related (slay!) so we can start from SMB or LDAP checking for null sessions, the former is fine to accept anonymous login!

![efaa01506c434451ec81f6676ef68e87.png](img/efaa01506c434451ec81f6676ef68e87.png)

The `profiles$` folder contains a long list of hidden directory with what seems to be usernames (forensic's folder contents cannot be retrieved because we don't have the permissions)

![760d6268b171deb01ae527e79adfcc25.png](img/760d6268b171deb01ae527e79adfcc25.png)

Well is not that bad to have a list of username in AD so I took the list and use `cut` utility to take just the username and have a clean username list

![ba5a9142fbe3e31d9c9db93fb8597993.png](img/ba5a9142fbe3e31d9c9db93fb8597993.png)

Now we can have fun, I always like to start with a old trick of use `impacket-getNPUsers` to check if this usernames are valid or not inside the domain (blackfield.local) , well we filter the majority of the users in the list

![042b7c68e8d6f11b36482dac0f734352.png](img/042b7c68e8d6f11b36482dac0f734352.png)

We also have an hash of the user `support` that we can use to perform **<u>ASREPRoasting</u>**, it required just a few minutes but was worth it because now we have a plaintext password!

![4e1d51375ea2f665740204b60de825bc.png](img/4e1d51375ea2f665740204b60de825bc.png)

I tried to check if we can get inside the SMB share called forensincs but we don't have enough permissions right now, my intuition push me to think that the user `audit2020` is the one with such permissions but is just an hypothesis and we don't hack using hypothesis we need information! Well we have some valid credentials so we can retrieve information for bloodhound with `bloofhound-python`!
After firing up bloodhound I choose the `support` node and use the **<u>First Degree Object Control</u>** query that highlights a powerful action can be performed, we can change the password of `audit2020` 

![9bf5915f310b7178e1b87ff34a86491c.png](img/9bf5915f310b7178e1b87ff34a86491c.png)

We can do this using `net` command like the following line

`net rpc password "audit2020" "newP@ssword123" -U "BLACKFIELD.local"/"support"%"#00^BlackKnight" -S 10.10.10.192`

Now we can finally access the `forensic` folder and I use mount to get the contents of the folder localy (using **mget** on smbclient is too much heacy and get broken after the first 2 files), without listing all the contents of the 3 direcotry inside this share I have found a zip file that contains a **dump of the lsass process**

![51765f7b68e574111d1108bcffe90afc.png](img/51765f7b68e574111d1108bcffe90afc.png)

LSASS process store hashed credentials for SSO purpose and having a dump of it is really really rare and powerfull, in order to extract information from a DMP file in linux we can use the python version of mimikatz **<u>pypykatz</u>**. We have the plaintext of the machine account but is not working (keep in mind the change of this password should occur at least 1 time per year so no surprise that is invalid the one in the dump file), what's new is the <u>`svc_backup` **NT hash**</u>!

![cd0e1f12016ba25a186aa8a4375b615b.png](img/cd0e1f12016ba25a186aa8a4375b615b.png)

Easy and smooth we can log in as this account through winRM and get the first flag

![041adf51e6ec165d7853708477e45b4e.png](img/041adf51e6ec165d7853708477e45b4e.png)

<br><br><br>

# PRIVILEGE ESCALATION
Noiw is game over! The `svc_backup` user is a member of the **<u>BACKUP OPERATORS</u>** group, this means we can create a copy of every file in the domain controller for backup purpose. The first thing I tried was to copy the SAM, SYSTEM and SECURITY registry locally 

![6dc87558ba16a1e84e962360dda40842.png](img/6dc87558ba16a1e84e962360dda40842.png)

Unlucky when I use secretsdump on these file locally nothing return back so we need to go a little further

![9392fcc39e5157608e5a6f6ce42d0d85.png](img/9392fcc39e5157608e5a6f6ce42d0d85.png)

Well be part of this group also means that we have `SeDebugPrivilege` available so we can use **<u>robocopy</u>** to copy the desktop of the Administrator user

![0657e6eafddf99a1203c185d5196b3fb.png](img/0657e6eafddf99a1203c185d5196b3fb.png)

Pretty strange we cannot retrieve the contents of `root.txt` because of a denied access but we have `notes.txt` which maybe can help us!

![c4f79d9f14b570f52e0c1b81a6cea389.png](img/c4f79d9f14b570f52e0c1b81a6cea389.png)

Ok so the root flag is encrypted and maybe this is why we cannot backup it. But there are other methods that can abuse the backups privilege like you can see [here](https://book.hacktricks.xyz/windows-hardening/active-directory-methodology/privileged-groups-and-token-privileges#a-d-attack). We failed with the register credentials dump but don't lose the hope there are other tricks that can be done especially with this strong privilege. Now the plan is pretty clear we can try to dump the NTDS.dit file and try to dump it but we cannot simply copy it because is used by windows processes




![3e2c1d61023e7be4a298ce8e4699916a.png](img/3e2c1d61023e7be4a298ce8e4699916a.png)

Well this is pretty bad but we can circumnavigate that with **<u>shadow-copies</u>**, if we create a shadow copy of the C drive we would be able to retrieve the file we are targetting, So first thing first I create the text file that you can find below acting as configuration file for the creation of the shadow disk that would be created on the Z drive with `diskshadow.exe`

![d0adde09ca625a34ec7f328133ed5356.png](img/d0adde09ca625a34ec7f328133ed5356.png)

![ceff889749aa68ce7633b3cd605a8eec.png](img/ceff889749aa68ce7633b3cd605a8eec.png)

Now that the setup is done we can play with the built-in windows utility and get a copy of the NTDS.dit from the Z drive and put it on the C drive temp folder


![6099e39fb7890938523128b1e058c135.png](img/6099e39fb7890938523128b1e058c135.png)

Now we can download this file and use it with the **<u>SYSTEM</u>** file in order to dump all the hashes, including the administrator one

![89ebec832fbb9a813a61ae36dd14ad79.png](img/89ebec832fbb9a813a61ae36dd14ad79.png)

No need to mention what I have done after that, a simple pass the hash on EvilWin-RM and the read of the root flag!

![008f647fed8baf2aa93327668bf35f8a.png](img/008f647fed8baf2aa93327668bf35f8a.png)



