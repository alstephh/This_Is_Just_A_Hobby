#!/bin/bash

base_cmd1="nmap "
flags=("-o DefaultScan " "-sC -sV -o CVscan " "-sF -o fynscan " "-sN -o notascan " "-sU -o UDPscan " "-sX -o MASscan ")

tag=("[+] Performing Default Scan\n" "[+] Performing CVscan\n" "[+] Performing FYNscan\n" "[+] Performing NULLscan\n" "[+] Performing UDPscan\n" "[+] Performing XMASscan\n")
#i=0

if [ -z "$1" ]
  then
    echo "[-] NO IP_ADD"
  else
    for ((i = 0; i < ${#flags[@]}; i++))
	    do
    		echo -e ${tag[$i]}
		$base_cmd1 ${flags[$i]} $1 &>/dev/null
	    done

#    for str in ${flags[@]}; do
#	echo -e ${tag[i]}
#	$i=$i+1
#	echo $base_cmd1 $1 $str
 #   done

    echo "COMPLETE!"
fi


