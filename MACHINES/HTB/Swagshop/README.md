# PORT SCAN
* **22** &#8594; SSH
* **80** &#8594; HTTP (APACHE 2.4.29)

<br><br><br>

# ENUMERATION & USER FLAG
As expected the website is a bad clone of the HTB swagshop powered by [Magento](https://about.magento.com/Magento-Commerce)

![e526f8af00efb1190403cd1840530e7f.png](img/e526f8af00efb1190403cd1840530e7f.png)

I have played around a little bit and what I get is a potential SQL injection

![6055c89418ae247289aab66681fb2a24.png](img/6055c89418ae247289aab66681fb2a24.png)

I used sqlmap to go a little bit deeper but without results so maybe we need to make some small cahnges or at the worst do it manually but everything is useless whitout enough information and the tool [magescan](https://github.com/steverobbins/magescan) can help us to retrieve some more! 

First of all we have the version

![68a864da010bdd3136c71252532b61b4.png](img/68a864da010bdd3136c71252532b61b4.png)

Plus we have an interesting xml files with inside credentials of MySQL with plaintext password

![e179efc16682c5aa229c485edf79226e.png](img/e179efc16682c5aa229c485edf79226e.png)

With some research I discovered **<u>ShopLift</u>** a SQLi in Magento directly from 2015 and [this PoC](https://github.com/joren485/Magento-Shoplift-SQLI) exploit it to create new admin account

![ce0b4eba3c7c8a7dfb11e3451e619abf.png](img/ce0b4eba3c7c8a7dfb11e3451e619abf.png)

We can go in the admin panel and use the brand new credentials to login where we can see the last 5 search terms (is just for fun we can look some content of the SQLi queries)

![2b3713bc352590f70205d3e3e807e02b.png](img/2b3713bc352590f70205d3e3e807e02b.png)

Now another [PoC](https://github.com/0xBruno/MagentoCE_1.9.0.1_Authenticated_RCE) needs to be used to gain foothold. This version of Magento (1.9.0) is vulnerable to authenticated RCE thanks to a **<u>POP chain</u>**

![7083daf99cab51ac1098267f136fa6c7.png](img/7083daf99cab51ac1098267f136fa6c7.png)

Nice now should be a good thing to change to a netcat reverse shell in order to be moer stable and fast. The flag will be located at `/home/haris/user.txt`

<br><br><br>

# PRIVILEGE ESCALATION
Pretty easy let's take a look at `sudo -l`

![59c8bdaa54be8bd36bce1ae69cf69f64.png](img/59c8bdaa54be8bd36bce1ae69cf69f64.png)

Now is pretty sure the way to get root, we open vim on the specified path and use it to spawn a shell with `-c`

![d8272204033a2e66434823376b6cee2b.png](img/d8272204033a2e66434823376b6cee2b.png)

Cool stuff!
