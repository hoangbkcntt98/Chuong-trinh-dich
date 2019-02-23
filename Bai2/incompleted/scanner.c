/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
// Dong 27 example6 ko co loi (file result lai co loi)
// khong doc duoc ki tu EOF  cua bai 3 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];
#define MAX_WORDS 100
struct {
  int a;
  TokenType tokenType;
}Special[14] = {
  {CHAR_MINUS,SB_MINUS},
  {CHAR_TIMES,SB_TIMES},
  {CHAR_SLASH,SB_SLASH},
  {CHAR_LT,SB_LT},
  {CHAR_GT,SB_GT},
  {CHAR_EQ,SB_EQ},
  {CHAR_COMMA,SB_COMMA},
  {CHAR_PERIOD,SB_PERIOD},
  {CHAR_COLON,SB_COLON},
  {CHAR_SEMICOLON,SB_SEMICOLON},
  {CHAR_UNKNOWN,TK_EOF},
  {CHAR_LPAR,SB_LPAR},

  {CHAR_RPAR,SB_RPAR}

};
TokenType checkSpecial(char string) {
  int i;
  for (i = 0; i < 14; i++)
    if ( string ==Special[i].a)
      return Special[i].tokenType;
  return TK_NONE;
}
/***************************************************************/

void skipBlank() {
  // TODO
  while(charCodes[currentChar]==CHAR_SPACE&&charCodes[currentChar]!=EOF)
  {
    readChar();
  }
}

void skipComment() {
  // TODO
  int state = 0;
  while ((currentChar != EOF) && (state < 2)) {
    switch (charCodes[currentChar]) {
    case CHAR_TIMES:
      state = 1;
      break;
    case CHAR_RPAR:
      if (state == 1) state = 2;
      else state = 0;
      break;
    default:
      state = 0;
    }
    readChar();
  }
  if (state != 2) 
    error(ERR_ENDOFCOMMENT, lineNo, colNo);

}

Token* readIdentKeyword(void) {
  // TODO
  int ln,cl;
  Token *token;
  char string[MAX_IDENT_LEN+1];
  int i=0;
  ln = lineNo;
  cl=colNo;
 
  while(charCodes[currentChar]==CHAR_LETTER||charCodes[currentChar]==CHAR_DIGIT)
  {
    string[i]=currentChar;
    i++;
    readChar();
  }
  string[i]='\0';
  if(checkKeyword(string)!=TK_NONE)
  {
    token = makeToken(checkKeyword(string), ln, cl);
    strcpy(token->string,string);
    return token;
  }
  else
  {
    token = makeToken(TK_IDENT,ln,cl);
    strcpy(token->string,string);
    return token;
  }
  
}
Token* readNumber(void) {
  // TODO
  char string[100];
  int i=0;
  Token *token;
  while(charCodes[currentChar]==CHAR_DIGIT)
  {
    string[i]=currentChar;
    i++;
    readChar();
  }
  string[i]='\0';
  token = makeToken(TK_NUMBER,lineNo,colNo);
  strcpy(token->string,string);
  return token;
}

Token* readConstChar(void) {//format : 'abcd'
  // TODO
  char string[MAX_WORDS];
  
  int i=0;
  int ln=lineNo;
  int cl=colNo;
  
  Token *token;
  readChar();
  do
  {
    string[i]=currentChar;
    i++;
    readChar();
  }while(charCodes[currentChar]!=CHAR_SINGLEQUOTE);
  readChar();
  
  string[i]='\0';
  
  token = makeToken(TK_CHAR,ln,cl);
  strcpy(token->string,string);
  
  return token;
}

Token* getToken(void) {
  Token *token;
  //int checkLPAR =0;
  int ln,cl;
  CharCode code=charCodes[currentChar];
  
  ln = lineNo;
  cl=colNo;
  if (currentChar<0)
  {
    
    return makeToken(TK_EOF,lineNo,colNo);//Khong nhan ki tu EOF
  }
  if(code==CHAR_SPACE)
  {
    skipBlank();
    return getToken();
  }
  else if(code==CHAR_LETTER)
  {
    return readIdentKeyword();
  }
  else if(code==CHAR_DIGIT)
  {
    return readNumber();
  }
  else if( code== CHAR_SINGLEQUOTE)
  {
    return readConstChar();

  }
  else if(code == CHAR_UNKNOWN)
  {
    return makeToken(TK_NONE,lineNo,colNo);
  }
  else if(code==CHAR_LPAR)
  {
   
    readChar();
    if(charCodes[currentChar]==CHAR_TIMES)
    {
      skipComment();
      return getToken();
    }
    else 
    {
      token = makeToken(SB_LPAR, ln, cl);
      return token;
    }
  }
 
  else if(code == CHAR_COLON)
  {
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {
      readChar();
      return makeToken(SB_ASSIGN, ln, cl);
    } else return makeToken(SB_COLON, ln, cl);
  }
  else if(code == CHAR_EXCLAIMATION)
  {
    readChar();
    if ((currentChar != EOF) && (charCodes[currentChar] == CHAR_EQ)) {
      readChar();
      return makeToken(SB_NEQ, ln, cl);
    } else {
      token = makeToken(TK_NONE, ln, cl);
      error(ERR_INVALIDSYMBOL, ln, cl);
      return token;}
  }
  else
  {
    token = makeToken(checkSpecial(charCodes[currentChar]),lineNo,colNo);
    readChar(); 
    return token;
  }

  
}

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}
