22 ---> ssh
==================================
80 ---> http
   ---> Apache/2.4.29
   ---> email = admin@megacorp.com
   ---> phone =  +44 (0)123 456 789   

   DIRECTORY BUSTING 
   ---> /index.php
   ---> /uploads        ---> 403
   ---> /cdn-cgi/login  ---> 304 [discovered with burp site map]
   ---> /server-status  ---> 403
   ---> /css       [NR] ---> 403
   ---> /images    [NR] ---> 403
   ---> /js        [NR] ---> 403
   ---> /font      [NR] ---> 403
   
   
   discover the login credential is tricky I've tried brute force and enumarate of all sort
   but reading the walkthrough the way was using credential discovered on the previous machine "Archetype" (password = MEGACORP_4dm1n!!)
   and try it with user like admin/administrator [this methodology is called CREDENTIAL STUFFING] 
===================================
 
[NR = Not Relevant]
