# PORT SCAN
* **22** &#8594; SSH
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **5985** &#8594;WinRM


<br><br><br>

# ENUMERATION & USER FLAG
SMB, as always good thing to start and we can login as anonymous

![4c9f01f31acea4cf867ddceddcf97d9d.png](img/4c9f01f31acea4cf867ddceddcf97d9d.png)

We have some txt files and a directory 

![ee6e6f2b6eb28ac6d1db5a86b0916c73.png](img/ee6e6f2b6eb28ac6d1db5a86b0916c73.png)

Inside we have a backup directory which contain a `VHD` file (Virtual Hard Disk). Time to learn how to approach this file and maybe retrieve something usefull

1) I have mounted the remote directory 
![40c036605f1ffb98eddb7b78c33eb67c.png](img/40c036605f1ffb98eddb7b78c33eb67c.png)

2) Now we have the VHD file ready to be mounted (if I just downloaded it as normal SMB files they will be empty but here is not the case)
![0166845934902d14fcb0b64a65571169.png](img/0166845934902d14fcb0b64a65571169.png)

3) To mount a remote filesystem linux have ready for us a command called `guestmount`
![62da01492dab27b9816ada7f1bb1673a.png](img/62da01492dab27b9816ada7f1bb1673a.png)

<br>

Now we are inside the virtual hard disk let's search for a SAM file. On the default location `Windows\System32\config` we have what we need

![cfaf520eb923995fb2094c2e766025e4.png](img/cfaf520eb923995fb2094c2e766025e4.png)

Now we need to take both **<u>SAM</u>** and **<u>SYSTEM</u>** file in order to use `samdump2` and retrieve the L4mpje hash

The <u>**SYSTEM**</u> file store information abut windows registry and is needed to access the crypto used to decrypt the hashes inisde SAM

![bd476ab677a5183f53199a0e2fcc9322.png](img/bd476ab677a5183f53199a0e2fcc9322.png)

Using hashcat we have the plaintext password we can use on ssh and grab the user flag

![ef94c7562826391eda1f17e3849a6ff3.png](img/ef94c7562826391eda1f17e3849a6ff3.png)

<br><br><br>

# PRIVILEGE ESCALATION
Inside `Program Files (x86)` I note we have a software called **<u>mRemoteNG</u>** which is a manager of different remote location (like VNC). Would be cool to check the version
Reading the file `Changelog.txt` we have the version 

![97a6bfcd4c50245909449528af12db52.png](img/97a6bfcd4c50245909449528af12db52.png)

This is vulnerable to plaintext password retrival

![421ac5687ce2f02ea88dd5babe5f0341.png](img/421ac5687ce2f02ea88dd5babe5f0341.png)

There a lot of method to retrieve it I decide to retrive manually the Administrator passwrod stored inside `L4mpje/AppData/mRemoteNG/confCons.xml`

![f2f7e78386265c70b9176e68c46d9f3d.png](img/f2f7e78386265c70b9176e68c46d9f3d.png)

Than I found [this PoC](https://github.com/haseebT/mRemoteNG-Decrypt) online so we just need to copy and paste the strings and will be decrypted

![9708406e3f1b3e442a266728cf488f9f.png](img/9708406e3f1b3e442a266728cf488f9f.png)

Essentially, the password is encrypted with a costum password but if is not decided a standard one is used which is the case here.

Cool we have the Administrator password to use on SSH and we are free to grab the flag

![8c7ca0e834cdabf6eed68f8d0bd0f3f9.png](img/8c7ca0e834cdabf6eed68f8d0bd0f3f9.png)
