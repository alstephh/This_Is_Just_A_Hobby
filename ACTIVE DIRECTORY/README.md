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
    2) Make the event occurs on the victim machine
    3) Get DEM HASHES
    4) Crack hash with hashcat
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
  2) Set up relay (ntlmrelayx -tf [target.txt] -smb2support / -i for interractive shell) 
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
  
  

_____________________________________  
  
</details>
    
_____________________________________
    
    
  </details>  
  
  _____________________________________
  
  
</details>
