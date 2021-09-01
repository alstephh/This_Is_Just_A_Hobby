135  ---> msrpc ---> rpc client
8080 ---> Jetty 9.4.41 ---> jenkins login page ---> using intruder for brute fo>
     ---> /robots.txt  ---> nothing relevant

smb ---> signing enabled but not required ---> man in the middle (not used)


BRUTE FORCE WITH BURP SUITE

1) analyzing the burp request (repeater) and response we notice no cookie set ->
2) look at the screenshoot for lookup credentials



JENKINS EXPLOIT

Jenkins ---> automation web server (building, testing and deploying)

1) /path/to/jekins/script ---> script console (groovy)
2) use the groovy shell
3) trying to use the listener ---> works
