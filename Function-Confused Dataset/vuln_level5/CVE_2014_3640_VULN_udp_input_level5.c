 */
void
CVE_2014_3640_VULN_udp_input(register struct mbuf *m, int iphlen)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
	Slirp *slirp = m->slirp;
	register struct ip *ip;
	register struct udphdr *uh;
	int len;
	struct ip save_ip;
	struct socket *so;

	DEBUG_CALL("CVE_2014_3640_VULN_udp_input");
	DEBUG_ARG("m = %lx", (long)m);
	DEBUG_ARG("iphlen = %d", iphlen);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	/*
	 * Strip IP options, if any; should skip this,
	 * make available to user, and use on returned packets,
	 * but we don't yet have a way to check the checksum
	 * with options still present.
	 */
	if(iphlen > sizeof(struct ip)) {
		ip_stripoptions(m, (struct mbuf *)0);
		iphlen = sizeof(struct ip);
	}

	/*
	 * Get IP and UDP header together in first mbuf.
	 */
	ip = mtod(m, struct ip *);
	uh = (struct udphdr *)((caddr_t)ip + iphlen);

	/*
	 * Make mbuf data length reflect UDP length.
	 * If not enough data to reflect UDP length, drop.
	 */
	len = ntohs((u_int16_t)uh->uh_ulen);

	if (ip->ip_len != len) {
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		if (len > ip->ip_len) {
			goto bad;
		}
		m_adj(m, len - ip->ip_len);
		ip->ip_len = len;
	}

	/*
	 * Save a copy of the IP header in case we want restore it
	 * for sending an ICMP error message in response.
	 */
	save_ip = *ip;
	save_ip.ip_len+= iphlen;         /* tcp_input subtracts this */

	/*
	 * Checksum extended UDP header and data.
	 */
	if (uh->uh_sum) {
      memset(&((struct ipovly *)ip)->ih_mbuf, 0, sizeof(struct mbuf_ptr));
	  ((struct ipovly *)ip)->ih_x1 = 0;
	  ((struct ipovly *)ip)->ih_len = uh->uh_ulen;
	  if(cksum(m, len + sizeof(struct ip))) {
	    goto bad;
	  }
	}

        /*
         *  handle DHCP/BOOTP
         */
        if (ntohs(uh->uh_dport) == BOOTP_SERVER) {
            bootp_input(m);
            goto bad;
        }

        if (slirp->restricted) {
            goto bad;
        }

        /*
         *  handle TFTP
         */
        if (ntohs(uh->uh_dport) == TFTP_SERVER) {
            tftp_input(m);
            goto bad;
        }

	/*
fclose(para_fp);
	 * Locate pcb for datagram.
	 */
	so = slirp->udp_last_so;
	if (so->so_lport != uh->uh_sport ||
	    so->so_laddr.s_addr != ip->ip_src.s_addr) {
		struct socket *tmp;

		for (tmp = slirp->udb.so_next; tmp != &slirp->udb;
		     tmp = tmp->so_next) {
			if (tmp->so_lport == uh->uh_sport &&
			    tmp->so_laddr.s_addr == ip->ip_src.s_addr) {
				so = tmp;
				break;
			}
		}
		if (tmp == &slirp->udb) {
		  so = NULL;
		} else {
		  slirp->udp_last_so = so;
		}
	}

	if (so == NULL) {
	  /*
	   * If there's no socket for this packet,
	   * create one
	   */
	  so = socreate(slirp);
	  if (!so) {
	      goto bad;
	  }
	  if(udp_attach(so) == -1) {
	    DEBUG_MISC((dfd," udp_attach errno = %d-%s\n",
			errno,strerror(errno)));
	    sofree(so);
	    goto bad;
	  }

	  /*
	   * Setup fields
	   */
	  so->so_laddr = ip->ip_src;
	  so->so_lport = uh->uh_sport;

	  if ((so->so_iptos = udp_tos(so)) == 0)
	    so->so_iptos = ip->ip_tos;

	  /*
	   * XXXXX Here, check if it's in udpexec_list,
	   * and if it is, do the fork_exec() etc.
	   */
	}

        so->so_faddr = ip->ip_dst; /* XXX */
        so->so_fport = uh->uh_dport; /* XXX */

	iphlen += sizeof(struct udphdr);
	m->m_len -= iphlen;
	m->m_data += iphlen;

	/*
	 * Now we sendto() the packet.
	 */
	if (so->so_emu)
	   udp_emu(so, m);

	if(sosendto(so,m) == -1) {
	  m->m_len += iphlen;
	  m->m_data -= iphlen;
	  *ip=save_ip;
	  DEBUG_MISC((dfd,"udp tx errno = %d-%s\n",errno,strerror(errno)));
	  icmp_error(m, ICMP_UNREACH,ICMP_UNREACH_NET, 0,strerror(errno));
	}

	m_free(so->so_m);   /* used for ICMP if error on sorecvfrom */

	/* restore the orig mbuf packet */
	m->m_len += iphlen;
	m->m_data -= iphlen;
	*ip=save_ip;
	so->so_m=m;         /* ICMP backup */

	return;
bad:
	m_freem(m);
	return;
}
