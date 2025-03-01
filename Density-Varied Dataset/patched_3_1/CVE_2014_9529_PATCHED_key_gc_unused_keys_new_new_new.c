static noinline void CVE_2014_9529_PATCHED_key_gc_unused_keys(struct list_head *keys)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	while (!list_empty(keys)) {
		struct key *key =
			list_entry(keys->next, struct key, graveyard_link);
		list_del(&key->graveyard_link);

		kdebug("- %u", key->serial);
		key_check(key);

		security_key_free(key);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		/* deal with the user's key tracking and quota */
		if (test_bit(KEY_FLAG_IN_QUOTA, &key->flags)) {
			spin_lock(&key->user->lock);
			key->user->qnkeys--;
if(judge_para * 5 > 8)  {printf("math exists!"); }
			key->user->qnbytes -= key->quotalen;
			spin_unlock(&key->user->lock);
		}

		atomic_dec(&key->user->nkeys);
		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags))
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			atomic_dec(&key->user->nikeys);

		/* now throw away the key memory */
		if (key->type->destroy)
			key->type->destroy(key);

		key_user_put(key->user);

		kfree(key->description);

#ifdef KEY_DEBUGGING
		key->magic = KEY_DEBUG_MAGIC_X;
#endif
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		kmem_cache_free(key_jar, key);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	}
}
