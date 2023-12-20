# PORT SCAN
* **80** &#8594; HTTP (Microsoft IIS 10.0)
* **3690** &#8594; SVNSERVER Subversion
* **5985** &#8594; WinRM

<br><br><br>

# ENUMERATION & USER FLAG
The HTTP port is just the standard page of IIS so probably will be usefull later so I decide to focus to the **<u>SVNSERVER</u>**. This type of service is version control used for store historical versions of projects, Subversion is open source made by Apache. We have a built/in command to approach this type of service called `svn` (really simple to remember lol) and we can use to execute commands like `list`

![a8967d8e008b33bbd10c2c78a46281f6.png](img/a8967d8e008b33bbd10c2c78a46281f6.png)

Ok cool we have something let's see what are the contents of that text file

![1401f0ee3d7201e7dc12b3b56bc1e476.png](img/1401f0ee3d7201e7dc12b3b56bc1e476.png)

We have multiple files downloaded as much as our `moved.txt` file which revealed to us a new subdomain

![fc1fc09ddc0a5c3483c6718e61909045.png](img/fc1fc09ddc0a5c3483c6718e61909045.png)

This is the `dimension.worker.htb` subdomain

![461c3b42679eb860ec7358760b1d49e5.png](img/461c3b42679eb860ec7358760b1d49e5.png)

The `devops.worker.htb` is protected with <u>Basic Authentication</u> so we need a set of credentials to access it, since I am not satisfied enough with SVN I wanted to checkthe changelog and see what else we can access through this service using the `log` command

![5e5db24521d0ed10bc25773eb99b05d8.png](img/5e5db24521d0ed10bc25773eb99b05d8.png)

Ok cool so starting from **<u>r4</u>** the dev teams moved everything in the protected devops domain and the **<u>r2</u>** looks interesting because add a <u>deployment script</u>. Through the `up` command we can update to the specified revision with the `-r` flag

![4ba3e39c6b72376b003e05a8e150a6e7.png](img/4ba3e39c6b72376b003e05a8e150a6e7.png)

Guess what? The `deploy.ps1` script have some credentials ready to be used ehehehe

![e734eafebe9b7e6f02781a33d0d642a5.png](img/e734eafebe9b7e6f02781a33d0d642a5.png)

This works on the <u>Basic Authentication</u> at `devops` subdomain and now we have access to it!

![3a8adb843f586099b3aea9015ca5b423.png](img/3a8adb843f586099b3aea9015ca5b423.png)

Something new again! `Azure DevOps`, a Microsoft SaaS (Software-as-a-Service, a sort of Git) offering a DevOps toolchain to develop and deploy applications. We can now see the page as `nathen` user and the only active projects is the **<u>SmartHotel360</u>**

Into the `Repos/Files` section we can have all the files for each branch, I have analyzed each of it but nothing cool. 

![d2a45e1e6fa3b809c86425b72e9bff09.png](img/d2a45e1e6fa3b809c86425b72e9bff09.png)

We can move on another feature of Azure DevOps, the **<u>pipelines</u> (CI/CD)**, let's pick the `Alpha-CI`

![d47c6bfdaafd04a2ddf5709c19405b04.png](img/d47c6bfdaafd04a2ddf5709c19405b04.png)

The **Target Folder** is interesting on how is built and the pipeline deployment is done with the use of a YAML file

![01748e7e2c388c00000e12efaed7069d.png](img/01748e7e2c388c00000e12efaed7069d.png)

Interesting, what we needto achieve here is to upload an <u>aspx reverse shell</u> somewhere where can be triggered easily and gain foothold, let's find out how since we don't have enough permission on the existing branch

![a43ca016bfde6aa68021c26d0f098fc3.png](img/a43ca016bfde6aa68021c26d0f098fc3.png)

But if we create a new branch (in my case named `malicious`) we can freely commit new files

![4210acbc36676b58fae7ca7e243d3753.png](img/4210acbc36676b58fae7ca7e243d3753.png)

I have created with msfvenom the aspx reverse shell and uploaded in our brand new branch now is time to use the pipelines to integrate our changes in the actual subdomain, I am going to `Pipelines > builds > Alpha-CI > Queue` and set the malicious branch

