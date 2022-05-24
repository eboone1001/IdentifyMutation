#ifndef list
#define list

class node {
	public:
		node(char* query, node* parent);
		char* getQuery();
		node* getParent();
		node* getChild();
		void addChild(node* Node);
		~node();
	private:
		node* next;
		node* prev;
		char* query;


};


#endif