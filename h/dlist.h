#ifndef _dlist_h_
#define _dlist_h_
#include "IOSTREAM.H"
#include "list.h"

class DoubleList
{
private:
    struct Node
    {
        int value;
        List* my;
        Node* next;
        Node(int value, int id)
        {
            this->value = value;
            my = new List();
            my->insert(id);
            this->next = NULL;
        }
        void insert(int id) {
            my->insert(id);
        }
        ~Node()
        {
            delete next;
            delete my;
        }
        static void del_node(Node *n)
        {
            if (!n)
                return;
            n->next = NULL;
            delete n;
        }
    };



public:
    Node *head;
    Node *tail;
    int n;
    DoubleList()
    {
        head = NULL;
        tail = NULL;
        n = 0;
    }
    void insert_sorted(int value, int id)
    {
       Node *place = head;
        while (place && place->value<=value)
        {
            if (place->value == value)
                break;
            place = place->next;
        }
        if (place) {
            if (place->value == value) {
                place->insert(id);
            }
            else {

                Node* tmp = head;
                while (tmp->next && tmp->next->value <value) {
                    tmp=tmp->next;
                }
                n++;
                Node* nn = new Node(value, id);
                if (tmp == head && tmp->value>value) {

                    head = nn;
                    nn->next = tmp;
                }
                else {
                    nn->next = tmp->next;
                    tmp->next = nn;
                }
                if (!nn->next) {
                    tail=nn;
                }
            }
            return;
        }
        n++;
        Node* nn = new Node(value, id);
        if (!head) {
            head = nn;
            tail = nn;
        }
        else {
            tail = tail->next = nn;
        }
    }
    Node *find(int ID)
    {
        Node *tmp = head;
        while (tmp && tmp->value<=ID)
        {
            if (tmp->value == ID)
                return tmp;
            tmp = tmp->next;
        }
        return tmp;
    }
    void lower() {
        Node* tmp = head;
        while (tmp) {
            tmp->value--;
            tmp=tmp->next;
        }
    }
    void del_head() {
        if (head) {
            Node* tmp = head;
            head = head->next;
            Node::del_node(tmp);
            n--;
        }
    }
    void del_by_id(int id) {
        Node* tmp = head;
        while (tmp) {
            List::Node* f = tmp->my->find(id);
            if (f) {
                tmp->my->delete_elem(id);
                if (tmp->my->empty()) {
                    delete_elem(tmp->value);
                }
                return;
            }
            tmp=tmp->next;
        }
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
    short empty() {
        return n==0;
    }
    void print() {
        Node* tmp = head;
        while (tmp) {
            List::Node* nn = tmp->my->head;
            cout <<"value = "<<tmp->value<<" ";
            while (nn) {
                cout <<nn->value<<' ';
                nn=nn->next;
            }
            tmp = tmp->next;
            cout <<endl;
        }
        cout <<"Gotov print\n";
    }
};

#endif