
/* this is the only place where dimension is used */
std::ostream& operator<<(std::ostream &out, const SparseVector &v) {
	int i,last_pos=-1; 
	ElementNode* p_e=v.pHead; 
	while (p_e) { 
		for (i=last_pos+1;i<p_e->pos;++i) out << " " << "0"; 
		out << " " << p_e->data; 
		last_pos=p_e->pos; 
		p_e = p_e->next; 
	} 
	for (i=last_pos+1;i<v.dimension;++i) out << " " << "0"; 

	return out; 
}

