/*
 * intermediate.cpp
 *
 *  Created on: 2020. 3. 22.
 *      Author: jujonghyeon25
 */

#include "table.h"
#include <iostream>
using namespace std;

EntryNode* create(string* s){
	EntryNode* x = new EntryNode;
	x->entry = s;
	x->next = nullptr;
	x->prev = nullptr;
	return x;
}

void delete_all(EntryNode*& p){
	if (p == nullptr)
		return;
	delete_all(p->next);
	p->next = nullptr;
	p->entry = nullptr;
	p->prev = nullptr;
	delete p;
	p = nullptr;
}

void nodeDelete(EntryNode*& cur){
	cur->entry = nullptr;
	cur->next =nullptr;
	cur->prev = nullptr;
	delete cur;
}

void swap(string*& a, string*& b){
	string* temp = a;
	a = b;
	b = temp;
}

Intermediate::Intermediate(const Table &table){
	attrs = table.attrs;
	numAttrs = table.numAttrs;
	if (table.numEntries == 0){
		head = nullptr;
		tail = nullptr;
	}
	else if(table.numEntries == 1){
		head = create(table.entries[0]);
		tail = head;
		head->next = nullptr;
		head->prev = nullptr;
	}
	else{
		head = create(table.entries[0]);
		int i = 0;
		for (EntryNode* p = head, *q = nullptr; i < table.numEntries; p = p->next, ++i){
			p->next = create(table.entries[i+1]);
			p->prev = q;
			q = p;
			if(i == table.numEntries - 1){
				p->prev = q;
				p->next =nullptr;
				tail = p;
			}
		}
	}
}

Intermediate::~Intermediate(){
	if(head == nullptr)
		return;
	delete_all(head);
	head = nullptr;
	tail = nullptr;
	attrs = nullptr;
}

Intermediate& Intermediate::where(const string &attr, enum compare mode, const string &value){

	bool check = false;

	int index = 0;
	for (; index < numAttrs; index++){
		if (attrs[index] == attr){
			check = true;
			break;
		}
	}

	if(!check)
		return *this;


	if (head==nullptr){
		return *this;
	}

	switch(mode){
		case EQ:{
			for(EntryNode* p = head, *q = nullptr; p != nullptr;){
				if(p->entry[index] != value){
					if(p != head && p != tail){
						p->next->prev = p->prev;
						p->prev->next = p->next;
						q = p;
						p = p->next;
						nodeDelete(q);
					}
					else if (p == head){
						q = p;
						p = p->next;
						head = p;
						nodeDelete (q);
					}
					else{
						tail = q;
						tail-> next = nullptr;
						q = p;
						nodeDelete (q);
					}
				}
				else{
					q = p;
					p = p->next;
				}
			}
			break;
		}
		case CONTAINS:{
			for(EntryNode* p = head, *q = nullptr; p != nullptr;){
				if(p->entry[index].std::string::find(value) == -1){
					if(p != head && p != tail){
						p->next->prev = p->prev;
						p->prev->next = p->next;
						q = p;
						p = p->next;
						nodeDelete(q);
					}
					else if (p == head){
						q = p;
						p = p->next;
						head = p;
						nodeDelete (q);
					}
					else{
						tail = q;
						tail-> next = nullptr;
						q = p;
						nodeDelete (q);
					}
				}
				else{
					q = p;
					p = p->next;
				}
			}
			break;
		}
	}
	return *this;
}

