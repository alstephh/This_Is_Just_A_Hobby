PORTS
----------
22   ---> ssh
443  ---> http/ssl ----> nginx/1.18.0 
                   ----> Apache Tomcat/9.0.31 (Ubuntu) (found on error page)
		   ----> got a couple of form ----> tried XSS and SQL Injection but seems not working
 					      ----> when trued for SQL injection a Java Error pop out (1.png)

8080 ---> http-proxy -----> login page (GitBucket) ----> default credentials didn't work
				|---> Git platfrom on JVM ----> rhis is why we get the Java Error while trying SQL injection




DIRECTORY BUSTING (PORT 443)
----------
----> /manager (/host-manager) ---> 403
	|---> /status ---> ask for PWD 	
	|---> /manager/admin/dashboard ----> 403
----> /a ----> nothing interesting



COOKIE
-----------
JSESSIONID ----> Path = shell ----> this is where our SESSIONID is given to us


I decide to take a look at the security certificate and I had 2 info 

A possible domain name ----> seal.htb       ----> adding that to the /etc/host file 
A email address        ----> admin@seal.htb



GITBUCKET

2.png ----> On the homepage we can se at the ToDo list giving us some info on the status of the webapp
3.png ----> There is a Issue open where we can find 2 user (luis,alex)
4.png ----> There was a file inside the TomCat folder called user.xml ----> in this file user are stored but no user is added 
					|----> looking at the previous commit I found a usefull credentials (5.png) ---> tomcat:42MrHBf*z8{Z%


CREDENTIAL STUFFING ---> before move on try to get inside GitBucket with other user using the PWD we get 
|---> luis   ---> worked! 
|---> alex   ---> not worked!
|---> tomcat ---> not worked!



Now lets go in with the credential on the http://10.10.10.250/manager/status ----> 6.png
We can have a lot of info about the server status 
|---> JVM version ----> 11.0.11+9-Ubuntu-0ubuntu2.20.04
|---> OS          ----> Ubuntu 5.4.0-80-generic
|---> Tomcat      ----> Apache Tomcat/9.0.31  
 
