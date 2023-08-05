
#include <iostream>
#include <fstream>
#include <string>


using namespace std;


typedef enum TokenType {
	EoF, IGNORE, INCREMENT, PLUS_EQUAL, PLUS, DECREMENT, MINUS_EQUAL, MINUS, MULTIPLY_EQUAL, MULTIPLY, DIVIDE_EQUAL, DIVIDE,
	STREAM_INSERTION, LESS_THAN_OR_EQUAL_TO, LESS_THAN, STREAM_EXTRACTION, GREATER_THAN_OR_EQUAL_TO, GREATER_THAN, EQUALITY, EQUAL,
	NOT_EQUAL, FACTORIAL, MODULUS, NUM, IF, ELSE, WHILE, DO, FOR, SWITCH, CASE, INT, LONG, FLOAT, DOUBLE, CHAR, STRING, VARIABLE, INVALID_INPUT,
	LEFT_ROUND_BRACKET, RIGHT_ROUND_BRACKET, LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET, LEFT_SQUARE_BRACKET, RIGHT_SQUARE_BRACKET,
	SEMICOLON, COLON, COMMA, LOGICAL_AND, LOGICAL_OR, BITWISE_AND, BITWISE_OR, BEGIN, END
};



typedef enum NodeKind { stmt_kind, exp_kind };
typedef enum ExpKind {operator_kind, constant_kind, variable_kind};
typedef enum StmtKind {if_kind, asmt_kind, do_kind, while_kind};


struct TokenRecord
{
	TokenType token = TokenType::EoF;
	string name = "";
	int value = 0;
};



struct STable
{
	int serial = 0;
	string name = "";
	int value = 0;
};



struct TreeNode
{
	NodeKind nodek;
	ExpKind expk;
	StmtKind stmtk;

	int value = 0;
	string name = "";
	TokenType opr;
	TreeNode* leftChild = NULL;
	TreeNode* middleChild = NULL;
	TreeNode* rightChild = NULL;

	TreeNode* sibling = NULL;
};



bool program();
TreeNode* statementList();
TreeNode* statement();

TokenRecord lexical();
TokenRecord symbolTable(string var);
void ungetToken();
TokenRecord getToken();

TreeNode* factor();
TreeNode* term();
TreeNode* expression();
TreeNode* insertionExtraction();
TreeNode* lesserGreaterEq();
TreeNode* eqNotEq();
TreeNode* logAnd();
TreeNode* logOr();

void display(TreeNode* tree);


fstream in;

STable symTable[100];

TokenRecord x;
TokenRecord currTok;
TokenRecord prevTok;

bool flag = false;



int main()
{
	
	in.open("sample.txt", ios::in);
	if (in)
	{
		if (program())
		{
			cout << "Successful " << endl;
		}
		else
		{
			cout << "Unsuccessful" << endl;
		}

	}
	in.close();

	//system("pause");

}





bool program()
{
	TreeNode* fullTree = NULL;
	x = getToken();


	if (x.token == BEGIN)
	{
		
		if ((fullTree = statementList()) != NULL)
		{

			display(fullTree);
			cout << endl;

			x = getToken();
			if (x.token == END);
			{
				return true;
			}
		}
	}
	return false;
}




TreeNode* statementList()  // statement list 
{
	TreeNode* stree = NULL;
	TreeNode* prevTree = NULL;
	TreeNode* stmtTree = NULL;


	if (prevTree = statement())
	{
		do
		{
			prevTree->sibling = NULL;
			stmtTree = prevTree;
			stmtTree->sibling = NULL;



			x = getToken();
			if (x.token == IF || x.token == DO || x.token == VARIABLE)
			{
				ungetToken();
				
				if ((stree = statement()) == NULL)
				{
					return NULL;
				}

				stree->sibling = NULL;
				prevTree->sibling = stree;
				prevTree = stree;
			}
			else
			{
				ungetToken();
				break;
			}

		} while (1);
		return stmtTree; // if , not, uid not equal then may be else
	}
	return NULL; //otherwise false

}




