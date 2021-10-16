PORTS

53  ---> DNS port
80  ---> http ----> HTB Printer Admin Panel
	      ----> on SETTINGS section we see that on the address the default string is "printer.return.local" ----> return is the domain 
														----> also we have a username "svc-printer"
	      ----> we can try to put our IP address listening on port 389 (which is LDAP port)
							|----> we get what seems like to be credentials of the "svc-printer" account ----> "1edFg43012!!"
											|----> let-s try with winRM (which is open)  ----> WE GET A SHELL!
88  ---> kerberos 
125 ---> msrpc 
389 ---> LDAP
445 ---> SMB ---> can't login with default credentials


other ACTIVE DIRECTORY ports ----> this mean this is a DNS server



DIRECTORY BUSTING ---> nothing interesting



