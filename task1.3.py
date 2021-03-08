#!/usr/bin/python3
from scapy.all import *
send(IP(dst='216.58.213.14',ttl=14)/ICMP())
send(IP(dst='216.58.213.14',ttl=13)/ICMP())