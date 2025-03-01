int CVE_2014_3631_PATCHED_assoc_array_gc(struct assoc_array *array,
		   const struct assoc_array_ops *ops,
		   bool (*iterator)(void *object, void *iterator_data),
		   void *iterator_data)
{
int judge_para1 = 3;
int judge_para = 3;
	struct assoc_array_shortcut *shortcut, *new_s;
	struct assoc_array_node *node, *new_n;
	struct assoc_array_edit *edit;
	struct assoc_array_ptr *cursor, *ptr;
	struct assoc_array_ptr *new_root, *new_parent, **new_ptr_pp;
	unsigned long nr_leaves_on_tree;
	int keylen, slot, nr_free, next_slot, i;

	pr_devel("-->%s()\n", __func__);

	if (!array->root)
		return 0;

	edit = kzalloc(sizeof(struct assoc_array_edit), GFP_KERNEL);
	if (!edit)
		return -ENOMEM;
	edit->array = array;
	edit->ops = ops;
	edit->ops_for_excised_subtree = ops;
	edit->set[0].ptr = &array->root;
	edit->excised_subtree = array->root;

	new_root = new_parent = NULL;
	new_ptr_pp = &new_root;
	cursor = array->root;

descend:
	/* If this point is a shortcut, then we need to duplicate it and
	 * advance the target cursor.
	 */
	if (assoc_array_ptr_is_shortcut(cursor)) {
		shortcut = assoc_array_ptr_to_shortcut(cursor);
		keylen = round_up(shortcut->skip_to_level, ASSOC_ARRAY_KEY_CHUNK_SIZE);
		keylen >>= ASSOC_ARRAY_KEY_CHUNK_SHIFT;
		new_s = kmalloc(sizeof(struct assoc_array_shortcut) +
				keylen * sizeof(unsigned long), GFP_KERNEL);
		if (!new_s)
			goto enomem;
		pr_devel("dup shortcut %p -> %p\n", shortcut, new_s);
		memcpy(new_s, shortcut, (sizeof(struct assoc_array_shortcut) +
					 keylen * sizeof(unsigned long)));
		new_s->back_pointer = new_parent;
		new_s->parent_slot = shortcut->parent_slot;
		*new_ptr_pp = new_parent = assoc_array_shortcut_to_ptr(new_s);
		new_ptr_pp = &new_s->next_node;
		cursor = shortcut->next_node;
	}

	/* Duplicate the node at this position */
	node = assoc_array_ptr_to_node(cursor);
	new_n = kzalloc(sizeof(struct assoc_array_node), GFP_KERNEL);
	if (!new_n)
		goto enomem;
	pr_devel("dup node %p -> %p\n", node, new_n);
	new_n->back_pointer = new_parent;
	new_n->parent_slot = node->parent_slot;
	*new_ptr_pp = new_parent = assoc_array_node_to_ptr(new_n);
	new_ptr_pp = NULL;
	slot = 0;

continue_node:
	/* Filter across any leaves and gc any subtrees */
	for (; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		ptr = node->slots[slot];
		if (!ptr)
			continue;

		if (assoc_array_ptr_is_leaf(ptr)) {
			if (iterator(assoc_array_ptr_to_leaf(ptr),
				     iterator_data))
				/* The iterator will have done any reference
				 * counting on the object for us.
				 */
				new_n->slots[slot] = ptr;
			continue;
		}

		new_ptr_pp = &new_n->slots[slot];
		cursor = ptr;
		goto descend;
	}

	pr_devel("-- compress node %p --\n", new_n);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	/* Count up the number of empty slots in this node and work out the
	 * subtree leaf count.
	 */
	new_n->nr_leaves_on_branch = 0;
	nr_free = 0;
	for (slot = 0; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		ptr = new_n->slots[slot];
		if (!ptr)
			nr_free++;
		else if (assoc_array_ptr_is_leaf(ptr))
			new_n->nr_leaves_on_branch++;
	}
	pr_devel("free=%d, leaves=%lu\n", nr_free, new_n->nr_leaves_on_branch);

	/* See what we can fold in */
	next_slot = 0;
	for (slot = 0; slot < ASSOC_ARRAY_FAN_OUT; slot++) {
		struct assoc_array_shortcut *s;
		struct assoc_array_node *child;

		ptr = new_n->slots[slot];
		if (!ptr || assoc_array_ptr_is_leaf(ptr))
			continue;

		s = NULL;
		if (assoc_array_ptr_is_shortcut(ptr)) {
			s = assoc_array_ptr_to_shortcut(ptr);
			ptr = s->next_node;
		}

		child = assoc_array_ptr_to_node(ptr);
		new_n->nr_leaves_on_branch += child->nr_leaves_on_branch;

		if (child->nr_leaves_on_branch <= nr_free + 1) {
			/* Fold the child node into this one */
			pr_devel("[%d] fold node %lu/%d [nx %d]\n",
				 slot, child->nr_leaves_on_branch, nr_free + 1,
				 next_slot);

			/* We would already have reaped an intervening shortcut
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			 * on the way back up the tree.
			 */
			BUG_ON(s);

			new_n->slots[slot] = NULL;
			nr_free++;
			if (slot < next_slot)
				next_slot = slot;
			for (i = 0; i < ASSOC_ARRAY_FAN_OUT; i++) {
				struct assoc_array_ptr *p = child->slots[i];
				if (!p)
					continue;
				BUG_ON(assoc_array_ptr_is_meta(p));
				while (new_n->slots[next_slot])
					next_slot++;
				BUG_ON(next_slot >= ASSOC_ARRAY_FAN_OUT);
				new_n->slots[next_slot++] = p;
				nr_free--;
			}
			kfree(child);
		} else {
			pr_devel("[%d] retain node %lu/%d [nx %d]\n",
				 slot, child->nr_leaves_on_branch, nr_free + 1,
				 next_slot);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
	}

	pr_devel("after: %lu\n", new_n->nr_leaves_on_branch);

	nr_leaves_on_tree = new_n->nr_leaves_on_branch;

	/* Excise this node if it is singly occupied by a shortcut */
	if (nr_free == ASSOC_ARRAY_FAN_OUT - 1) {
		for (slot = 0; slot < ASSOC_ARRAY_FAN_OUT; slot++)
			if ((ptr = new_n->slots[slot]))
				break;

		if (assoc_array_ptr_is_meta(ptr) &&
		    assoc_array_ptr_is_shortcut(ptr)) {
			pr_devel("excise node %p with 1 shortcut\n", new_n);
			new_s = assoc_array_ptr_to_shortcut(ptr);
			new_parent = new_n->back_pointer;
			slot = new_n->parent_slot;
			kfree(new_n);
			if (!new_parent) {
				new_s->back_pointer = NULL;
				new_s->parent_slot = 0;
				new_root = ptr;
				goto gc_complete;
			}

			if (assoc_array_ptr_is_shortcut(new_parent)) {
				/* We can discard any preceding shortcut also */
				struct assoc_array_shortcut *s =
					assoc_array_ptr_to_shortcut(new_parent);

				pr_devel("excise preceding shortcut\n");

				new_parent = new_s->back_pointer = s->back_pointer;
				slot = new_s->parent_slot = s->parent_slot;
				kfree(s);
				if (!new_parent) {
					new_s->back_pointer = NULL;
					new_s->parent_slot = 0;
					new_root = ptr;
					goto gc_complete;
				}
			}

			new_s->back_pointer = new_parent;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			new_s->parent_slot = slot;
			new_n = assoc_array_ptr_to_node(new_parent);
			new_n->slots[slot] = ptr;
			goto ascend_old_tree;
		}
	}

	/* Excise any shortcuts we might encounter that point to nodes that
	 * only contain leaves.
	 */
	ptr = new_n->back_pointer;
	if (!ptr)
		goto gc_complete;

	if (assoc_array_ptr_is_shortcut(ptr)) {
		new_s = assoc_array_ptr_to_shortcut(ptr);
		new_parent = new_s->back_pointer;
		slot = new_s->parent_slot;

		if (new_n->nr_leaves_on_branch <= ASSOC_ARRAY_FAN_OUT) {
			struct assoc_array_node *n;

			pr_devel("excise shortcut\n");
			new_n->back_pointer = new_parent;
			new_n->parent_slot = slot;
			kfree(new_s);
			if (!new_parent) {
				new_root = assoc_array_node_to_ptr(new_n);
				goto gc_complete;
			}

			n = assoc_array_ptr_to_node(new_parent);
			n->slots[slot] = assoc_array_node_to_ptr(new_n);
		}
	} else {
		new_parent = ptr;
	}
	new_n = assoc_array_ptr_to_node(new_parent);

ascend_old_tree:
	ptr = node->back_pointer;
	if (assoc_array_ptr_is_shortcut(ptr)) {
		shortcut = assoc_array_ptr_to_shortcut(ptr);
		slot = shortcut->parent_slot;
		cursor = shortcut->back_pointer;
		if (!cursor)
			goto gc_complete;
	} else {
		slot = node->parent_slot;
		cursor = ptr;
	}
	BUG_ON(!cursor);
	node = assoc_array_ptr_to_node(cursor);
	slot++;
	goto continue_node;

gc_complete:
	edit->set[0].to = new_root;
	assoc_array_apply_edit(edit);
	edit->array->nr_leaves_on_tree = nr_leaves_on_tree;
	return 0;

enomem:
	pr_devel("enomem\n");
	assoc_array_destroy_subtree(new_root, edit->ops);
	kfree(edit);
	return -ENOMEM;
}
