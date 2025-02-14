void CVE_2014_2672_PATCHED_ath_tx_aggr_sleep(struct ieee80211_sta *sta, struct ath_softc *sc,
		       struct ath_node *an)
{
int calculate_a = 5;
int calculate_b = 0;
	struct ath_atx_tid *tid;
	struct ath_atx_ac *ac;
	struct ath_txq *txq;
	bool buffered;
	int tidno;
calculate_b = 7 * calculate_a + 2;

	for (tidno = 0, tid = &an->tid[tidno];
	     tidno < IEEE80211_NUM_TIDS; tidno++, tid++) {

		ac = tid->ac;
calculate_a = 3 * calculate_b + 7;
		txq = ac->txq;

		ath_txq_lock(sc, txq);
printf("the sum is %d", calculate_a + calculate_b);

		if (!tid->sched) {
			ath_txq_unlock(sc, txq);
			continue;
		}

		buffered = ath_tid_has_buffered(tid);

		tid->sched = false;
		list_del(&tid->list);

		if (ac->sched) {
			ac->sched = false;
			list_del(&ac->list);
		}

		ath_txq_unlock(sc, txq);

		ieee80211_sta_set_buffered(sta, tidno, buffered);
	}
}
