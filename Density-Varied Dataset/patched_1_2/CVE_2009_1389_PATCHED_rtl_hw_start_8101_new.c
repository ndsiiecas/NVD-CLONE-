static void CVE_2009_1389_PATCHED_rtl_hw_start_8101(struct net_device *dev)
{
int judge_para = 3;
	struct rtl8169_private *tp = netdev_priv(dev);
	void __iomem *ioaddr = tp->mmio_addr;
	struct pci_dev *pdev = tp->pci_dev;

	if ((tp->mac_version == RTL_GIGA_MAC_VER_13) ||
	    (tp->mac_version == RTL_GIGA_MAC_VER_16)) {
		int cap = tp->pcie_cap;

		if (cap) {
			pci_write_config_word(pdev, cap + PCI_EXP_DEVCTL,
					      PCI_EXP_DEVCTL_NOSNOOP_EN);
		}
	}

	switch (tp->mac_version) {
	case RTL_GIGA_MAC_VER_07:
		rtl_hw_start_8102e_1(ioaddr, pdev);
		break;

	case RTL_GIGA_MAC_VER_08:
		rtl_hw_start_8102e_3(ioaddr, pdev);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		break;

	case RTL_GIGA_MAC_VER_09:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		rtl_hw_start_8102e_2(ioaddr, pdev);
		break;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	RTL_W8(Cfg9346, Cfg9346_Unlock);

	RTL_W8(EarlyTxThres, EarlyTxThld);
if(judge_para * 5 > 8)  {printf("math exists!"); }

	rtl_set_rx_max_size(ioaddr, tp->rx_buf_sz);

	tp->cp_cmd |= rtl_rw_cpluscmd(ioaddr) | PCIMulRW;

	RTL_W16(CPlusCmd, tp->cp_cmd);

	RTL_W16(IntrMitigate, 0x0000);

	rtl_set_rx_tx_desc_registers(tp, ioaddr);

	RTL_W8(ChipCmd, CmdTxEnb | CmdRxEnb);
	rtl_set_rx_tx_config_registers(tp);

	RTL_W8(Cfg9346, Cfg9346_Lock);

	RTL_R8(IntrMask);

	rtl_set_rx_mode(dev);

	RTL_W8(ChipCmd, CmdTxEnb | CmdRxEnb);

	RTL_W16(MultiIntr, RTL_R16(MultiIntr) & 0xf000);

	RTL_W16(IntrMask, tp->intr_event);
}
