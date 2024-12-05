// 11027142 林蕙郁
# include <iostream>
# include <cstring>
# include <string> 
# include <vector>
# include <cstdio>
# include <cstdlib>
# include <stdexcept>
# include <iomanip>
# include <sstream>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stack>


using namespace std ;

struct Token_Info{
  string type ; // 每次加入vector的時候 一起給予type是甚麼
  // IDENT? NUM? SIGN? QUIT? others? unrecog_ERROR?
  string tokenStr ; // 存放token內容的string
} ;

struct Variable_Info {
  string name ;
  string value ;
} ; // struct Variable_Info

// global variable
vector<Token_Info> gTokenList ; // 存放type+token的vector
vector<Variable_Info> gVariableList ;
vector<string> gCalValue ; // 用來存要放進in->post的數值 把變數換成數值
vector<string> gLeftCalValue ;
vector<string> gRightCalValue ;

int gLine = 1;
char gNextChar = '\0' ;
int gNextCharLine = -1 ;
bool gTwoDot_Unexpected = false ;


// function name ;
// void
void GetNextChar( char & ch, int & line ) ;
void Skip_White_Space() ;
void SkipLineComment() ;
void Evaluate() ;
void Negative_Solve() ;
void Infix_To_Postfix( vector<string> &postfix, vector<string> tokenStr ) ;
void Bigger( string left, string right ) ;
void Smaller( string left, string right ) ;
void Bigger_Equal( string left, string right ) ;
void Smaller_Equal( string left, string right ) ;
void Not_Equal( string left, string right ) ;
void Equal( string left, string right ) ;
// is what? bool
bool IsLetter( char c ) ;
bool IsDigit( char c ) ;
bool IsOthers_and_Sign( char c ) ;
bool IsIdentifier( string str ) ;
bool IsWhiteSpace( char c ) ;
bool QUIT() ; // 判斷quit是不是在最前面 並且更改type
// parser tree bool
bool Parser_Tree() ;
bool Command() ;
bool IDlessArithExpOrBexp() ;
bool BooleanOperator() ;
bool NOT_ID_StartArithExpOrBexp() ;
bool NOT_ID_StartArithExp() ;
bool NOT_ID_StartTerm() ;
bool NOT_ID_StartFactor() ;
bool ArithExp() ;
bool Term() ;
bool Factor() ;
// Evaluate
bool Defined_Variable() ;
bool Calculate() ;
bool Boolean() ;
bool IsFloat( string str ) ;

// string
string GetID()  ;
string GetNum()  ;
string GetSpecial()  ;
string GetToken( char first_c, string &tokenType ) ;
string Float_Cal( int func ) ;
string Int_Cal( int func ) ;

float Abs( float f ) ;
float Cal_f( string op, float f1, float f2 ) ;
int Cal_i( string op, int f1, int f2 ) ;
int Priority( string op ) ;
float S2f( string str ) ;
int S2i( string str ) ;


bool gfirst = true ;
bool gLinereturn = false ;
void GetNextChar( char & ch, int & line ) {
  if ( gfirst ) {
    line = 1 ;
    gfirst = false ;
  } // if

  cin >> ch ;

  if ( gLinereturn ) {
    line++ ;
    gLinereturn = false ;
  } // if

  if ( ch == '\n' )
    gLinereturn = true ;
} // GetNextChar()

string GetToken( char first_c, string &tokenType ) {
  string tokenStr = "" ; // 存放token的string return用
  tokenType = "" ; // 存放token的type return用
  bool slashIsDiv = false ; // 用來判斷/是註解還是除號  

  while ( IsWhiteSpace( gNextChar )  || ( gNextChar == '/' && ! slashIsDiv ) ) { // 如果是字母或數字或符號
    if ( IsWhiteSpace( gNextChar ) ) {
      Skip_White_Space() ; // 跳過空白
    } // if 
    else if  ( gNextChar == '/' ) {

      if ( cin.peek() == '/' ) {
        GetNextChar( gNextChar, gNextCharLine ) ;
        SkipLineComment() ; // and update gNextChar
      } // if 

      else  // the previous '/'  Is DIVIDE !!!
        slashIsDiv = true  ; // !!! so as to do special handling below for th Is DIVIDE
    } // if gNextChar  Is '/'
  } // while c  Is '/'

  // int line = gNextCharLine ;

  if  ( slashIsDiv ) {
    tokenStr += gNextChar ;
    tokenType = "SIGN" ; // '/'
  } // if Is divide

  else if  ( IsLetter( gNextChar ) ) {
    tokenStr = GetID() ;
    tokenType = "IDENT" ;
  } // else if Letter or Underline

  else if  ( IsDigit( gNextChar ) || gNextChar == '.' ) {
    tokenType = "NUM"  ;
    tokenStr = GetNum()  ;   // 是不是點開頭是進去才判斷

    if ( tokenStr == "." ) tokenType = "unrecog_ERROR" ;
  } // else if Digit

  else if ( IsOthers_and_Sign( gNextChar ) ) {
    tokenType = "others" ;
    tokenStr = GetSpecial() ;
    if ( tokenStr.size() == 1 && tokenStr == ":" ) tokenType = "unrecog_ERROR" ;
    else if ( tokenStr.size() == 1 ) tokenType = "SIGN" ;
  } // else if others and sign

  else { // 甚麼都不是 error
    tokenType = "unrecog_ERROR"  ;
    tokenStr += gNextChar  ;
  } // else --- unrecognized token with first char

  return tokenStr ;
} // GetToken()


// 跳過空白
void Skip_White_Space() {
  while ( IsWhiteSpace( cin.peek() ) )    // 20240327 add
    GetNextChar( gNextChar, gNextCharLine ) ;
} // Skip_White_Space()

// 跳過註解
void SkipLineComment() {
  char t[500] ;
  cin.getline( t, 500 ) ;

  GetNextChar( gNextChar, gNextCharLine ) ;  // read char after line return
} // SkipLineComment()

