# PORT SCAN
* **445** &#8594; SMB
* **4836** &#8594; Reporting Service V1.2

![bda09672ae8c6603981a02d7b0d62efc.png](img/bda09672ae8c6603981a02d7b0d62efc.png)

<br><br><br>

# ENUMERATION & USER FLAG
Uau a Windows Machine with just 2 ports open, rare!
The SMB accept anonymous login and we can read `Data` and `User` shared folder

![a3b17144b7aa73c728af5bd3c5479a07.png](img/a3b17144b7aa73c728af5bd3c5479a07.png)

Let's dive in! Starting with `Data`

![4f63a3b1b2b3d6a8ad2c83286b4b99f6.png](img/4f63a3b1b2b3d6a8ad2c83286b4b99f6.png)

All but `Shared` cannot be access with anonymous permission and I found the following files

![6a60422b4081acd3b76dda29bcb345de.png](img/6a60422b4081acd3b76dda29bcb345de.png)

Uau cool stuff we have a temp user credentials but I want to try to enumerate `User` directory before trying to use it some how

![27ccb42d528f5b49200182ff2892a236.png](img/27ccb42d528f5b49200182ff2892a236.png)

Ok cool stuff seems that we will deal only with this 2 ports without shell of some sorts, maybe(?). We can't access none of this directories as anonymous so we will use the `TempUser` and check his directory. Within it we just have a single TXT file

![31279ed2f623cdd835a928494fedee59.png](img/31279ed2f623cdd835a928494fedee59.png)

And this is empty...WTF! (I used exiftool to read metadat but nothing lol)
Don't lose the hope because we have access to the `Secure$` shared folder

![6e75008bfb467f9c9a4327af4a6a1171.png](img/6e75008bfb467f9c9a4327af4a6a1171.png)

But we cant access anything here...let's take a look at other shared folder and see if we can light up folder that are still in the shadow 

For instance now we can access the `Data/IT` folder

![7fbd000b9cdac1c0cc1c160d8cd8c6ee.png](img/7fbd000b9cdac1c0cc1c160d8cd8c6ee.png)

Interesting stuff right here 

![7e7092ec3717160ea8e1832c9668d6fd.png](img/7e7092ec3717160ea8e1832c9668d6fd.png)

`config/RU Scanner` try to hide something from us :)

![a6e6fb69dbedfe0693b3107fd73324a7.png](img/a6e6fb69dbedfe0693b3107fd73324a7.png)

This seems encrypted password and at the same time encrypted so not easy to brute-force it 

![24d0884e7e9219541761df4c16f5b039.png](img/24d0884e7e9219541761df4c16f5b039.png)

Another file that catch my eye was `Configs/NotepadPlusPlus/config.xml` especilly the <u>Find History tag</u> 

![5c77ecc5584739d772b1e3b9480a6155.png](img/5c77ecc5584739d772b1e3b9480a6155.png)

Cool let's try for the `Secure$` folder

![4bb1dec229662a23cc65c9ad70c51050.png](img/4bb1dec229662a23cc65c9ad70c51050.png)

`Docs` have something new for me

![a435b2ebccfaea19592b79423e8b2659.png](img/a435b2ebccfaea19592b79423e8b2659.png)

What is really cool here is the `VB Project` which contain a lot of Visual Studio files and we will aim here is the `Module1.vb`

![ab6599446500c4792cfa0c2ab67665c2.png](img/ab6599446500c4792cfa0c2ab67665c2.png)

Let's use Visual Studio and try to use this code to decrypt the string we have.

[dotnetfiddle](https://dotnetfiddle.net/) will be our buddy now, I copied the content of `Utils.vb` and added a Main() method printing the DecryptString output given the crypted string

![0b346f6d8b3c09fc7c50b0ccaac311cb.png](img/0b346f6d8b3c09fc7c50b0ccaac311cb.png)

`xRxRxPANCAK3SxRxRx` is the password now let's use it and see if we can grab the user flag for `c.smith` user

![9f3a855858b772f45a609fbe2b2c7afb.png](img/9f3a855858b772f45a609fbe2b2c7afb.png)

Gottch'a let's submit it and move on!

<br><br><br>

# PRIVILEGE ESCALATION

Not only the flag was present on the C.Smith folder but also a `HQK Reporting` direcotry (maybe now we will deal with the custom service on <u>port 4836</u>)

We have a XML configuration file

![458c8aa6241d8f4865dfefc042fbf3fe.png](img/458c8aa6241d8f4865dfefc042fbf3fe.png)

The `exe` running behind the curtains

![7751097034c17c3d3af69090570f9549.png](img/7751097034c17c3d3af69090570f9549.png)

and a empty txt file (again oh god)

![74ee4d350bffc5e36361e479a67b4792.png](img/74ee4d350bffc5e36361e479a67b4792.png)

But here's a little trick, if we use `allinfo` pointing the TXT file on the smbclient prompt we can see the alternative data stream in this case we have the standard `DATA` and `Password`

> Alternate Data Stream (ADS) are a feature of NTFS file system created and designed to improve compatibility of Windows and Macintosh system that use Hierical File System (HFS). HFS use streams (or fork) linked to a single name on the Macintosh file system and usually store metadata but also independent data streams to an NTFS file ([resource](https://vox.veritas.com/t5/Articles/What-you-need-to-know-about-alternate-data-streams-in-windows-Is/ta-p/807740))
> Also ADS are described as a security vulnerability and something that can be used by attackers like a "hidden compratment within a suitcase", [this article](https://blog.netwrix.com/2022/12/16/alternate_data_stream/) is really cool and exhaustive

We can use a simple GET command with a little enachment in order to retrieve the stream of our choice

![1782f61c7362dc31ea0a536bf9c9f6bf.png](img/1782f61c7362dc31ea0a536bf9c9f6bf.png)

![c9b104608608d16015db0552cbc24332.png](img/c9b104608608d16015db0552cbc24332.png)

Cool we have a password now and seems to be used for DEBUG option on the service on 4386

![88aa7ec0c31a8aaaa115bdb9450b88d4.png](img/88aa7ec0c31a8aaaa115bdb9450b88d4.png)

We have 3 Query

![303beba363a75eb6ba057ab21fd5d10c.png](img/303beba363a75eb6ba057ab21fd5d10c.png)

We are currenly in the `C:\Program Files\HQK\ALL QUERIES` directory so let's take a step back in the tree and use `LIST` command again

![efa48a5e688ef98ed36019cc7d30f3d2.png](img/efa48a5e688ef98ed36019cc7d30f3d2.png)

uu LDAP seems cool 

![76f82a582b887fc26c9f1573b81372db.png](img/76f82a582b887fc26c9f1573b81372db.png)

Again we have a password that seems in the same format as before but looking at the `EXE` (decompiled with dnSpy) we have the optionsu sed to encrypt the password

![dacaad962fdc66b72159916c5294fa55.png](img/dacaad962fdc66b72159916c5294fa55.png)

Inside the **CR** class we have all the functions (in `C#`), so I create an exe putting all togheter and hardcoding the strings the results is our cool

![9f83f09b94e354d4e8eb4f6055e10421.png](img/9f83f09b94e354d4e8eb4f6055e10421.png)

Now with a simple PSEXEC we have access as root!

![2d12aa95be1ba3376c051f8de13e0120.png](img/2d12aa95be1ba3376c051f8de13e0120.png)
