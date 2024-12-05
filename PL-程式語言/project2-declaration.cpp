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
  string tokenStr ; // 存放token內容的string
} ;

struct Variable_Info {
  string name ;
  string value ;
  string array_size ;
} ; // struct Variable_Info

struct Function_Info {
  string name ;
  string type ;
  vector<Token_Info> content ;
} ; // struct Function_Info


// global variable
vector<Token_Info> gTokenList ; // 存放type+token的vector
vector<Variable_Info> gVariableList ;
vector<Function_Info> gFunctionList ;
vector<Variable_Info> gLocalVariableList ; // 存大括號內的區域變數的vector


char gNextChar = '\0' ;
int gNextCharLine = 1 ;
bool gTwoDot_Unexpected = false ;

// 用於判斷是不是整個if,while,do都做完了 給main判斷是否要do_things
bool g_if_statement = false ;
bool g_else_statement = false ;
bool g_while_statement = false ;
bool g_do_statement = false ;
bool g_if_while_undef = false ;
bool g_function_def = false ;
int g_if_num = 0 ;
int g_else_num = 0 ;
int g_while_num = 0 ;

int gNum = 0 ; // uTestnum debug用


// function name ;
// void
void GetNextChar( char & ch, int & line ) ;
void Skip_White_Space() ;
void SkipLineComment() ;

// is what? bool
bool IsLetter( char c ) ;
bool IsDigit( char c ) ;
bool IsOthers_and_Sign( char c ) ;
bool IsWhiteSpace( char c ) ;
bool IsDefineToken( string str ) ;
bool DONE() ; // 判斷quit是不是在最前面 並且更改type

// Evaluate
void Do_Things( bool big_bracket, bool if_while, bool function_read_done ) ;
void Function_PrettyPrint( Function_Info f ) ;

// string
string GetID() ;
string GetNum() ;
string GetSpecial() ;
string GetConstant() ;
string GetToken( char first_c, string &tokenType ) ;

bool gFirst = true ;
bool gLinereturn = false ;
void GetNextChar( char & ch, int & line ) {
  if ( gFirst ) {
    line = 1 ;
    gFirst = false ;
  } // if

  // cin >> ch ; // get下一個非空字元
  cin.get( ch ) ; // 空白 換行會被讀進去

  if ( gLinereturn ) {
    gNextCharLine++ ;
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

  if  ( slashIsDiv ) {
    tokenStr += gNextChar ;
    tokenType = "SIGN" ; // '/'
  } // if Is divide

  else if ( gNextChar == '\"' || gNextChar == '\'' ) {
    tokenStr = GetConstant() ;
    if ( tokenStr == "\'" ) tokenType = "unrecog_ERROR" ;
    else tokenType = "Constant" ;
  } // else if " or '開頭

  else if  ( IsLetter( gNextChar ) ) {
    tokenStr = GetID() ;
    // if ( tokenStr == "ListAllVariables" )      tokenType = "ListAllVariables";
    // else if ( tokenStr == "ListVariable" )     tokenType = "ListVariable";
    // else if ( tokenStr == "ListAllFunctions" ) tokenType = "ListAllFunctions";
    // else if ( tokenStr == "ListFunction" )     tokenType = "ListFunction";
    // else if ( tokenStr == "cin" )              tokenType = "CIN";
    // else if ( tokenStr == "cout" )             tokenType = "COUT";
    // else if ( tokenStr == "Done" )             tokenType = "DONE";
    if ( tokenStr == "int" )              tokenType = "INT";
    else if ( tokenStr == "float" )            tokenType = "FLOAT";
    else if ( tokenStr == "char" )             tokenType = "CHAR";
    else if ( tokenStr == "bool" )             tokenType = "BOOL";
    else if ( tokenStr == "string" )           tokenType = "STRING";
    else if ( tokenStr == "void" )             tokenType = "VOID";
    else if ( tokenStr == "if" )               tokenType = "IF";
    else if ( tokenStr == "else" )             tokenType = "ELSE";
    else if ( tokenStr == "while" )            tokenType = "WHILE";
    else if ( tokenStr == "do" )               tokenType = "DO";
    else if ( tokenStr == "return" )           tokenType = "RETURN";
    else tokenType = "IDENT" ;
  } // else if Letter or Underline

  else if  ( IsDigit( gNextChar ) || gNextChar == '.' ) {
    tokenType = "Constant"  ;
    tokenStr = GetNum()  ;   // 是不是點開頭是進去才判斷

    if ( tokenStr == "." ) tokenType = "unrecog_ERROR" ;
  } // else if Digit

  else if ( IsOthers_and_Sign( gNextChar ) ) {
    tokenStr = GetSpecial() ;

    if ( tokenStr.size() == 1 ) tokenType = "SIGN" ;
    else if ( tokenStr == ">=" ) tokenType = "GE" ;
    else if ( tokenStr == "<=" ) tokenType = "LE" ;
    else if ( tokenStr == "==" ) tokenType = "EQ" ;
    else if ( tokenStr == "!=" ) tokenType = "NEQ" ;
    else if ( tokenStr == "&&" ) tokenType = "AND" ;
    else if ( tokenStr == "||" ) tokenType = "OR" ;
    else if ( tokenStr == "+=" ) tokenType = "PE" ;
    else if ( tokenStr == "-=" ) tokenType = "ME" ;
    else if ( tokenStr == "*=" ) tokenType = "TE" ;
    else if ( tokenStr == "/=" ) tokenType = "DE" ;
    else if ( tokenStr == "%=" ) tokenType = "RE" ;
    else if ( tokenStr == "++" ) tokenType = "PP" ;
    else if ( tokenStr == "--" ) tokenType = "MM" ;
    else if ( tokenStr == "<<" ) tokenType = "LS" ;
    else if ( tokenStr == ">>" ) tokenType = "RS" ;
    else     tokenType = "others" ; // more than one char
  } // else if others and sign

  else { // 甚麼都不是 error
    tokenType = "unrecog_ERROR"  ;
    tokenStr += gNextChar  ;
  } // else --- unrecognized token with first char

  return tokenStr ;
} // GetToken()

// 跳過空白
void Skip_White_Space() {
  while ( IsWhiteSpace( cin.peek() ) )  {
    GetNextChar( gNextChar, gNextCharLine ) ;
  } // while

  GetNextChar( gNextChar, gNextCharLine ) ;  // read char after space
} // Skip_White_Space()
// 跳過註解
void SkipLineComment() {
  while ( cin.peek() != '\n' ) {
    GetNextChar( gNextChar, gNextCharLine ) ;
  } // while

  GetNextChar( gNextChar, gNextCharLine ) ;  // read \n
  GetNextChar( gNextChar, gNextCharLine ) ;  // read char after \n
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
       c == '*' || c == '/' || c == '.' || c == '(' || c == ')' || c == '[' || c == ']' || c == '{' ||
       c == '}' || c == '%' || c == '^' || c == '!' || c == '&' || c == '|' || c == '?' || c == ',' ||
       c == '\'' || c == '\"' )
    return true ;

  return false ;
} // IsOthers_and_Sign()

bool IsDefineToken( string str ) {
  if ( str == "int" || str == "char" || str == "float" || str == "bool" || str == "void" || str == "string" )
    return true ;
  return false ;
} // IsDefineToken()

