#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>

#define DEBUG false

using namespace std;

struct word_t
{
	string word;
	signed short score;
};

// =========================================================================================
class BeatnikStack
{
	public:
		BeatnikStack();
		
		signed short Pop();
		void Push(signed short pushed);
	
	private:
		vector<signed short> stack;
};

BeatnikStack::BeatnikStack()
{
	stack.push_back(0);
}

signed short BeatnikStack::Pop()
{
	signed short value = stack.back();
	
	stack.pop_back();
	
	if(stack.size() == 0)
	{
		stack.push_back(0);
	}
	
	return value;
}

void BeatnikStack::Push(signed short pushed)
{
	stack.push_back(pushed);
}
// =========================================================================================

inline signed short ScoreLetter(char letter)
{
	switch(letter)
	{
		case 'E':
		case 'A':
		case 'I':
		case 'O':
		case 'N':
		case 'R':
		case 'T':
		case 'L':
		case 'S':
		case 'U':
		{
			return 1;
		}
		
		case 'D':
		case 'G':
		{
			return 2;
		}
		
		case 'B':
		case 'C':
		case 'M':
		case 'P':
		{
			return 3;
		}
		
		case 'F':
		case 'H':
		case 'V':
		case 'W':
		case 'Y':
		{
			return 4;
		}
		
		case 'K':
		{
			return 5;
		}
		
		case 'J':
		case 'X':
		{
			return 8;
		}
		
		case 'Q':
		case 'Z':
		{
			return 10;
		}
		
		case '&':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			return 0;
		}
		
		default:
		{
			cout <<"Warning: '"<< letter <<"' should not be scored."<< endl;
			return 0;
		}
	}
}

signed short ScoreWord(string word)
{
	signed short score = 0;
	
	signed short l = word.length();
	
	for(signed short i = 0; i < l; i++)
	{
		score += ScoreLetter(word.at(i));
	}
	
	return score;
}

inline void Trim(string &text)
{
	signed long start = 0;
	signed long end = text.length() - 1;
	signed long l = end;
	
	while(text.at(start) == ' ')
	{
		start++;
	}
	
	while(text.at(end) == ' ')
	{
		end--;
	}
	
	if(start != 0 or end != l)
	{
		text = text.substr(start, end + 1);
	}
}

inline char UpperCase(char c)
{
	signed short n = (short)c;
	
	if(n >= 97 and n <= 122)
	{
		n -= 32;
	}
	
	return (char)n;
}

inline bool IsSpaceFlag(char c)
{
	return (
		c == ' ' or
		c == '-' or // e.g. "words--words"
		c == '\t' or
		c == '\n' or
		c == '\r'
	);
}

inline bool IsLetter(char c)
{
	unsigned short n = (unsigned short)c;
	
	return (n >= 65 and n <= 90);
}

inline bool IsDigit(char c)
{
	unsigned short n = (unsigned short)c;
	
	return (n >= 48 and n <= 57);
}

string Sanitise(string text)
{
	string sanitised = "";
	bool space = true;
	
	signed long l = text.length();
	for(signed long i = 0; i < l; i++)
	{
		char c = UpperCase(text.at(i));
		
		if(IsSpaceFlag(c))
		{
			if(!space)
			{
				sanitised +=' ';
				space = true;
			}
		}
		else if(IsLetter(c) or IsDigit(c))
		{
			sanitised += c;
			space = false;
		}
		else if(c == '&') // ampersand is a zero-scoring word but should be counted
		{
			if(!space)
			{
				sanitised +=' ';
			}
			
			sanitised +="& ";
			
			space = true;
		}
		else if(c == '.') // handle 2/3-period ellipsis e.g. "word...word" by adding space - ignore other full stops like in "3.14"
		{
			if(!space and i >= 1 and text.at(i - 1) == '.')
			{
				sanitised +=' ';
				space = true;
			}
		}
	}
	
	Trim(sanitised);
	
	return sanitised;
}

vector<word_t> Words(string text)
{
	word_t word;
	word.word = "";
	word.score = 0;
	
	vector<word_t> words;
	words.push_back(word);
	
	unsigned long xWord = 0;
	
	unsigned long l = text.length();
	for(unsigned long i = 0; i < l; i++)
	{
		char c = UpperCase(text.at(i));
		
		if(c == ' ')
		{
			words.push_back(word);
			xWord++;
		}
		else
		{
			words[xWord].word += c;
		}
	}
	
	for(long i = 0; i <= xWord; i++)
	{
		words[i].score = ScoreWord(words[i].word);
	}
	
	return words;
}

inline void Error(string message, word_t word)
{
	cout << endl;
	cout <<"ERROR: "<< message << endl;
	cout <<"\tWORD:\t"<< word.word << endl;
	cout <<"\tSCORE:\t"<< word.score << endl;
}

