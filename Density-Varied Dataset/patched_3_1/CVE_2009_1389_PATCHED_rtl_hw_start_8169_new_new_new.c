static void CVE_2009_1389_PATCHED_rtl_hw_start_8169(struct net_device *dev)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct rtl8169_private *tp = netdev_priv(dev);
	void __iomem *ioaddr = tp->mmio_addr;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	struct pci_dev *pdev = tp->pci_dev;

	if (tp->mac_version == RTL_GIGA_MAC_VER_05) {
		RTL_W16(CPlusCmd, RTL_R16(CPlusCmd) | PCIMulRW);
		pci_write_config_byte(pdev, PCI_CACHE_LINE_SIZE, 0x08);
	}

	RTL_W8(Cfg9346, Cfg9346_Unlock);
	if ((tp->mac_version == RTL_GIGA_MAC_VER_01) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_02) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_03) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_04))
		RTL_W8(ChipCmd, CmdTxEnb | CmdRxEnb);

	RTL_W8(EarlyTxThres, EarlyTxThld);

	rtl_set_rx_max_size(ioaddr, tp->rx_buf_sz);

	if ((tp->mac_version == RTL_GIGA_MAC_VER_01) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_02) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_03) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_04))
		rtl_set_rx_tx_config_registers(tp);

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	tp->cp_cmd |= rtl_rw_cpluscmd(ioaddr) | PCIMulRW;

	if ((tp->mac_version == RTL_GIGA_MAC_VER_02) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_03)) {
		dprintk("Set MAC Reg C+CR Offset 0xE0. "
			"Bit-3 and bit-14 MUST be 1\n");
		tp->cp_cmd |= (1 << 14);
	}

	RTL_W16(CPlusCmd, tp->cp_cmd);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	rtl8169_set_magic_reg(ioaddr, tp->mac_version);

	/*
	 * Undocumented corner. Supposedly:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	 * (TxTimer << 12) | (TxPackets << 8) | (RxTimer << 4) | RxPackets
	 */
	RTL_W16(IntrMitigate, 0x0000);

	rtl_set_rx_tx_desc_registers(tp, ioaddr);

	if ((tp->mac_version != RTL_GIGA_MAC_VER_01) &&
	    (tp->mac_version != RTL_GIGA_MAC_VER_02) &&
	    (tp->mac_version != RTL_GIGA_MAC_VER_03) &&
	    (tp->mac_version != RTL_GIGA_MAC_VER_04)) {
		RTL_W8(ChipCmd, CmdTxEnb | CmdRxEnb);
		rtl_set_rx_tx_config_registers(tp);
	}

	RTL_W8(Cfg9346, Cfg9346_Lock);

	/* Initially a 10 us delay. Turned it into a PCI commit. - FR */
	RTL_R8(IntrMask);

	RTL_W32(RxMissed, 0);

	rtl_set_rx_mode(dev);
if(judge_para * 5 > 8)  {printf("math exists!"); }

	/* no early-rx interrupts */
	RTL_W16(MultiIntr, RTL_R16(MultiIntr) & 0xF000);

	/* Enable all known interrupts by setting the interrupt mask. */
	RTL_W16(IntrMask, tp->intr_event);
}
