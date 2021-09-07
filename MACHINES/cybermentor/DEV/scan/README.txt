port 22    ---> OpenSSH 7.9p1 Debian 10+deb10u2
port 80    ---> Apache httpd 2.4.38
port 111   ---> rpcbind  2-4
port 2049  ---> nfs_acl  3 (RPC #100227)
port 8080  ---> Apache httpd 2.4.38 (potentially OPEN proxy/PHP 7.3.27)
port 39397 ---> mountd
port 39417 ---> mountd
port 39583 ---> mountd
port 53233 ---> mountd
OS         ---> Linux 4.x|5.x


(potentially) interesting folder 
/.gitignore
/app	---> /app/config/config.yml ---> driver:sqlite/databasename:bolt/username:bolt/pwd:I_love_java 
/src
/vendor
/dev

80:
Bolt cms ---> Installation error [not in web root]
/var/www/html/ ---> current folder

8080 :
webmaster@localhost 
/var/www/htdev ---> document root



http://192.168.43.113:8080/dev/ ---> seems to be the website