TreeNode* statement()
{
	TreeNode* t1 = NULL;
	TreeNode* tree = NULL;
	
	
	x = getToken();
	if (x.token == VARIABLE)
	{
		t1 = new TreeNode;
		t1->nodek = stmt_kind;
		t1->stmtk = asmt_kind;
		t1->opr = x.token;
		t1->name = x.name;

		x = getToken();
		if (x.token == EQUAL)
		{
			tree = new TreeNode;
			tree->nodek = stmt_kind;
			tree->stmtk = asmt_kind;
			tree->opr = x.token;
			tree->name = x.name;
			tree->rightChild = t1;


			if (t1 = logOr())
			{
				tree->leftChild = t1;
				x = getToken();
				if (x.token == SEMICOLON)
				{
					return  tree;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	else if (x.token == IF)
	{
		tree = new TreeNode;
		tree->nodek = stmt_kind;
		tree->stmtk = if_kind;
		tree->name = "IF";


		x = getToken();
		if (x.token == LEFT_ROUND_BRACKET)
		{
			if (t1 = logOr())
			{
				tree->leftChild = t1;
				x = getToken();
				if (x.token == RIGHT_ROUND_BRACKET)
				{
					x = getToken();
					if (x.token == LEFT_CURLY_BRACKET)
					{
						if (t1 = statementList())
						{
							tree->middleChild = t1;			//no idea about the levels hence better to attach to middle child
							x = getToken();
							if (x.token == RIGHT_CURLY_BRACKET)
							{
								x = getToken();
								if (x.token == ELSE)
								{
									tree->rightChild = new TreeNode;
									tree->rightChild->nodek = stmt_kind;
									tree->rightChild->stmtk = if_kind;
									tree->rightChild->name = "ELSE";

									x = getToken();

									if (x.token == LEFT_CURLY_BRACKET)
									{
										if (t1 = statementList())
										{
											tree->rightChild->leftChild = t1;
											x = getToken();
											if (x.token == RIGHT_CURLY_BRACKET)
											{
												return tree;
											}
											return NULL;
										}
										return NULL;
									}
									return NULL;
								}
								else
								{
									ungetToken();
									return tree;
								}
								return NULL;
							}
							return NULL;
						}
						return NULL;
					}
					return NULL;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	else if (x.token == DO)
	{
		tree = new TreeNode;
		tree->nodek = stmt_kind;
		tree->stmtk = do_kind;
		tree->name = "DO";

		x = getToken();
		if (x.token == LEFT_CURLY_BRACKET)
		{
			if (t1 = statementList())
			{
				tree->leftChild = t1;
				x = getToken();
				if (x.token == RIGHT_CURLY_BRACKET)
				{
					x = getToken();
					if (x.token == WHILE)
					{
						tree->middleChild = new TreeNode;
						tree->middleChild->nodek = stmt_kind;
						tree->middleChild->stmtk = do_kind;
						tree->middleChild->name = "WHILE";
						
						x = getToken();
						if (x.token == LEFT_ROUND_BRACKET)
						{
							if (t1 = logOr())
							{
								tree->middleChild->leftChild = t1;
								x = getToken();

								if (x.token == RIGHT_ROUND_BRACKET)
								{
									x = getToken();
									if (x.token == SEMICOLON)
									{
										return tree;
									}
									return NULL;
								}
								return NULL;
							}
							return NULL;
						}
						return NULL;
					}
					return NULL;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	else if (x.token == WHILE)
	{
		tree = new TreeNode;
		tree->nodek = stmt_kind;
		tree->stmtk = while_kind;
		tree->name = "WHILE";

		x = getToken();
		if (x.token == LEFT_ROUND_BRACKET)
		{
			if (t1 = logOr())
			{
				tree->leftChild = t1;
				x = getToken();
				if (x.token == RIGHT_ROUND_BRACKET)
				{
					x = getToken();
					if (x.token == LEFT_CURLY_BRACKET)
					{
						if (t1 = statementList())
						{
							tree->middleChild = t1;
							x = getToken();
							if (x.token == RIGHT_CURLY_BRACKET)
							{
								return tree;
							}
							return NULL;
						}
						return NULL;
					}
					return NULL;
				}
				return NULL;
			}
			return NULL;
		}
		return NULL;
	}
	return NULL;
}





TokenRecord lexical()
{
	int currPos = in.tellg();
	in.seekg(0, ios::end);
	int endPos = in.tellg();
	in.seekg(currPos, ios::beg);

	TokenRecord tokenObj;

	char input;
	in.get(input);

	if (currPos == endPos || input == -52 || input == EOF)
	{
		tokenObj.token = TokenType::EoF;
		tokenObj.name = "EoF";
		return tokenObj;
	}

	if (input == '\n' || input == '\t' || input == ' ')
	{
		//++currPos;													//maybe
		while (input == '\n' || input == '\t' || input == ' ')
		{
			in.get(input);
			++currPos;

			if (currPos == endPos || input == -52 || input == EOF)
			{
				tokenObj.token = TokenType::EoF;
				tokenObj.name = "EoF";
				return tokenObj;
			}
		}
	}


	if (input == '+')
	{
		in.get(input);
		if (input == '+')
		{
			tokenObj.token = TokenType::INCREMENT;
			tokenObj.name = "INCREMENT";
			return tokenObj;
		}
		else if (input == '=')
		{
			tokenObj.token = TokenType::PLUS_EQUAL;
			tokenObj.name = "PLUS_EQUAL";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::PLUS;
			tokenObj.name = "PLUS";
			return tokenObj;
		}
	}
	else if (input == '-')
	{
		in.get(input);
		if (input == '-')
		{
			tokenObj.token = TokenType::DECREMENT;
			tokenObj.name = "DECREMENT";
			return tokenObj;
		}
		else if (input == '=')
		{
			tokenObj.token = TokenType::MINUS_EQUAL;
			tokenObj.name = "MINUS_EQUAL";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::MINUS;
			tokenObj.name = "MINUS";
			return tokenObj;
		}
	}
	else if (input == '*')
	{
		in.get(input);
		if (input == '=')
		{
			tokenObj.token = TokenType::MULTIPLY_EQUAL;
			tokenObj.name = "MULTIPLY_EQUAL";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::MULTIPLY;
			tokenObj.name = "MULTIPLY";
			return tokenObj;
		}
	}
	else if (input == '/')
	{
		in.get(input);
		++currPos;

		if (input == '=')
		{
			tokenObj.token = TokenType::DIVIDE_EQUAL;
			tokenObj.name = "DIVIDE_EQUAL";
			return tokenObj;
		}
		else if (input == '*')
		{
			while (1)
			{
				in.get(input);
				++currPos;

				if (input == '*' && currPos != endPos)
				{
					in.get(input);
					++currPos;

					if (input == '/')
					{
						tokenObj.token = TokenType::IGNORE;
						return tokenObj;
					}
				}
				else if (currPos == endPos || input == -52 || input == EOF)
				{
					tokenObj.token = TokenType::EoF;
					tokenObj.name = "EoF";
					return tokenObj;
				}
			}
		}
		else if (input == '/')
		{
			while (1)
			{
				in.get(input);
				++currPos;

				if (input == '\n')
				{
					tokenObj.token = TokenType::IGNORE;
					return tokenObj;
				}
				else if (currPos == endPos || input == -52 || input == EOF)
				{
					tokenObj.token = TokenType::EoF;
					tokenObj.name = "EoF";
					return tokenObj;
				}
			}
		}
		else
		{
			--currPos;
			in.putback(input);
			tokenObj.token = TokenType::DIVIDE;
			tokenObj.name = "DIVIDE";
			return tokenObj;
		}
	}
	else if (input == '<')
	{
		in.get(input);
		if (input == '<')
		{
			tokenObj.token = TokenType::STREAM_INSERTION;
			tokenObj.name = "STREAM_INSERTION";
			return tokenObj;
		}
		else if (input == '=')
		{
			tokenObj.token = TokenType::LESS_THAN_OR_EQUAL_TO;
			tokenObj.name = "LESS_THAN_OR_EQUAL_TO";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::LESS_THAN;
			tokenObj.name = "LESS_THAN";
			return tokenObj;
		}
	}
	else if (input == '>')
	{
		in.get(input);
		if (input == '>')
		{
			tokenObj.token = TokenType::STREAM_EXTRACTION;
			tokenObj.name = "STREAM_EXTRACTION";
			return tokenObj;
		}
		else if (input == '=')
		{
			tokenObj.token = TokenType::GREATER_THAN_OR_EQUAL_TO;
			tokenObj.name = "GREATER_THAN_OR_EQUAL_TO";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::GREATER_THAN;
			tokenObj.name = "GREATER_THAN";
			return tokenObj;
		}
	}
	else if (input == '=')
	{
		in.get(input);
		if (input == '=')
		{
			tokenObj.token = TokenType::EQUALITY;
			tokenObj.name = "EQUALITY";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::EQUAL;
			tokenObj.name = "EQUAL";
			return tokenObj;
		}
	}
	else if (input == '!')
	{
		in.get(input);
		if (input == '=')
		{
			tokenObj.token = TokenType::NOT_EQUAL;
			tokenObj.name = "NOT_EQUAL";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::FACTORIAL;
			tokenObj.name = "FACTORIAL";
			return tokenObj;
		}
	}
	else if (input == '%')
	{
		tokenObj.token = TokenType::MODULUS;
		tokenObj.name = "MODULUS";
		return tokenObj;
	}
	else if (input == '(')
	{
		tokenObj.token = TokenType::LEFT_ROUND_BRACKET;
		tokenObj.name = "LEFT_ROUND_BRACKET";
		return tokenObj;
	}
	else if (input == ')')
	{
		tokenObj.token = TokenType::RIGHT_ROUND_BRACKET;
		tokenObj.name = "RIGHT_ROUND_BRACKET";
		return tokenObj;
	}
	else if (input == '{')
	{
		tokenObj.token = TokenType::LEFT_CURLY_BRACKET;
		tokenObj.name = "LEFT_CURLY_BRACKET";
		return tokenObj;
	}
	else if (input == '}')
	{
		tokenObj.token = TokenType::RIGHT_CURLY_BRACKET;
		tokenObj.name = "RIGHT_CURLY_BRACKET";
		return tokenObj;
	}
	else if (input == '[')
	{
		tokenObj.token = TokenType::LEFT_SQUARE_BRACKET;
		tokenObj.name = "LEFT_SQUARE_BRACKET";
		return tokenObj;
	}
	else if (input == ']')
	{
		tokenObj.token = TokenType::RIGHT_SQUARE_BRACKET;
		tokenObj.name = "RIGHT_SQUARE_BRACKET";
		return tokenObj;
	}
	else if (input == ';')
	{
		tokenObj.token = TokenType::SEMICOLON;
		tokenObj.name = "SEMICOLON";
		return tokenObj;
	}
	else if (input == ':')
	{
		tokenObj.token = TokenType::COLON;
		tokenObj.name = "COLON";
		return tokenObj;
	}
	else if (input == ',')
	{
		tokenObj.token = TokenType::COMMA;
		tokenObj.name = "COMMA";
		return tokenObj;
	}
	else if (input == '&')
	{
		in.get(input);

		if (input == '&')
		{
			tokenObj.token = TokenType::LOGICAL_AND;
			tokenObj.name = "LOGICAL_AND";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::BITWISE_AND;
			tokenObj.name = "BITWISE_AND";
			return tokenObj;
		}

	}
	else if (input == '|')
	{
		in.get(input);

		if (input == '|')
		{
			tokenObj.token = TokenType::LOGICAL_OR;
			tokenObj.name = "LOGICAL_OR";
			return tokenObj;
		}
		else
		{
			in.putback(input);
			tokenObj.token = TokenType::BITWISE_OR;
			tokenObj.name = "BITWISE_OR";
			return tokenObj;
		}
	}
	else if (isdigit(input))
	{
		string num = "";
		num += input;
		++currPos;

		while (isdigit(input) != 0 && currPos != endPos && input != -52 && input != EOF)
		{
			in.get(input);
			++currPos;

			if (isdigit(input) != 0 && currPos != endPos && input != -52 && input != EOF)
			{
				num += input;
			}
			else
			{
				--currPos;
				in.putback(input);
				break; //some other operator
			}
		}

		tokenObj.token = TokenType::NUM;
		tokenObj.name = "NUM";
		tokenObj.value = stoi(num);
		return tokenObj;
	}
	else if (input == '_' || isalpha(input))
	{
		string word = "";
		word += input;
		++currPos;

		while (input != ' ' && currPos != endPos && input != -52 && input != EOF)
		{
			in.get(input);
			++currPos;

			if (input == '_' || isalnum(input))
			{
				word += input;
			}
			else
			{
				--currPos;
				in.putback(input);
				break; //some other operator
			}
		}


		if (word == "if")
		{
			tokenObj.token = TokenType::IF;
			tokenObj.name = "IF";
			return tokenObj;
		}
		else if (word == "else")
		{
			tokenObj.token = TokenType::ELSE;
			tokenObj.name = "ELSE";
			return tokenObj;
		}
		else if (word == "do")
		{
			tokenObj.token = TokenType::DO;
			tokenObj.name = "WHILE";
			return tokenObj;
		}
		else if (word == "while")
		{
			tokenObj.token = TokenType::WHILE;
			tokenObj.name = "WHILE";
			return tokenObj;
		}
		else if (word == "for")
		{
			tokenObj.token = TokenType::FOR;
			tokenObj.name = "FOR";
			return tokenObj;
		}
		else if (word == "switch")
		{
			tokenObj.token = TokenType::SWITCH;
			tokenObj.name = "SWITCH";
			return tokenObj;
		}
		else if (word == "case")
		{
			tokenObj.token = TokenType::CASE;
			tokenObj.name = "CASE";
			return tokenObj;
		}
		else if (word == "int")
		{
			tokenObj.token = TokenType::INT;
			tokenObj.name = "INT";
			return tokenObj;
		}
		else if (word == "long")
		{
			tokenObj.token = TokenType::LONG;
			tokenObj.name = "LONG";
			return tokenObj;
		}
		else if (word == "float")
		{
			tokenObj.token = TokenType::FLOAT;
			tokenObj.name = "FLOAT";
			return tokenObj;
		}
		else if (word == "double")
		{
			tokenObj.token = TokenType::DOUBLE;
			tokenObj.name = "DOUBLE";
			return tokenObj;
		}
		else if (word == "char")
		{
			tokenObj.token = TokenType::CHAR;
			tokenObj.name = "CHAR";
			return tokenObj;
		}
		else if (word == "string")
		{
			tokenObj.token = TokenType::STRING;
			tokenObj.name = "STRING";
			return tokenObj;
		}
		else if (word == "BEGIN")
		{
			tokenObj.token = TokenType::BEGIN;
			tokenObj.name = "BEGIN";
			return tokenObj;
		}
		else if (word == "END")
		{
			tokenObj.token = TokenType::END;
			tokenObj.name = "END";
			return tokenObj;
		}
		else
		{
			tokenObj = symbolTable(word);
			return tokenObj;
		}
	}
	else
	{
		in.putback(input);
		tokenObj.token = TokenType::INVALID_INPUT;
		tokenObj.name = "INVALID_INPUT";
		return tokenObj;
	}
}





TokenRecord symbolTable(string var)
{
	TokenRecord obj;

	bool check = false;
	int i = 0;

	for (; symTable[i].serial != 0 && i < 100; ++i)
	{
		if (symTable[i].name == var)
		{
			check = true;
			++symTable[i].value;
			break;
		}
	}

	if (check == false && i < 100)
	{
		symTable[i].serial = i + 1;
		symTable[i].name = var;
		symTable[i].value = 1;
	}

	obj.token = TokenType::VARIABLE;
	obj.name = var;
	return obj;
}





void ungetToken()
{
	flag = true;
}




TokenRecord getToken()
{
	if (!flag)
	{
		currTok = lexical();
		prevTok = currTok;
	}
	else
	{
		flag = false;
		currTok = prevTok;
	}
	return currTok;
}




TreeNode* factor()
{
	TreeNode* t = NULL;
	currTok = getToken();

	if (currTok.token == VARIABLE)
	{
		t = new TreeNode();
		t->expk = ExpKind::variable_kind;
		t->name = currTok.name;

		return t;
	}
	else if (currTok.token == NUM)
	{
		t = new TreeNode();
		t->expk = ExpKind::constant_kind;
		t->value = currTok.value;
		t->name = currTok.name;

		return t;
	}
	else if (currTok.token == LEFT_ROUND_BRACKET)
	{
		return t;
	}
}




TreeNode* term()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = factor();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == MULTIPLY || currTok.token == DIVIDE)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}

	} while (currTok.token == MULTIPLY || currTok.token == DIVIDE || currTok.token == MODULUS);

	ungetToken();
	return t;
}




TreeNode* expression()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = term();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == PLUS || currTok.token == MINUS)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == PLUS || currTok.token == MINUS);

	ungetToken();
	return t;
}




TreeNode* insertionExtraction()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = expression();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == TokenType::STREAM_EXTRACTION || currTok.token == TokenType::STREAM_INSERTION)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == TokenType::STREAM_EXTRACTION || currTok.token == TokenType::STREAM_INSERTION);

	ungetToken();
	return t;
}





TreeNode* lesserGreaterEq()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = insertionExtraction();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == TokenType::LESS_THAN || currTok.token == TokenType::LESS_THAN_OR_EQUAL_TO || currTok.token == TokenType::GREATER_THAN || currTok.token == TokenType::GREATER_THAN_OR_EQUAL_TO)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == TokenType::LESS_THAN || currTok.token == TokenType::LESS_THAN_OR_EQUAL_TO || currTok.token == TokenType::GREATER_THAN || currTok.token == TokenType::GREATER_THAN_OR_EQUAL_TO);

	ungetToken();
	return t;
}




