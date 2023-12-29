# PORT SCAN
* **80** &#8594; HTTP (IIS 10.0)

<br><br><br>

# ENUMERATION & USER FLAG
Port 80 redrect to the subdomain `forum.bart.htb` so let's add it to the hosts file and start enumeration 

![212be2bd31d00bd5d8500ef41ec650d1.png](img/212be2bd31d00bd5d8500ef41ec650d1.png)

A simple static brochure of a fictional company, on the footer we know is made by **<u>WordPress</u>** (this is good news for us). No other subdomain discovered as much as directory busting but seems strange because a `wp-content` directory should exist according to the source code

![de0de9a109854d5007b5b9351c472155.png](img/de0de9a109854d5007b5b9351c472155.png)

But when I try to access it an error raise (also `wpscan` say he can't find this directory)

![1c1258334bc1cbd293840dd01124efc4.png](img/1c1258334bc1cbd293840dd01124efc4.png)

By the way I have found some potential usernames (`d.simmons` and `h.potter`) in the source code looking at all the email present and always on the source code we can find the wordpress version

![ec46cb2afac3e38c7e9ef7e207b51d47.png](img/ec46cb2afac3e38c7e9ef7e207b51d47.png)

Another scan retrieve that `monitor.bart.htb` exist

![31f898dd93e92a22b7692cadbc865ca9.png](img/31f898dd93e92a22b7692cadbc865ca9.png)

Interesting stuff here we can make some bruteforce, I collected all username and used `cupp` to create a list, Hydra can't be used because every request will be considered valid (every request end with status `200`). Probably I need to make some tuningon hydra but I consider `burp intruder` a faster solution. I tried with the username list (**Cluster Bomb** attack) and the rockyou wordlist but nothing cool happen. So I will do something more tedious using the same list as password payload with some tuning on the `Resource Pool` to make it faster.

After a while the `harvey:potter` credential give us a unusual results

![b0dabe37a6f96c41cfec035e2f41904d.png](img/b0dabe37a6f96c41cfec035e2f41904d.png)

With this credentials we can get inside the dashboard

![02a45229cd990a683f0487b3f96a738c.png](img/02a45229cd990a683f0487b3f96a738c.png)

The dashboard monitor the status of `internal-01.bart.htb` which point to another login form

![33dc457d425dbf0688ccdc4f86c8393b.png](img/33dc457d425dbf0688ccdc4f86c8393b.png)

This doesn't look like a well done web-application looking at the poor style and bad graphics, I have come from the `StreamIO` machine so I have a good training on enumeration stuff

![932f552981489e4cc8e5db36f015583c.png](img/932f552981489e4cc8e5db36f015583c.png)

FFUF found the `register.php` page, we know from the login page source code that the 2 paramters are **uname** and **passwd**, with this information we can force a registration

![a588940c8e45a8a918be423c5849d9fb.png](img/a588940c8e45a8a918be423c5849d9fb.png)

I had some problemt just because the **content length** and **content type** headers need to be added after that we are inside the development chat

![642665bded11deacb8f10e8e16de6de2.png](img/642665bded11deacb8f10e8e16de6de2.png)

If we intercept the request when we press the `Log` button we can notice that the request is asking for a specific file and username

![3766de0cb953c4a955cd9bec846c2b3f.png](img/3766de0cb953c4a955cd9bec846c2b3f.png)

I tried with exfiltrate `/windows/wuin.ini`, it didn't not worked but raise an error telling us that we don't have enough permissions

![5855527d034406257ecb1fa24b50fc3d.png](img/5855527d034406257ecb1fa24b50fc3d.png)

Good news is that we can attack the application through `RFI`, I have checked asking for a resource hosted on my SMB server so I tried to host a PHP code with a RCE calling back to metasploit. It didn't worked but something curios happened...

![b20325fac40d576ae5defb3e6fd9ed2e.png](img/b20325fac40d576ae5defb3e6fd9ed2e.png)

The username and the `User-Agent` are appended in the PHP file, this is the behaviour of the **<u>log.php</u>** file.
Well the User-Agent can become a very strong poison since we can use it to inject PHP code inside it, a new php file can be created which will contain the PHP code

![a1471c7500e38a8553195c27ad8ca754.png](img/a1471c7500e38a8553195c27ad8ca754.png)

After that we can access the file and the RCE will be triggered!

![10c6dcc4351147802d8c56efad09b7aa.png](img/10c6dcc4351147802d8c56efad09b7aa.png)

Unfortunately the metasploit listener will not work (it happens since reliability as C2 of metasploit is not one of the best in the market) but we can use it for a simple **NC reverse shell**

![eca7dce0490ddee97de3b59bfd46288b.png](img/eca7dce0490ddee97de3b59bfd46288b.png)


Since there are no trails of the user flag  (obviously **seImpersonate** abuse will be skipped) we needto make some movement.
Defender is blocking the upload of **WinPEAS**, this explain why the metasploit payload was not working. Something different is needed, I used the [powerup](https://github.com/PowerShellMafia/PowerSploit/blob/master/Privesc/PowerUp.ps1) powershell script, I have appended at the end of the file `Invoke-AllChecks` and use **IEX** to execute it directly in memory

![59faf2fee677ade2fdff2a00e9068d86.png](img/59faf2fee677ade2fdff2a00e9068d86.png)

Well was worth trying this little trick I think we need to make some manual enumeration in order to find something and the IIS directory is a good place to start. We have some harcoded database credetials in the `include` directory of the `internal-01` subdomain

![39699cd11e4f6aaba499920e240c1439.png](img/39699cd11e4f6aaba499920e240c1439.png)

With some quick MySQL commands we are able to identify some precious hashes

![66a009df17e2bd6bd20946baeccbab77.png](img/66a009df17e2bd6bd20946baeccbab77.png)

Duting webapp enumeration I saw that anPHP file in `includes` are used on the password to validate, maybe it will not just validate the length of the password

![ce2ecf1c4e40856fb6f463e2b5f9b4ee.png](img/ce2ecf1c4e40856fb6f463e2b5f9b4ee.png)

Gottch'a some salt has been added before hash the plaintext this is good to know! I added the salt in the hash file and the first one (`harvey` user) has been successfully cracked with **-m 1410**

![5c2abc12ae93901d01ba2522befe4ca7.png](img/5c2abc12ae93901d01ba2522befe4ca7.png)

Harvey in the windows machine should be `h.potter` so we can create a new Powershell Session with this password

![76fc093590ad6e446f84a3cbcad19540.png](img/76fc093590ad6e446f84a3cbcad19540.png)

Now we can use a base64 encoded powershell reverse shell to impersonate this user which own the user flag

![e3f63daa573f2f42ea841b92b322c94d.png](img/e3f63daa573f2f42ea841b92b322c94d.png)

<br><br><br>

# PRIVILEGE ESCALATION
Since I have prepared **<u>PowerUp.ps1</u>** for the previous user is still worth trying for `h.potter`, maybe he have enough permissions to look further. This time will not be so innocent :)

![f2c0a43a6d57fb3998efc43409362412.png](img/f2c0a43a6d57fb3998efc43409362412.png)

We have credentials in the **<u>Autologon registry</u>** we can spawn another PowerShell instance and use it to call a reverse shell but this time as `Administrator` but before let's check if the password is still in use

![f37640b4ea1e006f10436d40056dd1af.png](img/f37640b4ea1e006f10436d40056dd1af.png)

Cool, we are free to grab the root flag right now!

![8e6ca549f26db95499a1040b4484c254.png](img/8e6ca549f26db95499a1040b4484c254.png)
