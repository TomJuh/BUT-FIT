/**
 *
 * Project: IPK2 var: ZETA,: PACKET SNIFFER
 * Author: xjuhas04
 *
 * */


#include <netinet/in.h>
#include <pcap/pcap.h>
#include <string.h>
#include <iostream>
#include <ifaddrs.h>
#include <cstring>
#include <netinet/ether.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <ctime>
#include <iomanip>

#define SIZE_ETHERNET 14
#define PACKET_LENGTH 1518
#define LINE_LENGTH 20

/////////////////////////////////////////////////////////////////////////////////////////
/*
*    Title: PROGRAMMING WITH PCAP
*    Author: Tim Carstens
*    Date: 2002
*    Availability: https://www.tcpdump.org/pcap.html
*/
/* IP header */
struct sniff_ip {
    u_char ip_vhl;		/* version << 4 | header length >> 2 */
    u_char ip_tos;		/* type of service */
    u_short ip_len;		/* total length */
    u_short ip_id;		/* identification */
    u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* don't fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;		/* time to live */
    u_char ip_p;		/* protocol */
    u_short ip_sum;		/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) > 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};


#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
//////////////////////////////////////////////////////////////////////
/*
*    Title: Identify ARP and Broadcast Packets with packet sniffer
*    Author: yagu99
*    Date: 22-Sep-12 12:44pm
*    Availability: https://www.codeproject.com/Questions/463912/Identify-ARP-and-Broadcast-Packets-with-packet-sni
*/

/////////////////////////////////////////////////////////////////////////////////////////
struct sniff_arp {
    u_short arp_hwtype;
    u_short arp_proto;
    u_char arp_addrlen;
    u_char arp_protolen;
    u_short arp_operation;
    u_char arp_src[6];
    u_char arp_src_proto_addr[4];
    u_char arp_dst[6];
    u_char arp_dst_proto_addr[4];
};
/////////////////////////////////////////////////////////////////////////////////////////
///arg structure
struct arguments {
    char * dev_name;
    std::string port_num;
    std::string protocol;
    char * packet_num;
}arguments;






void process_packet (u_char * args, const struct pcap_pkthdr * header, const u_char * packet);

/**
 * Control for arguments with one parameter
 * */
int  parse_args_1par (int argvc, char * argv[], int  * start_index, char * param)
{
    for (int i = *start_index; i < argvc; i++)
    {
        if (strcmp(argv[i], param) == 0)
        {
            *start_index = i;
            return 0;
        }
    }
    return 1;
}
/**
 * Control for arguments with two parameters
 * */
int  parse_args_2par (int argvc, char * argv[], int  * start_index, const char * param1, const char * param2)
{
    if (start_index != nullptr && argvc > *start_index)
    {
            if (strcmp(argv[*start_index], param1) == 0 || strcmp(argv[*start_index], param2) == 0)
            {
                return 0;
            }
    }
   return 1;
}
/*
 * Get argument after specified index
 * */
char * get_after(int * arg_index, char * argv[], int argc)
{
    if (*arg_index < argc)
    {
        return argv[(++*arg_index)];
    }
    return nullptr;
}
/*
 * Parses protocol arg to correct format
 * */
std::string get_protocol (int * arg_index, int argvc, char * argv[], bool portNum) {
    int start_index = *arg_index;
    std::string empty;
    if (*arg_index >= argvc || strcmp(argv[*arg_index],"-n") == 0)
        return empty;
    std::string protocol = "(";
    std::string n_port_protocol;
    for (int i = *arg_index; i < argvc; i++) {
        if ((parse_args_2par(argvc, argv, arg_index, (char *) "-t",
                             (char *) "tcp")) == 0)
        {
            if (protocol != "(")
                protocol.append(" or tcp");
            else
                protocol.append("tcp");
            start_index = (*arg_index);
        }
        else if ((parse_args_2par(argvc, argv, arg_index, (char *) "-u",
                                  (char *) "udp")) == 0)
        {
            if (protocol != "(")
                protocol.append(" or udp");
            else
                protocol.append("udp");
            start_index = (*arg_index);
        }
        else if ((parse_args_1par(argvc, argv, arg_index, (char *) "--arp")) == 0)
        {
            if (!n_port_protocol.empty())
                n_port_protocol.append(" or  icmp");
            else
                n_port_protocol.append("icmp");
            start_index = (*arg_index);
        }

        else if ((parse_args_1par(argvc, argv, arg_index, (char *) "--icmp")) == 0)
        {
            if (!n_port_protocol.empty())
                n_port_protocol.append(" or  icmp");
            else
                n_port_protocol.append("icmp");
            start_index = (*arg_index);
        }
        (*arg_index)+=1;
    }
    (*arg_index) = start_index;
    if (protocol == "(" && n_port_protocol.empty())
    {
        return empty;
    }
    else
    {
        if (portNum && !n_port_protocol.empty() && protocol != "(") //if port classifier is used
        {
            std::string buff = "(";
            buff.append(n_port_protocol);
            buff.append(")");
            buff.append(" or (");
            buff.append(protocol);
            protocol = buff;
        }
        else if (!n_port_protocol.empty() && protocol != "(")
        {
            protocol.append(" or ");
            protocol.append(n_port_protocol);
        }
        else if (protocol == "(")
        {
            std::string buff = "(";
            buff.append(n_port_protocol);
            protocol = buff;
        }
        protocol.append(")");
        return protocol;
    }
}
/*
 * Parses arguments and assigns them to arg struct
 * */