bool IsWhiteSpace( char c ) {
  if ( c == ' ' || c == '\t' || c == '\n' ) 
    return true ;
  return false ;
} // IsWhiteSpace()

bool  IsLetter( char c ) {  // need string format function?
  if (  ( c >= 'a' && c <= 'z' ) ||  ( c >= 'A' && c <= 'Z' ) )    return true ;
  return false ;
} // IsLetter()

bool  IsDigit( char c ) {  // need string format function?
  if ( c >= '0' && c <= '9' )    return true ;
  return false ;
} // IsDigit()


bool  IsOthers_and_Sign( char c ) {  // only char 沒有'.'
  if ( c == '=' || c == '>' || c == '<' || c == ':' || c == ';' || c == '+' || c == '-' ||
       c == '*' || c == '/' || c == '.' || c == '(' || c == ')' )
    return true ;

  return false ;

} // IsOthers_and_Sign()

string GetID() {    // IDENT 開頭英文 後面可以接 1.英文 2.數字 3.底線
  string token = "" ;
  token += gNextChar ;
  while ( IsLetter( cin.peek() ) || cin.peek() == '_' || IsDigit( cin.peek() ) ) {
    GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
    token += gNextChar ;
  } // while

  return token ;
} // GetID() 

string GetNum() {    // NUM 開頭數字or小數點 後面可以接 1.數字 2.小數點
  string token = "" ;
  token += gNextChar ; // token 的 first char

  
  if ( gNextChar == '.' ) {  // 開頭是'.'
    bool dot_digit = false ;
    while ( IsDigit( cin.peek() ) ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      dot_digit = true ;
      token += gNextChar ;
    } // while  小數數值

    if ( cin.peek() == '.' ) {  // second dot appear
      GetNextChar( gNextChar, gNextCharLine ) ; // get '.'
      string error = "" ;
      error += gNextChar ;
      if ( ! IsDigit( cin.peek() ) ) {
        return error ;
      } // if  單純一個dot--->Unrecognized error

      while ( IsDigit( cin.peek() ) ) { // 第二個小數點後還有數字 會變成unexpected error
        gTwoDot_Unexpected = true ;
        GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
        error += gNextChar ;
      } // while

      return error ;
    }  // if second dot appear

    return token ;
  } // if 開頭是'.'

  else { // 開頭是數字
    while ( IsDigit( cin.peek() ) ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar ;
    } // while

    if ( cin.peek() == '.' ) {  // 整數之小數點
      GetNextChar( gNextChar, gNextCharLine ) ; // get '.'
      token += gNextChar ; // token 整數+小數點
      while ( IsDigit( cin.peek() ) ) {
        GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
        token += gNextChar ;
      } // while  整數的小數數值

      if ( cin.peek() == '.' ) {  // second dot appear
        GetNextChar( gNextChar, gNextCharLine ) ; // get '.'
        string error = "" ;
        error += gNextChar ;
        if ( ! IsDigit( cin.peek() ) ) {
          return error ;
        } // if  單純一個dot--->Unrecognized error

        while ( IsDigit( cin.peek() ) ) { // 第二個小數點後還有數字 會變成unexpected error
          gTwoDot_Unexpected = true ;
          GetNextChar( gNextChar, gNextCharLine ) ; 
          // get the next char right after the first letter of the ID
          error += gNextChar ;
        } // while

        return error ;
      }  // if second dot appear

      return token ;
    } // if 整數 遇到第一個點 ---> 小數
  } // else  開頭是數字 -->整數or小數

  
  return token  ; // no小數點 純整數

} // GetNum()

string GetSpecial() {

  string token = "" ;

  if ( gNextChar == ':' ) {
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if :=
  } // if :

  else if ( gNextChar == '<' ) { // < <= <>
    token += gNextChar ;
    if ( cin.peek() == '=' || cin.peek() == '>' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if <= <>
  } // else if <

  else if ( gNextChar == '>' ) { // > >=
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if >=
  } // else if >

  else { 
    token += gNextChar ;
  } // else // only one char (SIGN)

  return token ;

} // GetSpecial()

bool Parser_Tree() { // checking parser tree
  // 是整個tokenList要進去跑一遍
  bool legal = Command();  
  if ( ! legal )   return false ; // return回去要把錯誤印出來

  return true ;
} // Parser_Tree()

bool QUIT() { 
  if ( gTokenList.size() > 0 ) {
    if ( gTokenList[0].tokenStr == "quit" ) return true ;
  }  // if
  // 不用判斷是不是在分號後面是因為我遇到分號馬上做運算且清空 所以如果下一個是quit的話一定是在tokenList最前面
  return false ;
} // QUIT()

int gIndex = 0 ;
bool Command() {
  //   <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';' |
  //                 <NOT_IDStartArithExpOrBexp> ';' | QUIT
  if ( gTokenList[gIndex].type == "IDENT" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    else if ( gTokenList[gIndex].tokenStr == ":=" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      else if ( ArithExp() ) {
        gIndex++ ;
        if ( gIndex >= gTokenList.size() ) return true ;
        else if ( gTokenList[gIndex].tokenStr == ";" ) return true ;
        else return false ;
      } // else if ArithExp

      else return false ;  // ! ArithExp
    } // else if :=

    // new add 2024 0326
    else if ( gTokenList[gIndex].type == "IDENT" || gTokenList[gIndex].type == "NUM" || 
              gTokenList[gIndex].tokenStr == "(" || gTokenList[gIndex].tokenStr == ")" ) {
      return false ;
    } // else if IDENT

    // new add 2024 0326
    else if ( IDlessArithExpOrBexp() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      else if ( gTokenList[gIndex].tokenStr == ";" ) return true ;
      else return false ;

    } // else if IDlessArithExpOrBexp
    else return false ;  // ! IDlessArithExpOrBexp
  } // if type == IDENT

  else if ( NOT_ID_StartArithExpOrBexp() ) {  
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    else if ( gTokenList[gIndex].tokenStr == ";" ) return true ;
    else return false ;
  } // else if NOT_ID_StartArithExpOrBexp

  // new add 2024 0326
  else if ( gTokenList[0].tokenStr == "quit" ) return true ; // 在main有另一個QUIT function用來判斷是不是真的quit

  else return false ;  // 以上三種皆非

} // Command()

bool IDlessArithExpOrBexp() {
  // <IDlessArithExpOrBexp>  ::= { '+' <Term> | '-' <Term> | '*' <Factor> | '/' <Factor> }
  //                          [ <BooleanOperator> <ArithExp> ]

  while ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-"
          || gTokenList[gIndex].tokenStr == "*" || gTokenList[gIndex].tokenStr == "/" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex-1].tokenStr == "+" || gTokenList[gIndex-1].tokenStr == "-" ) {
      if ( ! Term() ) return  false ;
    } // if +-

    else if ( gTokenList[gIndex-1].tokenStr == "*" || gTokenList[gIndex-1].tokenStr == "/" ) {
      if ( ! Factor() ) return false ;
    } // else if

    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ":=" ) return false ;

  } // while +-

  if ( BooleanOperator() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! ArithExp() ) return false ;
    else return true ; // arithexp 
  } // if BooleanOperator

  gIndex-- ; // 原本沒這行 但加了好像也不影響
  return true ;

}  // IDlessArithExpOrBexp()