/*
	5	Push the next word's value onto the stack.
	6	Pop a number from the stack and discard it.
	7	Pop two numbers, add them, and push the result.
	8	Input a character and push its value.
	9	Pop a number and output it as a character.
	10	Pop two numbers, subtract the first one popped from the second one popped, and push the result.
	11	Pop two numbers, swap them, and push them back.
	12	Pop a number and push it twice.
	13	Pop a number and skip ahead n (actually n+1) words if the number is zero.
	14	Pop a number and skip ahead n (actually n+1) words if the number isn't zero.
	15	Pop a number and skip back n words if the number is zero.
	16	Pop a number and skip back n words if the number isn't zero.
	17	Stop the program.
*/
bool Beatnik(string code)
{
	vector<word_t> words = Words(Sanitise(code));
	long nWords = words.size();
	
	#if DEBUG
		for(int i = 0; i < nWords; i++)
		{
			cout << words[i].score <<" ";
		}
		cout << endl;
	#endif
	
	BeatnikStack stack;
	
	for(signed long i = 0; i < nWords; i++)
	{
		short current = i;
		
		switch(words[i].score)
		{
			case 5: // Push the next word's value onto the stack.
			{
				if((i + 1) >= nWords)
				{
					Error("Tried to use parameter beyond end of code. [5]", words[i]);
					return false;
				}
				
				i++;
				
				stack.Push(words[i].score);
				break;
			}

			case 6: // Pop a number from the stack and discard it.
			{
				stack.Pop();
				
				break;
			}

			case 7: // Pop two numbers, add them, and push the result.
			{
				stack.Push(stack.Pop() + stack.Pop());
				
				break;
			}

			case 8: // Input a character and push its value.
			{
				stack.Push((signed short)getch());
				
				break;
			}

			case 9: // Pop a number and output it as a character.
			{
				cout << (char)stack.Pop();
				
				break;
			}

			case 10: // Pop two numbers, subtract the first one popped from the second one popped, and push the result.
			{
				signed short number1 = stack.Pop();
				signed short number2 = stack.Pop();
				
				stack.Push(number2 - number1);
				
				break;
			}

			case 11: // Pop two numbers, swap them, and push them back.
			{
				signed short number1 = stack.Pop();
				signed short number2 = stack.Pop();
				
				stack.Push(number1);
				stack.Push(number2);
				
				break;
			}

			case 12: // Pop a number and push it twice.
			{
				signed short number = stack.Pop();
				
				stack.Push(number);
				stack.Push(number);
				
				break;
			}

			case 13: // Pop a number and skip ahead n (actually n+1) words if the number is zero.
			{
				if(stack.Pop() == 0)
				{	
					if((i + 1) >= nWords)
					{
						Error("Tried to use parameter beyond end of code. [13]", words[current]);
						return false;
					}
					
					i += words[(i + 1)].score;
					
					if(i >= nWords)
					{
						Error("Tried to skip beyond end of code. [13]", words[current]);
						return false;
					}
				}
				else
				{
					i++;
				}
				
				break;
			}

			case 14: //Pop a number and skip ahead n (actually n+1) words if the number isn't zero.
			{
				if(stack.Pop() != 0)
				{
					if((i + 1) >= nWords)
					{
						Error("Tried to use parameter beyond end of code. [14]", words[current]);
						return false;
					}
					
					i += words[(i + 1)].score;
					
					if(i >= nWords)
					{
						Error("Tried to skip beyond end of code. [14]", words[current]);
						return false;
					}
				}
				else
				{
					i++;
				}
				
				break;
			}

			case 15: //Pop a number and skip back n words if the number is zero.
			{
				if(stack.Pop() == 0)
				{
					if((i + 1) >= nWords)
					{
						Error("Tried to use parameter beyond end of code. [15]", words[i]);
						return false;
					}
					
					i -= (words[(i + 1)].score + 1);
					
					if(i < -1)
					{
						Error("Tried to skip beyond start of code. [15]", words[current]);
						return false;
					}
				}
				else
				{
					i++;
				}
				
				break;
			}

			case 16: //Pop a number and skip back n words if the number isn't zero.
			{
				if(stack.Pop() != 0)
				{
					if((i + 1) >= nWords)
					{
						Error("Tried to use parameter beyond end of code. [16]", words[i]);
						return false;
					}
					
					i -= (words[i + 1].score + 1);
					
					if(i < -1)
					{
						Error("Tried to skip beyond start of code. [16]", words[current]);
						return false;
					}
				}
				else
				{
					i++;
				}
				
				break;
			}

			case 17: //Stop the program.
			{
				return true;
				
				break;
			}
			
			default:
			{
				break;
			}
		}
	}
	
	return true;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout <<"Please specify a file."<< endl
			<<"Usage: .\\beatnik FILENAME.beatnik"<< endl;
		
		return 0;
	}
	
	ifstream file(argv[1]);
	
	if(!file.good())
	{
		file.close();
		
		cout <<"File '"<< argv[1] <<"' not found."<< endl;
		
		return 0;
	}
	
	string code = "";
	string line = "";
	int nLines = 0;
	
	while(getline(file, line))
	{
		nLines++;
		
		code += ((nLines > 1) ? "\n"+ line : line);
	}
	
	file.close();
	
	Beatnik(code);
	
	cout << endl;
	
	return 0;
}
















