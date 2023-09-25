# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (Apache 2.4.52)
* **111** &#8594; RPC

![2.png](img/2.png)


 



<br><br>

# ENUMERATION & FOOTHOLD
The webpage is acting as a click-game

![1.jpeg](img/1.jpeg)

We have a **login** and a **register** page

![3.png](img/3.png)

![4.png](img/4.png)

Before try to use the webapp register a new account eccetera I want to take a look at the **nfs** of the server

![5.png](img/5.png)

Cool a backup file let's take a look at the contents and we have a `.zip` file, interesting! We have the full code of the webapp

![7.png](img/7.png)

Ok I have made a quick analysis and now I am ready to create an account, than I logged in with my brand new profile I have  new pages available

`/profile`
![8.png](img/8.png)

`/play`

![9.png](img/9.png)

I've started to understand how this page works and if there is a way to abuse it. When we **save & close** this is the request

![10.png](img/10.png)

We can both change the click and the level in whatever value we want, pretty bad game design here

![11.png](img/11.png)

Cool we can play with the parameters and apparently we can add whatever arguments we want, check this line in the `save_game.php` file that is inside the nfs

![12.png](img/12.png)

Ok we can't become directly **admin** but we can add costum arguments (apparently). Finally I found something, follow me!

![14.png](img/14.png)

The code use `/db_utils.php` especially the function `save_profile()`

![15.png](img/15.png)

Here we are, in short the function take the `$_GET[]` and create the `$setSTR` like this (take for instance the parameter in the upper image)

```
click = '21',level = '0'
```

Than use this string to create the following SQL statement

```
UPDATE players SET $setStr WHERE username = :player
```

So we will have this statement following the example

```
UPDATE players SET click = '21',level = '0' WHERE username = :player

```

Cool we can hijack the SQL statement in order to become admin, note that `/save_game.php` check if the key is equal to "admin" but not the value, make sense we have a way!

Setting click parameter to `2'+,+role+=+'Admin` I returned this

![a3ed692fc76500d1d17a0436769aed87.png](img/a3ed692fc76500d1d17a0436769aed87.png)

COOL! But somehow is not interpreting the second part as part of the SQL query but as string and this is not what I want....uhmmmm

Whatever other type of injection is not working, that's because `pdo ->prepare()`, I'm pretty sure about it

<br>

So if we can't do it with the values the only way is to inject with the key (I was wrong about my first assumption) but we had to bypass this `if` statement

![ce976cdae96a6c01d9223f7cc635f3f9.png](img/ce976cdae96a6c01d9223f7cc635f3f9.png)

Tria and error, trial and error, trial and error,........(2 hours later)....and I gotta it! Add a `%0A` (which is a newline URL encoded) befoe `role` and we will achieve 2 things :
1) Bypass the IF statement (because the `%0A` is interpreted as a normal string)
2) `%0A` in the SQL statement will act like a simple new line, will not broke the SQL query and **set role as Admin !**

We have a new section now ehehehehe

![bd19cde9f27abb4007a58361b6af175f.png](img/bd19cde9f27abb4007a58361b6af175f.png)

Afer we press "Export" we can access teh save data in a predefined URL

![788116d125022b258443ec5ff3082ea8.png](img/788116d125022b258443ec5ff3082ea8.png)

This is the request

![fc1c08bb4fe43f2080d181452e8abea7.png](img/fc1c08bb4fe43f2080d181452e8abea7.png)

We can change the extension freely (for instance, PDF will return an empty PDF) and the filename is created like this `top_players_[random].[extension]`, what's cool here is that the extension is not sanitized! I can't figure out what threshold is being used for, a quick check on the code and I had the answer


1) Here the php code set the variable if is numeric and call `get_top_players()`

![9c8fd8835ea845aaada7ec9638aaa6e9.png](img/9c8fd8835ea845aaada7ec9638aaa6e9.png)

2) `get_top_players()` will return all user with number of clicks >= threshold value

![8888051ef5c12f3aac53a232ff1cdf39.png](img/8888051ef5c12f3aac53a232ff1cdf39.png)

<br>

Well the most important thing is that we can **<u>set whatever extension we want</u>** like PHP and because a bad implementation of the `if` statement that handle the different extension we have control on what we cn put inside in case we decide to make extension in PHP

![1e66a2e6393b075bfe2d39253979f9da.png](img/1e66a2e6393b075bfe2d39253979f9da.png)

Cool using the `/save_game.php` I changed the **nickname** value (clicks is needed as numeric because the threshold check and level must be numeric) in a PHP RCE (URL encoded) and now I can execute whatever command I need after I access the php file in the export phase

![e1f2bffa079560734310050a3d9c084c.png](img/e1f2bffa079560734310050a3d9c084c.png)

Now I simply uploaded and execute a reverse shel and I am in :)