Intermediate& Intermediate::orderBy(const string &attr, enum order order){
	int index = 0;
	for (int i = 0; i < numAttrs; ++i){
		if (attrs[i] == attr){
			index = i;
			break;
		}
		if(i == numAttrs-1){
			return *this;
		}
	}

	if(order == ASCENDING){
		for (EntryNode* p = head; p != nullptr; p = p->next){
			for (EntryNode* q = p->next; q != nullptr; q = q->next){
				int d = p->entry[index].compare(q->entry[index]);
				if (d <= 0)
					swap( p->entry,q->entry);
			}
		}
		for(EntryNode* p = head,*q = tail; !((p->prev == q) || (p == q)); p = p->next, q = q->prev){
			swap(p->entry,q->entry);
		}
	}
	else{
		for (EntryNode* p = head; p != nullptr; p = p->next){
			for (EntryNode* q = p; q != nullptr; q = q->next){
				int d = p->entry[index].compare(q->entry[index]);
				if (d >= 0)
					swap( p->entry,q->entry);
			}
		}
		for(EntryNode*p = head,*q = tail; !((p->prev == q) || (p == q)); p = p->next, q = q->prev){
			swap(p->entry,q->entry);
		}
	}
	return *this;
}

Intermediate& Intermediate::limit(unsigned int limit){
	if(head == nullptr)
		return *this;
	else if(limit == 0){
		delete_all(head);
		head = nullptr;
		tail = nullptr;
		attrs = nullptr;
	}
	else{
		EntryNode* p = head;
		for (int i = 0; i < limit; i++)
			p = p->next;
		tail = p->prev;
		tail->next = nullptr;
		delete_all(p);

	}
	return *this;
}

void Intermediate::update(const string &attr, const string &new_value) const{
	bool check = false;

	int index = 0;
	for (; index < numAttrs; index++){
		if (attrs[index] == attr)
			check = true;
		if(check)
			break;
	}

	if(!check)
		return;

	for(EntryNode* p = head; p != nullptr; p = p->next){
		p->entry[index] = new_value;
	}
}

void Intermediate::select(const string *attrs, int numAttrs) const{
	int length[this->numAttrs];
	for (int i = 0; i < this->numAttrs; ++i){
		unsigned int maxLength = this->attrs[i].length();
		for (EntryNode* p = head; p != nullptr; p = p->next){
			if (p->entry[i].length() > maxLength)
				maxLength = p->entry[i].length();
			}
		length[i] = maxLength;
	}

	if (attrs == nullptr){

		int index[this->numAttrs];
		for (int i = 0; i < this->numAttrs; i++){
				index[i] = i;
		}

		for(int i = 0; i < this->numAttrs; ++i){
			if(i == this->numAttrs - 1){
				cout << _left_pad_until(this->attrs[index[i]],length[index[i]]) << endl;
				break;
			}
			cout << _left_pad_until(this->attrs[index[i]],length[index[i]]) << " | ";
		}

		for (EntryNode* p = head; p != nullptr; p = p->next){
			for (int i = 0; i < this->numAttrs; ++i){
				if(i == this->numAttrs - 1){
					cout << _left_pad_until(p->entry[index[i]],length[index[i]]) << endl;
					break;
				}
				cout << _left_pad_until(p->entry[index[i]],length[index[i]]) << " | ";
			}
		}
		return;
	}

	if (numAttrs == 0)
		return;

	if(head == nullptr){
		for (int i = 0; i < numAttrs; i++){
			if(i == numAttrs - 1){
				cout << attrs[i] << endl;
				break;
			}
			cout << attrs[i]<< " | ";
		}
		return;
	}

	int index[numAttrs];
	for (int i = 0; i < numAttrs; i++){
		for (int j = 0; j <this->numAttrs; j++){
			if (attrs[i]== this->attrs[j]){
				index[i] = j;
				break;
			}
		}
	}

	for(int i = 0; i < numAttrs; ++i){
		if(i == numAttrs - 1){
			cout << _left_pad_until(attrs[i],length[index[i]]) << endl;
			break;
		}
		cout << _left_pad_until(attrs[i],length[index[i]]) << " | ";
	}

	for (EntryNode* p = head; p != nullptr; p = p->next){
		for (int i = 0; i < numAttrs; ++i){
			if(i == numAttrs - 1){
				cout << _left_pad_until(p->entry[index[i]],length[index[i]]) << endl;
				break;
			}
			cout << _left_pad_until(p->entry[index[i]],length[index[i]]) << " | ";
		}
	}
}
