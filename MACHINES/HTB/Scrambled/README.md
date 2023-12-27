# PORT SCAN
* **53** &#8594; DNS
* **80** &#8594; HTTP (IIS 10.0)
* **88** &#8594; KERBEROS
* **135** &#8594; MSRPC
* **139/445** &#8594; SMB
* **389/636** &#8594; LDAP
* **1433** &#8594; MSSQL
* **4411** &#8594; ???
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
The domain of the target is `scrm.local` so it's time to update the hosts file. Also port `4411` is pretty uncommonand with a first research is still not clear what the service is located there, well I guess I can start from port **80**

![e87fe995133236f1043e3e1c5da02d29.png](img/e87fe995133236f1043e3e1c5da02d29.png)

Nothing usefull on the `Home` or `Reports` but `IT Services` have something for me

![c1a7053d346e0b4bcea3403c7c55ee9b.png](img/c1a7053d346e0b4bcea3403c7c55ee9b.png)

Bad news **<u>NTLM</u>** is down well we have to figure out how to overcome this because Kerberos should be used as authentication method but let's see the other link in this section like `/supportrequest.html`

![b7032301e918f19547f66d1b34038288.png](img/b7032301e918f19547f66d1b34038288.png)

Well we have a user called **<u>ksimpson</u>** now let's move on `/passwords.html`

![7355468dd380e88ff05e64e05d4f230f.png](img/7355468dd380e88ff05e64e05d4f230f.png)

I tried with `ksimpson:ksimpson` credentials but no way to access at least for now. Finally we have a clue about what is inside **port 4411** on `/salesordes.html`

![2318ca8afaa59d1b5375593f4f9d9fd0.png](img/2318ca8afaa59d1b5375593f4f9d9fd0.png)

![2657ce6c8ed9ac04800bec4860a2b3dd.png](img/2657ce6c8ed9ac04800bec4860a2b3dd.png)

Interesting stuff but without credentials is pretty uncommon that I will be ble to get an eye on that service.
Returning back to the credentials I am pretty sure they are correct but (obviously) with **NTLM <u>disabled</u>** the default crackmapexec will not work. We need the FQDN of the Domain Controller and returning back to the namp output is `DC1.scrm.local`su

Well know we can ask for a TGT ticket

![60c6924a3ced16b88a9c135f3a8f5036.png](img/60c6924a3ced16b88a9c135f3a8f5036.png)

We can use this TGT to get the SPNs related to this user with `GetUserSPNs`, we need to specify the `-k` flag to force Kerberos authentication and most importantly the `-dc-host` specifying the FQDN of the Domain Controller otherwise and error will occur

![ce750ac75a09982119dfa7ccf54aa1e1.png](img/ce750ac75a09982119dfa7ccf54aa1e1.png)

Cool the sql service account have a SPN for `ksimpson` so now we can make some kerberoasting with `-request`

![70b8c20d60aadb5d5776ef5337e58e71.png](img/70b8c20d60aadb5d5776ef5337e58e71.png)

With `hashcat` I was able to bruteforce it and retrieve the plain text of the password which is `Pegasus60` now I will switch the TGT getting another one but for the sql user instead of ksimpson and use it to access MSSQL

Bad news we still don't have access

![63e13c49abacb3ec6222694ae73e6f37.png](img/63e13c49abacb3ec6222694ae73e6f37.png)

Makes sense, remember everything is doing with Kerberos not with NTLM so we need to think with this in mind. To access the MSSQL we need a ticket, **<u>A SILVER TICKET!</u>** With one of that we would be able to access the DB and extend more.

Now we need the NTLM hash of the `sqlsvc` password, sounds funny usually we need the opposite . I used a simple generator online and use bash to convert all in lowercase

![4f8bdafeb4d860451dc917788f745cc0.png](img/4f8bdafeb4d860451dc917788f745cc0.png)

Next we need the Domain SID, I have already encounter this need and `GetPAC`sc of impacket kicks in

![e5a7335b3b5fac0becdb2d00cbf352ef.png](img/e5a7335b3b5fac0becdb2d00cbf352ef.png)

Now we can craft the ticket specifying the SPN of the target service

