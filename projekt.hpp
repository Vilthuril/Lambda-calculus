/*
 * projekt.hpp
 *
 *  Created on: Nov 12, 2016
 *      Author: inle
 */

#ifndef PROJEKT_HPP_
#define PROJEKT_HPP_

#include <iostream>
#include <typeinfo>
#include <vector>
#include <queue>
using namespace std;

class lambda{
	class Node{
	public:
		Node* firstc;
		Node* secondc;
		Node* parent;
		char param;
		virtual ~Node(){}
	};
	Node* act;
	Node* root;

	class Appl : public Node{
	public:
		Appl (){
			firstc=nullptr;
			secondc=nullptr;
			param = '\0';
			parent = nullptr;
		}
		Appl(char _param, Node* _parent){
			param = _param;
			parent = _parent;
			firstc=nullptr;
			secondc=nullptr;
		}
	};
	class Func : public Node{
	public:
		Func(){
			firstc=nullptr;
			secondc=nullptr;
			param = '\0';
			parent = nullptr;
		}
		Func(char _param, Node* _parent){
			param = _param;
			parent = _parent;
			firstc=nullptr;
			secondc=nullptr;
		}

	};
	class Subs : public Node{
	public:
		Subs(){
			firstc=nullptr;
			secondc=nullptr;
			param='\0';
			parent = nullptr;
		}
		Subs(char _param, Node* _parent){
			param = _param;
			parent = _parent;
			firstc=nullptr;
			secondc=nullptr;
		}
	};

public:
	lambda(string str){
		if(str.length()==0){
		root = nullptr;
		act = nullptr;
		}
		for(unsigned i=0; i<str.length(); i++){
			if(i==0){
				if(str[0] == '\\'){
					Node* seged = new Func();
					root = seged;
					act = seged;
				}
				else if(str[0] == '('){
					Node* seged = new Appl();
					root = seged;
					act = seged;
				}
				else {
					cerr<<"Hibas bemenet!";
					exit (EXIT_FAILURE);
				}
			}
			else{
			if(str[i]>=97 && str[i] <=122){
				if(str[i-1] == '\\'){
				act->param=str[i];
				}
				else{
					if(dynamic_cast<Subs*>(act)){
						cerr<<"Hibad bemenet!";
						exit(EXIT_FAILURE);
					}
					Node* seged = new Subs();
					seged->param = str[i];
					if(str[i-1] == ' '){
						act->secondc = seged;
					}
					else{
						act->firstc = seged;
					}
					seged->parent = act;
					act = seged;
				}
			}
			else if(str[i] == '\\'){
				if(dynamic_cast<Subs*>(act)){
						cerr<<"Hibad bemenet!";
						exit(EXIT_FAILURE);
					}
				Node* seged = new Func();
				if(str[i-1] == ' '){
					act->secondc = seged;
					}
				else{
					act->firstc = seged;
					}
				seged->parent = act;
				act = seged;
			}
			else if(str[i] == '('){
				if(dynamic_cast<Subs*>(act)){
						cerr<<"Hibad bemenet!";
						exit(EXIT_FAILURE);
					}
				Node* seged = new Appl();
				if(str[i-1] == ' '){
					act->secondc = seged;
					}
				else{
					act->firstc = seged;
					}
				seged->parent = act;
				act = seged;
			}
			else if(str[i] == ' ' ){
				while(act!=nullptr){
				act = act->parent;
				if(dynamic_cast<Appl*>(act) && act->secondc == nullptr) break;
				}
			}
			else if(str[i] == ')' || str[i] == '.') {}
			else{
				cerr<<"Hibas bemenet!";
				exit (EXIT_FAILURE);
			}

		}
	}
	normalise();
};


	~lambda() {_destroy(root); }
	void _destroy(Node *x);
	lambda& operator=(const lambda& l);
	lambda(const lambda& l);
	Node* copyOf( Node* n, Node* p );

	string to_string();
	void _string(Node* x, string& str)
	;
	Node* findappl(Node* n);
	void findsubs(Node*n, char c, vector<Node*>& v);
	void reduce();
	void normalise();


};


