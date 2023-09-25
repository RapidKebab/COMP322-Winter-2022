#include <iostream>

#define ARRSIZE 50
int datastore[ARRSIZE] = {};
int storetracker[ARRSIZE] = {}; //tracks store location and sizes
int storeids[ARRSIZE] = {}; //tracks store location and id, only necessary because stores need to not shift ids after another is deleted, I hate it too.
int fill = 0; //used to track how full the datastore is
int stores = 0; //used to track the next store id to assign

// Feel free to add any useful variables that you might need

// Question 1
int newstore(int ssize)
{
    if(ssize+fill>ARRSIZE||ssize==0){
        return -1;
    }
    else{
        storetracker[fill]=ssize;
        stores++;
        storeids[fill]=stores;//place id+1 into the array, cant do id directly as 0 = empty
        fill+=ssize;
        return (stores-1);
    }
}

int findstore(int sid){//helper func, returns the index at which a store starts
    for (int i = 0; i < ARRSIZE; i++)
    {
        if(storeids[i]-1==sid)return i;
    }
    return -1;
}

// Question 2
int add_element_to_store(int id, int val, int idx=-1)
{
    int loc=findstore(id);
    if(!(idx<storetracker[loc])||idx<-1)return -1;
    if(idx==-1){
        for(int i=0;i<storetracker[loc];i++){
            if(datastore[i]==0){
                datastore[i]=val;
                return 0;
            }
        }
        return -1;
    }
    datastore[loc+idx]=val;
    return 0;
}

void recursive_store_print(int loc, int length){//prints out a whole store, calls itself on the next store if such store exists.
    std::cout<<"store ";
    int id=(storeids[loc]-1);
    std::cout<<id<<": ";
    int i=0;
    while(i<length){
        std::cout<<datastore[i+loc]<<" ";
        i++;
    }
    std::cout<<std::endl;
    if(i+loc<ARRSIZE&&storetracker[i+loc]!=0){
        recursive_store_print(i + loc, storetracker[i+loc]);
    }
}

// Question 3
void print_debug()
{
    std::cout<<"available elements in datastore : "<<(ARRSIZE-fill)<<std::endl;
    std::cout<<"datastore : ";
    for(auto x :datastore){std::cout<<x<<" ";}
    std::cout<<std::endl;
    if(stores>0)recursive_store_print(0,storetracker[0]);
    std::cout<<std::endl<<"##############################"<<std::endl<<std::endl;
    // provide your implementation
}



// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    int loc = findstore(id);
    int size = storetracker[loc];
    for(int i=0;i<size;i++){
        if(datastore[i+loc]==val){
            datastore[i+loc]=0;
            break;
        }
    }
    // provide your implementation
}

void delete_element_from_store_by_index(int id, int idx)
{
    int loc = findstore(id);
    int size = storetracker[loc];
    if (idx<size) datastore[loc+idx]=0;
    // provide your implementation
}

// Question 5
void which_stores_have_element(int val)
{
    bool included = false;
    for(auto x : datastore)
    {
        if(x==val) included=true;
    }
    if(!included){
        std::cout<<"Element "<<val<<" is not available in any store"<<std::endl<<std::endl;
        return;
    }
    int currentstore=-1;
    std::cout<<"Element "<<val<<" is in store ID's: ";
    for(int i = 0; i < ARRSIZE; i++)
    {
        if(storetracker[i]!=0)currentstore++;
        if(datastore[i]==val)std::cout<<currentstore<<" ";
    }
    std::cout<<std::endl<<std::endl;
}

// Question 6
void delete_store(int id)
{
    int loc = findstore(id);
    int size = storetracker[loc];
    storetracker[loc]=0;
    storeids[loc]=0;
    for(int i=loc+size;i<ARRSIZE;i++){
        datastore[i-size]=datastore[i];
        storetracker[i-size]=storetracker[i];
        storeids[i-size]=storeids[i];
    }
    fill-=size;
}

// Question 7
int resize_store(int id, int newsize)
{
    int loc = findstore(id);
    int size = storetracker[loc];
    if(fill+newsize-size>ARRSIZE)return -1;
    storetracker[loc]=newsize;
    if (newsize>size){
        for(int i=ARRSIZE;i>=loc+size;i--){
            if(i<loc+newsize){
                datastore[i]=0;
            }
            else
            {
                datastore[i]=datastore[i-(newsize-size)];
                storetracker[i]=storetracker[i-(newsize-size)];
                storeids[i]=storeids[i-(newsize-size)];
            }
        }
    }
    else if(size>newsize){
        for (int j = loc+newsize; j < ARRSIZE; j++)
        {
            datastore[j]=datastore[j+(size-newsize)];
            storetracker[j]=storetracker[j+(size-newsize)];
            storeids[j]=storeids[j+(size-newsize)];
        }
    }
    fill+=newsize;//I don't know why but it seemed to automatically subtract size.
    return 0;
    // provide your implementation
}


// DO NOT ADD ANY LOGIC TO THE MAIN FUNCTION.
// YOU MAY MODIFY FOR YOUR OWN TESTS ONLY BUT THE ORIGINAL MAIN
// FUNCTION SHOULD BE SUBMITTED AS IS
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