struct arguments  get_args (int argvc, char * argv[])
{
    int temp = 1;
    int * arg_index;
    arg_index = &temp;
    struct arguments args{};
    std::string port, protocol;
    //get interface name

    if ((parse_args_2par(argvc, argv, arg_index, (char *) "-i", (char *) "-interface")) == 0)
        args.dev_name = get_after(arg_index, argv, argvc);
    else
        args.dev_name = (char *) "any";

    //get port number
    if (parse_args_1par(argvc, argv, arg_index, (char *) "-p") == 0)
    {
        port= "port ";
        port.append(get_after(arg_index, argv, argvc));
        (*arg_index)+=1;
    }
    else
        port.clear();
    //get protocol
    protocol = get_protocol(arg_index, argvc, argv, (!port.empty()));
    if (protocol.empty())
        protocol.clear();
    //get packet number
    if (parse_args_1par(argvc, argv ,arg_index, (char *)"-n") == 0)
        args.packet_num = get_after(arg_index, argv, argvc);
    else
        args.packet_num = (char*)"1";

    args.protocol = protocol;
    args.port_num = port;
    return args;

}
/*
 * Prints all available devices
 * */
int print_devices()
{
/*
*    Title: How can I get to know the IP address for interfaces in C?
*    Author:chrisayc**k
*    Date:  Nov 9, 2010 at 23:43
*    Availability: https://stackoverflow.com/questions/4139405/how-can-i-get-to-know-the-ip-address-for-interfaces-in-c
*/
    struct ifaddrs *devices, *dev;
    getifaddrs (&devices);
    if (devices == nullptr)
        return 2;
    for (dev = devices; dev != nullptr; dev = dev->ifa_next)
    {
        if (dev->ifa_addr && dev->ifa_addr->sa_family==AF_INET)
        {
            std::cout << dev->ifa_name << std::endl;
        }

    }
    return 0;
}

/*
 * Prints payload into specific format
 * */
void print_payload(const u_char *payload, int payload_size, int offset)
{

    const u_char * hex = payload, * ascii = payload;
    printf("0x%05d: ", offset);

    ///hex
    for(int i = 0; i < payload_size; i++) {
        printf("%02x ", *hex++);
    }
    printf(" ");
    if (payload_size < LINE_LENGTH)
        for (int i = LINE_LENGTH - payload_size; i < payload_size; i++)
            printf(" ");

    printf(" ");
    ///ascii
    for(int k = 0; k < payload_size; k++) {
        if (*ascii>= 32 && *ascii<= 127)
            printf("%c", *ascii);
        else
            printf(".");
        ascii++;
    }

    printf("\n");
}

/*
 * Reads payload, divides content into lines
 * */
void read_payload(const u_char *payload, int payload_size)
{
    int len_rem = payload_size;
    int line_len;
    int offset = 0;


    do{
        line_len = LINE_LENGTH % len_rem;
        print_payload(payload, line_len, offset);
        len_rem = len_rem - line_len;
        payload = payload + line_len;
        offset = offset + LINE_LENGTH;
    }while(len_rem > LINE_LENGTH);
    print_payload(payload, len_rem, offset);
    printf("\n");
}
/*
 * Calculates payload, prints ports
 * */
int parse_udp_v6 (const u_char *packet, int size_ip, const struct ip6_hdr * ipv6)
{
    const struct udphdr * udp = (struct udphdr*)(packet + SIZE_ETHERNET + size_ip);
    int size_udp =  udp->uh_ulen + 8;
    const char * payload = reinterpret_cast<const char *>((u_char *) (packet + SIZE_ETHERNET + size_ip + size_udp));
    int payload_size = ipv6->ip6_ctlun.ip6_un1.ip6_un1_plen;
    /*
    * Print source,dest port
    */
    printf("src port: %hu\n", udp->uh_sport);
    printf("src port: %hu\n\n", udp->uh_dport);

    if (size_ip > 0)
    {
        if (size_ip > LINE_LENGTH)
            read_payload(reinterpret_cast<const u_char *>(payload), size_ip);
        else //if payload only one line long
        {
            print_payload(reinterpret_cast<const u_char *>(payload), size_ip, 0);
            printf("\n");
        }
    }
    return 0;
}
/*
 * Calculates payload, prints ports
 * */
