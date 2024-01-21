# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.56)
* **389** &#8594; LDAP (OpenLdap 2.2.X-2.3.X)
* **443** &#8594; HTTPS (APACHE 2.4.56)

<br><br><br>

# ENUMERATION & USER FLAG
The HTTP(S) port redirect to subdomain `nagios` which run the well known Open-Source software [NagiosXI](https://library.nagios.com/)

![d882daffdd54a027f045bfdf57ac5124.png](img/d882daffdd54a027f045bfdf57ac5124.png)

Nagios use LDAP and SNMP to work, we can enumerate the OBJECTID with **<u>snmpwalk</u>** it will list a good amount of records but this one is pretty pretty good

![50778be053b7e326e42a4b18d53e02d6.png](img/50778be053b7e326e42a4b18d53e02d6.png)

This can be a valid set of credentials so I tried on `NagiosXI` but it didn't worked. With some enumeration I found the endpoint `/nagios` asking for credentials in a simple HTTP auth, this time the `svc` username and the password found worked!

![13512ede91100013200443f1de3a189b.png](img/13512ede91100013200443f1de3a189b.png)

Here we are a low privilege user and the standard admin `nagiosadmin` is present, my idea right now to get access of the admin user and move on `NagiosXI` where we can (probably) exploit some CVE and get access

![31674bdfefa4deb26a876e8ff83dec2a.png](img/31674bdfefa4deb26a876e8ff83dec2a.png)

I spend a klot of time on look what I can do with this service but nothing usefull the creds is all we have here. Some enumeration exfiltrate the `nagiosxi/api/v1` allows to interface with API but we need an API token which we don't have at the moment. With some more (and more more more more) enumeration on SNMP I found that there is a process running [shellinabox](https://github.com/shellinabox/shellinabox) and `/nagiosxi/terminal` is where we can interface with it

![40bff50cd056a6943f15653678f75b93.png](img/40bff50cd056a6943f15653678f75b93.png)

[Here](https://support.nagios.com/forum/viewtopic.php?f=16&t=58783) I found a forum thread where one of the answer allows us to retrieve the authentication token

![16a5fb1168f798f2e1c7bb4d1746e6db.png](img/16a5fb1168f798f2e1c7bb4d1746e6db.png)

Now that we have our token we can follow the [CVE-2023-40931](https://nvd.nist.gov/vuln/detail/CVE-2023-40931), I am not sure of the NagiosXI version but was worth to try with SQLMap. Luckly is vulnerable!

![f2bc7988f7028d71f37494ec8d38f0a0.png](img/f2bc7988f7028d71f37494ec8d38f0a0.png)

![c56141116456d84341d3ceb10aa6844d.png](img/c56141116456d84341d3ceb10aa6844d.png)

Cool finally we have the API key (and b2crypt hash) that we can use with the API and we can use `curl -XPOST "https://nagios.monitored.htb/nagiosxi/api/v1/system/user?apikey=[API_KEY]" -d "username=admin2&password=idontlikeyou&name=adm&email=pawn@localhost&auth_level=admin"` to create a new admin (with existing admin API key)

![b293dff6a13e264eea08502fa21bad6e.png](img/b293dff6a13e264eea08502fa21bad6e.png)

Finally we are inside **<u>NagiosXI</u>** as admin, I can finally try to create a costum command from here and run it. Is a good opportunity to check if I can ping my local machine so I crete the command as it follows (`check command` as Command Type)

![f14fe8effa058dca4462c43f3d6c1dea.png](img/f14fe8effa058dca4462c43f3d6c1dea.png)

Than under `Configure` I select the host and when creating a new service I used `check command` to the one I have created. It work really well!

![2c01193eead748761372272f247f5976.png](img/2c01193eead748761372272f247f5976.png)

We know have a way to call a reverse shell via `bash`

![ee3967dc765543baba97d6e549497dbb.png](img/ee3967dc765543baba97d6e549497dbb.png)

Gottch'a the user flag can be grabbed on the home directory!


<br><br><br>

# PRIVILEGE ESCALATION
We can run a lot of program with sudo without password, most of them are related to nargios but one in particlar is pretty interesting

![2d7212b6fcf1e2a0067968da7a6e923c.png](img/2d7212b6fcf1e2a0067968da7a6e923c.png)

This script allows us to stop, reset, stop and check status of different service, one of these is `npcd`

![07230f79f32ce1dbc4677c647d0dc24f.png](img/07230f79f32ce1dbc4677c647d0dc24f.png)

Looking at linpeas, t highlight the user permission to write on that specific service. this means we can write a reverse shell inside of it and than use the script to restart the service as root (you need to stop it before write on it obviously)

![b651001d6c7edc2a96c2418a67915de9.png](img/b651001d6c7edc2a96c2418a67915de9.png)

When the service will run back again the reverse shell will be triggered and we have access as root!

![2661ca984a1f7ea43bb6a5a00e458f44.png](img/2661ca984a1f7ea43bb6a5a00e458f44.png)
