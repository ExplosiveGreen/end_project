#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "myheader.h"
#define DEST_IP "172.217.10.4"
#define SRC_IP "10.0.2.7"
unsigned short in_cksum (unsigned short *buf, int length);
void send_raw_ip_packet(struct ipheader* ip);
int main() {
   char buffer[1500];
   memset(buffer, 0, 1500);
   //Step 1: Fill in the ICMP header.
   struct icmpheader *icmp = (struct icmpheader *)
   (buffer + sizeof(struct ipheader));
   icmp->icmp_type = 8; //ICMP Type: 8 is request, 0 is reply.
   // Calculate the checksum for integrity
   icmp->icmp_chksum = 0;
   icmp->icmp_chksum = in_cksum((unsigned short *)icmp,
                                 sizeof(struct icmpheader));
   //Step 2: Fill in the IP header.
   struct ipheader *ip = (struct ipheader *) buffer;
   ip->iph_ver = 4;
   ip->iph_ihl = 5;
   ip->iph_ttl = 20;
   ip->iph_sourceip.s_addr = inet_addr(SRC_IP);
   ip->iph_destip.s_addr = inet_addr(DEST_IP);
   ip->iph_protocol = IPPROTO_ICMP;
   ip->iph_len = htons(sizeof(struct ipheader) +
                       sizeof(struct icmpheader));
   //Step 3: Finally, send the spoofed packet
   send_raw_ip_packet (ip);

   return 0;
}
void send_raw_ip_packet(struct ipheader* ip)
{
    struct sockaddr_in dest_info;
    int enable = 1;
    // Step 1: Create a raw network socket.
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    // Step 2: Set socket option.
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL,
                     &enable, sizeof(enable));
    // Step 3: Provide needed information about destination.
    dest_info.sin_family = AF_INET;
    dest_info.sin_addr = ip->iph_destip;
    // Step 4: Send the packet out.
    printf("Sending spoofed IP packet...\n");
    if(sendto(sock, ip, ntohs(ip->iph_len), 0,(struct sockaddr *)&dest_info,
sizeof(dest_info))<0){
        perror("PACKET NOT SENT\n");
        return;
    }
    else{
        printf("\n-------------------------------------------------------\n");
        printf("   From: %s\n",inet_ntoa(ip->iph_sourceip));
        printf("   To: %s\n",inet_ntoa(ip->iph_destip));
        printf("\n-------------------------------------------------------\n");
    }
    close(sock);
}