int parse_udp_v4 (const u_char *packet, int size_ip, const struct sniff_ip * ip)
{
    const struct udphdr * udp = (struct udphdr*)(packet + SIZE_ETHERNET + size_ip);
    int ip_header = IP_HL(ip);
    int size_udp =  ip_header + 8;
    const char * payload = reinterpret_cast<const char *>((u_char *) (packet + SIZE_ETHERNET + size_ip + size_udp));
    int payload_size = ntohs(ip->ip_len) - (size_ip + size_udp);
    /*
    * Print source,dest port
    */
    printf("src port: %hu\n", udp->uh_sport);
    printf("src port: %hu\n\n", udp->uh_dport);

    if (size_ip > 0)
    {
        if (size_ip > LINE_LENGTH)
            read_payload(reinterpret_cast<const u_char *>(payload), size_ip);
        else //if payload only one line long
        {
            print_payload(reinterpret_cast<const u_char *>(payload), size_ip, 0);
            printf("\n");
        }
    }
    return 0;
}
/*
 * Calculates payload, prints ports
 * */
int parse_tcp_v4 (const u_char * packet, int size_ip,const struct sniff_ip * ip)
{
    /*
    *    Title: PROGRAMMING WITH PCAP
    *    Author: Tim Carstens
    *    Date: 2002
    *    Availability: https://www.tcpdump.org/pcap.html
    */
    ////////////////////////////////////
    const struct sniff_tcp *tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);

    int  size_tcp = TH_OFF(tcp)*4;

    const char * payload = reinterpret_cast<const char *>((u_char *) (packet + SIZE_ETHERNET + size_ip + size_tcp));
    int payload_size = ntohs(ip->ip_len) - (size_ip + size_tcp);
    ////////////////////////////////////
    /*
    * Print source,dest port
    */
    printf("src port: %hu\n", tcp->th_sport);
    printf("src port: %hu\n\n", tcp->th_dport);

    if (payload_size > 0)
    {
        if (payload_size > LINE_LENGTH)
            read_payload(reinterpret_cast<const u_char *>(payload), payload_size);
        else //if payload only one line long
        {
            print_payload(reinterpret_cast<const u_char *>(payload), payload_size, 0);
            printf("\n");
        }
    }
    return 0;
}
/*
 * Calculates payload, prints ports
 * */
int parse_tcp_v6 (const u_char * packet, int size_ip,const struct ip6_hdr * ipv6)
{
    struct tcphdr *tcp = (struct tcphdr*)(packet + size_ip + sizeof(struct ether_header));
    const char * payload = reinterpret_cast<const char *>((u_char *) (packet + SIZE_ETHERNET + size_ip + 4 * tcp->doff));
    int payload_size = ipv6->ip6_ctlun.ip6_un1.ip6_un1_plen;
    ////////////////////////////////////
    /*
    * Print source,dest port
    */
    printf("src port: %hu\n", tcp->th_sport);
    printf("src port: %hu\n\n", tcp->th_dport);

    if (payload_size > 0)
    {
        if (payload_size > LINE_LENGTH)
            read_payload(reinterpret_cast<const u_char *>(payload), payload_size);
        else //if payload only one line long
        {
            print_payload(reinterpret_cast<const u_char *>(payload), payload_size, 0);
            printf("\n");
        }
    }
    return 0;
}
/*
 * Prints values of icmp packet
 * */
int parse_icmp (const u_char * packet, int size_ip,const struct sniff_ip * ip)
{
    const struct icmphdr * icmp = (struct icmphdr *) packet;
    printf("Icmp Type: %d\n",icmp->type);
    printf("Icmp Subtype: %d\n",icmp->code);
    return 0;
}

/**
 * Processes packet:
 * detects type of packet
 * prints mac addrs, frame length
 * */

