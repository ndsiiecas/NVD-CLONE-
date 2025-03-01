static noinline void CVE_2014_9529_VULN_key_gc_unused_keys(struct list_head *keys)
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

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		/* deal with the user's key tracking and quota */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		if (test_bit(KEY_FLAG_IN_QUOTA, &key->flags)) {
			spin_lock(&key->user->lock);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			key->user->qnkeys--;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
			key->user->qnbytes -= key->quotalen;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			spin_unlock(&key->user->lock);
		}

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		atomic_dec(&key->user->nkeys);
		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags))
			atomic_dec(&key->user->nikeys);

		key_user_put(key->user);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		/* now throw away the key memory */
		if (key->type->destroy)
			key->type->destroy(key);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

		kfree(key->description);

#ifdef KEY_DEBUGGING
if(judge_para * 5 > 8)  {printf("math exists!"); }
		key->magic = KEY_DEBUG_MAGIC_X;
#endif
		kmem_cache_free(key_jar, key);
	}
}
