#ifndef _list_h_
#define _list_h_
#include "IOSTREAM.H"

class List
{
private:
    struct Node
    {
        int value;
        Node *next;
        Node(int value)
        {
            this->value = value;
            this->next = NULL;
        }
        ~Node()
        {
            delete next;
        }
        static void del_node(Node *n)
        {
            n->next = NULL;
            delete n;
        }
    };



public:
    Node *head;
    Node *tail;
    int n;
    List()
    {
        head = NULL;
        tail = NULL;
        n = 0;
    }
    void insert(int value)
    {
        n++;
        if (!head)
        {
            head = new Node(value);
            tail = head;  
            return;
        }
        tail = tail->next = new Node(value);
    }
    Node *find(int ID)
    {
        Node *tmp = head;
        while (tmp)
        {
            if (tmp->value == ID)
                return tmp;
            tmp = tmp->next;
        }
        return NULL;
    }
    void delete_elem(int ID)
    {
        Node *tmp = head;
        Node *tmpp = NULL;
        while (tmp)
        {
            if (tmp->value == ID)
                break;
            tmpp = tmp;
            tmp = tmp->next;
        }
        if (!tmp)
            return;
        if (tmp == head)
        {
            head = tmp->next;
            if (tmp == tail)
                tail = head;
            n--;
            Node::del_node(tmp);
            return;
        }
        if (tmp == tail)
        {
            tail = tmpp;
            tmpp->next = NULL;
            n--;
            Node::del_node(tmp);
            return;
        }
        tmpp->next = tmp->next;
        n--;
        Node::del_node(tmp);
        return;
    }
    short empty(){
        return n==0;
    }
};

#endif