![9edccc964f2340427143662ad49fb6f6.png](img/9edccc964f2340427143662ad49fb6f6.png)

Finally we have access! We can also enable `xp_cmdshell` so we have a way in

![3aa88eaa26bc8874ffe4bf62f3b33479.png](img/3aa88eaa26bc8874ffe4bf62f3b33479.png)

Is time to use the web delivery module of metasploit to gain a shell

![60a21119f91983218c8f1664b9102ec4.png](img/60a21119f91983218c8f1664b9102ec4.png)

This user doesn't have the flag so I need to go deeper a little more. The `sqlsvc` user have the infamous **<u>SeImpersonatePrivilege enabled</u>** so root can be easily achieved that way but I wanna learn so I will put this method on the shelf 

I enumerated further the DB and I found a credentials for another user which have a directory in `C:\Users`

![0a2b618a3498391465e3de7d42732717.png](img/0a2b618a3498391465e3de7d42732717.png)

This is cool now we can use this credentials to run a new powershell session and than re-establish a reverse shell with metasploit with the `-ScriptBlock`

![ebbe086bdc56f3eaa0a09694f4daa9f3.png](img/ebbe086bdc56f3eaa0a09694f4daa9f3.png)

Now the flag is in our hands!

![fdfe1d19600f65f8b28be49008218ce9.png](img/fdfe1d19600f65f8b28be49008218ce9.png)

<br><br><br>

# PRIVILEGE ESCALATION

We have 2 things here which are both related with the port `4411`, the first one is the path of the service inside the target

![68ebbc3bc2a0e2d632d3cc91789d53e0.png](img/68ebbc3bc2a0e2d632d3cc91789d53e0.png)

Plus this user is in the `ITShares` group so finally we can have a look at SMB where we can find the Client of the service

![f3983eaa61d6911701b0275e8cbce3a8.png](img/f3983eaa61d6911701b0275e8cbce3a8.png)

Cool, they are .NET assembly this means dnspy will be our best friend from now one, <u>DECOMPILE TIME!</u>
We have some serialization and deserialization inside the DLL file

![21cb23b2396d024d99d74453a1018871.png](img/21cb23b2396d024d99d74453a1018871.png)

Interesting let's see where this is used, let's keep in mind this dictionary object because we can interract through NetCat with those (they are under `ScrambleNetRequest`)

![906edf80002bb1a0cf52488aaeb5e1b8.png](img/906edf80002bb1a0cf52488aaeb5e1b8.png)

There is a method which use the `Serialize method`

![8428136bc877369c5275cec900ca7cb2.png](img/8428136bc877369c5275cec900ca7cb2.png)

And we have the one that is using the `Deserialized method`

![34b92810ade37a718e0b9cbaf2de305a.png](img/34b92810ade37a718e0b9cbaf2de305a.png)

Actually we need to focus on the `UploadOrder` function, serialize the input and will probably deserialize on the Server side. This sounds the way client and server communicate through serialized object, let's see if we can make a **<u>deserialization attack</u>**. So the `BinaryFormatter()` is used before being base64 encoded, this seems enough to run **<u>ysoserial</u>** on my windows VM and create a serialization object running a powershell command which will trigger the usuale web delivery module of metasploit

![96502e1ec0db2ad5b660ca6e5d23c882.png](img/96502e1ec0db2ad5b660ca6e5d23c882.png)

Gottch'a looks cool! We just need to figure out how to inject it, well we know that `UPLOAD_ORDER` will be the request type and the parameter are served adding a `;` whatever is next to it will be the value of the parameters. We can see that here

![2a2f54b3d64073e9f4fb474ac225a904.png](img/2a2f54b3d64073e9f4fb474ac225a904.png)

Cool, we can now use the ysoserial payload as parameter which would be serialized and than deserialize at the server side triggering the command we have embedded

![6ad8106282c88a9c04ce2dc68136c9c3.png](img/6ad8106282c88a9c04ce2dc68136c9c3.png)

Beautiful, the meterpeter shell is triggered wounderful!

![1e14b2404d7771cedbdcf757f40267bc.png](img/1e14b2404d7771cedbdcf757f40267bc.png)

We have high privilege now, the root flag is mine!