![97e6bcb8715bafc3858c1daa67cd4d17.png](img/97e6bcb8715bafc3858c1daa67cd4d17.png)

Wait some seconds and everything would look fine!

![b14998d8aae124f5b2b805a04691a325.png](img/b14998d8aae124f5b2b805a04691a325.png)

Now we can add the `alpha` subdomain to the hosts file and trigger the reverse shell!

![30435cf9fb39ade7eba859aed4388e40.png](img/30435cf9fb39ade7eba859aed4388e40.png)

Obviously we don't have enough permission to grab the first flag, now we are spawned in the `C` volume but we noted before the existance of a volume `W` so we cab swap it and see what we have inside that simply entering the command `W:` we are changing the volume and we can note organized differently 

![734d2e013bbe6469e34673e64056f650.png](img/734d2e013bbe6469e34673e64056f650.png)

Surfing around I found a file at `W:\svnrepos\www\conf\passwd` which contains a list of user and passwords

![ba27d062a6835580564f1b7c430e9ad9.png](img/ba27d062a6835580564f1b7c430e9ad9.png)

The only user we have located on the `C` volume is `robisl` which have assigned the password `wolves11` and this credentials work with WinRM as weel!

![d80a11b764b9f2dc43aae5f37376ff1b.png](img/d80a11b764b9f2dc43aae5f37376ff1b.png)

Now we have access to the user flag!

(btw this low user have SeImpersonate permissions but as I have said some writeup ago I would avoid to do it unless is strictly necessary, I dont wanna get bored doing the same thing also this let you jump the priv escalation part)

<br><br><br>

# PRIVILEGE ESCALATION
We can now login inside the Azure DevOps with this new user and we have access to a new project

![9507fa27faedfefd5bf6a6868b1f53d6.png](img/9507fa27faedfefd5bf6a6868b1f53d6.png)

We have a focal difference between this user and the previous one, now we are in the `Administrators` group

![b27ebc433ea3ef1ffd2ff9f6af83875f.png](img/b27ebc433ea3ef1ffd2ff9f6af83875f.png)

We have no pipeline ready for us but given our permissions we can create a new one, I choose the starter version of AzureDevOps, before confirm the creation we are dealing with the YAML file that would be used for the CI/CD integration, what's usefull here is the **<u>script</u>** section of the YAML which is used to execute OS command. Do the math and we can now try to read the root.txt file directly using the pipeline YAML file cahnging the appropiate section

![b4c4f38e47513e9fdeff6cab6a091a61.png](img/b4c4f38e47513e9fdeff6cab6a091a61.png)

But is not that easy, lol. First we can't write on the master branch but as we did vefore we can add the pipeline in a new branch but I still have an error after that

![c3c57ba3e988c276070655c8923fd8db.png](img/c3c57ba3e988c276070655c8923fd8db.png)

Plus we can't add the missing pool so we are in trouble

![92932982824ea631af96b6e4e188e9a8.png](img/92932982824ea631af96b6e4e188e9a8.png)

No worries we can easily discover an exisisting pool called **<u>Setup</u>**

![b982732c08032e9eeac201c5e389cf4b.png](img/b982732c08032e9eeac201c5e389cf4b.png)

Good, so know we can change tha YAML file to get something like this (I have replicated the same command to read the root flag to make sure it works)

![46072acc0fddda4c0f2cca8aaef18849.png](img/46072acc0fddda4c0f2cca8aaef18849.png)

Now we can build it successfully and this would be the results

![6c288ba72ba2bdb5cece3886a09dd551.png](img/6c288ba72ba2bdb5cece3886a09dd551.png)

The `I GOT YOU` script was executed not only once but twice so both the options works let's click on one of it to get the contents of the flag

![1c4a23f39f3f8d1731dd3543b3d97991.png](img/1c4a23f39f3f8d1731dd3543b3d97991.png)

I GOT YOU! Same thing can be applied to get a reverse shell, uau! I have tried with the `web delivery module` of metasploit and it worked!
