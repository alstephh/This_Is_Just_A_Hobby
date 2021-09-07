PORTS
=========================================================
3306 ---> mysql (unauthorized)
=========================================================
80   ---> http                                                                                                                 
     ---> title = IIS windows server [10.0] ---> Internet Information Service ---> general purpose web server                  
                                                                                                                               
[DIRECTORY BUSTING]                                                                                                            
wordpress              ---> http://10.10.10.29/wordpress/index.php/blog/ ---> exploring this site we discovered a user called "admin"
		       
		      ---> /wordpress/wp-login.php   ---> default credentials doesnt work                                                               
	                                             ---> trying credential stuffing     ---> P@s5w0rd! [WORK!]                                                  
==========================================================
OS = Microsoft Windows Server 2016
==========================================================
Credential Stuffing ---> USERNAME = admin
		    ---> PASSWORD = P@s5w0rd!
