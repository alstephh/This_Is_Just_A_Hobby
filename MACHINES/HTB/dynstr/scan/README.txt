PORTS 

22 ---> SSH
53 ---> DNS
80 ---> HTTP ----> get the credential on credentials.txt for the beta mode


Now we need to find this "beta mode"

DIRECTORY BUSTING 

---> /nic ----> folder for dynamic dns 
	  ----> after an depth Directory Busting -----> /nic/update ----> badauth

/nic/update ----> badauth ----> searching on this I-ve found how to authenticate 
						|
						|
						V
curl "http://10.10.10.244/nic/update?myip=10.10.14.196&hostname=dynamicdns.no-ip.htb" -H "Authorization: Basic ZHluYWRuczpzbmRhbnlk"
						|
						|
						V
					good 10.10.14.196 
						|
						|
						V
				   we got the authentication

FOOTHOLD

Now that we have the right path to authenticate I-ve searched some way to abuse this and I found that some RCE can be obtained
abusing the parameter "hostname" and I found an exploit for this case 
(look explotation.txt)