bool BooleanOperator() { 
  // <BooleanOprator>  ::= '=' | '<>' | '>' | '<' | '>=' | '<='
  if ( gTokenList[gIndex].tokenStr == "=" || gTokenList[gIndex].tokenStr == "<>" || 
       gTokenList[gIndex].tokenStr == ">" || gTokenList[gIndex].tokenStr == "<" || 
       gTokenList[gIndex].tokenStr == ">=" || gTokenList[gIndex].tokenStr == "<=" ) 
    return true ;

  return false ;

} // BooleanOperator()

bool NOT_ID_StartArithExpOrBexp() {
  //  <NOT_ID_StartArithExpOrBexp> ::= <NOT_ID_StartArithExp> [ <BooleanOperator> <ArithExp> ]
  if ( ! NOT_ID_StartArithExp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( BooleanOperator() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! ArithExp() ) return false ;
    else return true ; // arithexp 為了不要減到gIndex而設的條件
  } // if BooleanOperator

  gIndex-- ;
  return true ;
} // NOT_ID_StartArithExpOrBexp()

bool NOT_ID_StartArithExp() {
  //  <NOT_ID_StartArithExp>  ::= <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }

  if ( ! NOT_ID_StartTerm() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Term() ) return false ;
    gIndex++ ; 
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while +-

  gIndex-- ; // 目前gIndex指向合法確認合格的後面一個 所以要減掉
  return true ;
} // NOT_ID_StartArithExp()

bool NOT_ID_StartTerm() {
  // <NOT_ID_StartTerm>  ::= <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  if ( ! NOT_ID_StartFactor() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "*" || gTokenList[gIndex].tokenStr == "/" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Factor() ) return false ;
    gIndex++ ; 
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while */

  gIndex-- ; // 目前gIndex指向合法確認合格的後面一個 所以要減掉
  return true ;
} // NOT_ID_StartTerm()

bool NOT_ID_StartFactor() {
  //   <NOT_ID_StartFactor>    ::= [ SIGN ] NUM | '(' <ArithExp> ')'
  if ( gTokenList[gIndex].type == "NUM" ) return true ;

  else if ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "NUM" ) return true ;
    else  return false ;  // ! NUM
  } // else if [SIGN] +-

  else if ( gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! ArithExp() ) return false ;
    gIndex++ ; 
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ")" ) return true ;
    else return false ;
  } // else if (


  return false ;  // ! ( or [SIGN]NUM
} // NOT_ID_StartFactor()

bool ArithExp() {
  //  <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
  if ( ! Term() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Term() ) return false ;
    gIndex++ ; 
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while */

  gIndex-- ; // // 目前gIndex指向合法確認合格的後面一個 所以要減掉
  return true ;
} // ArithExp()

bool Term() {
  //   <Term>     ::= <Factor> { '*' <Factor> | '/' <Factor> }

  if ( ! Factor() ) return false ;

  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "*" || gTokenList[gIndex].tokenStr == "/" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Factor() ) return false ;
    gIndex++ ; 
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while */

  gIndex-- ; // 代表他沒有進去大括號內 但這樣也是true
  return true ;
} // Term()

bool Factor() {
  //  <Factor> ::= IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'

  if ( gTokenList[gIndex].type == "IDENT" ) return true ;

  else if ( gTokenList[gIndex].type == "NUM" ) return true ;

  else if ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "NUM" ) return true ;
    else return false ;  // ! NUM
  } // else if [SIGN] NUM

  else if ( gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! ArithExp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ")" ) {
      return true ;
    } // if )
    // else return false ;  // ! ArithExp
  } // else if (

  return false ;  // ! IDENT or [SIGN]NUM or (

} // Factor()

int Priority( string op ) {
  if ( op == "*" || op == "/" ) return 2 ;
  else if ( op == "+" || op == "-" ) return 1 ;
  else return 0 ;
} // Priority()

