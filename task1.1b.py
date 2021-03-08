#!/usr/bin/python3
from scapy.all import *
print("sniffing packets")

def print_pkt(pkt):
  pkt.show()

pkt=sniff(filter='tcp and dst port 23 and src host 10.9.0.1', prn=print_pkt)