![activeDirectory-1 png](https://user-images.githubusercontent.com/50571084/133897991-16f153c4-fb5c-4ab1-85be-8a94e66916b4.png)




<details>
  <summary>What is ACTIVE DIRECTORY</summary>
  
___________________________________________________________________________________
  
  AD is a Directory Service to manage Windows Domain Network with 2 main features :
  
    1) Store info related to objects (PC, Users, Devices, ecc...)
    2) Authenticates with KERBEROS   

  Other features can be performed with AD like :
  
  * Monitoring for security porpuse with tools
  * Password policies
  * Remote management
  * Disaster recovery plan
  * Other more...
  
  ___________________________________________________________________________________
</details>

<details>
  <summary>Why is so IMPORTANT in security?</summary>
  
  ___________________________________________________________________________________
  
  * Most common identity management (about 95% of network use AD)
  * Exploitable **whitout** patchable exploit (abuse features, trust, components, ecc...)
  
  ___________________________________________________________________________________
</details>

<details>
  <summary>COMPONENTS</summary>
  
  ___________________________________________________________________________________
  
  <details>
     <summary>PHYSICAL</summary>
   
   <p align="center">
    <img src="https://user-images.githubusercontent.com/50571084/133898232-82da4477-04bd-49d0-8b9d-0778094d71fc.png" />
   </p>
    
    1. DOMAIN CONTROLLER

      A) Essentially is a host with a copy of AD Directory
      B) Provide AUTHENTICATION & AUTHORIZATION 
      C) Allow Administrative access to manage users and resources inside the network
      D) Replicate updates to other DC inside the same forest
    
   <p align="center">
    <img src="https://user-images.githubusercontent.com/50571084/133906433-f7efc435-921f-4676-b09b-b7e103d32a90.png" />
   </p>
   
    2. DATA STORE
    
      A) Consist on NTDS.dit file (Sensitive file containing all AD objects and pwd hashes)
      B) Default store on %SystemRoot%/NTDS
      C) Accessible only through Domain Controller     
  </details>
  
  
  <details>
     <summary>LOGICAL</summary>
    
    1. SCHEMA
      
      A) Defines all types of objects that can be stored in directories
      B) Enforces rules on objects creation (and configuration)
      
    2. DOMAINS 
      
      A) Administrative boundaries where specificpolicies are apllied to groups of objects
    
    3. TREES
    
      A) Is a group of domains (Domain of domains)
      B) Composed with a parent domain and childs domain 
      C) Share namespace with the parent domain      
      
    4. FOREST
      
      A) Collection of trees
      B) Share common schema, Configuration partition, Catalog, Enterprise and Schema of Admin groups
    
    5. ORGANIZATIONAL UNITS (OUS)
      
      A) Containers which can store objects
      B) Rappresent the organizzation (Hierically & Logically)
      C) Delegate permission and apply policies
    
    6. TRUST
      
      A) Provide mechanism to gain resources in other domains (like interface in IP)
      B) Can b directional (PC1 ---> OTHER DOMAIN) or transitive (PC1 ---> PC2 ---> OTHER DOMAIN)
    
    7. OBJECTS
   </details>
  
  _____________________________________
  
</details>

<details>
  <summary>ATTACK ACTIVE DIRECTORY</summary>
  
  _____________________________________
  
  Attack AD have a lot of ways to get into for semplified things we can divide in 3 steps : 
    
    A) Initial Attack Vectors 
    B) Post-Compromise Enumeration
    C) Post-Compromise Attacks
  
  <details>
    <summary>Initial Attack Vectors</summary>
    
_____________________________________
    
In this step we are going to do 2 main things :
    
    A) Initially attempt to attack
    B) Find a way into the network (Get credentials and Abuse features)
    
<details>
  <summary>LLMNR POISONING</summary>
  
  _____________________________________
  
 ## WHAT IS LLMNR?
  
  ```
  Link Local Multicast Name Resolution [LLMNR] is a protocol which identify hosts when the dns failed to
  ```
  
  ## KEY FLAW
  
  ```
  This service use user's hash and NTLMv2 (password's hash) when properly respond to
  
  We will let the victim machine connect to a (wrong/not existing) domain, LLMNR comes into and the attacker
  (acting as a MAN-IN-THE-MIDDLE) we try to get a connection to the victim
  ```
  
<p allign="center">
  <img src="https://user-images.githubusercontent.com/50571084/133907611-6340eec8-ff2b-4299-b1b0-43f25059e8f4.png">
</p>

  ## STEPS
  
    1) On attack machine, run Responder.py
  
