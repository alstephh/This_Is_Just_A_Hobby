# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.29)


<br><br><br>

# ENUMERATION & USER FLAG
Nmap scan return that the website is done with **<u>Joomla (3.8)</u>**

![80fa43bbcd569f9ce17b44b19e7f3cc8.png](img/80fa43bbcd569f9ce17b44b19e7f3cc8.png)

Lol `CEWL` sound exactly like a hint but before trying something I want to enumerate and after a while gobust found `/secret.txt`, yeah....a secret....

![71725ac6a177cfe4d1284c5ff00e442f.png](img/71725ac6a177cfe4d1284c5ff00e442f.png)

If we decode it we have the plaintext which is `Curling2018!`, now believe me or not while I was enumerating I saw this blog post and I tried to use a similar word as password for `floris`

![3d54185044bc7d1cabb4568ac1f30c79.png](img/3d54185044bc7d1cabb4568ac1f30c79.png)

Well what's important here now is that we have access, through `/administrator`, in the Joomla admin panel. That's cool!

I have fuzzed and mess around a little bit and when was the turn of `Templates` we have one setted as default on all pages

![8c7c91d848063cfa0bac1b6207940ac3.png](img/8c7c91d848063cfa0bac1b6207940ac3.png)

We can chooseto costumise the template now and this would be really easy and straightforward path to RCE and we can change whatever PHP file we want to a reverse shell (I used the Ivan Sincek one). In my case I changed the `offline.php` file

![45309357484ee5b8c672042d6a641a25.png](img/45309357484ee5b8c672042d6a641a25.png)

Our job is not ended here because we don't haveenough privilege to read the flag on `floris` home directory (I tried the same password hoping for the reuse but this was not the case). As always we have some hardcoded database credential in **configuration.php** but actually nothing interesting where found inside. Pretty bad :(

Returning back to floris home directory we ca read some files despite the lock on the user flag
The file `password_backup` is a pretty interesting hex file 

![ff4ad2ef3dcd0ea78977d408c9b5c48f.png](img/ff4ad2ef3dcd0ea78977d408c9b5c48f.png)

When I try to reverse it it looks like a zipped file

![f47b206828301fc774cd52d6399cbb14.png](img/f47b206828301fc774cd52d6399cbb14.png)

![13cb1673ca1fcc5e0b99bec63ab72ea4.png](img/13cb1673ca1fcc5e0b99bec63ab72ea4.png)

Boom, was right! Playing with it I discover that password was the file inside it, by the way there is a tra in a bzip in a gzip in a bzip. Yeah I amnot joking lol, is pretty confusin to see but here the process lol

![db8cf786e6ee8afd106b0499c09b862b.png](img/db8cf786e6ee8afd106b0499c09b862b.png)

Now we have both access to SSH and user flag!


<br><br><br>

# PRIVILEGE ESCALATION
We are not in the sudoers but we have a cronjob running as **root**

![62077609bcb5edbe3854cfc27dc5b7c0.png](img/62077609bcb5edbe3854cfc27dc5b7c0.png)

Interesting we have controll on the configuration file used by **curl** (`-K`) it take a good 15 minutes to understand how I can exploit this butafterall is a configuration file and we have full controll on how to send the HTTP request.

For instance we can send to our machine the root flag that will be delivered

![c958b79a1fe6adb8e774713d17de8bb4.png](img/c958b79a1fe6adb8e774713d17de8bb4.png)

To received I used at first the simple HTTP server of python but raise an error because POST request were not supported so back to the basics, NetCat is the swissknif for something right?

![e912388b8159c44e78a82ba3b06fd25f.png](img/e912388b8159c44e78a82ba3b06fd25f.png)

Cool stuff!
