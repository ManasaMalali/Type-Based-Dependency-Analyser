#include "test.h"

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
bool ConsumeState::dispComments = false;													// comments are not displayed by default
bool ConsumeState::bConstruct = true;
bool ConsumeState::bDestruct = true;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatSQStrings = nullptr;
ConsumeState* ConsumeState::_pEatDQStrings = nullptr;
ConsumeState* ConsumeState::_pEatSpecialChar = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
std::vector<std::string> ConsumeState::_singleCharTokens; //= { "<", ">","[","]", "(",")","{" , "}", ":", "=", "+", "-", "*", "\n" };	//pre-defined special single character tokens
std::vector<std::string> ConsumeState::_doubleCharTokens;// = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };		//pre-defined special double character tokens

void testLog(const std::string& msg);

// Used to add a character to single special character list
void ConsumeState::setSingleSpecialChars(std::string strSSC)
{
	_singleCharTokens.push_back(strSSC);
}
ConsumeState* ConsumeState::nextStateExt()
{
	if (currChar == '\n')
		return _pEatNewline;
	if (std::isalnum(currChar) || currChar == '_')
		return _pEatAlphanum;
	if (ispunct(currChar))
	{
		if (currChar == '"')
			return _pEatDQStrings;
		else if (currChar == '\'')
			return _pEatSQStrings;
		else
			return _pEatPunctuator;
	}
	if (currChar == '"')
		return _pEatPunctuator;
	if (!_pIn->good())
		return _pEatWhitespace;
}
// Used to add a double character string to double special character list
void ConsumeState::setDoubleSpecialChars(std::string strDSC)
{
	_doubleCharTokens.push_back(strDSC);
}

// Used to retrieve single special characters
std::vector<std::string> ConsumeState::getSingleCharToks()
{
	return _singleCharTokens;
}

// Used to retrieve double special character strings
std::vector<std::string> ConsumeState::getDoubleCharToks()
{
	return _doubleCharTokens;
}

// Used to display comments as tokens
void ConsumeState::displayComments()
{
	dispComments = true;
}

// Checks if the character is a single special character
bool ConsumeState::isSingleCharToken(int tok, int next)
{
	if (tok == '\\' && next == 'n')
		return true;
	std::string temp;
	temp += tok;
	for (size_t i = 0; i < _singleCharTokens.size(); i++)
	{
		if (_singleCharTokens[i] == temp)
			return true;
	}
	return false;
}

// Checks if the string is a double special character string
bool ConsumeState::isDoubleCharToken(std::string tok)
{
	for (size_t i = 0; i < _doubleCharTokens.size(); i++)
	{
		if (_doubleCharTokens[i] == tok)
			return true;
	}
	return false;
}

// Used to determine the next character's state
ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();
	if (chNext == EOF)
		_pIn->clear();
	if (isSingleCharToken(currChar, chNext))
		return _pEatSpecialChar;
	std::string temp;
	temp += currChar;
	temp += chNext;
	if (isDoubleCharToken(temp))
		return _pEatSpecialChar;
	if (std::isspace(currChar) && currChar != '\n')
		return _pEatWhitespace;
	if (currChar == '/' && chNext == '/')
		return _pEatCppComment;
	if (currChar == '/' && chNext == '*')
		return _pEatCComment;
	return nextStateExt();

	throw(std::logic_error("invalid type"));
}

// Identifies and discards white spaces
class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatWhitespace");
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

// Identifies C++ comments
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatCppComment");
		token.clear();
		do {
			if (dispComments)
				token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
	}
};

// Identifies C comments
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatCComment");
		token.clear();
		do {
			if (dispComments)
				token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		if (dispComments)
			token += "*/";
		_pIn->get();
		currChar = _pIn->get();
	}
};

// Identifies Single and double special characters
class EatSpecialChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatSpecialCharacter");
		std::string temp;
		int chNext = _pIn->peek();
		token.clear();
		if (isSingleCharToken(currChar, chNext) && currChar == 92 && chNext == 'n')
		{
			token += currChar;
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
			return;
		}
		token += currChar;
		temp += currChar;
		temp += chNext;
		if (isDoubleCharToken(temp))
		{
			currChar = _pIn->get();
			token += currChar;
		}
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		return;
	}
};

// Identifies punctuators
class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatPunctuators");
		token.clear();
		do {
			if (isSingleCharToken(currChar))
				break;
			std::string temp;
			temp += currChar;
			temp += _pIn->peek();
			if (isDoubleCharToken(temp))													// Checks if it is a double special character
				break;
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar) && currChar != '"' && currChar != '\'');
	}
};

// Identifies strings enclosed in single quotes
class EatSQStrings : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatSQStrings");
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			if (currChar == '\\' && _pIn->peek() == '\\')
			{
				currChar = _pIn->get();
				if (_pIn->peek() == '\'')
				{
					token += currChar;
					currChar = _pIn->get();
					break;
				}
			}
			if (currChar == '\\' && _pIn->peek() == '\'')
			{
				token += _pIn->get();
			}
			else if (currChar != '\'')
				token += currChar;
		} while (currChar != '\'');
		currChar = _pIn->get();
	}
};

// Identifies strings enclosed in double quotes
class EatDQStrings : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatDQStrings");
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			if (currChar == '\\' && _pIn->peek() == '\\')
			{
				currChar = _pIn->get();
				if (_pIn->peek() == '\'')
				{
					token += currChar;
					currChar = _pIn->get();
					break;
				}
			}
			if (currChar == '\\' && _pIn->peek() == '"')
				token += _pIn->get();
			else if (currChar != '"')
				token += currChar;
		} while (currChar != '"');
		currChar = _pIn->get();
	}
};

// Identifies alphanumeric characters
class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatAlphanum");
		token.clear();
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar) || currChar == '_');
	}
};

// Identifies newline
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		testLog("state: eatNewLine");
		token.clear();
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

// Resets all static variables in ConsumeState
void ConsumeState::resetConsumeState()
{
	bConstruct = true;
	bDestruct = true;
	dispComments = false;
}

ConsumeState::ConsumeState()
{
	if (bConstruct)
	{
		bConstruct = false;
		_singleCharTokens = { "<", ">","[","]", "(",")","{" , "}", ":", "=", "+", "-", "*", "\n" };
		_doubleCharTokens = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatSQStrings = new EatSQStrings();
		_pEatDQStrings = new EatDQStrings();
		_pEatSpecialChar = new EatSpecialChar();
		_pEatNewline = new EatNewline();
		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	if (bDestruct)
	{
		bDestruct = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatSQStrings;
		delete _pEatDQStrings;
		delete _pEatSpecialChar;
		delete _pEatNewline;
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

// Used to provide the file as input
bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

// Used to collect tokens
std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

// Used to add characters as single special characters
void Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSingleSpecialChars(ssc);
}

// Used to add characters as double special character string
void Toker::setSpecialCharPairs(std::string scp)
{
	pConsumer->setDoubleSpecialChars(scp);
}

// Used to display C and C++ comments if any
void Toker::showComments()
{
	pConsumer->displayComments();
}

// Resets consumeState object
void Toker::resetTok()
{
	pConsumer->resetConsumeState();
}

// Checks if the input file is accessible
bool Toker::canRead() { return pConsumer->canRead(); }

// Logs the state of the token
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	//toker.showComments();
	//toker.setSpecialSingleChars("\n");
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	return 0;
}
#endif