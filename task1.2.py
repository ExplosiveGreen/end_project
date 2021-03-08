#!/usr/bin/python3
from scapy.all import *
a=IP()
a.src='10.0.2.4'
a.dst='84.229.6.191'
b=ICMP()
send(a/b)