string GetConstant() {
  string token = "" ;
  token += gNextChar ; // " or '
  char c ;
  if ( gNextChar == '\'' ) {
    cin.get( c ) ; // get next char
    token += c ;
    if ( cin.peek() == '\'' ) {
      cin.get( c ) ; // get next char
      token += c ;   
      return token ; 
    } // if 

    else {
      token = "" ;
      token += "\'" ;
      return token ;
    } // else
  } // if

  while ( cin.peek() != '\"' ) { // check if is the second " or '
    cin.get( c ) ; // 不用get next char 因為也要把空白的東西讀進去
    token += c ;
  } // while

  cin.get( c ) ; // get second " or '
  token += c ;

  return token ;
} // GetConstant()

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

  if ( gNextChar == '<' ) { // < <= <<
    token += gNextChar ;
    if ( cin.peek() == '=' || cin.peek() == '<' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if <= <<
  } // if <

  else if ( gNextChar == '>' ) { // > >= >>
    token += gNextChar ;
    if ( cin.peek() == '=' || cin.peek() == '>' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if >= >>
  } // else if >

  else if ( gNextChar == '=' ) { // ==
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if ==
  } // else if =

  else if ( gNextChar == '!' ) { // !=
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if !=
  } // else if !

  else if ( gNextChar == '&' ) { // &&
    token += gNextChar ;
    if ( cin.peek() == '&' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if &&
  } // else if &

  else if ( gNextChar == '|' ) { // ||
    token += gNextChar ;
    if ( cin.peek() == '|' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if ||
  } // else if |

  else if ( gNextChar == '+' ) { // ++ +=
    token += gNextChar ;
    if ( cin.peek() == '=' || cin.peek() == '+' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if ++ +=
  } // else if +

  else if ( gNextChar == '-' ) { // -- -=
    token += gNextChar ;
    if ( cin.peek() == '=' || cin.peek() == '-' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if -= --
  } // else if -

  else if ( gNextChar == '*' ) { // *=
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if *=
  } // else if *

  else if ( gNextChar == '/' ) { // /=
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if /=
  } // else if /

  else if ( gNextChar == '%' ) { // %=
    token += gNextChar ;
    if ( cin.peek() == '=' ) {
      GetNextChar( gNextChar, gNextCharLine ) ; // get the next char right after the first letter of the ID
      token += gNextChar  ;
    } // if %=
  } // else if %

  else { 
    token += gNextChar ;
  } // else // only one char (SIGN)

  return token ;
} // GetSpecial()

bool DONE() {
  if ( gTokenList.size() >= 4 && 
       ( gTokenList[0].tokenStr == "Done" && gTokenList[1].tokenStr == "(" &&
         gTokenList[2].tokenStr == ")" && gTokenList[3].tokenStr == ";" ) ) {
    return true ;
  } // if

  return false ;
} // DONE()

// parser tree
bool User_Input() ;
bool Definition() ;
bool Type_Specifier() ;
bool Function_Definition_Or_Declarators() ;
bool Rest_Of_Declarators() ;
bool Function_Definition_Without_ID() ;
bool Formal_Parameter_List() ;
bool Compound_Statement() ;
bool Declaration() ;
bool Statement() ;
bool Expression() ;
bool Basic_Expression() ;
bool Rest_Of_Identifier_Started_Basic_Exp() ;
bool Rest_Of_PPMM_Identifier_Started_Basic_Exp() ;
bool Sign() ;
bool Actual_Parameter_List() ;
bool Assignment_Operator() ;
bool Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ;
bool Rest_Of_Maybe_Logical_OR_Exp() ;
bool Maybe_Logical_AND_Exp() ;
bool Rest_Of_Maybe_Logical_AND_Exp() ;
bool Maybe_Bit_OR_Exp() ;
bool Rest_Of_Maybe_Bit_OR_Exp() ;
bool Maybe_Bit_Ex_OR_Exp() ;
bool Rest_Of_Maybe_Bit_Ex_OR_Exp() ;
bool Maybe_Bit_AND_Exp() ;
bool Rest_Of_Maybe_Bit_AND_Exp() ;
bool Maybe_Equality_Exp() ;
bool Rest_Of_Maybe_Equality_Exp() ;
bool Maybe_Relational_Exp() ;
bool Rest_Of_Maybe_Relational_Exp() ;
bool Maybe_Shift_Exp() ;
bool Rest_Of_Maybe_Shift_Exp() ;
bool Maybe_Additive_Exp() ;
bool Rest_Of_Maybe_Additive_Exp() ;
bool Maybe_Mult_Exp() ;
bool Rest_Of_Maybe_Mult_Exp() ;
bool Unary_Exp() ;
bool Signed_Unary_Exp() ;
bool Unsigned_Unary_Exp() ;

bool If_Before_While() {
  int first_if = -1 ;
  int first_while = -1 ;
  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].tokenStr == "if" ) {
      first_if = i ;
      i = gTokenList.size() ; // break
    } // if
  } // for

  for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
    if ( gTokenList[i].tokenStr == "while" ) {
      first_while = i ;
      i = gTokenList.size() ; // break
    } // if
  } // for

  if ( first_if < first_while && first_if != -1 ) return true ; // if包while---gwhile is false
  return false ; // while包if or根本沒有if在while內

} // If_Before_While()

int gIndex = 0 ;
// parser tree
bool User_Input() {
  // : ( definition | statement ) { definition | statement }
  if ( Statement() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    while ( Definition() || Statement() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // while

    return true ;
  } // if Definition

  else if ( Definition() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    while ( Definition() || Statement() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // while

    return true ;
  } // if Definition or Statement

  return false ;
} // User_Input()

bool Definition() {
  // :  VOID Identifier function_definition_without_ID 
  //  | type_specifier Identifier function_definition_or_declarators

  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "void" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "IDENT" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( Function_Definition_Without_ID() ) return true ;
      else return false ; // ! Function_Definition_Without_ID
    } // if type is IDENT

    else return false ; // ! IDENT
  } // if token string == void

  else if ( Type_Specifier() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "IDENT" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( Function_Definition_Or_Declarators() ) return true ;
      else return false ; // ! Function_Definition_Or_Declarators
    } // if type is IDENT

    else return false ; // ! IDENT
  } // else if Type_Specifier

  return false ;
} // Definition()

bool Type_Specifier() {
  // : INT | CHAR | FLOAT | STRING | BOOL
  if ( gIndex < gTokenList.size() && 
       ( gTokenList[gIndex].tokenStr == "int" || gTokenList[gIndex].tokenStr == "float" ||
         gTokenList[gIndex].tokenStr == "char" || gTokenList[gIndex].tokenStr == "bool" || 
         gTokenList[gIndex].tokenStr == "string" ) ) {
    return true ;
  } // if

  return false ;
} // Type_Specifier()

bool Function_Definition_Or_Declarators() {
  // : function_definition_without_ID | rest_of_declarators
  if ( Function_Definition_Without_ID() ) return true ;
  else if ( Rest_Of_Declarators() ) return true ;
  return false ; // else
} // Function_Definition_Or_Declarators()

bool Rest_Of_Declarators() {
  // : [ '[' Constant ']' ] 
  //    { ',' Identifier [ '[' Constant ']' ] } ';'
  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "[" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type != "Constant" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // if [

  while ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "," ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type != "IDENT" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == "[" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].type != "Constant" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if [
  } // while

  if ( gIndex < gTokenList.size() && 
       gTokenList[gIndex].tokenStr == ";" && gTokenList[gIndex-1].tokenStr != ")" ) {
    return true ;
  } // if  ;

  return false ; // ! ;
} // Rest_Of_Declarators()

bool Function_Definition_Without_ID() {
  // : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    g_function_def = true ;
    if ( gTokenList[gIndex].tokenStr == "void" || Formal_Parameter_List() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if void || formal_parameter_list

    if ( gTokenList[gIndex].tokenStr == ")" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( Compound_Statement() ) return true ;
      else return false ; // ! Compound_Statement
    } // if )

    else return false ; // ! )
  } // if (

  return false ; // ! (
} // Function_Definition_Without_ID()

bool Formal_Parameter_List() {
  // : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  //    { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  if ( ! Type_Specifier() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( gTokenList[gIndex].tokenStr == "&" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // if &

  if ( gTokenList[gIndex].type != "IDENT" ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;

  if ( gTokenList[gIndex].tokenStr == "[" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type != "Constant" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // if [

  while ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "," ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Type_Specifier() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == "&" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if &

    if ( gTokenList[gIndex].type != "IDENT" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;

    if ( gTokenList[gIndex].tokenStr == "[" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].type != "Constant" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if [
  } // while

  gIndex-- ; // not sure 是否要減一 照理來說應該要減 
  // 沒減的gIndex是指到下一個非,的東西 a,c 是指到c 但確定a,是合乎文法 所以要減一 使其指在,的位置 進下一個func才能gIndex++去作c的檢查
  return true ; // not sure is true or false
} // Formal_Parameter_List()

bool Compound_Statement() {
  // : '{' { declaration | statement } '}'
  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr != "{" )  return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( Declaration() || Statement() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "}" ) {
    return true ;
  } // if  '}'

  return false ;
} // Compound_Statement()

bool Declaration() {
  // : type_specifier Identifier rest_of_declarators
  if ( ! Type_Specifier() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( gTokenList[gIndex].type != "IDENT" ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( ! Rest_Of_Declarators() ) return false ;
  return true ;
} // Declaration()

bool Statement() {
  //    : ';'     // the null statement
  //  | expression ';'  /* expression here should not be empty */
  //  | RETURN [ expression ] ';'
  //  | compound_statement
  //  | IF '(' expression ')' statement [ ELSE statement ]
  //  | WHILE '(' expression ')' statement
  //  | DO statement WHILE '(' expression ')' ';'

  if ( gTokenList.size() == 1 && gTokenList[0].tokenStr == ";" )  return true ;

  else if ( Expression() ) { // expression ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ";" ) return true ;
    else return false ;
  } // else if expression 
  // 這裡應該要return true 

  else if ( gIndex >= 0 && gTokenList[gIndex].tokenStr == "return" ) { // return [ expression ] ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( Expression() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr == ";" ) return true ;
      else return false ; // ! ;
    } // if expression

    else if ( gIndex >= 0 && gTokenList[gIndex].tokenStr == ";" ) return true ; // 沒有進[ expression ]的話

    else return false ; // else
  } // else if return

  else if ( Compound_Statement() ) return true ;

  else if ( gTokenList[gIndex].tokenStr == "if" && 
            ( gTokenList.size() > 0 && gTokenList[0].type != "Constant" ) ) { 
    // if (exp) statement [else statement]
    g_if_num++ ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "(" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Statement() ) return false ;
    // else if ( ! ( gIndex >= gTokenList.size() ) ) g_if_statement = true ; // 0505 1238增加 未測
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == "else" ) { // else statement
      g_else_num++ ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( Statement() ) {
        // 以下那行之條件是為了不要讓他在還沒做完的時候就return true
        // 要確保他是整個statement都做完了才return true

        if ( g_if_num == g_else_num && ! ( gIndex >= gTokenList.size() ) )
          g_else_statement = true ;
        return true ;
      } // if 

      else return false ;
    } // if tokenStr == "else"

    else { // if tokenStr != "else"&&未進入中括號
      gIndex-- ; // 沒有進入else statement 其gIndex應該指在if (exp) statement的最後一個token 這行結束之後gIndex指在預計是文法else的前一個
      if ( ! ( gIndex >= gTokenList.size() ) ) g_if_statement = true ; // 0505 1238增加 未測
    } // else

    return true ;
  } // else if if

  else if ( gTokenList[gIndex].tokenStr == "while" && 
            ( gTokenList.size() > 0 && gTokenList[0].type != "Constant" ) )   { // while '('exp')' statement
    g_while_num++ ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "(" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( Statement() ) {
      if ( ! ( gIndex >= gTokenList.size() ) && ! If_Before_While() ) 
        g_while_statement = true ;  // ! if before while??
      return true ;
    } // if

    else return false ;
  } // else if while

  // do statement while '('exp')' ';'
  else if ( gTokenList[gIndex].tokenStr == "do" && 
            ( gTokenList.size() > 0 && gTokenList[0].type != "Constant" ) ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Statement() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "while" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "(" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ";" ) {
      if ( gTokenList.size() > 0 && gTokenList[0].tokenStr == "do" )
        g_do_statement = true ; 
      return true ;
    } // if
  } // else if do

  return false ; // else
} // Statement()

