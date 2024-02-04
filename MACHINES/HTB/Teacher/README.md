# PORT SCAN
* **80** &#8594; HTTP (APACHE 2.4.25)


<br><br><br>

# ENUMERATION & USER FLAG
The website is a static brochure that act as a presentation for a highschool

![fe7ab33b0e35cfe189d6de7fa4ac6f43.png](img/fe7ab33b0e35cfe189d6de7fa4ac6f43.png)

Dirbusting found nothing interesting but the `/images` path where a file is significatively small than others

![6323277cd2806224366d0d382590b10a.png](img/6323277cd2806224366d0d382590b10a.png)

Since I was not able to retrieve the contents I tried to curl it directly and well is a sort of steganography on this fake image file

![4805fe5d050ebb8ef128603bc82620fc.png](img/4805fe5d050ebb8ef128603bc82620fc.png)


After that, nothing interesting at all, just a little bit of time and the endpoint `/moodle` has been found! Makes sense with the box name

![500f67b014339cda611c0eace8ee05d5.png](img/500f67b014339cda611c0eace8ee05d5.png)

Cool, now we know the prefix of a password and we need the last character to find, we can use moodle login to test out bruteforce but first of all let's create a password wordlist. I used `crunch` to generate a worldist with same base string and adding a last character (letter, number and special char) and I used `cupp` for username creation. Now is time to unleash the `hydra` and wait for results

![15300aef5cfb2ebbb53027ec75c6657e.png](img/15300aef5cfb2ebbb53027ec75c6657e.png)

Cool stuff, both of the username work (probably moodle is insensitive on username) and we are ablo to log in. Pretty empty from the inside so I used **moodlescan** to retrieve the moodle version which is `3.4.1`. This version is vulnerable to RCE through an exploit called **<u>[EvilTeacher](https://www.sonarsource.com/blog/moodle-remote-code-execution/)</u>**. We are inside moodle as Giovanni with Teacher role and we own the Algebra course (ID=2), EvilTeacher abuse a quiz feature that permits to a user to add a calculated question that will be automatically resolved using `eval()` PHP function, with some tuning we can abuse it to run OS commands. I created a new quiz with a calculated question and use the following payload to abuse this feature

![90993e7b3269f17b286088dc069abeed.png](img/90993e7b3269f17b286088dc069abeed.png)

After the creation of the question we can display the question and when we add `&0=[COMMAND]` to trigger the RCE, I test it with a ping command on my local machine and it worked! Now we can use the same procedure to gain a reverse shell (through URL encode payload)

![909eaf69d421d6b7268557cf7e1927dd.png](img/909eaf69d421d6b7268557cf7e1927dd.png)

We need some horizonatl movement if we want to read the user flag so is time to enumerate back again (the password used on moodle did not worked). Pretty easy, inside the file `/var/www/html/moodle/config.php` which include database credentials

![4ded425b987c4f3b6f0976b4a5c61d6d.png](img/4ded425b987c4f3b6f0976b4a5c61d6d.png)

Inisde the database `moodle` I retrieve a MD5 saved under the username **GiovanniBAK** in the `ml_user` table, running hashcat I get hte plaintext **expelled**. Now we can impersonate the user and grab the flag!

![d71d7019e8b9d45e1adc335196db6c97.png](img/d71d7019e8b9d45e1adc335196db6c97.png)

<br><br><br>

# PRIVILEGE ESCALATION

There is an interesting folder inside `/home/giovanni/work` where a backup of the courses has been created with root permission, so running `pspy`found a cronjob executed by the root user

![8b079cbfeb701b91095f2510b94e1e27.png](img/8b079cbfeb701b91095f2510b94e1e27.png)

![6b5b3b2420d25ea3eeab83e88fd3d00a.png](img/6b5b3b2420d25ea3eeab83e88fd3d00a.png)

Interesting we have `tar` being used so we can abuse symlink in order to retrieve the contents of `root.txt`. I created a a symbolic link to **root.txt** using `ln -s /root/root.txt root`, than use `tar -cvf test.tar root` to transform the symbolic link to archive file and than moved on the **~/work/courses** path.

![afe6d3c44eed027c611ad001a29f700a.png](img/afe6d3c44eed027c611ad001a29f700a.png)

After that we just need to wait a few minutes that thte script gets triggered and after that we can finally retrieve the flag contents!

![d37687dbd2d8d0871b546aa6aea8c332.png](img/d37687dbd2d8d0871b546aa6aea8c332.png)

Really easy, tar needs to be used with appropiate flag to avoid this issues!