![efc9ffe60e62883eead74ac8cc1b3902.png](img/efc9ffe60e62883eead74ac8cc1b3902.png)

<br><br><br>

# USER FLAG
We have a single user inside the home directory called **jack**, let's hunt this guy :)

Checking the SUID binary I am interested in this 

![b35994374177e6ca62589ed3b22d0468.png](img/b35994374177e6ca62589ed3b22d0468.png)

I am pretty sure that the jack's SUID in bash is a consequence of a successfull explotation done by another player so I will avoid to take advantage on this, sounds like cheating. On the other hand we have an unknown binary also with jack SUID let's take a look at `/opt/manage/`

We have a `README.txt`

![4f095d40da5873eb2ddcd64e64594589.png](img/4f095d40da5873eb2ddcd64e64594589.png)\

Nothing more I can do about it if not use **<u>Ghidra</u>** and decompile the binary

<br>

First we have a switch case with a **default case** 

![6f3489e3f19d198b563298ca60e5a1db.png](img/6f3489e3f19d198b563298ca60e5a1db.png)

This is cool we have a case which is not listed in the README file 

![aed6abb7197dfe51e152a9fd8b738f30.png](img/aed6abb7197dfe51e152a9fd8b738f30.png)

Cool cool cool stuff! It save in a variable called `pcVar3` the argument after the number, I discovered (with just my thinking gonna be honest I was not able to understand in the decompiled code itself, at least at the start. I'm a noob loool) that if we use a random parameter we get the error (like image above) but when we choose one of the switch case we obtain a legit output

![2ffb3629f99d4c56db82e6f56ae0cca9.png](img/2ffb3629f99d4c56db82e6f56ae0cca9.png)

After that we have a check using `access()`

![dd060e9a58851b19d6ab1a12c6f478d3.png](img/dd060e9a58851b19d6ab1a12c6f478d3.png)

**local_98**, **local_90** and **local_88** seems to be to the path `/home/jack/queries`

![db810d7d7c339d0b14031c5f44989b35.png](img/db810d7d7c339d0b14031c5f44989b35.png)

While the variable after the `if` statement is the line of bash that call mysql with parameters (username, password and database, the same in the php files) and concat the string with the file provided

`/usr/bin/mysql -u clicker_db_user --password='clicker_db_password' clicker -v < [file]`

Makes sense since every sql file (inside `/home/jack/queries`) contain a SQL statement

First thing I get is this making the obvious thing I can think about

![04ffb2d08845f22b18af4eb464de71a8.png](img/04ffb2d08845f22b18af4eb464de71a8.png)

Cool is taking the directory `/home/jack/queries/../` and use it successfully as parameter for MySQL, I tried directly with the flag but we can't use it this is probabably for `access(_dest,4)` the second parameter is the mode a XOR sum about the 3 type of access bit and if I am not wrong the flag can't be changed this is why the error occurs

![9d0be22fd0132bbfe0ba2b7ba1f51498.png](img/9d0be22fd0132bbfe0ba2b7ba1f51498.png)

But we can have the ssh private key :)

![3803914e5086ca015fe9c58fc0b7cf75.png](img/3803914e5086ca015fe9c58fc0b7cf75.png)

Just copy the key (I had to fix the hypens, easily recognize that comparing a brand new generated key with this one) and use it to log in

I have the flag!

![833adc4210589283fc5a904e4c3b17d9.png](img/833adc4210589283fc5a904e4c3b17d9.png)

<br><br><br>

# PRIVILEGE ESCALATION
We are `sudoers` with jack so obv `sudo -l` is a must

![e6079d2d641e249b32ea4821b2cab95f.png](img/e6079d2d641e249b32ea4821b2cab95f.png)

This is the `monitor.sh`

![a224978d57654d7bd2ed618588aa6d35.png](img/a224978d57654d7bd2ed618588aa6d35.png)

Finally I dicovered what the diagnostic PHP page is used for I know have to be used somehow. Btw this is the output we have

![d1c466ad616f59becc94f42bdf021843.png](img/d1c466ad616f59becc94f42bdf021843.png)

The script force the PATH variable and unset PERL library, makes sense since `xml_pp` use PERL and **<u>we have the possibility to set a variable before running the script</u>**.
This mean **<u>enviroment hijacking</u>** so the question is : which enviroment variable?

My way to solve this was this

1) Set the NOSAVE variable equal to true so will hit the `exit` bash function
2) Change the enviroment variable `BASH_FUNC_exit%%=() { id;}` so we can change the behaviourof `exit` and will execute command as root

![4da244d70b0b1035803233c78d61cc67.png](img/4da244d70b0b1035803233c78d61cc67.png)

Well, simply we can spawn a bash right now, let's take the root flag and finally this box has been pwned!

![8ba89f3f5773bf0381f3c0ba87e8217b.png](img/8ba89f3f5773bf0381f3c0ba87e8217b.png)