bool Expression() {
  // : basic_expression { ',' basic_expression }
  if ( ! Basic_Expression() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "," ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Basic_Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; 
  // 0502 2031拿掉註解

  return true ;
} // Expression()

bool Basic_Expression() {
  // : Identifier rest_of_Identifier_started_basic_exp
  //  | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  //  | sign { sign } signed_unary_exp romce_and_romloe
  //  | ( Constant | '(' expression ')' ) romce_and_romloe

  if ( gIndex < gTokenList.size() && gTokenList[gIndex].type == "IDENT" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Rest_Of_Identifier_Started_Basic_Exp() ) return false ;
    return true ;
  } // if IDENT

  else if ( gIndex < gTokenList.size() && 
            ( gTokenList[gIndex].tokenStr == "++" || gTokenList[gIndex].tokenStr == "--" ) ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "IDENT" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Rest_Of_PPMM_Identifier_Started_Basic_Exp() ) return false ;
      return true ;
    } // if IDENT

    return false ; // ! IDNET
  } // else if PP or MM

  else if ( Sign() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    while ( Sign() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // while

    if ( ! Signed_Unary_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ) return false ;
    return true ;
  } // else if sign

  // ( Constant | '(' expression ')' ) romce_and_romloe
  else if ( gIndex < gTokenList.size() && 
            ( gTokenList[gIndex].type == "Constant" || gTokenList[gIndex].tokenStr == "(" ) ) {
    if ( gTokenList[gIndex].type == "Constant" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if Constant

    else if ( gTokenList[gIndex].tokenStr == "(" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Expression() ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ; 
      if ( gTokenList[gIndex].tokenStr != ")" ) return false ; 
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // else if (

    else return false ; // ! Constant or (

    if ( Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ) return true ;
  } // else if Constant or (

  return false ;
} // Basic_Expression()

bool Rest_Of_Identifier_Started_Basic_Exp() {
  // : [ '[' expression ']' ] ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
  //  | '(' [ actual_parameter_list ] ')' romce_and_romloe
  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( Actual_Parameter_List() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if Actual_Parameter_List

    if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
    gIndex++ ;
    int temp = gIndex ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ) return true ;
    else { // 0508
      return false ;
    } // else
  } // if (

  else {
    if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "[" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Expression() ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // if [

    if ( Assignment_Operator() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Basic_Expression() ) return false ;
      return true ;
    } // if Assignment_Operator

    else if ( gIndex < gTokenList.size() && 
              ( gTokenList[gIndex].tokenStr == "++" || gTokenList[gIndex].tokenStr == "--" ) ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // else if PP or MM

    if ( Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ) return true ;
    else return false ;

  } // else 

  return false ;
} // Rest_Of_Identifier_Started_Basic_Exp()

bool Rest_Of_PPMM_Identifier_Started_Basic_Exp() {
  // : [ '[' expression ']' ] romce_and_romloe 
  if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "[" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // if [

  if ( ! Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() ) return false ;
  return true ;

} // Rest_Of_PPMM_Identifier_Started_Basic_Exp()

bool Sign() {
  // : '+' | '-' | '!' 
  if ( gIndex < gTokenList.size() && 
       ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" || 
         gTokenList[gIndex].tokenStr == "!" ) ) {
    return true ;
  } // if

  return false ;
} // Sign()

