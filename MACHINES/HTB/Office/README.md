# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; HTTP (APACH 2.4.56)
* **88** &#8594; KERBEROS
* **139 / 445** &#8594; SMB
* **389 / 636** &#8594; LDAP
* **443** &#8594; HTTPS
*  **5985** &#8594; WINRM

<br><br><br>

# ENUMERATION & USER FLAG
Port HTTPS start with a 403 error while the simple HTTP port return with a blog website

![eb4a1b1a6dcaeeba9890b9549b62524a.png](img/eb4a1b1a6dcaeeba9890b9549b62524a.png)

Using some traditional enumeration tool (like GoBuster or FFUF) is not working, after a few attempts we get blocked somohow and we can't access the website for a while. With some manual enumeration I found that **<u>Joomla</u>** is the CMS used (just take a look at the website tab-logo or the source code) and with droopescan I was ablot to retireve an endpoint that contains the **<u>Joomla version</u>** at `/administrator/manifests/files/joomla.xml`

![ca9a2084aa4adece5821bf241f2c9470.png](img/ca9a2084aa4adece5821bf241f2c9470.png)

This specific version is vulnerable to [CVE-2023-23752](https://nvd.nist.gov/vuln/detail/CVE-2023-23752), this is the second time I encounter this vulnerability and instead of do it manually I want to try this [PoC](https://github.com/Acceis/exploit-CVE-2023-23752) to exfiltrate some juicy information

![3a986a187be4118c2287cf2709cf50a5.png](img/3a986a187be4118c2287cf2709cf50a5.png)

All right, this is cool! Both username displayed doesn't work with the password on the login page and brute force it with hydra would just lock us outside. SMB turned to be usefull using a long list of username (provided by SecList) I was able to find a pair of credentials that worked!

![7dab77803b12bc1357528d869544a7a0.png](img/7dab77803b12bc1357528d869544a7a0.png)

Inside `SOC Analysis` share we have a PCAP file with 1900 packets captured, interesting. I used [BustaPcap](https://github.com/abaker2010/bustaPcap) for some static analysis, this is pretty usefull especially for long PCAP and get an overview of the contents. Interesting we have some Kerberos Traffic in a small percentage

![18b9275f778720ba9b714a290236c97d.png](img/18b9275f778720ba9b714a290236c97d.png)

There are just 2 packets which are AS-REQ data so they contins an hash that we can crack (cipher value). The hash is of the `tstark` user which belong to `office.htb` and use the mode 18 of kerberos (`-m 19900` on hashcat)

![4543f96b51f359d6fcc08e744a9fd908.png](img/4543f96b51f359d6fcc08e744a9fd908.png)

After setting the right hash, hashcat would able to retrieve the plaintext

![9b541c865f3b5c72bb73d5810faa7b6c.png](img/9b541c865f3b5c72bb73d5810faa7b6c.png)

This password work with `administrator` user on the admin dashboard, now is pretty straightforward and common with Joomla. I am going to cange a template php page into a php reverse shell

![777a401ddeb1b425060021ff80a9fd8f.png](img/777a401ddeb1b425060021ff80a9fd8f.png)

After that just visit `/templates/cassiopeia/error.php` will trigger the reverse shell

![841c9fda2c08307ff57ba9641100e5fe.png](img/841c9fda2c08307ff57ba9641100e5fe.png)

We are inside as the service account `web_account` so we need to leverage a little more to grab the user flag. But we already have the password for `tstark` taht we retrieved thanks the kerberos hash on the PCAP file so we can use `RunAScs.exe` to read the FLAG

![a5bc51e83a61c1a21922e6a47fb03c7f.png](img/a5bc51e83a61c1a21922e6a47fb03c7f.png)

Now I used the same approach to have areverse shell as tstark and continue my journey on this machine.

<br><br><br>

# PRIVILEGE ESCALATION
The machine from the inside looks pretty empty, no interesting software installed, scheduled task or something that catch my eye for a potential privilege escalation. MySQL (xampp) can be accessed just within the machine itself so I tried to use the credential I've found before, luckly it worked (I'v spawned a [ConPtyShell](https://github.com/antonioCoco/ConPtyShell?tab=readme-ov-file) to achieve full powershell interaction)

![4c8e2c8c46af7aa803e468a05f8fab90.png](img/4c8e2c8c46af7aa803e468a05f8fab90.png)

After a few research I was able to abus User Defined Functions (UDF) and get `NT AUTHORITY` access, first we need to locate the plugins directory used by MySQL in order to know where we have to upload our malicious DLL

![448cf2515e3bca56e815f44b975a2525.png](img/448cf2515e3bca56e815f44b975a2525.png)

In that directory I have uploaded the DLL in [this repository](https://github.com/koparmalbaris/MySQL-UDF-Exploitation) (`sys_64.dll` version) which also include a cheatsheet of the command we need to use to load the UDF into mysql, `sys_eval` is the one I am pointing to so we can execute OS command.

![096b4b18528df365d5768cde25bdc542.png](img/096b4b18528df365d5768cde25bdc542.png)

I tried with a simple `whoami /all` and we are impersonating again `web_account` user but with the infamous **<u>SeImpersonatePrivilege</u>** privilege enabled, this is perfect!

![ad43d6363186368c75bfe7a1f0597a72.png](img/ad43d6363186368c75bfe7a1f0597a72.png)

I get reverse shell on this uer and simply used run [GodPotato](https://github.com/BeichenDream/GodPotato) to execute command as root!

![d93d342f7cf6e52edd11045151be5779.png](img/d93d342f7cf6e52edd11045151be5779.png)
