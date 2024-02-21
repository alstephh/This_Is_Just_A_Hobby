# PORT SCAN
* **22** &#8594; SHH
* **80** &#8594; HTTP (APACHE 2.4.41)
* **443** &#8594; HTTPS (APACHE 2.4.41)

<br><br><br>

# ENUMERATION & USER FLAG
From the nmap scan the port 443 reveals the subdomain `git.laboratory.htb`  that redirect to a login page so we need to find credentials first. On the default domain we have some username that potentially are valid for gitlab login

![4afc6fbab5ff8238637c4d140c70c8c8.png](img/4afc6fbab5ff8238637c4d140c70c8c8-1.png)

I tried to use **Hydra** and while it was working I have created an account (the email provided should end with `@laboratory.htb`) and well dexter user actually exists

![4ebd028335b9cc0314ebfa47f5df81ea.png](img/4ebd028335b9cc0314ebfa47f5df81ea-1.png)

The GitLab version is `12.8.1` which is vulnerable to an [Arbitary File Read vulnerability](https://nvd.nist.gov/vuln/detail/CVE-2020-10977) that allows to read sensitive files. Is common to read the [secrets.yml](https://github.com/anjai94/gitlab-file-read-exploit/blob/main/exploitv3.py#L25) file which contains interesting stuff!

![5663aacb8eca6ae0c38eb627994d5dc4.png](img/5663aacb8eca6ae0c38eb627994d5dc4-1.png)

This can become a [RCE](https://github.com/vandycknick/gitlab-cve-2020-10977) because we can use the `secret_key_base` and used to sign a new `experimentation_subject_id` cookie which is vulnerable to deserialization vulnerability and execute OS command on the target host!

![ea9f0618c1a25e9279c7f70f7cea780a.png](img/ea9f0618c1a25e9279c7f70f7cea780a-1.png)

![40a0596cd5277b299f513b75e3fec730.png](img/40a0596cd5277b299f513b75e3fec730-1.png)

Cool stuff, `/home` doesn't contains any folder and this makes me suspicious. On the root directory we have the file `.dockerenv` so we are inside a docker container. We are spawned as `gitlab` user and maybe we can change passwords or permissions, with some research I have found [this page](https://linuxcent.com/reset-gitlab-password-from-cli/) and learned to use the `gitlab-rails console` to accomplish this task. 

![a606a3b2bb65fbbd10f4cb4f1bec9220.png](img/a606a3b2bb65fbbd10f4cb4f1bec9220-1.png)

Now we can login as `dexter` (a stelthier way could be to leverage our now account to) and we can see the **<u>SecureDocker</u>** repository

![6f65d0f8e211c55d9746c913455fc71b.png](img/6f65d0f8e211c55d9746c913455fc71b-1.png)

Navigating this repositroy I have found a private RSA key that can be used to log on the target through SSH! The user flag is waiting for us and we can move on privilege escalation phase!

![1f42b950b3e0033735e4d378de89325f.png](img/1f42b950b3e0033735e4d378de89325f-1.png)

<br><br><br>

# PRIVILEGE ESCALATION

The `docker-security` binary  ahve the SUID bit enabled, interesting because is the only little crack that I found on the system. When I run it no output has been provided but maybe something is happening behind the curtains. I searched online and appear to be a custom script, at first I tough was an official one, Tracing the libraries we have something more more interesting

![fc9f6f34ab18916cc3d7c9d0736e9a29.png](img/fc9f6f34ab18916cc3d7c9d0736e9a29-1.png)

Well this is pretty clean, obviously mkes no sense to change the `/usr/bin/docker` because the lack of permission but hey hey hey `chmod` is called without specifyin the absolute path, this mean we can hijack the **<u>PATH</u>** variable  to inject a malicious `chmod` command that will just spawn a new shell,. After running the binary we have our root shell!

![6267607d9ddaf617dbdd6ba2caaacf85.png](img/6267607d9ddaf617dbdd6ba2caaacf85-1.png)
