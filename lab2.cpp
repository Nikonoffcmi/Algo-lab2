#include <iostream> 
#include <vector> 
#include <array>
#include <random>
#include <algorithm>

using namespace std; 

#define MAX_SIZE 10000001ll 

class Hash_table { 
    bool collision;
	int TABLE_SIZE, keysPresent; 
	vector<int> hashTable; 
    int search_counter = 0;



    
	int inline hash1(int value){ 
		return value%TABLE_SIZE; 
	} 
	
	int inline hash2(int value){	 
		return 1 + (value % (TABLE_SIZE - 1));
	} 

	bool inline isFull(){ 
		return (TABLE_SIZE == keysPresent); 
	} 

	public: 

    int get_search_counter ()
    {
        return search_counter;
    }

	Hash_table(int n, bool set_collision){ 
		TABLE_SIZE = n; 
        collision = set_collision; //false - линейное зондирование false, true - двойное хеширование

		keysPresent = 0; 

		/* заполняем таблицу пустыми элементами */
		for(int i = 0; i < TABLE_SIZE; i++) 
			hashTable.push_back(-1); 
	} 

	void insert(int value){ 
		if(value == -1 || value == -2){ 
			cout<<("ERROR : -1 and -2 can't be inserted in the table\n"); 
		} 

		if(isFull()){ 
			cout<<("ERROR : Hash Table Full\n"); 
			return; 
		} 

        int offset = 0;
		if (collision)
            offset = hash2(value);
        else
            offset = 1;
        
        int probe = hash1(value); 
		
        while(hashTable[probe] != -1){ 
            if(hashTable[probe] == -2)
                break;
            probe = (probe+offset) % TABLE_SIZE; 
        } 

        hashTable[probe] = value; 
        keysPresent += 1; 
        
	} 

	void erase(int value){
		
        int offset = 0;  
		if (collision){
            offset = hash2(value);
            int probe = hash1(value), initialPos = probe; 
            bool firstItr = true; 
            
            while(hashTable[probe] != -1){
                if(hashTable[probe] == value){ 
                    hashTable[probe] = -2;          // пометить элемент как удаленный при двойном хеширование
                    keysPresent--; 
                    return; 
                }
                else if(probe == initialPos && !firstItr)
                    break;
                else
                    probe = (probe + offset) % TABLE_SIZE; 

                firstItr = false;
            }
            
        }
        else{
            offset = 1;
            int probe = hash1(value), initialPos = probe; 
            bool firstItr = true;  

            while(hashTable[probe] != -1) 
                if(hashTable[probe] == value){ 
                    hashTable[probe] = -1;
                    keysPresent--; 

                    if (hashTable[probe+1] == -1)
                        return;
                    
                    int old_probe = probe;
                    probe = (probe + offset) % TABLE_SIZE; 

                    while(true)
                    {
                        if (hashTable[probe] == -1)
                            break;
                        else 
                        {
                            int buf = hash1(hashTable[probe]);
                            if (buf != probe && buf <= old_probe){
                                hashTable[old_probe] = hashTable[probe];
                                hashTable[probe] = -1;
                                old_probe = probe;   
                                probe = (probe + offset) % TABLE_SIZE;              
                            }
                            else
                                probe = (probe + offset) % TABLE_SIZE; 
                        }                   
                    }

                    return; 
                } 
                else if(probe == initialPos && !firstItr)
                    break;
                else
                    probe = (probe + offset) % TABLE_SIZE; 

                firstItr = false;
        }
	} 

	bool search(int value){ 
        search_counter = 0;

        int offset = 0;
		if (collision)
            offset = hash2(value);
        else
            offset = 1;

		int probe = hash1(value), initialPos = probe; 
		bool firstItr = true; 

		while(1){ 
            search_counter++;
			if(hashTable[probe] == -1)				 // прекратить поиск если место пустое
				break; 
			else if(hashTable[probe] == value)		 // прекратить поиск если элемент нашли
				return true; 
			else if(probe == initialPos && !firstItr) // прекратить поиск если была проверена вся таблица
				break; 
			else
				probe = ((probe + offset) % TABLE_SIZE); // обновление индекса

			firstItr = false;
		} 
		return false; 
	} 

	void print(){ 
		for(int i = 0; i < TABLE_SIZE; i++) 
			cout<<hashTable[i]<<", "; 
		cout<<"\n"; 
	} 

}; 

int main(){ 
    random_device random_device; 
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(0, 500); 
    binomial_distribution<int> bin_distribution(500,0.5);
    normal_distribution<double> norm_distribution(250.0, 80.0);
    
    int size = 10;
	Hash_table myHash(size, true); // создает хеш-таблицу, false - линейное зондирование false, true - двойное хеширование
	Hash_table myHash2(size, false); // создает хеш-таблицу, false - линейное зондирование false, true - двойное хеширование

	/* массив элементов для вставки */
	
	int insertions[] = {2, 22, 12, 32, 6, 66, 52, 56, 92}, 
		n1 = sizeof(insertions)/sizeof(insertions[0]); 
	
	for(int i = 0; i < n1; i++) 
		myHash.insert(insertions[i]); 
	
	cout<< "Status of hash table after initial insertions: "; myHash.print();
	
    // cout<< "Status of hash table after initial insertions: "; myHash.print(); 
	// cout<< "Status of hash table after initial insertions: "; myHash2.print(); 
	

    /* массив элементов для поиска*/
	
	int queries[] = {2, 0, 12, 123, 66, 92}, 
		n2 = sizeof(queries)/sizeof(queries[0]); 
	
	cout<<"\n"<<"Search operation after insertion : \n"; 

    vector<int> s_search;
    vector<int> u_search;

	for(int i = 0; i < n2; i++) 
		if(myHash.search(queries[i])) 
        {
            cout<<queries[i]<<" present\n"; 
        }
        else
            cout<<queries[i]<<" not present\n"; 

	/* массив элементов для удаления */
	
	int deletions[] = {12, 52}, 
		n3 = sizeof(deletions)/sizeof(deletions[0]); 
	
	for(int i = 0; i < n3; i++) 
		myHash.erase(deletions[i]); 
    cout<< "elements remove: ";
    for(int i = 0; i < n3; i++)
        cout<< deletions[i] << ", ";
    
	cout<< "\nStatus of hash table after deleting elements : "; myHash.print(); 
	
	return 0; 
}
