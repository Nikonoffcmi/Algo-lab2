#include <iostream> 
#include <vector> 
#include <bitset> 
using namespace std; 

#define MAX_SIZE 10000001ll 

class Hash_table { 
    bool collision;
	int TABLE_SIZE, keysPresent, PRIME; 
	vector<int> hashTable; 
	bitset<MAX_SIZE> isPrime; 
    int search_counter = 0;

	void __setSieve(){ 
		isPrime[0] = isPrime[1] = 1; 
		for(long long i = 2; i*i <= MAX_SIZE; i++) 
			if(isPrime[i] == 0) 
				for(long long j = i*i; j <= MAX_SIZE; j += i) 
					isPrime[j] = 1; 
	} 

    
	int inline hash1(int value){ 
		return value%TABLE_SIZE; 
	} 
	
	int inline hash2(int value){	 
		return 1 + (value%(TABLE_SIZE - 1)); 
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
		__setSieve(); 
		TABLE_SIZE = n; 
        collision = set_collision; //false - линейное зондирование false, true - двойное хеширование

		/* Находим самое большое простое число, которое меньше размера таблицы */
		PRIME = TABLE_SIZE - 1; 
		while(isPrime[PRIME] == 1) 
			PRIME--; 

		keysPresent = 0; 

		/* заполняем таблицу пустыми элементами */
		for(int i = 0; i < TABLE_SIZE; i++) 
			hashTable.push_back(-1); 
	} 

	void __printPrime(long long n){ 
		for(long long i = 0; i <= n; i++) 
			if(isPrime[i] == 0) 
				cout<<i<<", "; 
		cout<<endl; 
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
		/* Return если элемента нет в таблице */
		if(!search(value)) 
			return;
		
        int offset = 0;
		if (collision){
            offset = hash2(value);
            int probe = hash1(value); 
            
            while(hashTable[probe] != -1) 
            if(hashTable[probe] == value){ 
                hashTable[probe] = -2;          // пометить элемент как удаленный при двойном хеширование
                keysPresent--; 
                return; 
            } 
            else
                probe = (probe + offset) % TABLE_SIZE;  
        }
        else{
            offset = 1;
            int probe = hash1(value); 

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
                        if (probe >= hashTable.size())
                            break;
                        if (hashTable[probe] == -2)
                        {
                            probe = (probe + offset) % TABLE_SIZE; 
                            continue;
                        }
                        else if (hashTable[probe] == -1)
                            break;
                        else 
                        {
                            int buf = hash1(hashTable[probe]);
                            if (buf != probe && buf < old_probe){
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
                else
                    probe = (probe + offset) % TABLE_SIZE; 
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
				return false; 
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

	Hash_table myHash(10, true); // создает хеш-таблицу, false - линейное зондирование false, true - двойное хеширование

	/* массив элементов для вставки */
	
	int insertions[] = {2, 22, 12, 32, 6, 66, 52, 56}, 
		n1 = sizeof(insertions)/sizeof(insertions[0]); 
	
	for(int i = 0; i < n1; i++) 
		myHash.insert(insertions[i]); 
	
	cout<< "Status of hash table after initial insertions: "; myHash.print(); 
	

	/* массив элементов для поиска*/
	
	// int queries[] = {1, 49, 65, 12, 2, 43, 56, 3, 69, 88, 115, 49}, 
	// 	n2 = sizeof(queries)/sizeof(queries[0]); 
	
	// cout<<"\n"<<"Search operation after insertion : \n"; 

    // vector<int> s_search;
    // vector<int> u_search;

	// for(int i = 0; i < n2; i++) 
	// 	if(myHash.search(queries[i])) 
    //     {
    //         s_search.push_back(myHash.get_search_counter());
    //         cout<<queries[i]<<" present\n"; 
    //     }
    //     else
    //         u_search.push_back(myHash.get_search_counter());

    // double s_sum = 0, u_sum = 0;
    // double s_result = 0, u_result = 0; 
    // for (int i = 0; i < s_search.size(); i++)
    // {
    //     s_sum += s_search[i];
    // }
    // for (int i = 0; i < u_search.size(); i++)
    // {
    //     u_sum += u_search[i];
    // }
    // s_result = (double)s_sum/s_search.size();
    // if (u_search.size() == 0)
    //     u_result = 0;
    // else
    //     u_result = (double)u_sum/u_search.size();

    // cout<< "Average of successful search : " << s_result; 
    // cout<< "\nAverage of unsuccessful search : " << u_result << "\n"; 
	

	/* массив элементов для удаления */
	
	int deletions[] = {22}, 
		n3 = sizeof(deletions)/sizeof(deletions[0]); 
	
	for(int i = 0; i < n3; i++) 
		myHash.erase(deletions[i]); 

	cout<< "Status of hash table after deleting elements : "; myHash.print(); 
	
	return 0; 
}
