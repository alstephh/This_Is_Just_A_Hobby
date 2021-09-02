HOST 192.168.43.98 --------> DNS SERVER

1] port 22 ---> ssh
2] port 53 ---> dns 
3] OS      ---> debian
4] port 80 ---> http        ---> title = nginx (vulnerable to CVE-2011-3192 [DoS])
           ---> source code ---> Webmaster: alek@blackpearl.tcm
           ---> /secret     ---> "hint" to not doing directory busting
	

try to use DNSRECON ---> blackpearl.tcm [added to /etc/hosts]

