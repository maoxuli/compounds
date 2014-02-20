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
#define _TEST 

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
	
	TrieNode(bool compound = true) // Root of Trie is marked as compound
	: isWord(false)
	, isCompound(compound)
	{
		for(int i=0; i<LETTER_NUMBER; i++)
		{
			children[i] = NULL;
		}
	}
	
	~TrieNode()
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
		if(!infile)
		{
			return false;
		}
		
		std::string line;
		const std::string delim = {" \t\n\r"};
		while(getline(infile, line)) 
		{
			std::string::size_type beg = line.find_first_not_of(delim);
			if(beg == std::string::npos)
			{
				continue;
			}
		    else 
			{
				std::string::size_type end = line.find_first_of(delim, beg);
				if(end == std::string::npos)
				{
					end = line.length();
				}
		      	insert(line.substr(beg, end - beg));
		    }
		}
		return true;
	}
	
	void loadTestWords()
	{
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
		std::cout << "Search cat: " << search("cat") << std::endl;
		std::cout << "Search cats: " << search("cats") << std::endl;
		std::cout << "Search catsdogcats: " << search("catsdogcats") << std::endl;
		std::cout << "Search catxdogcatsrat: " << search("catxdogcatsrat") << std::endl;
		std::cout << "Search dog: " << search("dog") << std::endl;
		std::cout << "Search dogcatsdog: " << search("dogcatsdog") << std::endl;
		std::cout << "Search hippopotamuses: " << search("hippopotamuses") << std::endl;
		std::cout << "Search rat: " << search("rat") << std::endl;
		std::cout << "Search ratcatdogcat: " << search("ratcatdogcat") << std::endl;
		
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
	
	// Find n longest compound word
	// Return value is the total number of compound words in dictionary
	size_t findCompounds(std::vector<std::string>& words, size_t n)
	{
		// Traverse Trie to calculate DP value for each prefix

		return 0;
	}
	
private:
	// Root of customized Trie
	TrieNode _root;
	
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
	            node->children[index] = new TrieNode(false);
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
	
	// Traverse Trie
	// Calculate DP 
	// Output compound words (node->isWord && node->isCompound)
	void traverse(std::string& prefix, const TrieNode* node) 
	{
#ifdef _TEST
	  	if(node->isWord)
		{
			std::cout << prefix << std::endl;
		}
#endif

		for(unsigned char index = 0; index < LETTER_NUMBER; index++) 
		{
	    	char next = 'a' + index;
	    	const TrieNode* child = node->children[index];
	    	if(child != NULL) 
			{
	      		prefix.push_back(next);
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
	
	// Test
#ifdef _TEST
	dict.testSearch();
	//dict.testTraverse();
#endif
	
	std::vector<std::string> words;
	size_t n = 2;
	size_t count = dict.findCompounds(words, n); // Identify the longest and second longest
	std::cout << "Total number of compound words is: " << count << std::endl;
	std::cout << "The longest " << n << " compound words are: " << std::endl; 
	for(std::vector<std::string>::const_iterator it = words.begin(); it != words.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	
	return 0;
}