//side function for destructing
void lambda ::_destroy(Node *x){
    if (x != nullptr){
     	_destroy(x->firstc);
		_destroy(x->secondc);
    	delete x;
    }
}
//side function for copying
lambda::Node* lambda :: copyOf(Node* n, Node* p){
	if( n == nullptr ) return nullptr;
	Node* x;
	if(dynamic_cast<Appl*>(n)) x = new Appl (n->param, p);
	if(dynamic_cast<Func*>(n)) x = new Func (n->param, p);
	if(dynamic_cast<Subs*>(n)) x = new Subs (n->param, p);

	x->firstc = copyOf( n->firstc, x );
	x->secondc = copyOf( n->secondc, x );	
	return x;
}
//Copy constructor
lambda::lambda(const lambda& l) {
	act = nullptr;
	Node* n = l.root;
	Node* x;
	if(dynamic_cast<Appl*>(n)) x = new Appl (n->param, n->parent);
	if(dynamic_cast<Func*>(n)) x = new Func (n->param, n->parent);
	if(dynamic_cast<Subs*>(n)) x = new Func (n->param, n->parent);
	root = x;
	x->firstc = copyOf( n->firstc, x );
	x->secondc = copyOf( n->secondc, x );
}
//Assignment operator
lambda& lambda::operator=(const lambda& l) {

	if(&l == this) return *this;
	_destroy(root);
	Node* n = l.root;
	Node* x;
	if(dynamic_cast<Appl*>(n)) x = new Appl (n->param, n->parent);
	if(dynamic_cast<Func*>(n)) x = new Func (n->param, n->parent);
	if(dynamic_cast<Subs*>(n)) x = new Func (n->param, n->parent);
	root = x;
	x->firstc = copyOf( n->firstc, x );
	x->secondc = copyOf( n->secondc, x );
	return *this;
}

void lambda::_string(Node* x, string& str){

	if(dynamic_cast<Appl*>(x)){
		str+='(';
		_string(x->firstc, str);
		str+=' ';
		_string(x->secondc, str);
		str+=')';
	}
	if(dynamic_cast<Func*>(x)){
		str+='\\';
		str+=x->param;
		str+='.';
		_string(x->firstc, str);
	}
	if(dynamic_cast<Subs*>(x)){
		str+=x->param;
	}
}

string lambda::to_string(){
	string str;
	_string(root, str);
	return str;
}

lambda::Node* lambda::findappl(Node* n){
if(dynamic_cast<Func*>(n->firstc) && dynamic_cast<Appl*>(n)) {
	return n;
}
else if(n->firstc!=nullptr || n->secondc!=nullptr){
		if(n->firstc!=nullptr){

			return findappl(n->firstc);
		}		
		else if(n->secondc!=nullptr){
			
			return findappl(n->secondc);
		} 
	}

return nullptr;
}

void lambda::findsubs(Node*n, char c, vector<Node*>& v){
	if(!(dynamic_cast<Subs*>(n) && n->param == c)){
		if(n->firstc!=nullptr &&
			!(dynamic_cast<Func*>(n->firstc) && n->firstc->param == c))
			findsubs(n->firstc, c, v);
		if(n->secondc!=nullptr &&
			!(dynamic_cast<Func*>(n->secondc) && n->secondc->param == c))
			findsubs(n->secondc, c, v);
	}
	else {
		v.push_back(n);
		if(n->firstc!=nullptr && 
			!(dynamic_cast<Func*>(n->firstc) && n->firstc->param == c))
			findsubs(n->firstc, c, v);
		if(n->secondc!=nullptr &&
			!(dynamic_cast<Func*>(n->secondc) && n->secondc->param == c))
			findsubs(n->secondc, c, v);

}
}



void lambda::reduce(){
	while(findappl(root)!=nullptr)
	{
	Node* h = root;
	Node* n = findappl(h);

vector<Node*> v;

	findsubs(n->firstc, n->firstc->param, v);

	for(unsigned i=0; i<v.size(); i++){
		if(v[i] == v[i]->parent->firstc)
		v[i]->parent->firstc = copyOf(n->secondc, v[i]->parent);
		if(v[i] == v[i]->parent->secondc)
		v[i]->parent->secondc = copyOf(n->secondc, v[i]->parent);

	}

	//appl jobb reszfa torlese
	_destroy(n->secondc);

	//func törlése
	Node* tmp = n->firstc;
	n->firstc->firstc->parent = n;
	n->firstc = n->firstc->firstc;
	delete tmp;

	//appl torlese
	tmp = n->firstc;
	tmp->parent = n->parent;
	if(n->parent  == nullptr) root = tmp;
	else if(n == n->parent->firstc) n->parent->firstc = tmp;
	else n->parent->secondc = tmp;
	delete n;
	normalise();
	}
}

void lambda::normalise(){
	queue<Node*> q;
	q.push(root);
    vector<vector<Node*>> vec;

    while (!q.empty()){
    	Node* v = q.front();
        if(dynamic_cast<Func*>(v)){
        vector<Node*> tmp;
        tmp.push_back(v);
        findsubs(v, v->param, tmp);
        vec.push_back(tmp);

   }
        if (v->firstc!=nullptr)
       		q.push(v->firstc);

        if (v->secondc!=nullptr)
        	q.push(v->secondc);
        q.pop();
   }

for(unsigned i=0; i<vec.size(); i++){
	for (unsigned j=0; j<vec[i].size(); j++){
		vec[i][j]->param = 97+i;
	}
}
}



#endif /* PROJEKT_HPP_ */
