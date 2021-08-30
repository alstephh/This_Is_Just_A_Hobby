1) OS = Linux 4.15 - 5.6
2) FTP anonymous login ALLOWED (port 21)
3) OpenSSH 7.9p1 Debian 10+deb10u2 (port 22)
4) Apache 2.4.38 Debian (port 80) 
5) http-enum  ---> 192.168.43.117/phpmyadmin
6) phpMyAdmin ---> Version 4.9.7 ---> probably file upload rce OR arbitary file read


DIRECTORY BUSTING

http://192.168.43.117/phpmyadmin/phpinfo.php (CODE:200|SIZE:14557)                                                         
http://192.168.43.117/phpmyadmin/README (CODE:200|SIZE:1520)                                                               
http://192.168.43.117/phpmyadmin/robots.txt (CODE:200|SIZE:26)                                                             
http://192.168.43.117/phpmyadmin/setup (CODE:401|SIZE:461)  