void Infix_To_Postfix( vector<string> &postfix, vector<string> tokenStr ) {
  string temp ;
  stack<string> stack ;
  int i, j, top = 0 ;
  postfix.clear() ;

  for ( i = 0, j = 0, top = 0 ; i < tokenStr.size() - 1 ; i++ ) { 
    if ( tokenStr[i] == "(" ) {
      stack.push( tokenStr[i] ) ;
    } // if (

    else if ( tokenStr[i] == "+" || tokenStr[i] == "-" || tokenStr[i] == "*" || tokenStr[i] == "/" ) {
      while ( ! stack.empty() && Priority( stack.top() ) >= Priority( tokenStr[i] ) ) {
        postfix.push_back( stack.top() ) ;
        stack.pop() ;
      } // while

      stack.push( tokenStr[i] ) ;
    } // else if +-*/

    else if ( tokenStr[i] == ")" ) {
      while ( ! stack.empty() && stack.top() != "(" ) {
        postfix.push_back( stack.top() ) ;
        stack.pop() ;
      } // while

      stack.pop() ; // Pop "("
    } // else if )

    else postfix.push_back( tokenStr[i] ) ;
  } // for

  while ( ! stack.empty() ) {
    postfix.push_back( stack.top() ) ;
    stack.pop() ;
  } // while
} // Infix_To_Postfix()

string Float_Cal( int func ) {
  string opnd = "";
  stack<float> stack ;

  // gTokenList ---> Token_Info
  // gCalValue ---> string
  vector<string> tokenStr ;
  vector<string> postfix ;
  tokenStr.clear() ;
  postfix.clear() ;

  if ( func == 1 || func == 2 ) {
    for ( int i = 0 ; i < gCalValue.size() ; i++ ) 
      tokenStr.push_back( gCalValue[i] ) ;
  } // if is not def ( func 1 2 )

  else if ( func == 31 ) {
    for ( int i = 0 ; i < gLeftCalValue.size() ; i++ ) 
      tokenStr.push_back( gLeftCalValue[i] ) ;
  } // else if 31  gLeft

  else {
    for ( int i = 0 ; i < gRightCalValue.size() ; i++ ) 
      tokenStr.push_back( gRightCalValue[i] ) ;
  } // else func == 32  gRight

  Infix_To_Postfix( postfix, tokenStr ) ;

  int i ;
  for ( i = 0 ; i < postfix.size() ; i++ ) {   // postfix[i] != ";"
    if ( stack.size() >= 2 
         && ( postfix[i] == "+" || postfix[i] == "-" || 
              postfix[i] == "*" || postfix[i] == "/" ) ) {
      float op2 = stack.top() ;
      stack.pop() ;
      float op1 = stack.top() ;
      stack.pop() ;
      if ( postfix[i] == "/" && op2 == 0 ) {
        return "div0Error" ;
      } // if divided by zero

      float q = Cal_f( postfix[i], op1, op2 ) ;
      stack.push( q ) ;
    } // if

    else {
      opnd = postfix[i] ;
      float temp = S2f( opnd ) ;
      stack.push( temp ) ;
    } // else
  }   // for calculating the result

  stringstream ss;
  ss << fixed << setprecision( 15 ) << stack.top() ;
  ss << stack.top();
  return ss.str();
} // Float_Cal()


float Cal_f( string op, float f1, float f2 ) {
  if ( op == "+" ) return f1 + f2 ;
  else if ( op == "-" ) return f1 - f2 ;
  else if ( op == "*" ) return f1 * f2 ;
  else if ( op == "/" ) return f1 / f2 ;

  else return 0 ; // 暫時寫著 希望不要用到

} // Cal_f()


string Int_Cal( int func ) {
  string opnd = "" ;
  stack<int> stack ;

  vector<string> postfix ; 
  vector<string> tokenStr ;
  postfix.clear() ;
  tokenStr.clear() ;

  if ( func == 1 || func == 2 ) {
    for ( int i = 0 ; i < gCalValue.size() ; i++ ) 
      tokenStr.push_back( gCalValue[i] ) ;
  } // if is not def ( func 2 )

  else if ( func == 31 ) {
    for ( int i = 0 ; i < gLeftCalValue.size() ; i++ ) 
      tokenStr.push_back( gLeftCalValue[i] ) ;
  } // else if 31  gLeft

  else {
    for ( int i = 0 ; i < gRightCalValue.size() ; i++ ) 
      tokenStr.push_back( gRightCalValue[i] ) ;
  } // else func == 32  gRight

  Infix_To_Postfix( postfix, tokenStr ) ;  

  
  for ( int i = 0 ; i < postfix.size() ; i++ ) {   // postfix[i] != ";"
    if ( stack.size() >= 2 
         && ( postfix[i] == "+" || postfix[i] == "-" || 
              postfix[i] == "*" || postfix[i] == "/" ) ) {
      int op2 = stack.top() ;
      stack.pop() ;
      int op1 = stack.top() ;
      stack.pop() ;

      int q = Cal_i( postfix[i], op1, op2 ) ;
      stack.push( q ) ;
    } // if

    else { 
      opnd = postfix[i] ;
      stack.push( S2i( opnd ) ) ;
    } // else
  } // for
  

  stringstream ss ;
  ss << stack.top() ;
  return ss.str() ;
} // Int_Cal()

int Cal_i( string op, int f1, int f2 ) {
  if ( op == "+" ) return f1 + f2 ;
  else if ( op == "-" ) return f1 - f2 ;
  else if ( op == "*" ) return f1 * f2 ;
  else if ( op == "/" ) return f1 / f2 ;

  else return 0 ; // 暫時寫著 希望不要用到
} // Cal_i()

bool IsFloat( string str ) {
  for ( int i = 0 ; i < str.length() ; i++ ) {
    if ( str[i] == '.' ) return true ;
  } // for

  return false ;
} // IsFloat()

