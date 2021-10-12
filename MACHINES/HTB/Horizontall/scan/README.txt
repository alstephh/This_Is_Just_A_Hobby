PORTS 

22 ------> ssh 
80 ------> http ------> horizontall.htb
		------> nginx/1.14.0



DIRECTORY BUSTING -----> nothing important

searched with various method to find some domain linked to Horizontall ----> dns busting ----> not working 
								       ----> js file     ----> found a method that redirect to http://api-prod.horizontall.htb/

-----> add the new domain on /etc/hosts and trying some enumeration on that domain

DIRECTORY BUSTING 2


----> /reviews ----> json file with reviews 
	       ----> some username are in (john / doe wail)
----> /users   ----> 403
----> /admin   ----> Strapi admin login page ----> Strapi is a headless CMS
					     ----> default credentials are admin:admin but now working
               ----> Searching on strapi documentation in http://x.x.x.x/admin/Strapiversion -----> where version is stored
	       ----> using curl we obtained the version of the actual Strapi -----> {"strapiVersion":"3.0.0-beta.17.4"} ----> cool

Let's find some vuln of this version 
|----> CVE-2019-18818 ----> reset password of user vuln (from unauthenticated) -----> 1.png show the output of the exploit
		      ----> we gain access!	
		


