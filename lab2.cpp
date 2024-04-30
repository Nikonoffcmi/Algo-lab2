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
    int count = 50;
    double s_result1, u_result1, s_result2, u_result2;
    for (size_t j = 0; j < count; j++)
    {
        int size = 211;
	Hash_table myHash(size, true); // создает хеш-таблицу, false - линейное зондирование false, true - двойное хеширование
	Hash_table myHash2(size, false); // создает хеш-таблицу, false - линейное зондирование false, true - двойное хеширование

	/* массив элементов для вставки */
	
	const size_t n1 = 10; 
	vector<int>  insertions(n1);
	for(int i = 0; i < n1; i++){ 
        int x = -1;
        while (x < 0 || find(insertions.begin(), insertions.end(), x) != insertions.end()){
            x = (int)norm_distribution(generator);
        }
        
        insertions[i] = x;
        myHash.insert(insertions[i]); 
        myHash2.insert(insertions[i]);
    }
	
    // cout<< "Status of hash table after initial insertions: "; myHash.print(); 
	// cout<< "Status of hash table after initial insertions: "; myHash2.print(); 
	

	/* массив элементов для поиска*/
	
    const size_t n2 = 300; 
	vector<int>  queries(n2);
	for(int i = 0; i < n2; i++){ 
        int x = -1;
        while (x < 0 || find(queries.begin(), queries.end(), x) != queries.end()){
            x = norm_distribution(generator);
        }
        queries[i] = x;
        
    }

    double s_sum1 = 0, u_sum1 = 0, s_sum2 = 0, u_sum2 = 0;
    double s_count1 = 0, s_count2 = 0, u_count1 = 0, u_count2 = 0; 

	for(int i = 0; i < n2; i++){
		if(myHash.search(queries[i])) 
        {
            s_sum1 += myHash.get_search_counter();
            s_count1++;
        }
        else{
            u_sum1 += myHash.get_search_counter();
            u_count1++;
        }
        if(myHash2.search(queries[i])) 
        {
            s_sum2 += myHash2.get_search_counter();
            s_count2++;
        }
        else{
            u_sum2 += myHash2.get_search_counter();
            u_count2++;
        }
    }

    // cout<< "Average of successful search 1: " << s_sum1/s_count1 << "    " << s_count1;
    // cout<< "\nAverage of unsuccessful search 1: " << u_sum1/u_count1 << "    " << u_count1<< "\n"; 
	
    // cout<< "Average of successful search 2: " << s_sum2/s_count2 << "    " << s_count2; 
    // cout<< "\nAverage of unsuccessful search 2: " << u_sum2/u_count2 << "    " << u_count2 << "\n"; 

    s_result1 += s_sum1/s_count1;
    u_result1 += u_sum1/u_count1;
    s_result2 += s_sum2/s_count2;
    u_result2 += u_sum2/u_count2;
    }
    
    
    

    cout<< "Average of successful search 1: " << s_result1/count;
    cout<< "\nAverage of unsuccessful search 1: " << u_result1/count << "\n"; 
	
    cout<< "Average of successful search 2: " << s_result2/count; 
    cout<< "\nAverage of unsuccessful search 2: " << u_result2/count << "\n"; 

	// /* массив элементов для удаления */
	
	// int deletions[] = {2}, 
	// 	n3 = sizeof(deletions)/sizeof(deletions[0]); 
	
	// for(int i = 0; i < n3; i++) 
	// 	myHash.erase(deletions[i]); 

	// cout<< "Status of hash table after deleting elements : "; myHash.print(); 
	
	return 0; 
}