<p allign ="center">
   <img src="https://user-images.githubusercontent.com/50571084/133921862-9d9dd65f-02e8-4af2-9a2c-ad6511110f3c.png")
</P>
      
    2) Make the event occurs on the victim machine
    3) Get DEM HASHES
  
<p allign ="center">
   <img src="https://user-images.githubusercontent.com/50571084/133921973-4d111645-c716-4c83-b7ec-2728aff7e319.png")
</P>

    4) Crack hash with hashcat

<p allign ="center">
   <img src="https://user-images.githubusercontent.com/50571084/133922018-d3bf9e52-cfa0-4b2d-b86e-e2846d38308b.png")
</P>  

    5) after gaining the credentials we can try to get a first shell (psexec.py,smbexec.py,smbexec.py)
 
  ## MITIGATIONS
    
    A) Disable LLMNR (NBT-NS too)
       OR (if can't disable)
    B) Require NAC (Network Access Controll)
    C) Strong password (> 14 char)
  
  _____________________________________
  
</details>
    
    
<details> 
  <summary>SMB RELAY</summary>
  
  _____________________________________
  
  
  ## KEY FLAW
  
  ```
  Relay hashes to specify machine (potentially) gain access
  ```
  
  ## REQUIREMENTS
  
  ```
  A) SMB signing disabled
  B) Credentials relayed must be local admin on the machine
  ```
  
  ## STEPS
  
  ```
  1) Run Responder.py (before run it change responder.conf disabling SMB anf HTTP)
  ```
<p allign ="center">
   <img src="https://user-images.githubusercontent.com/50571084/133922121-201536fb-9d6a-45f3-a63a-74d59d3ffecc.png")
</P>
  
  ```
  2) Set up relay (ntlmrelayx -tf [target.txt] -smb2support / -i for interractive shell) 
  ```

<p allign ="center">
   <img src="https://user-images.githubusercontent.com/50571084/133922193-96a62581-cf11-479f-aa1b-49ccee2270c6.png")
</P> 
  
  ```
  3) Event Occours
  4) Automatically on the relay we won (dump SAM HASHES, get SHELL, ecc...)
  ```
  
  ## MITIGATION
  
  ```
  1) Enable SMB signing (- speed / + complex copying files)
  2) Disable NTLM (if kerberos won't work NTLM will be backup)
  3) Account tiering = limit domains to specific tasks
  4) Local admin restriction (need more service desk tickets which are vulnerable)
  ```  
    
  _____________________________________
  
</details>
    
<details>
  <summary>IPv6 ATTACK</summary>

_____________________________________
  
  ## KEY FLAW
  
  ```
  If IPv4 is ON, IPv6 will have no one as DNS (cuz is used by the IPv4) so we will pretend to be the DNS and get the traffic.
  We can do a lot of things :
  
    ° Get NTLM credentials
    ° Log to the the DC
    ° Create an account for us
  ```

  ## STEPS
  ```
  1) Run MITM6 specifying the domain to work on
  ```
  
<p allign ="center">
  <img src="https://user-images.githubusercontent.com/50571084/133922601-b2e92146-0941-4f77-87fc-135e962e4826.png">
</p>

  ```
  2) Set the NTLMRELAYX (-t [DC_IP] -wh [FAKE_DC] -l [NAME_OF_LOOT_DIR])
  ```
  
<p allign="center">  
  <img src="https://user-images.githubusercontent.com/50571084/133922675-9a94e921-2341-4ddf-80f8-cc01bc9c834d.png">
</p>  

  ```
  3) Events occurs (wait for a machine log in, every 30 min IPv6 send request, restar machine, ecc...) 
  ```
  
<p allign="center">  
  <img src="https://user-images.githubusercontent.com/50571084/133922741-0a2c7e16-eadc-4f4f-a8fb-54ae56829ad0.png"> 
</p>

  ```
  4) ENJOY! (in loot directory files are ready to be read like domain_user_by_group.html) 
  ```
  
<p allign="center">
  <img src="https://user-images.githubusercontent.com/50571084/133922849-d1972e12-8b66-499a-a553-3a41847a1c33.png">
  <img src="https://user-images.githubusercontent.com/50571084/133922860-d4dc4f53-d0f2-48a8-9bf9-ab55881fdb21.png">
</p>  
  
  ```
  EXTRA 1) When an admin account log into a machine automatically a user should be created for us (with a restore file too)
  ```
  
<p allign="center">
  <img src="https://user-images.githubusercontent.com/50571084/133922954-bc65ead7-fae6-4ccd-afd9-d9f94b3065e5.png">
  <img src="https://user-images.githubusercontent.com/50571084/133923018-18e0581c-c755-4421-8d1b-59689b80545d.png">