bool Actual_Parameter_List() {
  // : basic_expression { ',' basic_expression }
  if ( ! Basic_Expression() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "," ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Basic_Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ;  // 沒有進入大括號
  return true ;
} // Actual_Parameter_List()

bool Assignment_Operator() {
  // : '=' | TE*= | DE/= | RE%= | PE+= | ME-=
  if ( gIndex < gTokenList.size() && 
       ( gTokenList[gIndex].tokenStr == "=" || gTokenList[gIndex].tokenStr == "+=" ||
         gTokenList[gIndex].tokenStr == "-=" || gTokenList[gIndex].tokenStr == "*=" || 
         gTokenList[gIndex].tokenStr == "/=" || gTokenList[gIndex].tokenStr == "%=" ) ) {
    return true ;
  } // if

  return false ;
} // Assignment_Operator()

bool Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp() {
  // : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
  if ( ! Rest_Of_Maybe_Logical_OR_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( gTokenList[gIndex].tokenStr == "?" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Basic_Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != ":" ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Basic_Expression() ) return false ;
    return true ;
  } // if ?

  else gIndex-- ;  // 沒有進入中括號
  return true ; 
} // Rest_Of_Maybe_Conditional_Exp_And_Rest_Of_Maybe_Logical_OR_Exp()

bool Rest_Of_Maybe_Logical_OR_Exp() {
  // : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  if ( ! Rest_Of_Maybe_Logical_AND_Exp() ) return false ;
  gIndex++ ; 
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "||" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Logical_AND_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; 
  return true ; 
} // Rest_Of_Maybe_Logical_OR_Exp()

bool Maybe_Logical_AND_Exp() {
  // : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  if ( ! Maybe_Bit_OR_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "&&" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_OR_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 0502 1648註解掉了 不知道這樣對不對 hello=20*((x-hello)/34.5-17-(hello-x));
  return true ;

} // Maybe_Logical_AND_Exp()

bool Rest_Of_Maybe_Logical_AND_Exp() {
  // : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  if ( ! Rest_Of_Maybe_Bit_OR_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "&&" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_OR_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 0501 2150註解掉了
  // 0502 2036 拿掉註解為解決hello=20*....問題
  return true ;

} // Rest_Of_Maybe_Logical_AND_Exp()

bool Maybe_Bit_OR_Exp() {
  // : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  if ( ! Maybe_Bit_Ex_OR_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "|" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_Ex_OR_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 0501 1014註解掉了 不知道這樣對不對 str="this is a fine day"
  return true ;

} // Maybe_Bit_OR_Exp()

bool Rest_Of_Maybe_Bit_OR_Exp() {
  // : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  if ( ! Rest_Of_Maybe_Bit_Ex_OR_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "|" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_Ex_OR_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 0502 1656 註解掉了 不知道這樣對不對 hello=20*((x-hello)/34.5-17-(hello-x));
  return true ;
} // Rest_Of_Maybe_Bit_OR_Exp()

bool Maybe_Bit_Ex_OR_Exp() {
  // : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  if ( ! Maybe_Bit_AND_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "^" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_AND_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Maybe_Bit_Ex_OR_Exp()

bool Rest_Of_Maybe_Bit_Ex_OR_Exp() {
  // : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  if ( ! Rest_Of_Maybe_Bit_AND_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "^" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Bit_AND_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Bit_Ex_OR_Exp()

bool Maybe_Bit_AND_Exp() {
  // : maybe_equality_exp { '&' maybe_equality_exp }
  if ( ! Maybe_Equality_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "&" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Equality_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;

} // Maybe_Bit_AND_Exp()

bool Rest_Of_Maybe_Bit_AND_Exp() {
  // : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  if ( ! Rest_Of_Maybe_Equality_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "&" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Equality_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Bit_AND_Exp()

bool Maybe_Equality_Exp() {
  // : maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
  if ( ! Maybe_Relational_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "==" || gTokenList[gIndex].tokenStr == "!=" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Relational_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Maybe_Equality_Exp()

bool Rest_Of_Maybe_Equality_Exp() {
  // : rest_of_maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
  if ( ! Rest_Of_Maybe_Relational_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "==" || gTokenList[gIndex].tokenStr == "!=" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Relational_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Equality_Exp()

bool Maybe_Relational_Exp() {
  // : maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  if ( ! Maybe_Shift_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "<" || gTokenList[gIndex].tokenStr == ">" ||
          gTokenList[gIndex].tokenStr == "<=" || gTokenList[gIndex].tokenStr == ">=" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Shift_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Maybe_Relational_Exp()

bool Rest_Of_Maybe_Relational_Exp() {
  // : rest_of_maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  if ( ! Rest_Of_Maybe_Shift_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "<" || gTokenList[gIndex].tokenStr == ">" ||
          gTokenList[gIndex].tokenStr == "<=" || gTokenList[gIndex].tokenStr == ">=" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Shift_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Relational_Exp()

bool Maybe_Shift_Exp() {
  // : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  if ( ! Maybe_Additive_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "<<" || gTokenList[gIndex].tokenStr == ">>" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Additive_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Maybe_Shift_Exp()

bool Rest_Of_Maybe_Shift_Exp() {
  // : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  if ( ! Rest_Of_Maybe_Additive_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "<<" || gTokenList[gIndex].tokenStr == ">>" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Additive_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Shift_Exp()

bool Maybe_Additive_Exp() {
  // : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  if ( ! Maybe_Mult_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Mult_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Maybe_Additive_Exp()

bool Rest_Of_Maybe_Additive_Exp() {
  // : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  if ( ! Rest_Of_Maybe_Mult_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  while ( gTokenList[gIndex].tokenStr == "+" || gTokenList[gIndex].tokenStr == "-" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Maybe_Mult_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 目前指向合法的東西之後面那個位置 所以要減掉
  return true ;
} // Rest_Of_Maybe_Additive_Exp()

bool Maybe_Mult_Exp() {
  // : unary_exp rest_of_maybe_mult_exp
  if ( ! Unary_Exp() ) return false ;
  gIndex++ ;
  if ( gIndex >= gTokenList.size() ) return true ;
  if ( ! Rest_Of_Maybe_Mult_Exp() ) return false ;
  return true ;
} // Maybe_Mult_Exp()

bool Rest_Of_Maybe_Mult_Exp() {
  // : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */
  while ( gIndex < gTokenList.size() && 
          ( gTokenList[gIndex].tokenStr == "*" || 
            gTokenList[gIndex].tokenStr == "/" || gTokenList[gIndex].tokenStr == "%" ) ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Unary_Exp() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
  } // while

  gIndex-- ; // 沒有進入大括號內 所以要返回到上一個合法的位置
  // not sure
  return true ;

} // Rest_Of_Maybe_Mult_Exp()

bool Unary_Exp() {
  // : sign { sign } signed_unary_exp
  //  | unsigned_unary_exp
  //  | ( PP | MM ) Identifier [ '[' expression ']' ]

  if ( Sign() ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    while ( Sign() ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
    } // while

    if ( ! Signed_Unary_Exp() ) return false ;
    return true ;
  } // if sign

  else if ( Unsigned_Unary_Exp() ) return true ; // 這有點怪 會導致往下跑 但應該在這要return true

  else if ( gIndex < gTokenList.size() && 
            ( gTokenList[gIndex].tokenStr == "++" || gTokenList[gIndex].tokenStr == "--" ) ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].type == "IDENT" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr == "[" ) {
        gIndex++ ;
        if ( gIndex >= gTokenList.size() ) return true ;
        if ( ! Expression() ) return false ;
        gIndex++ ;
        if ( gIndex >= gTokenList.size() ) return true ;
        if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
        return true ;
      } // if [

      else gIndex-- ; // 沒有進入中刮號內 所以要返回到上一個合法的位置 也就是IDENT的位置
      return true ; // 沒有進入中刮號內也是true
    } // if IDENT

    return false ; // ! IDENT
  } // else if PP or MM

  return false ; // else
} // Unary_Exp()

bool Signed_Unary_Exp() {
  // : Identifier [ '(' [ actual_parameter_list ] ')' 
  //                 |
  //                 '[' expression ']'
  //               ]
  //  | Constant 
  //  | '(' expression ')'

  if ( gIndex < gTokenList.size() && gTokenList[gIndex].type == "IDENT" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == "(" ) { // '(' [actual_parameter_list] ')'
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Actual_Parameter_List() ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
      return true ;
    } // if (

    else if ( gTokenList[gIndex].tokenStr == "[" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Expression() ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
      return true ;
    } // else if [

    else gIndex-- ; // 沒有進入中括號內 所以要返回到上一個合法的位置 也就是IDENT的位置
    return true ;
  } // if IDENT

  else if ( gIndex < gTokenList.size() && gTokenList[gIndex].type == "Constant" )
    return true ;

  else if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
    return true ;
  } // else if (

  return false ;
} // Signed_Unary_Exp()

bool Unsigned_Unary_Exp() {
  // : Identifier [ '(' [ actual_parameter_list ] ')' 
  //                 |
  //                 [ '[' expression ']' ] [ ( PP | MM ) ]
  //               ]
  //  | Constant 
  //  | '(' expression ')'

  if ( gIndex < gTokenList.size() && gTokenList[gIndex].type == "IDENT" ) {
    // if ( gTokenList[gIndex].tokenStr == "while" || ) 0510
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == "(" ) {
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Actual_Parameter_List() && gTokenList[gIndex].tokenStr != ")" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != ")" ) return false ;
      return true ;
    } // if (

    else if ( gTokenList[gIndex].tokenStr == "[" ) { // [exp]+[++--]
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( ! Expression() ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr != "]" ) return false ;
      gIndex++ ;
      if ( gIndex >= gTokenList.size() ) return true ;
      if ( gTokenList[gIndex].tokenStr == "++" || gTokenList[gIndex].tokenStr == "--" ) {
        return true ;
      } // if ++ or --

      else gIndex-- ; // 因為有進[exp]但沒有進++-- 所以要減掉 回到]的位置
      return true ; // else it's not ++ or --
    } // else if [

    else if ( gTokenList[gIndex].tokenStr == "++" || gTokenList[gIndex].tokenStr == "--" ) { // [++--]
      return true ;
    } // else if ++ or -- (沒有進[exp] 但有進++--)

    gIndex-- ; // IDENT後沒有進入中括號內 所以要返回到上一個合法的位置 也就是IDENT的位置
    return true ;
  } // if IDENT

  else if ( gIndex < gTokenList.size() && gTokenList[gIndex].type == "Constant" )
    return true ;

  else if ( gIndex < gTokenList.size() && gTokenList[gIndex].tokenStr == "(" ) {
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( ! Expression() ) return false ;
    gIndex++ ;
    if ( gIndex >= gTokenList.size() ) return true ;
    if ( gTokenList[gIndex].tokenStr == ")" ) return true ;
    else return false ;
  } // else if (

  return false ;
} // Unsigned_Unary_Exp()

bool CompareNameV( Variable_Info a, Variable_Info b ) {
  return a.name < b.name ;
} // CompareNameV()

bool CompareNameF( Function_Info a, Function_Info b ) {
  return a.name < b.name ;
} // CompareNameF()

void BubbleSortV( vector<Variable_Info>& varList ) {
  bool swapped ;
  for ( int i = 0 ; i < varList.size() - 1 ; i++ ) {
    swapped = false ;
    for ( int j = 0 ; j < varList.size() - i - 1 ; j++ ) {
      if ( ! CompareNameV( varList[j], varList[j + 1] ) ) {
        Variable_Info temp = varList[j] ;
        varList[j] = varList[j + 1] ;
        varList[j + 1] = temp ;
        swapped = true ;
      } // if
    } // for

    if ( ! swapped ) 
      i = varList.size() ; // break ;
  } // for
} // BubbleSortV()

void BubbleSortF( vector<Function_Info>& funcList ) {
  bool swapped ;
  for ( int i = 0 ; i < funcList.size() - 1 ; i++ ) {
    swapped = false ;
    for ( int j = 0 ; j < funcList.size() - i - 1 ; j++ ) {
      if ( ! CompareNameF( funcList[j], funcList[j + 1] ) ) {
        Function_Info temp = funcList[j] ;
        funcList[j] = funcList[j + 1] ;
        funcList[j + 1] = temp ;
        swapped = true ;
      } // if
    } // for

    if ( ! swapped ) 
      i = funcList.size() ; // break ;
  } // for
} // BubbleSortF()

bool IsFunction( string s ) ;

void Function_PrettyPrint( Function_Info f ) {
  // ====================第一行之宣告始=====================
  int i = 0 ;
  int space = 0 ;
  cout << f.type << " " << f.name ;
  // ====================第一行之宣告末=====================
  for ( int n = 0 ; n < f.content.size() - 1 ; n++ ) {
    cout << f.content[n].tokenStr ;

    if ( f.content[n].tokenStr != "{" && f.content[n].tokenStr != "}" &&
         f.content[n].tokenStr != ";" && f.content[n+1].tokenStr != "[" && 
         f.content[n+1].tokenStr != "," ) {
      // {}要判斷換行 跟印下一行的空格
      // ;要判斷換行 跟印下一行的空格
      // [前面的不需要接空格   例如: a[ 30 ]
      // ,前面的不需要接空格   例如: a, b
      if ( f.content[n+1].tokenStr == "(" ) { // open par  去判斷前面是甚麼 決定要不要接空格
        if ( f.content[n].tokenStr == "while" || f.content[n].tokenStr == "if" || 
             f.content[n].tokenStr == "(" )  // if (),  while () , ( ( int a ) ) 
          cout << " " ;
      } // if

      else if ( f.content[n+1].tokenStr == "++" || f.content[n+1].tokenStr == "--" ) {
        if ( f.content[n].type != "IDENT" ) 
          cout << " " ; 
      } // else if IDENT++ (中間是否需要空白 如果前一個不是IDENT就要印空白)

      else if ( f.content[n].tokenStr == "++" || f.content[n].tokenStr == "--" ) {
        if ( f.content[n+1].type != "IDENT" ) // 如果是IDENT 就不用印空白 因為第二個+是正號的意思
          cout << " " ; 
      } // else if

      else cout << " " ;
    } // if

    // { } 要計算空格數量和印換行
    if ( f.content[n].tokenStr == "{" ) { // 是{就印換行和space+2 但還要判斷下一個是不是}
      cout << endl ;
      space = space + 2 ;

      if ( f.content[n+1].tokenStr == "}" ) { // 如果{後馬上接} 則space要再減2 不然會導致  
      // {    space是0 這行結束之後space是2
      // __}  space是2 這行結束之後space是0 但這樣排版會錯 所以要再遇到}的時候直接再減2 
        for ( int j = 0 ; j < space - 2 ; j++ ) 
          cout << " " ;
      } // if

      else { // 後面是要印出內容 所以要印空格
        for ( int j = 0 ; j < space ; j++ ) 
          cout << " " ;
      } // else
    } // if

    if ( f.content[n].tokenStr == ";" ) { // semi
      cout << endl ;
      if ( f.content[n+1].tokenStr == "}" ) {
        for ( int j = 0 ; j < space - 2 ; j++ ) 
          cout << " " ;
      } // if

      else {
        for ( int j = 0 ; j < space ; j++ ) 
          cout << " " ;
      } // else   
    } // if

    if ( f.content[n+1].tokenStr == "}" ) space = space - 2 ;

    if ( f.content[n].tokenStr == "}" ) { // } while
      int temp = -1 ;
      int leftright_parsum = 1 ;
      for ( temp = n - 1 ; temp > -1 && leftright_parsum > 0 ; temp-- ) {
        if ( f.content[temp].tokenStr == "{" ) leftright_parsum-- ;
        if ( f.content[temp].tokenStr == "}" ) leftright_parsum++ ;
      } // for
      

      if ( f.content[n+1].tokenStr == "while" && f.content[temp].tokenStr == "do" )  // do while
        cout << " " ;
      else { // } 後面不接while 要印換行和空格
        cout << endl ;
        for ( int j = 0 ; j < space ; j++ ) 
          cout << " " ;
      } // else
    } // if
  } // for

  cout << f.content[f.content.size()-1].tokenStr ;
} // Function_PrettyPrint()


bool IsFunction( string s ) {
  for ( int i = 0 ; i < gFunctionList.size() ; i++ ) {
    if ( s == gFunctionList[i].name ) return true ;
  } // for

  return false ;
} // IsFunction()

bool IsLocalDefined( string s, int &position ) {
  for ( int i = 0 ; i < gLocalVariableList.size() ; i++ ) {
    if ( s == gLocalVariableList[i].name ) {
      position = i ;
      return true ;
    } // if
  } // for

  position = -1 ;
  return false ;
} // IsLocalDefined()

void Do_Things( bool big_bracket, bool if_while, bool function_read_done ) { // 做事
  char c[500] ; // 遇到error時用來清空cin的buffer
  bool undefined = false ; // 用來判斷是否有undefined identifier
  // ===============================big_bracket=========================================
  if ( big_bracket ) {
    // 檢查這個variable有沒有在外面被宣告過(對照gVariableList-全域變數的概念)
    // ex int a; { int a; } ---OK   在這裡只考慮{}的東西
    for ( int i = 0 ; gTokenList.size() >= 1 && i < gTokenList.size() ; i++ ) { 
      // not變數或function的def才要判斷這個東西是不是def identifier
      // {}內的變數宣告 放入LocalVariableList內
      int position = -1 ;
      if ( IsDefineToken( gTokenList[i].tokenStr ) ) {
        for ( int j = i + 1 ; j < gTokenList.size() && gTokenList[j].tokenStr != ";" ; j++ ) {
          if ( gTokenList[j].type == "IDENT" ) {
            cout << "" ;
            /*
            if ( IsLocalDefined( gTokenList[j].tokenStr, position ) ) {
              string arrsize = "" ;
              if ( j + 1 < gTokenList.size() && gTokenList[j+1].tokenStr == "[" ) {
                for ( int w = j + 1 ; w < gTokenList.size() ; w++ ) {
                  if ( gTokenList[w].tokenStr != "]" ) {
                    arrsize += gTokenList[w].tokenStr ;
                    if ( w == j + 1 ) arrsize += " " ;
                  } // if

                  else {
                    arrsize += " " ;
                    arrsize += gTokenList[w].tokenStr ;
                    w = gTokenList.size() ; // break ;
                  } // else
                } // for (store the array size)
              } // if 

              gLocalVariableList[position].array_size = arrsize ;
              gLocalVariableList[position].value = gTokenList[i].tokenStr ;
              gLocalVariableList[position].name = gTokenList[j].tokenStr ;
              // i = j ; // break ;   // 不確定這行對不對
            } // if IsLocalDefined

            else {
              string arrsize = "" ;
              if ( j + 1 < gTokenList.size() && gTokenList[j+1].tokenStr == "[" ) {
                for ( int w = j + 1 ; w < gTokenList.size() ; w++ ) {
                  if ( gTokenList[w].tokenStr != "]" ) {
                    arrsize += gTokenList[w].tokenStr ;
                    if ( w == j + 1 ) arrsize += " " ;
                  } // if

                  else {
                    arrsize += " " ;
                    arrsize += gTokenList[w].tokenStr ;
                    w = gTokenList.size() ; // break ;
                  } // else
                } // for (store the array size)
              } // if 

              Variable_Info variable_info_temp ;
              variable_info_temp.array_size = arrsize ;
              variable_info_temp.value = gTokenList[i].tokenStr ;
              variable_info_temp.name = gTokenList[j].tokenStr ;
              gLocalVariableList.push_back( variable_info_temp ) ; 
            } // else
            */
          } // if ident
        } // for int  a,c,d,f;之a,c,d,f
      } // if int, float, char, bool

      // {}內檢查是否有使用到未宣告的變數(global, local)
      else if ( gTokenList[i].type == "IDENT" && gTokenList[i].tokenStr != "cout" && 
                gTokenList[i].tokenStr != "cin" ) {
        bool def1 = false ; // global
        bool def2 = false ; // local
        bool def3 = false ; // function
        for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
          if ( gTokenList[i].tokenStr == gVariableList[j].name ) {  // global
            def1 = true ;
            j = gVariableList.size() ; // break ;
          } // if
        } // for

        for ( int j = 0 ; j < gLocalVariableList.size() ; j++ ) { // local
          if ( gTokenList[i].tokenStr == gLocalVariableList[j].name ) {
            def2 = true ;
            j = gLocalVariableList.size() ; // break ;
          } // if
        } // for

        for ( int j = 0 ; j < gFunctionList.size() ; j++ ) { // local
          if ( gTokenList[i].tokenStr == gFunctionList[j].name ) {
            def3 = true ;
            j = gFunctionList.size() ; // break ;
          } // if
        } // for

        if ( ! def1 && ! def2 && ! def3 ) {
          cout << "Line " << gNextCharLine << " : undefined identifier : \'" << gTokenList[i].tokenStr 
               << "\'" << endl ;
          cout << "> " ;
          gNextCharLine = 1 ;
          undefined = true ;
          cin.getline( c, 500, '\n' ) ;
          gTokenList.clear() ;
          i = gTokenList.size() ; // break ;
          return ; // back to main
        } // if ! def
      } // else if IDENT
    } // for token size
    // 最終他只會印出"一個"statement executed
  } // if big_bracket

  // ==================================if while do=========================================
  else if ( if_while ) {
    // ====================================if==============================================
    // 可能也會有大刮{}的問題 要記得處理(但好像算文法問題?之後寫完測了再說 先不特別寫了:D)
    // 直接處理if裡面的內容就好 因為if條件是否違反三種原則已經在main做檢查
    // 最終只會輸出一行statement executed(寫在main了)
    int end = 0 ;
    if ( g_if_statement ) end = gTokenList.size() - 2 ;  // 用來判斷是不是else那個token不可以算進去!!
    else end = gTokenList.size() - 1 ; // last position of token list

    for ( int i = 0 ; gTokenList.size() >= 1 && i <= end ; i++ ) { 
      // not變數或function的def才要判斷這個東西是不是def identifier
      // {}內的變數宣告 放入LocalVariableList內
      int position = -1 ;
      if ( IsDefineToken( gTokenList[i].tokenStr ) ) {
        for ( int j = i + 1 ; j < gTokenList.size() && gTokenList[j].tokenStr != ";" ; j++ ) {
          if ( gTokenList[j].type == "IDENT" ) {
            cout << "" ;
            /*
            if ( IsLocalDefined( gTokenList[j].tokenStr, position ) ) {
              string arrsize = "" ;
              if ( j + 1 < gTokenList.size() && gTokenList[j+1].tokenStr == "[" ) {
                for ( int w = j + 1 ; w < gTokenList.size() ; w++ ) {
                  if ( gTokenList[w].tokenStr != "]" ) {
                    arrsize += gTokenList[w].tokenStr ;
                    if ( w == j + 1 ) arrsize += " " ;
                  } // if

                  else {
                    arrsize += " " ;
                    arrsize += gTokenList[w].tokenStr ;
                    w = gTokenList.size() ; // break ;
                  } // else
                } // for (store the array size)
              } // if 

              gLocalVariableList[position].array_size = arrsize ;
              gLocalVariableList[position].value = gTokenList[i].tokenStr ;
              gLocalVariableList[position].name = gTokenList[j].tokenStr ;
              // i = j ; // break ;   // 不確定這行對不對
            } // if IsLocalDefined

            else {
              string arrsize = "" ;
              if ( j + 1 < gTokenList.size() && gTokenList[j+1].tokenStr == "[" ) {
                for ( int w = j + 1 ; w < gTokenList.size() ; w++ ) {
                  if ( gTokenList[w].tokenStr != "]" ) {
                    arrsize += gTokenList[w].tokenStr ;
                    if ( w == j + 1 ) arrsize += " " ;
                  } // if

                  else {
                    arrsize += " " ;
                    arrsize += gTokenList[w].tokenStr ;
                    w = gTokenList.size() ; // break ;
                  } // else
                } // for (store the array size)
              } // if 

              Variable_Info variable_info_temp ;
              variable_info_temp.array_size = arrsize ;
              variable_info_temp.value = gTokenList[i].tokenStr ;
              variable_info_temp.name = gTokenList[j].tokenStr ;
              gLocalVariableList.push_back( variable_info_temp ) ; 
            } // else
            */
          } // if ident
        } // for int  a,c,d,f;之a,c,d,f
      } // if int, float, char, bool

      // {}內檢查是否有使用到未宣告的變數(global, local)
      else if ( gTokenList[i].type == "IDENT" && gTokenList[i].tokenStr != "cout" && 
                gTokenList[i].tokenStr != "cin" ) {
        bool def1 = false ; // global
        bool def2 = false ; // local
        bool def3 = false ; // function
        for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
          if ( gTokenList[i].tokenStr == gVariableList[j].name ) {  // global
            def1 = true ;
            j = gVariableList.size() ; // break ;
          } // if
        } // for

        for ( int j = 0 ; j < gLocalVariableList.size() ; j++ ) { // local
          if ( gTokenList[i].tokenStr == gLocalVariableList[j].name ) {
            def2 = true ;
            j = gLocalVariableList.size() ; // break ;
          } // if
        } // for

        for ( int j = 0 ; j < gFunctionList.size() ; j++ ) { // local
          if ( gTokenList[i].tokenStr == gFunctionList[j].name ) {
            def3 = true ;
            j = gFunctionList.size() ; // break ;
          } // if
        } // for

        if ( ! def1 && ! def2 && ! def3 ) {
          g_if_while_undef = true ;
          cout << "Line " << gNextCharLine << " : undefined identifier : \'" << gTokenList[i].tokenStr 
               << "\'" << endl ;
          cout << "> " ;
          gNextCharLine = 1 ;
          undefined = true ;
          cin.getline( c, 500, '\n' ) ;
          gTokenList.clear() ;
          i = gTokenList.size() ; // break ;
        } // if ! def
      } // else if IDENT
    } // for token size

    return ;
  } // else if if_while

  else { // 不是大括號
    // 宣告系列 放入對應vector內且印出該有的輸出

    // ==============================宣告變數==========================================
    if ( gTokenList.size() > 0 && IsDefineToken( gTokenList[0].tokenStr ) && ! g_function_def ) {
      for ( int i = 1 ; i < gTokenList.size() && gTokenList.size() >= 3 && gTokenList[2].tokenStr != "(" ;
            i++ ) {
      // check if exist in variable list or not --- cout << "new def....."
        if ( gTokenList[i].type == "IDENT" ) {
          bool def = false ;
          int new_def_pos = 0 ;
          for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
            if ( gTokenList[i].tokenStr == gVariableList[j].name ) {
              def = true ;
              new_def_pos = j ;
              j = gVariableList.size() ; // break ;
            } // if
          } // for

          if ( def ) {
            string arrsize = "" ;
            if ( i + 1 < gTokenList.size() && gTokenList[i+1].tokenStr == "[" ) {
              for ( int j = i + 1 ; j < gTokenList.size() ; j++ ) {
                if ( gTokenList[j].tokenStr != "]" ) {
                  arrsize += gTokenList[j].tokenStr ;
                  if ( j == i + 1 ) arrsize += " " ;
                } // if

                else {
                  arrsize += " " ;
                  arrsize += gTokenList[j].tokenStr ;
                  j = gTokenList.size() ; // break ;
                } // else
              } // for (store the array size)
            } // if [

            gVariableList[new_def_pos].value = gTokenList[0].tokenStr ;
            gVariableList[new_def_pos].array_size = arrsize ;
            cout << "New definition of " << gVariableList[new_def_pos].name << " entered ..." << endl ;
          } // if def 

          // if not exist ---> store in variable list
          else {
            Variable_Info variable_info_temp ;
            variable_info_temp.name = gTokenList[i].tokenStr ;
            string arrsize = "" ;
            if ( i + 1 < gTokenList.size() && gTokenList[i+1].tokenStr == "[" ) {
              for ( int j = i + 1 ; j < gTokenList.size() ; j++ ) {
                if ( gTokenList[j].tokenStr != "]" ) {
                  arrsize += gTokenList[j].tokenStr ;
                  if ( j == i + 1 ) arrsize += " " ;
                } // if

                else {
                  arrsize += " " ;
                  arrsize += gTokenList[j].tokenStr ;
                  j = gTokenList.size() ; // break ;
                } // else
              } // for (store the array size)
            } // if [

            variable_info_temp.array_size = arrsize ;
            variable_info_temp.value = gTokenList[0].tokenStr ;
            gVariableList.push_back( variable_info_temp ) ;  // *****放入global variable list內*****
            cout << "Definition of " << variable_info_temp.name << " entered ..." << endl ;
          } // else if not exist is var list
        } // if ident
      } // for int  a,c,d,f;之a,c,d,f

      return ;
    } // if int, float, char, bool

    // ==================================宣告function=====================================
    else if ( gTokenList.size() >= 3 && g_function_def ) {
      int first_left_small_par = 0 ;
      for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
        if ( gTokenList[i].tokenStr == "(" ) {
          first_left_small_par = i ;
          i = gTokenList.size() ; // break ;
        } // if
      } // for finding first left big parenthesis

      // check if exist in function list or not
      if ( ! function_read_done ) {
        int first_left_small_par = 2, first_right_small_par = 0 ;
        for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
          if ( gTokenList[i].tokenStr == ")" ) {
            first_right_small_par = i ;
            i = gTokenList.size() ; // break ;
          } // if
        } // for finding first right big parenthesis

        /*
        for ( int i = first_left_small_par ; i <= first_right_small_par ; i++ ) {
          if ( IsDefineToken( gTokenList[i].tokenStr ) ) {
            if ( i + 1 <= first_right_small_par && gTokenList[i+1].type == "IDENT" ) {
              Variable_Info variable_info_temp ;
              variable_info_temp.name = gTokenList[i+1].tokenStr ;
              string arrsize = "" ;
              if ( i + 2 <= first_right_small_par && gTokenList[i+2].tokenStr == "[" ) {
                for ( int j = i + 2 ; j <= first_right_small_par ; j++ ) {
                  if ( gTokenList[j].tokenStr != "]" ) {
                    arrsize += gTokenList[j].tokenStr ;
                    if ( j == i + 2 ) arrsize += " " ;
                  } // if

                  else {
                    arrsize += " " ;
                    arrsize += gTokenList[j].tokenStr ;
                    j = gTokenList.size() ; // break ;
                  } // else
                } // for (store the array size)

                variable_info_temp.array_size = arrsize ;
                variable_info_temp.value = gTokenList[i+1].tokenStr ;
                gLocalVariableList.push_back( variable_info_temp ) ;
              } // if [
            } // if j+1 IDNET
          } // if define token
        } // for (...)

        for ( int i = first_right_small_par ; gTokenList.size() >= 1 && i < gTokenList.size() ; i++ ) { 
        // not變數或function的def才要判斷這個東西是不是def identifier
        // {}內的變數宣告 放入LocalVariableList內
          if ( IsDefineToken( gTokenList[i].tokenStr ) ) {
            for ( int j = i + 1 ; j < gTokenList.size() && gTokenList[j].tokenStr != ";" ; j++ ) {
              // cout << "1958 " << gTokenList[i].tokenStr << " " << gTokenList[j].tokenStr << endl;
              if ( gTokenList[j].type == "IDENT" ) {
                Variable_Info variable_info_temp ;
                variable_info_temp.name = gTokenList[j].tokenStr ;
                string arrsize = "" ;
                if ( i + 1 < gTokenList.size() && gTokenList[i+1].tokenStr == "[" ) {
                  for ( int j = i + 1 ; j < gTokenList.size() ; j++ ) {
                    if ( gTokenList[j].tokenStr != "]" ) {
                      arrsize += gTokenList[j].tokenStr ;
                      if ( j == i + 1 ) arrsize += " " ;
                    } // if

                    else {
                      arrsize += " " ;
                      arrsize += gTokenList[j].tokenStr ;
                      j = gTokenList.size() ; // break ;
                    } // else
                  } // for (store the array size)
                } // if

                variable_info_temp.array_size = arrsize ;
                variable_info_temp.value = gTokenList[j].tokenStr ;
                gLocalVariableList.push_back( variable_info_temp ) ; 
                i = j ; // break ;   // 不確定這行對不對
              } // if ident
            } // for int  a,c,d,f;之a,c,d,f
          } // if int, float, char, bool

          // {}內檢查是否有使用到未宣告的變數(global, local)
          else if ( gTokenList[i].type == "IDENT" && gTokenList[i].tokenStr != "cout" && 
                    gTokenList[i].tokenStr != "cin" ) {
            bool def1 = false ; // global
            bool def2 = false ; // local
            bool def3 = false ; // function
            for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
              if ( gTokenList[i].tokenStr == gVariableList[j].name ) {  // global
                def1 = true ;
                j = gVariableList.size() ; // break ;
              } // if
            } // for

            for ( int j = 0 ; j < gLocalVariableList.size() ; j++ ) { // local
              if ( gTokenList[i].tokenStr == gLocalVariableList[j].name ) {
                def2 = true ;
                j = gLocalVariableList.size() ; // break ;
              } // if
            } // for

            for ( int j = 0 ; j < gFunctionList.size() ; j++ ) { // local
              if ( gTokenList[i].tokenStr == gFunctionList[j].name ) {
                def3 = true ;
                j = gFunctionList.size() ; // break ;
              } // if
            } // for

            if ( ! def1 && ! def2 && ! def3 ) {
              g_if_while_undef = true ;
              cout << "Line " << gNextCharLine << " : undefined identifier : \'" << gTokenList[i].tokenStr 
                   << "\'" << endl ;
              cout << "> " ;
              undefined = true ;
              gNextCharLine = 1 ;
              g_function_def = false ;
              cin.getline( c, 500, '\n' ) ;
              gTokenList.clear() ;
              i = gTokenList.size() ; // break ;
            } // if ! def
          } // else if IDENT
        } // for token size
        */
      } // if ! function read done

      else { // if function read done 存function資料
        // 檢查是否存在 存在的話就印new def
        // 不存在的話就存進去
        bool def = false ;
        int new_def_pos = 0 ;
        for ( int j = 0 ; j < gFunctionList.size() ; j++ ) {
          if ( gTokenList[1].tokenStr == gFunctionList[j].name ) {
            def = true ;
            new_def_pos = j ;
            j = gFunctionList.size() ; // break ;
          } // if
        } // for

        if ( def ) {
          gFunctionList[new_def_pos].type = gTokenList[0].tokenStr ;
          gFunctionList[new_def_pos].content.clear() ;
          for ( int i = first_left_small_par ; i < gTokenList.size() ; i++ )
            gFunctionList[new_def_pos].content.push_back( gTokenList[i] ) ;
          cout << "New definition of " << gFunctionList[new_def_pos].name << "() entered ..." << endl ;
          g_function_def = false ;
        } // if exist in function list

        else {
          Function_Info function_info_temp ;
          function_info_temp.name = gTokenList[1].tokenStr ;
          function_info_temp.type = gTokenList[0].tokenStr ;
          for ( int i = first_left_small_par ; i < gTokenList.size() ; i++ )
            function_info_temp.content.push_back( gTokenList[i] ) ;

          gFunctionList.push_back( function_info_temp ) ; // *****放入global function list內*****
          cout << "Definition of " << function_info_temp.name << "() entered ..." << endl ;
          g_function_def = false ;
        } // else if not exist is var list
      } // else function read done


      return ;
    } // else if def function


    // ===========================ListAll系列=====================================
    if ( gTokenList.size() >= 4 && gTokenList[0].tokenStr == "ListAllVariables" && 
         gTokenList[1].tokenStr == "(" && gTokenList[2].tokenStr == ")" ) {
      BubbleSortV( gVariableList ) ;
      for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
        cout << gVariableList[i].name << endl ;
      } // for print all variable

      // cout << "Statement executed ..." << endl ;
      return ;
    } // if ListAllVariables

    else if ( gTokenList.size() >= 4 && gTokenList[0].tokenStr == "ListAllFunctions" && 
              gTokenList[1].tokenStr == "(" && gTokenList[2].tokenStr == ")" ) {
      BubbleSortF( gFunctionList ) ;
      for ( int i = 0 ; i < gFunctionList.size() ; i++ ) {
        cout << gFunctionList[i].name << "()" << endl ;
      } // for print all function

      // cout << "Statement executed ..." << endl ;
      return ;
    } // if ListAllVariables

    // =======================ListVar, ListFunc系列=====================================
    else if ( gTokenList.size() >= 4 && gTokenList[0].tokenStr == "ListVariable" &&
              gTokenList[1].tokenStr == "(" && gTokenList[2].type == "Constant" && 
              gTokenList[3].tokenStr == ")" ) {
      // ListVariable("a");
      string find_name = "" ;
      find_name = gTokenList[2].tokenStr.substr( 1, gTokenList[2].tokenStr.size() - 2 ) ;
      for ( int i = 0 ; i < gVariableList.size() ; i++ ) {
        if ( gVariableList[i].name == find_name ) {
          cout << gVariableList[i].value << " " << gVariableList[i].name << gVariableList[i].array_size
               << " ;" << endl ;
          // cout << "2133Statement executed ..." << endl ;
          return ;
        } // if
      } // for
    } // else if

    else if ( gTokenList.size() >= 4 && gTokenList[0].tokenStr == "ListFunction" &&
              gTokenList[1].tokenStr == "(" && gTokenList[2].type == "Constant" && 
              gTokenList[3].tokenStr == ")" ) {
      // ListFunction("a");
      string find_name = "" ;
      find_name = gTokenList[2].tokenStr.substr( 1, gTokenList[2].tokenStr.size() - 2 ) ;
      for ( int i = 0 ; i < gFunctionList.size() ; i++ ) {
        if ( gFunctionList[i].name == find_name ) {
          Function_PrettyPrint( gFunctionList[i] ) ;
          // print content of function
          // cout << "2030 after print function" << endl;
          // for ( int h = 0 ; h < gFunctionList[i].content.size() ; h++ ) { // from { to }的內容
          // cout << gFunctionList[i].content[h] << " " ;
          // } // for print content of function

          cout << endl;
          // cout << "2155Statement executed ..." << endl ;
          return ;
        } // if
      } // for
    } // else if

    // 其他
    /*
    if ( g_if_num != g_else_num ) cout << "" ; // 這行是為了讓他不要印statement executed
    if ( gTokenList.size() == 1 && gTokenList[0].tokenStr == ";" ) cout << "Statement executed ..." << endl ;
    else if ( ! DONE() && ! g_if_while_undef && ! undefined ) cout << "2160Statement executed ..." << endl ;
    return ;
    */
  } // else

} // Do_Things()


int main() {
  int uTestNum ;
  cin >> uTestNum ;
  gNum = uTestNum ;
  char d ; // get '\n'
  char c[500] ;
  cin.get( d ) ;
  bool done = false ;

  cout << "Our-C running ..." << endl ;

  gTokenList.clear() ; // 清空vector
  gVariableList.clear() ;
  gFunctionList.clear() ;
  string tokenStr, tokenType ; // using for store the token
  Token_Info token_info_temp ; // using for store the token type and token

  cout << "> " ;
  GetNextChar( gNextChar, gNextCharLine ) ;
  // int i = 0 ;
  while ( ! done ) {               
    done = false ;
    bool unrecognized = false ;
    bool dot_Error = false ;

    tokenStr = "" ;
    tokenType = "" ;
    tokenStr = GetToken( gNextChar, tokenType ) ;  // tokenType會在做完這個function後回傳  (&tokenType)
    // cout << "1725tokenStr : " << tokenStr << " tokenType : " << tokenType << endl ;


    if ( tokenType == "unrecog_ERROR" ) {   // 第一道防線     印出錯誤之後整行要去掉 所以直接把List清空
      cout << "Line " << gNextCharLine << " : unrecognized token with first char : \'" 
           << tokenStr << "\'" << endl ;
      cout << "> " ;
      gNextCharLine = 1 ;      
      cin.getline( c, 500, '\n' ) ;
      gTokenList.clear() ; // 先這樣 如果有 123+10+@56;A:=; 則 A = 133
      unrecognized = true ;
    } // if 這個if是用來處理不認識的東西的



    if ( gTwoDot_Unexpected ) {  // 第二道防線之其中一種錯誤   兩個點且點後有數字.123.456  (ps兩個點且點後沒數字屬於unrecog_error) .123.
      cout << "Line " << gNextCharLine << " : unexpected token : \'" << tokenStr << "\'" << endl ;
      cout << "> " ;
      gNextCharLine = 1 ;      
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


    bool undef = false ;
    bool def_Var_Func = false ; // 用來判斷是不是在宣告變數或function
    gIndex = 0 ; // 每次進parser tree都要從list的頭開始檢查
    g_if_num = 0 ; // init
    g_else_num = 0 ; // init
    g_while_num = 0 ; // init
    if ( ! unrecognized && ! dot_Error && ! DONE() ) {       // 放這是因為如果是 1.23.65;  這樣分號也會變成error msg且印出來
      if ( ! User_Input() ) { // Parser_Tree
        cout << "Line " << gNextCharLine << " : unexpected token : \'" << tokenStr << "\'" << endl ;
        cout << "> " ;
        gNextCharLine = 1 ;      
        cin.getline( c, 500, '\n' ) ;
        gTokenList.clear() ;
      } // if parser tree is false, then allocate the vector and print error msg

      else { // 如果tokenlist內有IDENT 要馬上檢查她有沒有宣告過 沒有的話要印出error msg且getline後面不必要的資料        
        // 這個判斷是 int a[];這種東西 讓他在遇到a時候不要檢查她是不是define過
        if ( gTokenList.size() >= 1 && 
             ( IsDefineToken( gTokenList[0].tokenStr ) ||
               gTokenList[0].tokenStr == "cout" || gTokenList[0].tokenStr == "cin" ||
               gTokenList[0].tokenStr == "Done" || gTokenList[0].tokenStr == "{" ||
               gTokenList[0].tokenStr == "if" || gTokenList[0].tokenStr == "while" ||
               gTokenList[0].tokenStr == "do" ) ) 
          def_Var_Func = true ;

        int start = 0;
        if ( gTokenList.size() >= 1 && 
             ( gTokenList[0].tokenStr == "cout" || gTokenList[0].tokenStr == "cin" ||
               gTokenList[0].tokenStr == "ListVariable" || gTokenList[0].tokenStr == "ListAllVariables" ||
               gTokenList[0].tokenStr == "ListFunction" || gTokenList[0].tokenStr == "ListAllFunctions" ) ) {
          start = 1 ;
          def_Var_Func = false ;
        } // if cin || cout
        // 用意:cin >> IDENT或cout << IDENT都需要去檢查該IDENT是否被宣告過
        // 但我不能在上面修改條件 如果修改了就會跑錯(cout cin type皆為IDENT才會文法正確)
        // 所以在這裡我直接讓他從>>或<<開始檢查 cin也不會被判斷成undefined identifier

        for ( int i = start ; gTokenList.size() >= 1 && i < gTokenList.size() && ! def_Var_Func ; i++ ) { 
          // not變數或function的def才要判斷這個東西是不是def identifier
          if ( gTokenList[i].type == "IDENT" ) {
            bool def = false ;
            for ( int j = 0 ; j < gVariableList.size() ; j++ ) {
              if ( gTokenList[i].tokenStr == gVariableList[j].name ) {
                def = true ;
                j = gVariableList.size() ; // break ;
              } // if
            } // for

            for ( int j = 0 ; j < gFunctionList.size() ; j++ ) {
              if ( gTokenList[i].tokenStr == gFunctionList[j].name ) {
                def = true ;
                j = gFunctionList.size() ; // break ;
              } // if
            } // for

            if ( ! def ) {
              cout << "Line " << gNextCharLine << " : undefined identifier : \'" 
                   << gTokenList[i].tokenStr << "\'" << endl ;
              cout << "> " ;
              gNextCharLine = 1 ;      
              gLinereturn = false ;
              cin.getline( c, 500, '\n' ) ;
              gTokenList.clear() ;
              i = gTokenList.size() ; // break ;
            } // if ! def
          } // if IDENT
        } // for token size
      } // else
    }  // if parser tree is false, then allocate the vector


    if ( gTokenList.size() != 0 && DONE() )
      done = true ;


    gIndex = 0 ;
    g_if_while_undef = false ; // init
    if ( gTokenList.size() != 0 && ! undef && ! done )  {
      // ============================= 大括號 ===================================
      bool big_bracket = false ;
      bool if_while = false ;
      bool function_read_done = false ;
      int left = 0, right = 0 ; // 左括號數量要和右括號數量一樣才會正確

      // def_Var_Func是用來判斷是不是在宣告變數或function
      if ( gTokenList.size() >= 1 && gTokenList[0].tokenStr == "{" ) def_Var_Func = false ; 
      // 上面那行是為了正常執行才寫的 上面的bool def_Var_Func = true
      // 如果一開始就是false的話 {int a 的a會變判斷成undeifined identifier
      if ( ! def_Var_Func && gTokenList.size() >= 1 && gTokenList[0].tokenStr == "{" ) { // 這個if是用來判斷大括號的
        big_bracket = true ;
        left++ ; // [0]
        for ( int i = 1 ; i < gTokenList.size() ; i++ ) {
          if ( gTokenList[i].tokenStr == "{" ) left++ ;
          if ( gTokenList[i].tokenStr == "}" ) right++ ;
        } // for

        Do_Things( big_bracket, if_while, function_read_done ) ; // big bracket --- true

        if ( left == right ) {
          cout << "Statement executed ..." << endl ;
          gTokenList.clear() ; // 清空後要重新開始讀取新的一行命令
          gLocalVariableList.clear() ; // 清空後要重新開始讀取新的一行命令
          bool finish = false ;
          char w = '\0' ;
          while ( ! finish && ( IsWhiteSpace( cin.peek() ) || cin.peek() == '/' ) ) {
            if ( cin.peek() == '\n' && ! finish ) {
              finish = true ;
              cin.get( w ) ;
            } // if

            else if ( cin.peek() == '/' ) {
              cin.get( w ) ;
              if ( cin.peek() == '/' ) {
                cin.get( w ) ;
                while ( cin.peek() != '\n' ) cin.get( w ) ; // 註解
                if ( cin.peek() == '\n' ) cin.get( w ) ;
                finish = true ;
              } // if
            } // else if              

            else cin.get( w )  ; // other white space
          } // while 讀掉;後面之註解 空白 換行 確保Line不會跑錯

          gNextCharLine = 1 ;
          cout << "> " ;
        } // if left == right
      } // if 大括號
      // ============================= 大括號 ===================================

      // ================非大括號 正常執行(遇到分號就做事)(還有特殊的if while情況)=====================

      // ================================if else/ while/ do while==================================
      if ( ! big_bracket && gTokenList.size() > 0 && 
           ( gTokenList[0].tokenStr == "if" || gTokenList[0].tokenStr == "while" || 
             gTokenList[0].tokenStr == "do" ) ) { 
        if_while = true ;
        g_if_while_undef = false ; // init
        int wleft_bigpara = 0, wright_bigpara = 0 ;
        int ifleft_bigpara = 0, ifright_bigpara = 0 ;
        for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
          if ( gTokenList[i].tokenStr == "{" ) ifleft_bigpara++ ;
          if ( gTokenList[i].tokenStr == "}" ) ifright_bigpara++ ;
        } // for {}


        if ( g_while_num > 1 ) {
          wleft_bigpara = 0, wright_bigpara = 0 ;
          for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
            if ( gTokenList[i].tokenStr == "{" ) wleft_bigpara++ ;
            if ( gTokenList[i].tokenStr == "}" ) wright_bigpara++ ;
          } // for {}
        } // if g_while_num > 1

        else { // gwhile_num == 1 為了讓他不要相等而已
          wright_bigpara = -99 ; 
          wleft_bigpara = -77 ;
        } // else

        if ( gTokenList[0].tokenStr == "if" )  // gTokenList的最後一個不能清掉!!因為我是靠最後一個去判斷是不是else 如果不是才會進這裡!
          Do_Things( big_bracket, if_while, function_read_done ) ; // if only 
        // else if ( g_else_statement )                   
        // Do_Things( big_bracket, if_while, function_read_done ) ; // if else
        else if ( gTokenList[0].tokenStr == "while" ) //  not sure條件
          Do_Things( big_bracket, if_while, function_read_done ) ; // while true
        else if ( gTokenList[0].tokenStr == "do" )     
          Do_Things( big_bracket, if_while, function_read_done ) ; // do_while true
        // ============做完事情才能印跟init!!!!!!!!============
        if ( ! g_if_while_undef && 
             ( g_if_statement || g_else_statement || g_while_statement || g_do_statement ) ) {

          // 總覺得這邊怪怪的 邏輯可能要釐清一下

          // init
          if ( g_if_statement && g_while_num <= 0 ) {
            // cout << "2361" << endl;
            cout << "Statement executed ..." << endl ;
            gTokenList.erase( gTokenList.begin(), gTokenList.end() - 1 ) ; // 最後一個不能珊!!
            gLocalVariableList.clear() ; // 清空後要重新開始讀取新的一行命令
            if_while = false ; // 如果剛好Only if 且清空後剩下的為; 如果不把這個init的話下面那個條件跑不進去 會發生錯誤
            
            // bool finish = false ;
            // char w = '\0' ;
            // char temp = '\0' ; // 用來放cin.peek()
            // cout << "2372 " << cin.peek() << endl ;
            // 0509 註解掉   if..else..int b;; 
            /* 
            // //sta... 因為我清掉還會剩; cin.peek()是換行 但不能讀!!
            while ( ! finish && ( IsWhiteSpace( cin.peek() ) || cin.peek() == '/' ) ) {
              if ( cin.peek() == '\n' && ! finish ) {
                finish = true ;
                cin.get( w ) ;
              } // if

              else if ( cin.peek() == '/' ) {
                cin.get( w ) ;
                if ( cin.peek() == '/' ) {
                  cin.get( w ) ;
                  while ( cin.peek() != '\n' ) cin.get( w ) ; // 註解
                  if ( cin.peek() == '\n' ) cin.get( w ) ;
                  finish = true ;
                } // if
              } // else if              

              else cin.get( w )  ; // other white space
            } // while 讀掉;後面之註解 空白 換行 確保Line不會跑錯
            */

            gNextCharLine = 1 ;
            g_if_statement = false, g_else_statement = false ; 
            g_do_statement = false, g_while_statement = false ; // init
            cout << "> " ;
          }  // if if

          else if ( gTokenList.size() >= 1 &&
                    ( ( g_if_num == g_else_num && g_else_statement && gTokenList[0].tokenStr != "do" && 
                        ifleft_bigpara == ifright_bigpara ) || 
                      ( g_while_statement && 
                        ( ( g_while_num > 1 && wright_bigpara == wleft_bigpara ) || g_while_num == 1 ) 
                        && gTokenList[0].tokenStr != "do" ) || g_do_statement ) ) {
            cout << "Statement executed ..." << endl ;
            gTokenList.clear() ;
            gLocalVariableList.clear() ; // 清空後要重新開始讀取新的一行命令
            bool finish = false ;
            char w = '\0' ;
            while ( ! finish && ( IsWhiteSpace( cin.peek() ) || cin.peek() == '/' ) ) {
              if ( cin.peek() == '\n' && ! finish ) {
                finish = true ;
                cin.get( w ) ;
              } // if

              else if ( cin.peek() == '/' ) {
                cin.get( w ) ;
                if ( cin.peek() == '/' ) {
                  cin.get( w ) ;
                  while ( cin.peek() != '\n' ) cin.get( w ) ; // 註解
                  if ( cin.peek() == '\n' ) cin.get( w ) ;
                  finish = true ;
                } // if
              } // else if              

              else cin.get( w )  ; // other white space
            } // while 讀掉;後面之註解 空白 換行 確保Line不會跑錯

            gNextCharLine = 1 ;
            g_if_statement = false, g_else_statement = false ;
            g_do_statement = false, g_while_statement = false ; // init
            cout << "> " ;
          } // else if else, while, do while
        } // if global bool____if,else,while,do while + init
      } // if   gTokenList[0]__if,while,do while

      // ======================================function declaration========================================
      if ( ! big_bracket && ! if_while && g_function_def ) { // gTokenList.size()>=5
        int wleft_bigpara = 0, wright_bigpara = 0 ;
        for ( int i = 0 ; i < gTokenList.size() ; i++ ) {
          if ( gTokenList[i].tokenStr == "{" ) wleft_bigpara++ ;
          if ( gTokenList[i].tokenStr == "}" ) wright_bigpara++ ;
        } // for {}

        if ( wleft_bigpara == wright_bigpara && wleft_bigpara != 0 ) 
          function_read_done = true ;

        Do_Things( big_bracket, if_while, function_read_done ) ; // function declaration

        if ( wleft_bigpara == wright_bigpara && wleft_bigpara != 0 ) {
          // cout << "Statement executed ..." << endl ;
          gTokenList.clear() ; // 清空後要重新開始讀取新的一行命令
          gLocalVariableList.clear() ; // 清空後要重新開始讀取新的一行命令
          // 做完事情之後要把後面的空白 換行 註解讀掉才能把Line number設為1
          bool finish = false ;
          char w = '\0' ;
          while ( ! finish && ( IsWhiteSpace( cin.peek() ) || cin.peek() == '/' ) ) {
            if ( cin.peek() == '\n' && ! finish ) {
              finish = true ;
              cin.get( w ) ;
            } // if

            else if ( cin.peek() == '/' ) {
              cin.get( w ) ;
              if ( cin.peek() == '/' ) {
                cin.get( w ) ;
                while ( cin.peek() != '\n' ) cin.get( w ) ; // 註解
                if ( cin.peek() == '\n' ) cin.get( w ) ;
                finish = true ;
              } // if
            } // else if              

            else cin.get( w )  ; // other white space
          } // while 讀掉;後面之註解 空白 換行 確保Line不會跑錯

          gNextCharLine = 1 ;
          g_function_def = false ; // init
          function_read_done = false ; // init
          cout << "> " ;
        } // if left == right
      } // if function declaration 

      // =======================================遇到分號就做事的情況=========================================
      // 宣告變數也在這個範疇
      if ( ! big_bracket && gTokenList.size() > 0 && ! if_while && ! g_function_def &&
           gTokenList[gTokenList.size()-1].tokenStr == ";" ) {  
        Do_Things( big_bracket, if_while, function_read_done ) ;  // 遇到分號就做事 並且清空vector big_bracket is false
        // 做完事情之後要把後面的空白 換行 註解讀掉才能把Line number設為1
        if ( gTokenList.size() > 0 && IsDefineToken( gTokenList[0].tokenStr ) ) cout << "" ;
        else cout << "Statement executed ..." << endl ;
        bool finish = false ;
        char w = '\0' ;
        while ( ! finish && ( IsWhiteSpace( cin.peek() ) || cin.peek() == '/' ) ) {
          if ( cin.peek() == '\n' && ! finish ) {
            finish = true ;
            cin.get( w ) ;
          } // if

          else if ( ! finish && cin.peek() == '/' ) {
            cin.get( w ) ;
            if ( cin.peek() == '/' ) {
              cin.get( w ) ;
              while ( cin.peek() != '\n' ) cin.get( w ) ; // 註解
              if ( cin.peek() == '\n' ) cin.get( w ) ;
              finish = true ;
            } // if
          } // else if              

          else cin.get( w )  ; // other white space
        } // while 讀掉;後面之註解 空白 換行 確保Line不會跑錯


        gTokenList.clear() ; // 清空後要重新開始讀取新的一行命令
        cout << "> " ;
        gNextCharLine = 1 ;
      } // if最後一個是分號
    }  // if do things


    if ( ! done ) 
      GetNextChar( gNextChar, gNextCharLine ) ;
    // i++ ; // debug 
  } // while


  cout << "Our-C exited ..." << endl ;
  return 0 ;
} // main()