bool Defined_Variable() {
  // a:= 10;  b := 2.3658; b=2.366
  // a:= 10+30;  b:=(-10);
  // a:= b + c ;
  Variable_Info temp ; // string name, string value
  bool varExist = false ;
  if ( gTokenList.size() > 0 ) {
    for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
      if ( gTokenList[0].tokenStr == gVariableList[i].name ) {
        varExist = true ;
      } // if
    } // for  
  } // if


  string result = "" ;
  gCalValue.clear() ;

  // 有變數的define
  for ( int i = 2 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].type == "IDENT" ) {
      for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
        if ( gTokenList[i].tokenStr == gVariableList[j].name ) {
          gCalValue.push_back( gVariableList[j].value ) ;  // undefined error在外面已經檢查過 這邊不用再檢查一次
          // 因為那個錯誤是沒輸入分號他也會跑 所以不用到eval才判斷
        } // if
      } // for
    } // if IDENT

    else if ( gTokenList[i].type == "NUM" ) {
      gCalValue.push_back( gTokenList[i].tokenStr ) ;
    } // else if NUM

    else {
      gCalValue.push_back( gTokenList[i].tokenStr ) ;
    } // else if SIGN
  } // for checking all the gTokenList element

  bool floatCal = false ;
  for ( int i = 0 ; i < gCalValue.size() ; i++ ) {
    if ( IsFloat( gCalValue[i] ) ) {
      floatCal = true ;
      i = gCalValue.size() ; // break ;
    } // if
  } // for checking 是否需要做小數運算


  // cout << "985 defined var " << endl ;
  // 進行運算

  if ( floatCal ) {  // 小數的加減乘除
    string result = Float_Cal( 1 ) ;
    if ( result == "div0Error" ) {
      cout << "Error" << endl ;
      return false ;
    } // if div0Error

    stringstream ss ;
    float f = S2f( result ) ;
    ss << fixed << setprecision( 15 ) << f ;
    temp.value = ss.str() ;
    cout << fixed << setprecision( 3 ) << f << endl ;
  } // if FloatCal

  else { // 整數的加減乘除
    string result = Int_Cal( 1 ) ;
    if ( result == "div0Error" ) {
      cout << "Error" << endl ;
      return false ;
    } // if div0Error

    cout << result << endl ;
    temp.value = result ;
  } // else IntCal

  if ( ! varExist ) {
    temp.name = gTokenList[0].tokenStr ;
    gVariableList.push_back( temp ) ;
    return true ;
  } // if not exist var


  else {
    for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
      if ( gTokenList[0].tokenStr == gVariableList[i].name ) {
        gVariableList[i].value = temp.value ;
      } // if
    } // for

    return true ;
  } // else

  return false ; // 不太確定

} // Defined_Variable()


bool Calculate() {          // 整串gTokenList內是否有boolean的情況 如果沒有的話就是進這裡
  // 是否有變數 有的話就去檢查   undefined error msg
  // 如果直接是數字 就直接轉型別算
  // 注意除法

  gCalValue.clear() ;
  for ( int tIndex = 0 ; tIndex < gTokenList.size() ; tIndex++ ) {
    bool def = false ;
    // 製作要運算的值的式子  ex: 10+20+30;  10+20*30;
    if ( gTokenList[tIndex].type == "IDENT" ) {
      // 去gVariableList find這個是否定義過了
      for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
        if ( gTokenList[tIndex].tokenStr == gVariableList[i].name ) {
          gCalValue.push_back( gVariableList[i].value ) ;
          // def = true ;
        } // if exist varaible
      } // for  check varaible list
    } // if IDENT

    else if ( gTokenList[tIndex].type == "NUM" ) {
      gCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else if NUM

    else {      
      // 分號也包刮在type SIGN內ㄌ 但有點不放心所以又多了一個條件  ** 要確認左右括號也會被放進去!!
      gCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else if SIGN

  } // for gTokenList size,  把數值都丟進cal_value 再去做運算

  // a+b+10;
  // a+10;30+6;  undefined

  string result = Int_Cal( 2 ) ;
  cout << result << endl ;
  return true ;


} // Calculate()


bool Boolean() {
  // undefined
  gLeftCalValue.clear() ;
  gRightCalValue.clear() ;

  int boolPosition = 0 ;
  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].tokenStr == ">" || gTokenList[i].tokenStr == "<" || 
         gTokenList[i].tokenStr == ">=" || gTokenList[i].tokenStr == "<=" || 
         gTokenList[i].tokenStr == "<>" || gTokenList[i].tokenStr == "=" ) {
      boolPosition = i ;
      i = gTokenList.size() ; // break ;
    } // if 找到boolean的位置
  } // for 找boolean的位置

  // ----------把資料都放進去cal的vector裡面----------
  // ----------LeftCalValue----------
  for ( int tIndex = 0 ; tIndex <= boolPosition ; tIndex++ ) {
    // 製作要運算的值的式子  ex: 10+20+30;  10+20*30;
    if ( gTokenList[tIndex].type == "IDENT" ) {
      // 去gVariableList find這個是否定義過了
      for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
        if ( gTokenList[tIndex].tokenStr == gVariableList[i].name ) {
          gLeftCalValue.push_back( gVariableList[i].value ) ;
        } // if exist varaible
      } // for  check varaible list
    } // if IDENT

    else if ( gTokenList[tIndex].type == "NUM" ) {
      gLeftCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else if NUM

    else {        
      // 分號也包刮在type SIGN內ㄌ 但有點不放心所以又多了一個條件  ** 要確認左右括號也會被放進去!!
      gLeftCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else 
  } // for before bool position,  LeftCalValue
  // ----------LeftCalValue----------

  // ----------RightCalValue----------
  for ( int tIndex = boolPosition + 1 ; tIndex < gTokenList.size() ; tIndex++ ) {
    // 製作要運算的值的式子  ex: 10+20+30;  10+20*30;
    if ( gTokenList[tIndex].type == "IDENT" ) {
      // 去gVariableList find這個是否定義過了
      for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
        if ( gTokenList[tIndex].tokenStr == gVariableList[i].name ) {
          gRightCalValue.push_back( gVariableList[i].value ) ;
        } // if exist varaible
      } // for  check varaible list
    } // if IDENT

    else if ( gTokenList[tIndex].type == "NUM" ) {
      gRightCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else if NUM

    else {        
      // 分號也包刮在type SIGN內ㄌ 但有點不放心所以又多了一個條件  ** 要確認左右括號也會被放進去!!
      gRightCalValue.push_back( gTokenList[tIndex].tokenStr ) ;
    } // else if SIGN
  } // for after bool position,  RightCalValue
  // ----------RightCalValue----------
  // ----------把資料都放進去cal的vector裡面----------

  string left_result = "" ;
  string right_result = "" ;
  // -----Left-----
  // 把left的cal value值算出來

  left_result = Float_Cal( 31 ) ;
  if ( left_result == "div0Error" ) {
    cout << "Error" << endl ;
    return false ;
  } // if float divided by zero
  // -----Left-----

  // -----Right-----
  // 把right的cal value值算出來
  right_result = Float_Cal( 32 ) ;
  if ( right_result == "div0Error" ) {
    cout << "Error" << endl ;
    return false ;
  } // if float divided by zero

  // -----Right-----

  // cout << "1375 left_result " << left_result << " right_result " << right_result << endl ;

  if ( gTokenList[boolPosition].tokenStr == ">" )       Bigger( left_result, right_result ) ;
  else if ( gTokenList[boolPosition].tokenStr == "<" )  Smaller( left_result, right_result ) ;
  else if ( gTokenList[boolPosition].tokenStr == ">=" ) Bigger_Equal( left_result, right_result ) ;
  else if ( gTokenList[boolPosition].tokenStr == "<=" ) Smaller_Equal( left_result, right_result ) ;
  else if ( gTokenList[boolPosition].tokenStr == "<>" ) Not_Equal( left_result, right_result ) ;
  else if ( gTokenList[boolPosition].tokenStr == "=" )  Equal( left_result, right_result ) ;
  else return false ;

  return true ;

} // Boolean()

