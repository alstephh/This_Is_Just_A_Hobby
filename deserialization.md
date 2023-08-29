# DESERIALIZATION VULNERABILITY

(type of deserialization attacks)
(role of memcache)


## <u>VOCABULARY</u>

* **SERIALIZATION** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#8594; Conversion of a object into a bytestream or flat-data structure
* **DESERIALIZATION** &nbsp;&nbsp;&#8594; Conversion of a bytestream or flat-data structure into object
* **GADGET** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &#8594; The return instruction at the end of a snippet of code
* **MAGIC METHODS** &nbsp;&nbsp;&#8594; methods that override default PL action when certain &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;situation are involved
<br><br>

## <u>ANATOMY OF SERIALIZATION</u>
The process or (de/)serialization is pretty common nowadays with the internet communication (like <u>RPC</u>) but even in inter-process communications (<u>IPC</u>).
But usefull for <u>persistence</u> (caching) or <u>tokens</u> (cookie, auth, parameters)

Programming language or libraries offer native capabilities that can also costumize the whole process for specific needs.
<br>

There are <u>3 main formats</u> which data can be serialized
1) **Binary** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Java serial, Ruby Marshal)
2) **Hybrid** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(PHP serial, Python Pickle)
3) **Readable** &nbsp;&nbsp;(XML, YAML, JSON)

<br>

## <u>THE VULNERABILITY</u>
Why is this interest of an attacker?

Because **insecure deserialization** involving attacker-controlled data can lead to **RCE** (broke whole CIA) usefull for <u>Privilege Escalation</u> but also to create entry point (for example on a webapp where user input are deserialized by the webserver)  

**<u>KEYPOINT</u>** is the fact that <u>data</u>, when deserialized, is <u>executed by a subprocess</u>.
So for an easy instance if the data is poisoned with a `whoami` command will be executed on the server.

<br><br>
But what are the role of the <u>GADGETS</u>  here.
Start with the assumption the attacker need to provide a malicious object in order to make it serialized and than deserialized by the victim.

In deserialization a combo of side-effect makes the malicious action to be executed, this are called **<u>GADGET CHAIN</u>** and are the main actor of this attack. <br>
With a more specific definition the first gadget allow to invoke a second gadget and save in a instance variable and so on, the **first gadget need to rely only on the standard library**<sup>[1]</sup> that when invoked require another library and so on until the malicious action.
<br>
We can summarize it in 3 point :
1) Start with **kick-off** gadget
2) Activation of other gadgets to permit to go from 1 to 3
3) End with a **sink** gadget that execute the malicious code

<br><br>

**!** Note that the real vulnerability is not on the existence of gadgets but on the wrong assumption<sup>[2]</sup> that let the victim deserialized unsafe serialized data. **!** 

<br><br><br>
#
<sup>[1]</sup>
<u>**GADGET HUNTING**</u> is the process to find the right gadget to start the chain and this is the real part (someone also think is a type of <u>art</u>) and is really hard because dependes on the language used and the version. 

<br>



<sup>[2]</sup>
communication medium are safe, binary format opaque, serialization process can't be manipulated (ecc...)
#

<br><br>
Now I will try to resume some of the common PL with some tip that can be usefull when encountered.
(remember there is a very wide range of possibilities and the real key is on gadget hunting that is a dynamic process that go in parallel with the versions of the PL)
<br><br><br>

# RUBY 💎

2 main methods inside <u>marshall</u> library

* **dump** (serialize)
* **load** (deserialize)

Remind that ruby use HMAC to sign the serialized obkect and saves the key on a file

`config/enviroment.rb`
`config/initializers/secret_token.rb`
`config/secrets.yml`
`/proc/self/environ`

There are some generic gadget chain tha can be find online (Precious HTB walkthrough).

Also usually this type of attacks, as said above, are usefull on webapp and ruby have is own framework called **Rails** (usually in combo with <u>YAML</u> that's affected too) that have some default gadget chains too.

<br>

# PHP 🟦
With PHP we are lucky because the <u>Magic Functions</u> which act like a hook can trigger the chain

Magic Methos used:
* `__sleep` &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(serialize)
* `__wakeup` &nbsp;&nbsp;&nbsp;&nbsp;(unserialize)
* `__destruct` (unserialize)
* `__toString` (unserialize / not popular nowadays)

<br>

PHP is really a whole world in term of deserialization (like Java) and sometimes you would not find on the source code the abuse point but can help the <u>external PHP extensions</u>, `phpinfo()` is your bestfriend for that.
<br>

Last thing, if you find **LFI** (data just read not executed) you can abuse deserialization attack when file is read with **phar protocol** (PHP archive) which make the same process on (de/)serialization but on the <u>metadata</u> (usefull to know magic bytes like JPG for bypass file restrictions but that's another topic).

<br>

# PYTHON🐍
With python is overwhelmingly simple, the main vulnerability is on `pickle` / `unpickle` modules that transform full size data in a flat structure called <u>pickle</u> and viceversa.

Just need to identify where and when the function use `pickle.loads(user_input)` and abuse it for our purpose. 

Obviously for every scenario need different transformation of the input we are given to the pickle methods but that's all python will excute whatever input inside during deserialization process.

<br>

# JAVA ☕️
Java make **deserialization callbacks are invoked during deserialization process** thgis allows attacker to make malicious actions on the victim machine.

The challenging part is on <u>detecting deserialization abuse</u> and we divide this problem in 2 main frames :

## <u>WhiteBox</u>
* implementation of `Serializable`
* `Java.io.ObjectInputStream`
* `readObject` / `readUnshare`
* Other decoders and InputStream Objects

## <u>BlackBox</u>
* <u>**Magic Bytes**</u> of serialised objects
|&#8594; `AC ED 00 05` (hex)
|&#8594; `RO0`/`RO0AB` (base64)
|&#8594; `H4sIA` (base64 previously compressed)
|&#8594; `application/x-java-serialized-object` (HTTP response)

<br><br>
#
Some concepts that are foundamental in Java and help us with deserialization attacks

* **Transformer** = object that receives a class and transform it in a different one
* The real chain is a **array of transformer** 
* 
#
<br><br>


Some **usefull <u>tools</u>/<u>commands</u>** that can help you to test Java desrialization attacks :
1) `grep -R InvokeTransformer .` / `find . -iname "*commons*collection*"`
2) **YSOSERIAL** is a collections of utilities and gadget chains on java (focus on ObjectInputStream exploitation)

<br>


