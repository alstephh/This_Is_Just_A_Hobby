# WEB APPLICATION PENTEST


### ENUMERATION TOOLS
(Note 1 : different tools use different techniques and some can give other results so use all types of tools is the key of a good enumeration)
(Note 2 : with a good script you can sort all unique results automatically also you will run multiple commands in one saving time) 
  
  1) ASSETFINDER/AMASS    &#8594; Find subdomains
  2) HTTPROBE             &#8594; Check if hosts are alive
  3) GOWITNESS/EYEWITNESS &#8594; Screenshot of the URLs


### TOP 10 OWASP VULNERABILITIES

<details>
  <summary>SQL INJECTION</summary>

___________________________________
  
## DEFINITION
  Malicious SQL statement in a SQL database that if triggered can read,extract,modify data and potentially get a shell
  
  Blind SQL injection = When a WebApp is vulnerable but the HTTP response do not contain result (or details)

## STEPS  
  
_1. Find something in the WebApp that can be vulnerable to injection (like a login form) and analyze how it works_
  
  ![1](https://user-images.githubusercontent.com/50571084/134237814-538eda5b-c60c-4889-9617-7f8eb508e93b.PNG)

  Now let's check what happens behind the scenes :
  
  
   A sql query like this will be activated with the info we insert in the form
   
   ```
   SELECT * FROM Users WHERE Email = 'test';
   ```
  
  Now let's take a look at the HTTP response
   
  ![1,2](https://user-images.githubusercontent.com/50571084/134239723-d02b10a8-2f3d-47bf-b097-ee35d81835a3.PNG)

  
  Now if we changed the email "variable" a lil bit adding another ``` ' ``` we will have something like this 
  
   ```
   SELECT * FROM Users WHERE Email = 'test'';
   ```
  
  And this will give a SQL syntax error on the HTTP request prove us that SQL injection is avaiable (and this will happen) 
  
  ![2(proof)](https://user-images.githubusercontent.com/50571084/134239818-6e5f7532-2e1e-4795-9413-67be2737fcd8.PNG)

  _2. Abuse of the vulnerability for gaining access_
  
   Always using the login form as email let's try with ``` test' or 1=1;-- ``` and check what happens
  
  ![3](https://user-images.githubusercontent.com/50571084/134240168-b266e9f9-cfcb-4bae-8522-81ee082a55bd.PNG)

   This prove that our SQL injection was a success and let us login (whit whatever password we insert)
  
  ![3,2](https://user-images.githubusercontent.com/50571084/134240325-8152126e-8681-4010-b430-5dba931d553c.PNG)
  
  
## MITIGATION
  
* Parameterized statements (instead of insert on the SQL statements whatever we provide on the for use parameter)
  
  ex:/ ``` SELECT * FROM Users WHERE Email = +email+; ```
* Sanitizing input (delete all dangerous char from the form inputs)
  
  
___________________________________

</details>




<details>
  <summary>BROKEN AUTHENTICATION</summary>

___________________________________
  
  ## DEFINITION 
  It's a group of different vulnerability/inefficiencies which can be abused such as :
  
  * Brute force attack (no limit of the request, no cookie, no blacklist)
  * Default/weak password
  * Weak/ineffective passwords recovery 
  
    ![1(user_enum)](https://user-images.githubusercontent.com/50571084/134241449-6c9c9aef-f917-4a10-9823-18d034d6bfbd.PNG)

    (If the email was wrong the recovery will be blocked / just knowing the email we can see what the security question is)
  
  * Exposed/ineffective cookie (like SESSID)
  * Missing/ineffective multifactor authentication
   
___________________________________

</details>




<details>
  <summary>SENSITIVE DATA EXPOSURE</summary>

___________________________________
  
  ## DEFINITION
  Sometimes data are stored and avaiable when they shouldn't giving to attackers sensitive info
  
  * Keys that can be easly stolen or replicate 
  * Data in clear text (really dangerous)
  * Structures that let perform man-in-the-middle attack
  * Backup/important files stored on the webserver
  
  ![1](https://user-images.githubusercontent.com/50571084/134242732-36eec005-a4fc-4764-b2de-a5e9df0a1215.PNG)

  
  ## MITIGATION 
  * Not store data if it isn't strictly necessary
  * Use good and secure passwords/protocols
  * Good encrypt method
  * Avoid clear text
   
  
___________________________________

</details>
