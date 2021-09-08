INTERSTING PORTS

53   ---> DNS 
88   ---> kerberos
135  ---> RPC
389  ---> LDAP (active directory) ---> DOMAIN : Megacorp
3268 ---> LDAP (active directory) ---> DOMAIN : Megacorp

smb2 ---> signing enabled [and required]

1] try to logging with the credentials discovered in the last machine (Sandra:Password1234!) with rpcclient [WORK!]
2] smbclient with the credential work too [NETLOGON (empty) / SYSVOL (interesting)]
3] SYSVOL ---> MEGACORP.LOCAL ---> Policies ---> {31B2F340-016D-11D2-945F-00C04FB984F9}
                                            ---> {6AC1786C-016F-11D2-945F-00C04fB984F9}
4] ldap_enum = output of nmap script for enum ldap

CONCLUSION ---> Machine is a Domain Controller ---> cuz port 88 (Kerberos) and port 389 (LDAP) are open
                                               ---> domain = MEGACORP.LOCAL
