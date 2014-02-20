//
// Find compound words in a given dictionary
// Author: Maoxu "Max" Li
// Email: max@maoxuli.com
//

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <fstream>

// Swith to print out testing info
// #define _TEST 

//
// Trie node
// Customized for finding coumpound words
// Words in dictionary consist of 26 lower case letters
//
#define LETTER_NUMBER 26
#define CHAR_TO_INDEX(c) (c - 'a')
struct TrieNode
{
	bool isWord; // Terminaton of a word
	bool isCompound; // Prefix is a compound
	struct TrieNode* children[LETTER_NUMBER]; // Children[0] for letter 'a' ...
	struct TrieNode* parent; // Back track to parent
	
	TrieNode(TrieNode* owner = NULL)
	: isWord(false)
	, isCompound(false)
	, parent(owner)
	{
		for(int i=0; i<LETTER_NUMBER; i++)
		{
			children[i] = NULL;
		}
	}
	
	~TrieNode()
	{
		clear();
	}
	
	// Clear the sub Trie following this node 
	void clear()
	{
		for(int i=0; i<LETTER_NUMBER; i++)
		{
			if(children[i] != NULL)
			{
				delete children[i];
			}
		}
	}
};

//           
// Dictionary is a customized Trie
// for finding compound words
// 
class Dictionary
{
public:
	bool loadFromFile(const std::string& path)
	{		
		std::ifstream infile(path.c_str());
		if(!infile) // Open file failed
		{
			return false;
		}
		
		// Clear and reload the Trie
		_root.clear();
		
		std::string line;
		const std::string delim = {" \t\n\r"};
		clock_t t1 = clock(); // Timer start
		while(getline(infile, line)) 
		{
			std::string::size_type beg = line.find_first_not_of(delim); // Forward blank
			if(beg == std::string::npos)
			{
				continue; // Blank line
			}
		    else 
			{
				std::string::size_type end = line.find_first_of(delim, beg); // Backward blank
				if(end == std::string::npos)
				{
					end = line.length();
				}
		      	insert(line.substr(beg, end - beg));
		    }
		}
		clock_t t2 = clock(); // Timer end
		std::cout << "Running time for loading dictionary:" << (t2-t1)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
		return true;
	}
	
	void loadTestWords()
	{
		// Clear and reload the Trie
		_root.clear();
		
		insert("cat");
		insert("cats");
		insert("catsdogcats");
		insert("catxdogcatsrat");
		insert("dog");
		insert("dogcatsdog");
		insert("hippopotamuses");
		insert("rat");
		insert("ratcatdogcat");
	}
	
#ifdef _TEST
	void testSearch()
	{
		// Search the existing words
		std::cout << "Search cat: " << search("cat") << std::endl;
		std::cout << "Search cats: " << search("cats") << std::endl;
		std::cout << "Search catsdogcats: " << search("catsdogcats") << std::endl;
		std::cout << "Search catxdogcatsrat: " << search("catxdogcatsrat") << std::endl;
		std::cout << "Search dog: " << search("dog") << std::endl;
		std::cout << "Search dogcatsdog: " << search("dogcatsdog") << std::endl;
		std::cout << "Search hippopotamuses: " << search("hippopotamuses") << std::endl;
		std::cout << "Search rat: " << search("rat") << std::endl;
		std::cout << "Search ratcatdogcat: " << search("ratcatdogcat") << std::endl;
		
		// Search non-existing words
		std::cout << "Search aaaa: " << search("aaaa") << std::endl;
		std::cout << "Search abcd: " << search("abcd") << std::endl;
		std::cout << "Search aaddcdkkekekdk: " << search("aaddcdkkekekdk") << std::endl;
		std::cout << "Search dkdkdldk: " << search("dkdkdldk") << std::endl;
	}
	
	void testTraverse()
	{
		std::string prefix;
		traverse(prefix, &_root);
	}
#endif
	
	// Find coumpounds
	// Return total number and the first two longest
	size_t findCompounds(std::vector<std::string>& words)
	{
		// Clear results
		_count = 0;
		_longest.clear();
		_second.clear();
		
		// Traverse to calculate DP and find compounds
		std::string prefix;
		clock_t t1 = clock(); // Timer start
		traverse(prefix, &_root);
		clock_t t2 = clock(); // Timer end
		std::cout << "Running time for find compounds:" << (t2-t1)*1000/CLOCKS_PER_SEC << "ms" << std::endl;
		words.push_back(_longest);
		words.push_back(_second);
		return _count;
	}
	
private:
	// Root of customized Trie
	// Constructed with functions of load*()
	TrieNode _root;
	
	// Totoal compounds number and first two longest compounds
	// Filled with function of findCompounds()
	size_t _count;
	std::string _longest;
	std::string _second;
	
	// General operation of Trie
	// Insert a word into Trie
	void insert(const std::string& s)
	{
		size_t len = s.length();
	    TrieNode* node = &_root;

		unsigned char index = 0;
	    for(size_t i = 0; i < len; i++ )
	    {
	        index = CHAR_TO_INDEX(s[i]);
	        if(node->children[index] == NULL)
	        {
	            node->children[index] = new TrieNode(node);
	        }
	        node = node->children[index];
			assert(node != NULL);
	    }
	    node->isWord = true; // mark termination of a word
	}
	
	// General operation of Trie
	// Search a word from Trie
	bool search(const std::string& s)
	{
	    int len = s.length();
	    TrieNode* node = &_root;

		unsigned char index = 0;
	    for(size_t i = 0; i < len; i++ )
	    {
	        index = CHAR_TO_INDEX(s[i]);
	        if(node->children[index] == NULL)
	        {
	            return false;
	        }
	        node = node->children[index];
	    }
	    return node->isWord;
	}
	
	// Customized traversal of Trie
	// Calculate DP for each node
	void traverse(std::string& prefix, TrieNode* node) 
	{
#ifdef _TEST
	  	if(node->isWord)
		{
			std::cout << prefix << std::endl;
		}
#endif
		// Back track 
		// find a node where isCompound == true
		// check suffix is a word
		size_t n = 0; // Length of suffix
		TrieNode* p = node;
		while((p = p->parent) != NULL)
		{
			n++;
			if(p->isCompound || p->isWord)
			{
				if(search(prefix.substr(prefix.length() - n))) // suffix is a word
				{
					node->isCompound = true;
					if(node->isWord)
					{
						_count++;
						if(prefix.length() > _longest.length())
						{
							_second = _longest;
							_longest = prefix;
						}
						else if(prefix.length() > _second.length())
						{
							_second = prefix;
						}
					}
					break;
				}
			}
		}
		
		for(unsigned char index = 0; index < LETTER_NUMBER; index++) 
		{
	    	char letter = 'a' + index;
	    	TrieNode* child = node->children[index];
	    	if(child != NULL) 
			{
	      		prefix.push_back(letter);
	      		traverse(prefix, child);
	      		prefix.erase(prefix.end() - 1);
	    	}
	  	}
	}
};

// 
// Main entry
// 
int main(int argc, const char* argv[])
{
	std::string file("wordsforproblem.txt");
	Dictionary dict;
	if(!dict.loadFromFile(file))
	{
		std::cout << "Failed to load " << file << std::endl;
		dict.loadTestWords();
	}
	
#ifdef _TEST
	dict.testSearch();
	dict.testTraverse();
#endif
	
	std::vector<std::string> words;
	size_t count = dict.findCompounds(words); // return the total number of compounds
	std::cout << "Total number of compound words is: " << count << std::endl;
	std::cout << "The longest " << words.size() << " compound words are: " << std::endl; 	
	for(std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	
	return 0;
}