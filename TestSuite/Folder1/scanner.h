/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.3                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Lexical Scanner, CSE687 - Object Oriented Design  //
// Author:		Pratap Rao Rayachoti, Syracuse University		   //
//				prayacho@syr.edu								   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Project1.sln /rebuild debug
*/
/*
* Maintenance History:
* --------------------
* ver 3.3: 10 Feb 2016
* - added the functionalities to collect single and double
*   special character tokens
* - added functionalities to add new single and double special
*   character tokens
* ver 3.2: 2 Feb 2016
* - Helper code that does not attempt to handle all tokenizing
*   special cases like escaped characters.
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

namespace Scanner
{
	class ConsumeState																		// private class that determines the state and contents of the tokens
	{
	public:
		enum Days { Saturday,Sunday,Tuesday,Wednesday,Thursday,Friday};
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		ConsumeState* nextStateExt();
		void setSingleSpecialChars(std::string);
		void setDoubleSpecialChars(std::string);
		std::vector<std::string> getSingleCharToks();
		std::vector<std::string> getDoubleCharToks();
		bool isSingleCharToken(int tok, int next = 0);
		bool isDoubleCharToken(std::string);
		void displayComments();
		void resetConsumeState();
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static bool bConstruct;
		static bool bDestruct;
		static bool dispComments;
		static std::vector<std::string> _singleCharTokens;
		static std::vector<std::string> _doubleCharTokens;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatSQStrings;
		static ConsumeState* _pEatDQStrings;
		static ConsumeState* _pEatSpecialChar;
		static ConsumeState* _pEatNewline;
	};
}