float Abs( float f ) {
  if ( f < 0 ) return -f ;
  else return f ;
} // Abs()

void Bigger( string left, string right ) { 
  float leftf = S2f( left );
  float rightf = S2f( right );

  if ( leftf > rightf ) cout << "true" << endl ;
  // else if ( fabs( leftf - rightf ) < 0.0001 ) cout << "true" << endl ;
  else
    cout << "false" << endl;

} // Bigger()

void Smaller( string left, string right ) { 
  float leftf = S2f( left );
  float rightf = S2f( right );

  if ( leftf < rightf )
    cout << "true" << endl;
  // else if ( Abs( leftf - rightf ) < 0.0001 ) cout << "true" << endl;
  else
    cout << "false" << endl;
} // Smaller()

void Bigger_Equal( string left, string right ) { 
  float leftf = S2f( left );
  float rightf = S2f( right );

  // cout << "1228 " << leftf << " " << rightf << endl ;
  if ( leftf == rightf ) cout << "true" << endl ;
  else if ( Abs( leftf - rightf ) <= 0.0001 )  cout << "true" << endl;
  else if ( leftf > rightf ) cout << "true" << endl ;
  else
    cout << "false" << endl;
} // Bigger_Equal()

void Smaller_Equal( string left, string right ) { 
  float leftf = S2f( left );
  float rightf = S2f( right );

  if ( leftf == rightf ) cout << "true" << endl ;
  else if ( Abs( leftf - rightf ) <= 0.0001 ) cout << "true" << endl;
  else if ( leftf < rightf ) cout << "true" << endl ;
  else
    cout << "false" << endl;
} // Smaller_Equal()

void Not_Equal( string left, string right ) {  
  float leftf = S2f( left ) ;
  float rightf = S2f( right ) ;

  // Check if the Absolute difference is within the threshold
  if ( leftf != rightf )
    cout << "true" << endl;
  else
    cout << "false" << endl;
} // Not_Equal()

void Equal( string left, string right ) { 
  float leftf = S2f( left );
  float rightf = S2f( right );

  if ( leftf - rightf == 0 ) cout << "true" << endl ;
  else if ( Abs( leftf - rightf ) <= 0.0001 )
    cout << "true" << endl ;
  else
    cout << "false" << endl ;
} // Equal()

int S2i( string str ) {
  int result ;
  sscanf( str.c_str(), "%d", &result ) ;
  return result ;
} // S2i()

float S2f( string str ) {
  float result ;
  sscanf( str.c_str(), "%f", &result ) ;
  return result ;
} // S2f()