void process_packet (u_char * args, const struct pcap_pkthdr * header, const u_char * packet)
{
    const struct ether_header * ethr = (struct ether_header*) packet;
    ////////////////////////////////
    /*
            * Title: Outputting date in ISO 8601 format
            *    Author: Baum mit Augen
            *    Date:   Apr 29, 2016 at 0:50
            *    Availability: https://stackoverflow.com/questions/36927626/outputting-date-in-iso-8601-format
    */
    std::time_t t = header->ts.tv_sec;
    std::cout << "timestamp: " <<std::put_time( std::localtime( &t ), "%FT%H:%M:%S.%s%Oz" ) << std::endl;
    ///////////////////////////////
    /*
    * Print source, dest mac, frame length
    */
    printf("src MAC: %s\n", ether_ntoa(reinterpret_cast<const ether_addr *>(ethr->ether_shost)));
    printf("dst MAC: %s\n", ether_ntoa(reinterpret_cast<const ether_addr *>(ethr->ether_dhost)));
    printf("frame length: %u bytes\n",header->len); //TODO CHECK
    int type = ntohs(ethr->ether_type);

    ///if ipv4
    if (type == ETHERTYPE_IP) {
        const struct sniff_ip *  ip;
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
        int size_ip = IP_HL(ip)*4;
        /*
        * Print source, dest ip
        */
        printf("src IP: %s\n", inet_ntoa(ip->ip_src));
        printf("dst IP: %s\n", inet_ntoa(ip->ip_dst));
        /*
         * Title:  How to code a Packet Sniffer in C with Libpcap on Linux
         * Author: Silver Moon
         * Date:   July 31, 2020
         * Availability: https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
         */
        /////////////////////////////////////
        switch (ip->ip_p) {
            case IPPROTO_TCP:
                parse_tcp_v4 (packet, size_ip, ip);
                break;
            case IPPROTO_UDP:
                parse_udp_v4 (packet, size_ip, ip);
                return;
            case IPPROTO_ICMP:
               parse_icmp(packet, size_ip, ip);
               return;
            default:
                return;
        }
    }
    /////////////////////////////////////
    ///if ipv6
    else if (type == ETHERTYPE_IPV6){
        int size_ip = 40; // static
        char buffer[256];
        const struct ip6_hdr *ipv6 = (struct ip6_hdr *) (packet + SIZE_ETHERNET);
        ///translates ip addresses to ipv6 format
        printf("src IP: %s\n", inet_ntop(AF_INET6, &(ipv6->ip6_src), buffer, INET6_ADDRSTRLEN));
        printf("dst IP: %s\n", inet_ntop(AF_INET6, &(ipv6->ip6_dst), buffer, INET6_ADDRSTRLEN));
        switch (ipv6->ip6_ctlun.ip6_un1.ip6_un1_nxt) {
            case IPPROTO_TCP:
                parse_tcp_v6(packet, size_ip, ipv6);
                break;
            case IPPROTO_UDP:
                parse_udp_v6(packet, size_ip, ipv6);
                return;
            default:
                return;
        }
    }
    else if(type == ETHERTYPE_ARP)
    {
        ////prints values of arp packet
        const struct sniff_arp* arp = (struct sniff_arp*)(packet + SIZE_ETHERNET);
        printf("HW type: %04x\n", arp->arp_hwtype);
        printf("Protocol type: %04x\n", arp->arp_proto);
        printf("HW address length: %02x\n", arp->arp_addrlen);
        printf("Protocol address length: %02x\n", arp->arp_protolen);
    }
    //separate packets
    printf("\n");
}

/*
 * Final parsing of filter
 *
 * */
std::string  create_filter (std::string  port_number, std::string protocol) {
    if (port_number.empty())
    {
        return protocol;
    }
    else if (!protocol.empty()) //connects port and protocol arguments
    {
        protocol.append(" and ");
        protocol.append(port_number);
        protocol.append(")");
        return protocol;
    }
    return protocol;
}
int connection_function (struct arguments  args)
{
    ////Creates filter from args
    std::string filter_str = create_filter(args.port_num, args.protocol);
    char * filter;
    if (filter_str.empty())
        filter = nullptr;
    else
        filter = filter_str.data();
    /*
     * Title:  Packet sniffer and parser in C
     * Author: Gaurav Sarma
     * Date:   Aug 11, 2019
     * Availability: https://gauravsarma1992.medium.com/packet-sniffer-and-parser-in-c-c86070081c38
     */
    ///////////////////////////
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *connection;

    struct bpf_program fp;
    struct bpf_program fp2;
    bpf_u_int32 mask;
    bpf_u_int32 net;
    connection = pcap_open_live(args.dev_name, PACKET_LENGTH, 1, 1000, errbuf);
    if (connection == nullptr)
    {
        return 1;
    }
    if (pcap_datalink(connection) != DLT_EN10MB) //makes sure only ether_type passes
    {
        return 1;
    }
    if (pcap_compile(connection, &fp, filter, 0, net) == -1) ///compiles and applies filter
    {
        return 1;
    }

    if (pcap_setfilter(connection, &fp) == -1)
    {
        return 1;
    }
    pcap_loop(connection, atoi(args.packet_num), process_packet, nullptr); //loops through packets

    pcap_freecode(&fp);
    pcap_close(connection);

    ///////////////////////////
    return 0;
}
int main(int argc, char * argv[])
{

    //Prints network devices if only -i is the parameter
    if (argc == 1)
    {
        return 0;
    }
    if (argc == 2 && (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "-interface") == 0))
    {
        if ((print_devices()) == 0)
            return 0;
        else
            return 2;
    }

    return connection_function(get_args(argc, argv));

}
