#include "List.h"

node::node(char* query, node* parent):query(query),prev(parent),next(nullptr){}
char* node::getQuery()
{
	return query;
}
node* node::getParent()
{
	return prev;
}
node* node::getChild()
{
	return next;
}
void node::addChild(node* Node)
{
	next = Node;
}
node::~node()
{
	delete next;
	delete[] query;

}