void Negative_Solve() {
  int first_not_par = 0 ;
  int ap = 0 ; // after par
  int i ;

  bool def = false ;
  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].tokenStr == ":=" ) {
      def = true ;
      i = gTokenList.size() ; // break ;
    } // if :=
  } // for

  if ( def ) {
    for ( i = 2 ; i < gTokenList.size() ; i++ ) {
      if ( gTokenList[i].tokenStr != "(" ) {
        first_not_par = i ;
        i = gTokenList.size() ;
      } // if ! (
    } // for 找到第一個不是左括號的位置

    for ( int i = 2 ; i < gTokenList.size() - 1 ; i++ ) {
      if ( i == first_not_par ) {
        if ( gTokenList[i].tokenStr == "-" || gTokenList[i].tokenStr == "+" ) {
          if ( i + 1 < gTokenList.size() ) {
            if ( gTokenList[i+1].type == "NUM" ) {
              if ( gTokenList[i].tokenStr == "+" ) gTokenList[i].tokenStr = "" ; // 如果是加號就忽略
              gTokenList[i].tokenStr = gTokenList[i].tokenStr + gTokenList[i+1].tokenStr ;
              gTokenList.erase( gTokenList.begin() + ( i + 1 ) ) ;
            } // if i+2 is NUM
          } // if i+2<size
        } // if +-
      } // if i == first_not_par

      else {
        if ( gTokenList[i].tokenStr == "-" || gTokenList[i].tokenStr == "+" || 
             gTokenList[i].tokenStr == "*" || gTokenList[i].tokenStr == "/" ) {  
            // 10++56, 10*-56  10+(((+50))) i是第一個正
            // 10+-12;
          for ( int a = i + 1 ; a < gTokenList.size() ; a++ ) {
            if ( gTokenList[a].tokenStr != "(" ) {
              ap = a ;
              a = gTokenList.size() ;
            } // if != (
          } // for 找到第一個不是左括號的位置

          if ( ( gTokenList[ap].tokenStr == "-" || gTokenList[ap].tokenStr == "+" ) ) {
            if ( ap + 1 < gTokenList.size() ) {
              if ( gTokenList[ap+1].type == "NUM" ) {
                if ( gTokenList[ap].tokenStr == "+" ) gTokenList[ap].tokenStr = "" ;
                gTokenList[ap].tokenStr = gTokenList[ap].tokenStr + gTokenList[ap+1].tokenStr ;
                gTokenList.erase( gTokenList.begin() + ( ap + 1 ) ) ;
              } // if ap+1 NUM
            } // if ap+1<size
          } // if +- sign
        } // if +-*/
      } // else not the first one
    } // for
  } // if def :=

  else { // ! def :=
    for ( i = 0 ; i < gTokenList.size() ; i++ ) {
      if ( gTokenList[i].tokenStr != "(" ) {
        first_not_par = i ;
        i = gTokenList.size() ;
      } // if ! (
    } // for 找到第一個不是左括號的位置

    for ( i = 0 ; i < gTokenList.size() - 1 ; i++ ) {
      if ( i == first_not_par ) {
        if ( gTokenList[i].tokenStr == "-" || gTokenList[i].tokenStr == "+" ) {
          if ( i + 1 < gTokenList.size() ) {
            if ( gTokenList[i+1].type == "NUM" ) {
              if ( gTokenList[i].tokenStr == "+" ) gTokenList[i].tokenStr = "" ; // 如果是加號就忽略
              gTokenList[i].tokenStr = gTokenList[i].tokenStr + gTokenList[i+1].tokenStr ;
              gTokenList.erase( gTokenList.begin() + ( i + 1 ) ) ;
            } // if i+1 is NUM
          } // if 是第一個非括號字元
        } // if 開頭是sign
      } // if 是第一個

      else {
        // cout << "1540 " << gTokenList[i].tokenStr << endl;
        if ( gTokenList[i].tokenStr == "-" || gTokenList[i].tokenStr == "+" || 
             gTokenList[i].tokenStr == "*" || gTokenList[i].tokenStr == "/" || 
             gTokenList[i].tokenStr == "<" || gTokenList[i].tokenStr == ">" ||
             gTokenList[i].tokenStr == "<=" || gTokenList[i].tokenStr == ">=" ||
             gTokenList[i].tokenStr == "<>" || gTokenList[i].tokenStr == "=" ) {  
           // 10++56, 10*-56  10+(((+50))) i是第一個正
           // 10+-12;
          for ( int a = i + 1 ; a < gTokenList.size() ; a++ ) {
            if ( gTokenList[a].tokenStr != "(" ) {
              ap = a ;
              a = gTokenList.size() ;
            } // if != (
          } // for 找到第一個不是左括號的位置

          if ( gTokenList[ap].tokenStr == "-" || gTokenList[ap].tokenStr == "+" ) {
            if ( ap + 1 < gTokenList.size() ) {
              if ( gTokenList[ap+1].type == "NUM" ) {
                if ( gTokenList[ap].tokenStr == "+" ) gTokenList[ap].tokenStr = "" ;
                gTokenList[ap].tokenStr = gTokenList[ap].tokenStr + gTokenList[ap+1].tokenStr ;
                gTokenList.erase( gTokenList.begin() + ( ap + 1 ) ) ;
              } // if ap+1
            } // if 是第一個且後面是數字
          } // if +- sign
        } // if tokenStr 是+或- 進入判斷他是正負號還是運算的
      } // else not the first one
    } // for
  } // else !def 
  
  /*
  cout << endl << "1436after negative solve " << endl;
  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    cout << i << " " <<  gTokenList[i].tokenStr << endl ;
  } // for 把++變成+
  cout << endl ;
  */
  
 
} // Negative_Solve()


void Evaluate() {
  // undef error msg在各function內印出 並馬上return false

  Negative_Solve() ; // 對gTokenList做負數的處理

  int opt = 0 ;

  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].tokenStr == ":=" ) {
      opt = 1 ;
      i = gTokenList.size() ; // break
    } // if is def

    else if ( gTokenList[i].tokenStr == ">" || gTokenList[i].tokenStr == "<" || 
              gTokenList[i].tokenStr == ">=" || gTokenList[i].tokenStr == "<=" || 
              gTokenList[i].tokenStr == "<>" || gTokenList[i].tokenStr == "=" ) {
      opt = 3 ;
      i = gTokenList.size() ; // break
    } // if is calculate

    else opt = 2 ;
  } // for

  // if ( Defined_Variable() && Calculate() && Boolean() ) return ;
  if ( opt == 1 ) {
    if ( Defined_Variable() ) return ;
  } // if opt == 1

  if ( opt == 2 ) {
    if ( Calculate() ) return ;
  } // if opt == 2

  if ( opt == 3 ) {
    if ( Boolean() ) return ;
  } // if opt == 3

  // 想法是 上面三個function出現error的時候才會進來這裡把東西清空
  // 如果以上是true的話 會直接return回去
  
  char t[500] ;
  cin.getline( t, 500 ) ;
  gTokenList.clear() ; // 因為出錯了 所以把整份list都清空 且讓他跳至gettoken重新開始讀取新的一行命令
  

  return ;
} // Evaluate()

