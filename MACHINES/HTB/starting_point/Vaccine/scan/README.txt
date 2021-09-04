PORTS
21 ---> ftp ---> credential stuffing with previous machines ---> ftpuser / mc@F1l3ZilL4 [WORKING]
22 ---> ssh
80 ---> http ---> Apache httpd/2.4.41
	     ---> httponly flag not set [PHPSESSID] ---> client side can read cookie
             ---> title = Megacorp Login (Megacorp is a recurrent name of the previous machines) ---> possible credential stuffing
OS = Ubuntu

================================================================================================================
DIRECTORY BUSTING

+ http://10.10.10.46/index.php (CODE:200|SIZE:2312)
+ http://10.10.10.46/server-status (CODE:403|SIZE:276) 
================================================================================================================

FTP ---> login ---> backup.zip file ---> encrypted ---> fcrackzip ---> PASSWORD FOUND!!!!: pw == 741852963
				    ---> index.php ---> user = admin 
						   ---> password = 2cb42f8734ea607eefed3b70af13bbd3 ---> hash ---> qwerty789
				    ---> style.css ---> nothing relevant


[this is the right cookie for gain access, change at every session if problem pop out delete cookies from the browser]
PHPSESSID=995v7g006bc4e0e4m9i8nji13s

LOGIN SUCCESFULL!
