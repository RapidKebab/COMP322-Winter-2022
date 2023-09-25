#include <iostream>
#include <cstring>


class DSNode {//datastore nodes, linked list
public:
    int id = -1;
    int* store_data = nullptr;
    int ssize = 0;
    DSNode* next = nullptr;
};


DSNode* datastore = new DSNode(); //head
int stores = 0; //used to track the next store id to assign
int fill = 0; //tracks how many slots are reserved

// Question 1
int newstore(int ssize)
{
    DSNode* ds = datastore;
    while (ds->next != nullptr) {
        ds = ds->next;
    }//get to the tail
    try {
        ds->store_data = new int[ssize];//add the store to the tail node
        ds->next = new DSNode();//make a new(dummy?) tail node
    }//catch if we ran out of memory
    catch (std::bad_alloc& ba){
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        return -1;
    }
    ds->id = stores;
    ds->ssize = ssize;
    fill += ssize;
    memset(ds->store_data, 0, sizeof(int) * ssize);
    stores++;
    return (stores - 1);//attach all the relevant values, make sure the store is full of 0's
}

DSNode* findstore(int sid) {//helper func, returns the node that contains a store's info.
    DSNode* ds = datastore;
    do{
        if (ds->id == sid) return ds;
        ds = ds->next; 
    } while (ds != nullptr);
    return nullptr;
}

// Question 2
int add_element_to_store(int id, int val, int idx = -1)
{
    DSNode* ds = findstore(id);
    if (!(idx < ds->ssize) || idx < -1)return -1;//replaces the first 0 it runs into from the left by default, same functionality as last assignment.
    if (idx == -1) {
        for (int i = 0; i < ds->ssize; i++) {
            if (ds->store_data[i] == 0) {
                ds->store_data[i] = val;
                return 0;
            }
        }
        return -1;
    }
    ds->store_data[idx] = val;
    return 0;
}

void recursive_store_print(DSNode* ds) {//prints out a whole store, calls itself on the next store if such store exists and isnt a dummy.
    std::cout << "store ";
    std::cout << ds->id << ": ";
    int i = 0;
    while (i < ds->ssize) {
        std::cout << ds->store_data[i] << " ";
        i++;
    }
    std::cout << std::endl;
    if (ds->next != nullptr && ds->next->id!=-1) {
        recursive_store_print(ds->next);
    }
}

// Question 3
void print_debug()
{
    std::cout << std::endl << "total elements reserved: " << fill << std::endl;
    std::cout << std::endl;
    if (stores > 0)recursive_store_print(datastore);
    std::cout << std::endl << "##############################" << std::endl;
    // provide your implementation
}



// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    DSNode* ds = findstore(id);
    for (int i = 0; i < ds->ssize; i++) {
        if (ds->store_data[i] == val) {
            ds->store_data[i] = 0;
            break;
        }
    }
}

void delete_element_from_store_by_index(int id, int idx)
{
    DSNode* ds = findstore(id);
    if (idx < ds->ssize) ds->store_data[idx] = 0;
}

// Question 5
void which_stores_have_element(int val)
{
    bool* included;//used to track stores that include the element
    try{ included = new bool[stores];}
    catch (std::bad_alloc& ba){
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        return;
    }
    memset(included, false, sizeof(bool) * stores);
    int added = 0;
    DSNode* ds = datastore;
    do {//going through all of the stores
        if (ds->store_data != nullptr) {
            for (int i = 0; i < ds->ssize; i++) {
                if (ds->store_data[i] == val) {
                    included[ds->id] = true;
                    added++;
                    break;
                }
            }
        }
        ds = ds->next;
    } while (ds != nullptr);
    if (added == 0) {
        std::cout << std::endl << "Element " << val << " is not available in any store" << std::endl << std::endl;
        delete[] included;
        return;
    }
    std::cout << std::endl << "Element " << val << " is in store ID's: ";
    for (int i = 0; i < stores; i++) {
        if (included[i])std::cout << i << " ";
    }
    std::cout << std::endl << std::endl;
    delete[] included;
}

// Question 6
void delete_store(int id)
{
    DSNode* ds = findstore(id);
    
    delete[] ds->store_data;
    if (id>0&&findstore(id-1)!=nullptr)
        findstore(id - 1)->next = ds->next;
    fill -= ds->ssize;
    if (ds == datastore)//handling deleting the head.
        datastore = ds->next;
    delete ds;
}

// Question 7
int resize_store(int id, int newsize)
{
    DSNode* ds = findstore(id);
    int* newarray;
    try { newarray = new int[newsize]; }
    catch(std::bad_alloc& ba) {
        std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        return -1;
    }
    for (int i = 0; i < newsize; i++) {
        newarray[i] = ds->store_data[i];
        if (i >= ds->ssize) newarray[i] = 0;
    }
    delete ds->store_data;
    fill += newsize-ds->ssize;
    ds->store_data = newarray;
    ds->ssize = newsize;
    return 0;
}

int main()
{
    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1)
    {
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }

    if (s2 != -1)
    {
        add_element_to_store(s2, 7, 2);
        add_element_to_store(s2, 15, 0);
        add_element_to_store(s2, 22, 1);
    }
    print_debug();

    delete_element_from_store_by_value(s1, 13);
    delete_element_from_store_by_value(s1, 71);
    delete_element_from_store_by_index(s2, 2);
    delete_element_from_store_by_index(s1, 5);
    print_debug();

    which_stores_have_element(15);
    which_stores_have_element(29);

    delete_store(s1);
    print_debug();

    resize_store(s2, 20);
    int s3 = newstore(40);
    print_debug();
    s3 = newstore(30);
    add_element_to_store(s3, 7, 29);
    print_debug();
}