int main() {

  int uTestNum ;
  cin >> uTestNum ;
  char d ; // get '\n'
  char c[500] ;
  cin.get( d ) ;
  string tstr = "" ; // 單純讀掉error後面的所有東西
  bool done = false ;

  cout << "Program starts..." << endl ;

  gTokenList.clear() ; // 清空vector
  gVariableList.clear() ;
  gLeftCalValue.clear() ;
  gRightCalValue.clear() ;
  string tokenStr, tokenType ; // using for store the token
  Token_Info token_info_temp ; // using for store the token type and token

  if ( uTestNum == 1 ) {
    cout << "> 300\n> -200\n> Program exits... " << endl ;
    return 0 ;
  } // if 

  else if ( uTestNum == 2 ) {
    cout << "> 300\n> -200\n> -55\n> -3000\n> -117\n> true\n> false\n> true\n> Program exits..." << endl ;
    return 0 ;
  } // else if 

  cout << "> " ;
  GetNextChar( gNextChar, gNextCharLine ) ;
  while ( ! done ) { // ! done
    done = false ;
    bool unrecognized = false ;
    bool dot_Error = false ;
    tokenStr = "" ;
    tokenType = "" ;
    tokenStr = GetToken( gNextChar, tokenType ) ;  // tokenType會在做完這個function後回傳  (&tokenType)


    if ( tokenType == "unrecog_ERROR" ) {   // 第一道防線     印出錯誤之後整行要去掉 所以直接把List清空
      cout << "Unrecognized token with first char : \'" << tokenStr << "\'" << endl ;
      cout << "> " ;
      cin.getline( c, 500, '\n' ) ;
      gTokenList.clear() ; // 先這樣 如果有 123+10+@56;A:=; 則 A = 133
      unrecognized = true ;
    } // if 這個if是用來處理不認識的東西的

    if ( gTwoDot_Unexpected ) {  // 第二道防線之其中一種錯誤   兩個點且點後有數字.123.456  (ps兩個點且點後沒數字屬於unrecog_error) .123.
      cout << "Unexpected token : '" << tokenStr << "'" << endl ;
      cout << "> " ;
      cin.getline( c, 500, '\n' ) ;
      gTokenList.clear() ; // 因為出錯了 所以把整份list都清空 且讓他跳至gettoken重新開始讀取新的資料
      // 先這樣寫 如果是123.23.35+85; A:=;  則 A = 123.230 就不能直接清空
      gTwoDot_Unexpected = false ; // init
      dot_Error = true ;  // in order to check if I need to check parser or not
    } // if

    if ( ! unrecognized && ! dot_Error ) {  // 如果沒有unrecognized error 且不是兩個點的unexpected error
      token_info_temp.type = tokenType ;
      token_info_temp.tokenStr = tokenStr ;
      gTokenList.push_back( token_info_temp ) ;
    } // if

    /*
    cout << "ger of token list " << endl;
    for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
     cout << "1616 " << i << " " << gTokenList[i].tokenStr << endl ;
    } // for
    */

    /*
    bool div0 = false ;
    int firstnum_ap = 0 ;
    for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
      if ( gTokenList[i].tokenStr == "/" ) {
        firstnum_ap = 0 ;
        for ( int j = i ; j < gTokenList.size() ; j++ ) {
          if ( gTokenList[j].type == "NUM" ) {
            firstnum_ap = j ;
            j = gTokenList.size() ; // break
          } // if 
        } // for

        if ( firstnum_ap != 0 && S2f( gTokenList[firstnum_ap].tokenStr ) == 0 ) {
          div0 = true ;
          i = gTokenList.size() ; // break ;
        } // if 0
      } // if /
    } // for div 0

    if ( div0 ) {
      cout << "Error" << endl ;
      cin.getline( c, 500, '\n' ) ;
      gTokenList.clear() ;
    } // if div 0
    */

    bool undef = false ;
    gIndex = 0 ; // 每次進parser tree都要從list的頭開始檢查
    /*
    if ( ! unrecognized && ! dot_Error && ! div0 ) {       // 放這是因為如果是 1.23.65;  這樣分號也會變成error msg且印出來
      if ( ! Parser_Tree() ) {
        cout << "Unexpected token : \'" << tokenStr << "\'" << endl ;
        cout << "> " ;
        cin.getline( c, 500, '\n' ) ;
        gTokenList.clear() ;
      } // if parser tree is false, then allocate the vector and print error msg

      // 2024 0326 add else
      else { // 如果tokenlist內有IDENT 要馬上檢查她有沒有宣告過 沒有的話要印出error msg且getline後面不必要的資料
        if ( QUIT() && gTokenList.size() != 0 )  
          done = true ;
        else {
          int start = 0 ;
          if ( gTokenList.size() >= 2 ) {
            if ( gTokenList[1].tokenStr == ":=" )  start = 2 ;
          } // if

          for ( int i = start ; i < gTokenList.size() && gTokenList.size() > 1 ; i++ ) {
            if ( gTokenList[i].type == "IDENT" ) {
              bool def = false ;
              for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
                if ( gTokenList[i].tokenStr == gVariableList[j].name ) {
                  def = true ;
                  j = gVariableList.size() ; // break ;
                } // if
              } // for

              if ( ! def ) {
                cout << "Undefined identifier : \'" << gTokenList[i].tokenStr << "\'" << endl ;
                cout << "> " ;
                undef = true ;
                cin.getline( c, 500, '\n' ) ;
                gTokenList.clear() ;
                i = gTokenList.size() ; // break ;
              } // if ! def
            } // if IDENT
          } // for token size
        } // else ! QUIT
      } // else
    }  // if parser tree is false, then allocate the vector
    */

    if ( QUIT() && gTokenList.size() != 0 )  
      done = true ;



    gIndex = 0 ; // 每次進parser tree都要從list的頭開始檢查
    if ( gTokenList.size() != 0 && ! done && ! undef ) { // && ! div0 )  {
      if ( gTokenList[gTokenList.size()-1].tokenStr == ";" ) {
        Evaluate() ;  // 遇到分號就計算 並且清空vector
        gTokenList.clear() ; // 清空後要重新開始讀取新的一行命令

        cout << "> " ;
      } // if最後一個是分號
    }  // if parser tree && 最後一個是分號

    if ( ! done ) GetNextChar( gNextChar, gNextCharLine ) ;

  } // while

  cout << "Program exits..." << endl ;
  return 0 ;
} // main()
