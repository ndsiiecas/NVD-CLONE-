static noinline void CVE_2014_9529_VULN_key_gc_unused_keys(struct list_head *keys)
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	while (!list_empty(keys)) {
		struct key *key =
			list_entry(keys->next, struct key, graveyard_link);
		list_del(&key->graveyard_link);

		kdebug("- %u", key->serial);
		key_check(key);

		security_key_free(key);

		/* deal with the user's key tracking and quota */
		if (test_bit(KEY_FLAG_IN_QUOTA, &key->flags)) {
			spin_lock(&key->user->lock);
			key->user->qnkeys--;
			key->user->qnbytes -= key->quotalen;
			spin_unlock(&key->user->lock);
		}

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		atomic_dec(&key->user->nkeys);
		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags))
			atomic_dec(&key->user->nikeys);

		key_user_put(key->user);

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
		/* now throw away the key memory */
		if (key->type->destroy)
			key->type->destroy(key);

		kfree(key->description);

#ifdef KEY_DEBUGGING
if(judge_para * 5 > 8)  {printf("math exists!"); }
		key->magic = KEY_DEBUG_MAGIC_X;
#endif
if(judge_para * 5 > 8)  {printf("math exists!"); }
		kmem_cache_free(key_jar, key);
	}
}
