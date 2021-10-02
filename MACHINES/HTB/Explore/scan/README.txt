PORTS

2222 ---> ssh     ----> after a deep scan ----> EtherNetIP-1 ----> industrial automation protocol
|
|---> using NSE script "enip-info" ----> fingerprint strings ---->  SSH-2.0-SSH Server - Banana Studio
												|---> Famous developer since 2014
												|---> This port is open cuz "SSH/SFTP Server" app 
	

5555 ---> freeciv ----> gaming protcol (lol) ----> after a quick research ----> Android Debug Bridge (adb)
|
|----> try with " adb connect <IP>:PORT " ---> not working cuz we need a wired connection with the devices (nothing relevant but something new that I learned)
|
|----> after a research I found that freeciv is vulnerable to CVE-2010-2445 for version 2.2 before 2.0 and 2.3 before 2.3.0 (score 10!) 
									



DEEPER SCAN
----------------
PORT
 
40947 ---->  unknown ----> cant figured out what this can be

59777 ---->  Bukkit JSONAPI httpd for Minecraft game server 3.6.0 or older ----> server for MineCraft (LOL pt.2)

42135 ---->  ES File Explorer Name Response httpd -----> File manager (both local and network) ----> Used mainly for shared file with no data cost
			|---> first thing that I found is CVE-2019-6447 ----> able to send JSON file ----> abuse for gain a shell
								|
								|
								V
			Gonna try it with this exploit (https://www.exploit-db.com/exploits/50070)
