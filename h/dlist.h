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
    ~DoubleList(){
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
    void insert_sorted(int value, int id)
    {
        Node *place = head, *last = 0;
        while (place && place->value<=value)
        {
            if (place->value == value)
                break;
            value -=place->value;
            last = place;
            place = place->next;
        }
        if (place) {
            if (place->value == value) {
                place->insert(id);
            }
            else {
                n++;
                Node* nn = new Node(value, id);
                if (place == head) {
                    head = nn;
                    nn->next = place;
                }
                else {
                    nn->next = place;
                    last->next = nn;
                }
                if (!nn->next) {
                    tail=nn;
                }else{
                    nn->next->value -=nn->value;
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
            tail->next = nn;
            tail = tail->next;
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
        // Node* tmp = head;
        // while (tmp) {
        //     tmp->value--;
        //     tmp=tmp->next;
        // }
        if (head){
            head->value--;
        }
    }
    void del_head() {
        if (head) {
            Node* tmp = head;
            head = head->next;
            if (head == NULL){
                tail = NULL;
            }
            if(tmp->next){
                tmp->next->value-=tmp->value;
            }
            Node::del_node(tmp);
            n--;
        }
    }
    void del_by_id(int id) {
        Node* tmp = head;
        Node* last = 0;
        while (tmp) {
            int o = tmp->my->delete_elem(id);
            if (tmp->my->empty()) {
                if (last == 0){
                    head = head->next;
                    if (head == NULL){
                        tail = NULL;
                    }
                }else{
                    last->next = tmp->next;
                    if (last->next == NULL){
                        tail = last;
                    }
                }
                if (tmp->next){
                    tmp->next->value -= tmp->value;
                }
                Node::del_node(tmp);
                n--;
                return;
            }
            if (o) return;
            last = tmp;
            tmp=tmp->next;
        }
    }
    // void delete_elem(int ID)
    // {
    //     Node* tmp = head, *last = 0;
    //     while(tmp != NULL){
    //         if (tmp->value == ID)
    //             break;
    //         last = tmp;
    //         tmp = tmp->next;
    //     }
    //     if (!tmp) return;
    //     n--;
    //     if (last){
    //         last->next = tmp->next;
    //         if (last->next == NULL){
    //             tail = last;
    //         }
    //     }else{
    //         head = tmp->next;
    //         if (!head){
    //             tail = NULL;
    //         }
    //     }
    //     Node::del_node(tmp);
    //     return;
    // }
    short empty() {
        return head == NULL;
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