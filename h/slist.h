#ifndef _slist_h_
#define _slist_h_
#include "IOSTREAM.H"


typedef void (*SignalHandler)();

class SignalHandlerList
{
private:
    struct Node
    {
        SignalHandler value;
        Node *next;
        Node(SignalHandler value)
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
    SignalHandlerList()
    {
        head = NULL;
        tail = NULL;
        n = 0;
    }
    ~SignalHandlerList(){
        while(head){
            Node* tmp;
            tmp = head;
            head = head->next;
            Node::del_node(tmp);
        }
        tail = NULL;
        delete head;
        delete tail;
    }
    void insert(SignalHandler value)
    {
        n++;
        if (head == NULL)
        {
            head = new Node(value);
            tail = head;  
            return;
        }
        tail->next = new Node(value);
        tail = tail->next;
    }
    Node *find(SignalHandler ID)
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
    void del_head(){
        if(head){
            Node* tmp = head;
            head = head->next;
            if(head == NULL){
                tail = NULL;
            }
            Node::del_node(tmp);
        }
    }
    int delete_elem(SignalHandler ID)
    {
        Node* tmp = head, *last = 0;
        while(tmp != NULL){
            if (tmp->value == ID)
                break;
            last = tmp;
            tmp = tmp->next;
        }
        if (!tmp) return 0;
        n--;
        if (last){
            last->next = tmp->next;
            if (last->next == NULL){
                tail = last;
            }
        }else{
            head = head->next;
            if (!head){
                tail = NULL;
            }
        }
        Node::del_node(tmp);
        return 1;
    }
    short empty(){
        return head == NULL;
    }
};

#endif