TreeNode* eqNotEq()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = lesserGreaterEq();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == TokenType::EQUALITY || currTok.token == TokenType::NOT_EQUAL)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == TokenType::EQUALITY || currTok.token == TokenType::NOT_EQUAL);

	ungetToken();
	return t;
}




TreeNode* logAnd()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = eqNotEq();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == TokenType::LOGICAL_AND)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == TokenType::LOGICAL_AND);

	ungetToken();
	return t;
}




TreeNode* logOr()
{
	TreeNode* t = NULL, * p = NULL;
	int flag = 0;

	do
	{
		t = logAnd();
		if (flag)
		{
			p->rightChild = t;
			flag = 0;
			t = p;
		}

		currTok = getToken();

		if (currTok.token == TokenType::LOGICAL_OR)
		{
			p = new TreeNode();
			p->expk = ExpKind::operator_kind;
			p->opr = currTok.token;
			p->name = currTok.name;
			p->leftChild = t;
			flag = 1;

		}
	} while (currTok.token == TokenType::LOGICAL_OR);

	ungetToken();
	return t;
}


void display(TreeNode* tree)
{
	if (tree->leftChild != NULL)
	{
		display(tree->leftChild);
	}

		cout << "\nNode Name: " << tree->name;
		cout << "\nNode Value: " << tree->value;
		cout << "\nNode Oprtr: " << tree->opr << endl;

	if (tree->middleChild != NULL)
	{
		display(tree->middleChild);
	}

	if (tree->rightChild != NULL)
	{
		display(tree->rightChild);
	}

}
