# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.6)


<br><br><br>

# ENUMERATION & USER FLAG

This is the homepage

![44ae2aa5009e3296fdafeb545046af7c.png](img/44ae2aa5009e3296fdafeb545046af7c.png)

The source code suggest there is a lot more "covered"

![1ddf8e4b949bb37d719b787c05251746.png](img/1ddf8e4b949bb37d719b787c05251746.png)

GoBuster has been my best friend for some dirbusting (I found no subdomain) all of them pretty interesting especially the `/backup`

![1349127b50d02da033bc2d6609fc7011.png](img/1349127b50d02da033bc2d6609fc7011.png)


In the last endpoint we can download a tar file called **backup** that contained the PHP code of the other endpoints, that's cool!

![268e74350f6e45197f8ed6de486bef8f.png](img/268e74350f6e45197f8ed6de486bef8f.png)

`upload.php` have a strange behaviour it ask for a image file but when I ty with a standard PNG or JPG return an error so I will start looking there.
First of all the PHP code check the file type using a function from `lib.php`

![b977366090b00b0b21d44cf77a4f2173.png](img/b977366090b00b0b21d44cf77a4f2173.png)

Okay so now we can take a look on the library php file to get a look how this function work and is a wrapper of the `file_mime_type($file)` function

![4912c618c1ad01e8c705f4be85163fa4.png](img/4912c618c1ad01e8c705f4be85163fa4.png)

"[...] magic MIME [...]" in the comment is a hint, we can play with magic number to change the MIME of a PHP reverse shell. I used `hexeditor` to change the first 4 bytes into the JPG MIME magic bytes `FF D8 FF E0` 

![5a39b99ca22bdd86fcf96c7e1f3ecbcf.png](img/5a39b99ca22bdd86fcf96c7e1f3ecbcf.png)

Using **file** returned as JPG so I changed the extension to `php.jpg` and I was finally able to upload it!

![eb436bcd3b01b84db6392b15cf126114.png](img/eb436bcd3b01b84db6392b15cf126114.png)

On `/photos,php` we can find our file but when we access it something strange occured

![a9b32c152ad5193eb5f95b2593f67898.png](img/a9b32c152ad5193eb5f95b2593f67898.png)

![15f42f7c99768bf5344c0ef93d22e015.png](img/15f42f7c99768bf5344c0ef93d22e015.png)

Ok let's not give up we have a piece of the puzzle placed correctly, well was actually pretty easy to make it work. Is a good practice to try create a JPG (or whatever other file) from scratch. I always forgot this type of things (I have a bad memory lol) so I used the official walkthrough and here the step to create a fake JPG file

![9e654663aff23a78d789c631e8de44a0.png](img/9e654663aff23a78d789c631e8de44a0.png)

After the upload we have gained RCE successfully! It create a small difference when we overwrite the magic bytes and when we create a file starting from the magic bytes

![b247121239eeb0cc2a7e92b9e2a4912f.png](img/b247121239eeb0cc2a7e92b9e2a4912f.png)

Nice we can now use this RCE to spawn a reverse shell

![8c28b84a4bff88134d8053cf926e2a0d.png](img/8c28b84a4bff88134d8053cf926e2a0d.png)

Gottch'a, this is not enough to grab the user flag so we need to explore further (finally in a turf that I am more comfortable with) but we can get inside the `guly` home directory and there is an interesting file called `crontab.guly`

![31a111b87d5d33a8165e4c759620c4c4.png](img/31a111b87d5d33a8165e4c759620c4c4.png)

et's take a look at the `check_attack.php` (obviously we cannot change it)

![7cafe6800bead68cba99210cbf94f20e.png](img/7cafe6800bead68cba99210cbf94f20e.png)

We "control" the `$value` variable (every 3 minute the `/uploads` directory scan all the name of the file) and is "injected" inside the `exec()` function, so I created a dummy file with `touch ';ping -c 3 10.10.14.15'` and after 3 minutes the command executed correctly!

![036fc3a6c1f1203f1c58098ccedf9a1f.png](img/036fc3a6c1f1203f1c58098ccedf9a1f.png)

With the same approach I can retrieve a reverse shell, this time as **<u>guly</u>** with the netcat binary installed in the target machine!

![df8ef332cc8667a757e684e99ee6714f.png](img/df8ef332cc8667a757e684e99ee6714f.png)

Now we can grab the flag and free to move on privilege escalation!

<br><br><br>

# PRIVILEGE ESCALATION

We can execute the script `/usr/local/sbin/changename.sh` as root without password thanks on how SUDO has been cofnigured, thanks for that sysadmin! The script is the following

![c445e8a4b8cd202c409416ddea4fccbe.png](img/c445e8a4b8cd202c409416ddea4fccbe.png)

What's interesting here is the `regexp` we can't inject something like `/bin/bash` but `lolol /bin/bash` would pass smoothly (because hte space character is part of the regular expression, bad choice). This is a good combo with the file we are going to write on the way **<u>network-scripts</u>** (CentOS) are parsed permis to execute what ever is after a space in the parameters field so something like `NAME=dsa /bin/bash` will execute a bash as root.

![608d09a5b335b562d2a076d0ab8681c0.png](img/608d09a5b335b562d2a076d0ab8681c0.png)

Cool, regular expression are something confusing at first but with some experience are pretty easy to understand and is essential if we need to work with bash script!