</p>   
  
  ```
  EXTRA 2) Here's the proof of the accounts created (on DC Server Manager)
  ```
  
<p allign="center">
  <img src="https://user-images.githubusercontent.com/50571084/133923092-c070b2df-4fba-4315-a958-dd28a1fcf036.PNG">
</p> 

  ## MITIGATION
* If not used, disable IPv6 or instead set tish rules on BLOCK :
  
  1) DHCPv6-in
  
  2) DHCPv6-out 
  
  3) Ruter advertisement
  
* If wpad not used disable it
* LDAP signing & LDAP channel binding
* Admin user on protected group (or set thgat user as sensitive account) preventing impersonation
  
_____________________________________  
  
</details>
 
    
<details>
  <summary> PASSBACK ATTACKS </summary>
  
_____________________________________  
  
  ## KEY FLAW 
  Attack not directly the network but devices as printer, IoT device and more to get credentials
  
  (ex:\ printer with default credentials ans SMb and LDAP as protcol)

  ## STEPS
  (Based on the example above)
  
  ```
  1) Logging in with the default credentials
  2) Change LDAP server address to ATK machine IP
  3) Set a listener (netcat) on the LDAP port and you will receive user and password in clear
  4) Same thing can be done with SMB
  ```
  
_____________________________________  
  
</details>  
    
_____________________________________
          
  </details>  
  
  <details>
  <summary>Post Compromise Enumeration</summary>

  _____________________________________
  
<details>
  <summary>PowerView</summary>
  
  _____________________________________
  
  ## WHAT IS?
  A group of scripts usefull to enumerate and get info of the network
  
  ## USAGE
  First of all you need to load all the modules on the victim machine
  
  ![Immagine 2021-09-19 123426](https://user-images.githubusercontent.com/50571084/133924303-ee2f8c53-917c-4946-a6c3-7187302bd7eb.png)
  
  Now we can use all the modules and get the info
  
  ```
   Get-NetDomain           = info of the domain
   Get-NetDomainController = info on the DC
  ```
  
  ![1](https://user-images.githubusercontent.com/50571084/133924437-d2491e05-ebef-4dd5-88ed-ea189edb293a.PNG)

  ```
    Get-DomainPolicy = info on policies
    Get-NetUser      = info on user 
  ```
  
  ![2](https://user-images.githubusercontent.com/50571084/133924490-6b5482c8-8574-49e2-9e1f-28e0eca4e43d.PNG)

  ```
   Get-NetComputer = info on computers
   Get-NetGroup    = info on groups 
  ```
  
  ![4](https://user-images.githubusercontent.com/50571084/133924551-7b1bca6f-6fdc-4da5-85c8-9b1f1d71a280.PNG)

  ```
   Get-NetGPO  = info on groups policies
  ```

  ![6](https://user-images.githubusercontent.com/50571084/133924603-55babcdd-8a4c-49eb-81e8-5ca1c32bda17.PNG)

  ```
   A lot more !
  ```
  
  _____________________________________
  
  
</details>
    
  
    
<details>
  <summary>BloodHound</summary>
  
  _____________________________________
  
  ## WHAT IS
  (exciting) Tool for download info on AD and visualize it (graph theory for visualize uninthended relationship).
  Run with NEO4J database.
  
  The objective using BloodHound is to find the shortest path to gain access to DC Admin

  ## STEPS
    1) Grab data (can be done with various tool [.ps1, .py, .c#, ecc..])

<p allign="center">
  <img src="https://user-images.githubusercontent.com/50571084/133925800-31e2f14a-8fff-4004-8537-05f18fd1dfd1.PNG">
</p>
  
    2) Upload data on BloodHound
    3) Use queries! (here's some examples, a lot more) 
          
    
  ![2(find_domain_admin)](https://user-images.githubusercontent.com/50571084/133925953-ac6bf7f5-93c8-4d03-a289-3655b6a4f0da.PNG)
  
  (query for find the domain admins)
  
  ![3(shot_path_kerborastable_user)](https://user-images.githubusercontent.com/50571084/133926077-c8f876cc-f94e-4819-ac41-eef48e7575df.png)
  
  (shortest path to kerberastable user)
  
  ![4(shortest_path_to_high_value_targets)](https://user-images.githubusercontent.com/50571084/133926101-24effa34-270a-417b-a5bc-87d242d2ccd1.png)
  
  (shortest path to high values targets)
  
  _____________________________________
  
</details>    
    
    
</details>
  
  
  
  _____________________________________
  
  
</details>
