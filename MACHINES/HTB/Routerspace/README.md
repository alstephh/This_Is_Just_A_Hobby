# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP 

<br><br><br>

# ENUMERATION & USER FLAG

The homepage is a brochure for a mobile app which permit to connect your smartphone to your router, nothing interesting in the site itself but on the `apk` download (`RouterSpace.apk`).

This is the frist time I play with this file extension, so I used to decompile it with `apktool`

![9032ebbf991545c4bd84735dc8b35a75.png](img/9032ebbf991545c4bd84735dc8b35a75.png)

I tried to look inside for some information or, in the best scenario, hardcoded credentials but nothing the APK internal is pretty dense (and obfuscated) so we can try to emulate it to perform some dynamic analysis...but how? There are a lot of ways and honestly talking I look at walkthroughs made by other to find the way suitable for me. I opted to use `Genymotion` in combination with `adb` (as always installation phases are skipped because is not relevant at all, I am not a teacher lol)

While I download a Mobile instance with **Genymotion** we can extract and manipulate the burpsuite certificate which can be used by the mobile device and intercept the request with burpsuite. Most of the times vulnerabilities in Android or IOS involve API that's why we need burpsuite to work with the mobile device

![d7e20e3fd48cd9d36512804b28461c2f.png](img/d7e20e3fd48cd9d36512804b28461c2f.png)

Now is time to import the certificate in the android device and this is where `adb` will kicks in, genymotion attach automatically the virtualized devices to adb (which is great!)
First we can change the fylesystem permission adding write capabilities (as standard is read-only) and "push" the certificate we made

![7e2dc4e119164cbaf0136b8afab3f9c2.png](img/7e2dc4e119164cbaf0136b8afab3f9c2.png)

We can check that everything is ok in `Trusted Credentials` section inside the security settings, PortSwigger should be present there

![8e47878d95ab049c16a176635187b934.png](img/8e47878d95ab049c16a176635187b934.png)

Now we can use the `adb shell` command to add the proxy pointing to BurpSuite (which we hav to change the proxy to be on all interface)

![1178a7490eca3deeb25f932b226c8e27.png](img/1178a7490eca3deeb25f932b226c8e27.png)

Now we are free to intercept the request with BurpSuite (easier than expected and I have learned quite few things!)

![3de00b50f6771f3d9b6f927ac3f999ad.png](img/3de00b50f6771f3d9b6f927ac3f999ad.png)

Now we can drag the apk from our machine to the mobile screen and will be automatically installed and run, we are good to go now we just need to fix the network error when we push the only button available `Check Status`

![29e0455ad8d3b436c872f274f0698b02.png](img/29e0455ad8d3b436c872f274f0698b02.png)

I tried different way to fix this but was easier and faster to run another device, with the standard `Google Pixel` now it works fine!

![738bbec3f04d2d32aa49b8b6c1d31509.png](img/738bbec3f04d2d32aa49b8b6c1d31509.png)

Cool stuff! Now we can intercept the request and put it on the repeater to see the response

![9b33e02f525f7ea5d46e0e75c76c6b29.png](img/9b33e02f525f7ea5d46e0e75c76c6b29.png)

Whatever is the strings on the `ip` field would be echoed back, I tried to do some OS injection and we have it!

![32ea3fe6f41b39c12b72640d509ea092.png](img/32ea3fe6f41b39c12b72640d509ea092.png)

Cool RCE is accomplished correctly now but we are not able to reach our attacking machine (`ping` and `wget` apparently doesn't work) but we can inject our ssh public key in the `authorized_keys`. Now we can ssh inside the machine and grab the flag!

![a4a601fbd74c2900a71535621c63f31f.png](img/a4a601fbd74c2900a71535621c63f31f.png)


<br><br><br>

# PRIVILEGE ESCALATION
We still can't ping the attacker machine but SSH should be fine alone if we need to upload or download files. By the way the target system seems pretty clean, no special capabilities or SUID binaries probably we need to perform some **<u>Linux Exploit</u>**

LinPeas suggest [GNU Screeen 4.5.0](https://www.exploit-db.com/exploits/41154) but is not working instead we can use a more common one, the [Sudo Baron Samedit](https://www.qualys.com/2021/01/26/cve-2021-3156/baron-samedit-heap-based-overflow-sudo.txt) a heap overflow which affect `sudo` and used to spawn a shell as root

We can easily check if is vulnerable or not with this command, if it ask for the password we have a good chance we can exploit it

![0f45d84e42e4f5baadf6b4c67c53ffd5.png](img/0f45d84e42e4f5baadf6b4c67c53ffd5.png)

[This](https://github.com/worawit/CVE-2021-3156) is a cool repository with a lot of version of this exploit, the author suggest to use **<u>exploit_nss.py</u>** as first

![faeb6733c2e845cc29ff4416f94cc1b5.png](img/faeb6733c2e845cc29ff4416f94cc1b5.png)

Luckly is all for this box!
