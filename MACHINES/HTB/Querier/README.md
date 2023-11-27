# PORT SCAN
* **135** &#8594; MSRPC
* **139 / 445** &#8594; SMB
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
Cute scan, really simple ports and we are lucky since we have a WinRM port for obtain shell access. On SMB through anonymous login we have the `Reports` directory let's get inside and jump in

![b3e041e189883ac2cc3ba0abc77e8933.png](img/b3e041e189883ac2cc3ba0abc77e8933.png)

WIthin the directory we have a excel file (`.xlsm`), opened that with a online viewer appears to be empty but using strings we have a interesting `.bin` file 

![e38f6a46c1e57a58c88b76c938bff4fb.png](img/e38f6a46c1e57a58c88b76c938bff4fb.png)

If you are keen enough with excel as attacker or defender you should hae already heard about **<u>macros</u>**, this should definetly the way let's see how we can access it. On [this stackoverflow question](https://unix.stackexchange.com/questions/494917/view-excel-files-in-linux-command-line-without-installing-any-packages) we have something interesting 

![cb038b4890009a903cdd9ecd8d091f18.png](img/cb038b4890009a903cdd9ecd8d091f18.png)

Interesting stuff so we can use unzip on the file we have retrieved from the SMB folder

![ec54570c89b195d33080f0a3256f8241.png](img/ec54570c89b195d33080f0a3256f8241.png)

Cool now we can read the `.bin` file

![982d2c4f25a74ef8f656e42f6bb771d9.png](img/982d2c4f25a74ef8f656e42f6bb771d9.png)

The username seems to be `reporting` adn we can use the `mssqclient.py` script of impacket to connect on the remote machine database. Inside is just a dummy DB with nothing inside but the default databse, **xp_cmdshell is disabled** and can't be activated with the current user, I have been in a similar situation already and we abuse `xp_dirtree` on a smb server we controll to get the hash of the user. In this case we have captures the `mssql-svc` hash

![1a7338b430ce2f4b205ce24e4ef74a06.png](img/1a7338b430ce2f4b205ce24e4ef74a06.png)

Really easy to crack

![eb79660967a511adc179222b179cf87f.png](img/eb79660967a511adc179222b179cf87f.png)

We can't login directly on WinRM but we can reuse this credentials to get access on a privileged instance of mssql and enable `xp_cmdshell`

![5b8f778c7557af71349a1bd45a9b44c6.png](img/5b8f778c7557af71349a1bd45a9b44c6.png)

Everything cool but we have a problem here, we can't call a (simple) reverse shell because is blocked by Defender

![a9c62e8f7d060a160f57929650242ea2.png](img/a9c62e8f7d060a160f57929650242ea2.png)

But using directly [nishang](https://github.com/samratashok/nishang) will trigger the reverse shell, now we are inside time to grab the flag!

![3cea1627b529fa5bc3aa87dfc9737a60.png](img/3cea1627b529fa5bc3aa87dfc9737a60.png)

Another simple way was to host a `nc.exe` file and run directly from kali through SAMBA (this solution was widely better because I had issue to run script on previous shell)

![b352088e1883d90c77ff09e6430031b4.png](img/b352088e1883d90c77ff09e6430031b4.png)



<br><br><br>

# PRIVILEGE ESCALATION

Here was easy than expected, just run a WinPeas instance and will retrieve for use some GPP stored passwords, we are lucky and are ownd by the administrator

![6699904ec6c263bcad47daded33b7372.png](img/6699904ec6c263bcad47daded33b7372.png)

Yes just like that! We can finally use evil-WinRM and get access as administrator (pretty easy for a medium)

> The basic user have the **SeImpersonatePrivilege enabled** but I have decided to avoid to exploit that because is something that I have used too many times and if possible I would like to change attack paths in order to achieve a better understanding of Windows Internal 
