PORTS 

53   ----> DNS 
80   ----> http
     ----> found a email (contact@intelligence.htb)
     ----> found 2 pdf to download ----> 1 ----> creator = William.lee     
                                   ----> 2 ----> creator = Jose.Williams
	  
88   ----> kerberos
135  ----> RPC
445  ----> SMB ----> using mbclient ---> SMB1 disabled



DIRECTORY BUSTING

----> /documents ----> 403
	|-------------------> I notice that the 2 PDF have this format "year-month-day-upload.pdf"
					|---> script1 to download all pdf inside Documents (if avaiable)
					|---> after download all files ---> script2 for find documents with keywords (user,username,pass,password) 
					|---> after gainingdefault pwd ---> 2020-06-04-upload.pdf ----> 1.png for the content of the PDF
										|----> we get a default password --> "NewIntelligenceCorpUser9876"
					|---> lastly with script3      ---> gain all creator of the pdf and save it in user.txt


					

----> lets find how to use this credentials
	|----> use crackmapexec smb --------> brute force with user.txt and the password we discovered
				    --------> Tiffany.Molina:NewIntelligenceCorpUser9876
						|----> on SMB shares we have an interesting folder called "Users"
						|----> other folder (IT) have downdetector.ps1 script
							|---> sort of cronjob running every 5 minutes sending a request to dns starting with "web"
							|---> we can try use responder to get ted.graves hashes
								|---> we need to add our ip as DNS on the DNS record in the victim machine
								|---> dnstools.py (https://github.com/dirkjanm/krbrelayx)					
									|----> 2.png for full command 
							|---> we just need to wait 5 min for the response
							|---> we have the NTLMv2 hash of Ted.Graves
								|---> hashcat ---> we get the password "Mr.Teddy"


----> with the new credentials we have let's try to enumerate LDAP port ----> GMSADumper.py
		|----> we got this hash ----> svc_int$:::c699eaac79b69357d9dabee3379547e6

----> with this hash we can do Silver Ticket attack and try to impersonate Administrator 
		|----> getST 10.10.10.248/svc.int$ -spn WWW/dc.intelligence.htb -hashes :d64b83fe606e6d3005e20ce0ee932fe2 -impersonate Administrator
			|
			|---> give this error ---> Kerberos SessionError: KDC_ERR_WRONG_REALM(Reserved for future use)
				|----> cuz the clock skew ----> we just need to sync time between server and machine
									|----> ntpdate

		|----> after that we have the ticket saved as Administrator.ccache
			|----> first use this command --->  export KRB5CCNAME=Administrator.ccache
		|----> we have access to SMB as Administrator ----> get root.txt flag
