#include "interpreter.h"
#include "parser.h"
#include <iostream>
#include <vector>

void lexerTests() {
  Lexer lx("123+342;");
  vector<Token> output;
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("123")));
  assert(output[1] == Token(ADD, string("+")));
  assert(output[2] == Token(NUM, string("342")));
  assert(output[3] == Token(SEMI, string(";")));

  lx = Lexer("123456;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("123456")));
  assert(output[1] == Token(SEMI, string(";")));

  // test overflow case
  lx = Lexer("5000000000;");
  bool exceptionThrown = false;
  try {
    lx.getNextToken();
  } catch (int e) {
    if(e == Lexer::INVALID_NUMBER)
      exceptionThrown = true;
  }
  assert(exceptionThrown);

  lx = Lexer("1+2-3*4;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("1")));
  assert(output[1] == Token(ADD, string("+")));
  assert(output[2] == Token(NUM, string("2")));
  assert(output[3] == Token(SUB, string("-")));
  assert(output[4] == Token(NUM, string("3")));
  assert(output[5] == Token(MULT, string("*")));
  assert(output[6] == Token(NUM, string("4")));
  assert(output[7] == Token(SEMI, string(";")));

  lx = Lexer("(4/2)+1;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(OPAREN, string("(")));
  assert(output[1] == Token(NUM, string("4")));
  assert(output[2] == Token(DIV, string("/")));
  assert(output[3] == Token(NUM, string("2")));
  assert(output[4] == Token(CPAREN, string(")")));
  assert(output[5] == Token(ADD, string("+")));
  assert(output[6] == Token(NUM, string("1")));

  // whitespace tests
  lx = Lexer("4  + 1;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("4")));
  assert(output[1] == Token(ADD, string("+")));
  assert(output[2] == Token(NUM, string("1")));
  assert(output[3] == Token(SEMI, string(";")));

  lx = Lexer("   123456  ;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("123456")));
  assert(output[1] == Token(SEMI, string(";")));

  // negative numbers
  lx = Lexer("4+ -1;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("4")));
  assert(output[1] == Token(ADD, string("+")));
  assert(output[2] == Token(SUB, string("-")));
  assert(output[3] == Token(NUM, string("1")));
  assert(output[4] == Token(SEMI, string(";")));

  lx = Lexer("-(1+2);");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(SUB, string("-")));
  assert(output[1] == Token(OPAREN, string("(")));
  assert(output[2] == Token(NUM, string("1")));
  assert(output[3] == Token(ADD, string("+")));
  assert(output[4] == Token(NUM, string("2")));
  assert(output[5] == Token(CPAREN, string(")")));
  assert(output[6] == Token(SEMI, string(";")));

  lx = Lexer("-12--15-10;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(SUB, string("-")));
  assert(output[1] == Token(NUM, string("12")));
  assert(output[2] == Token(SUB, string("-")));
  assert(output[3] == Token(SUB, string("-")));
  assert(output[4] == Token(NUM, string("15")));
  assert(output[5] == Token(SUB, string("-")));
  assert(output[6] == Token(NUM, string("10")));
  assert(output[7] == Token(SEMI, string(";")));

  lx = Lexer("-(-3);");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(SUB, string("-")));
  assert(output[1] == Token(OPAREN, string("(")));
  assert(output[2] == Token(SUB, string("-")));
  assert(output[3] == Token(NUM, string("3")));
  assert(output[4] == Token(CPAREN, string(")")));
  assert(output[5] == Token(SEMI, string(";")));

  lx = Lexer("-1 + variable;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(SUB, string("-")));
  assert(output[1] == Token(NUM, string("1")));
  assert(output[2] == Token(ADD, string("+")));
  assert(output[3] == Token(ID, string("variable")));
  assert(output[4] == Token(SEMI, string(";")));

  lx = Lexer("-x+y - -z *(a);");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(SUB, string("-")));
  assert(output[1] == Token(ID, string("x")));
  assert(output[2] == Token(ADD, string("+")));
  assert(output[3] == Token(ID, string("y")));
  assert(output[4] == Token(SUB, string("-")));
  assert(output[5] == Token(SUB, string("-")));
  assert(output[6] == Token(ID, string("z")));
  assert(output[7] == Token(MULT, string("*")));
  assert(output[8] == Token(OPAREN, string("(")));
  assert(output[9] == Token(ID, string("a")));
  assert(output[10] == Token(CPAREN, string(")")));
  assert(output[11] == Token(SEMI, string(";")));

  lx = Lexer("a=b*c; x=a*10;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(ID, string("a")));
  assert(output[1] == Token(EQUAL, string("=")));
  assert(output[2] == Token(ID, string("b")));
  assert(output[3] == Token(MULT, string("*")));
  assert(output[4] == Token(ID, string("c")));
  assert(output[5] == Token(SEMI, string(";")));
  assert(output[6] == Token(ID, string("x")));
  assert(output[7] == Token(EQUAL, string("=")));
  assert(output[8] == Token(ID, string("a")));
  assert(output[9] == Token(MULT, string("*")));
  assert(output[10] == Token(NUM, string("10")));
  assert(output[11] == Token(SEMI, string(";")));

  // test comparison
  lx = Lexer("1<2;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(NUM, string("1")));
  assert(output[1] == Token(LESSTHAN, string("<")));
  assert(output[2] == Token(NUM, string("2")));

  lx = Lexer("a>b;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(ID, string("a")));
  assert(output[1] == Token(GREATERTHAN, string(">")));
  assert(output[2] == Token(ID, string("b")));

  lx = Lexer("a==b;");
  output.clear();
  while(lx.hasNextToken())
    output.push_back(lx.getNextToken());
  assert(output[0] == Token(ID, string("a")));
  assert(output[1] == Token(EQUAL, string("=")));
  assert(output[2] == Token(EQUAL, string("=")));
  assert(output[3] == Token(ID, string("b")));
}

void interpreterTests() {
  Lexer lx("t=123+342;");
  Interpreter i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 465);

  lx = Lexer("t=4  - 12;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == -8);

  lx = Lexer("t=4 + 3 * 8;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 28);

  lx = Lexer("t=(4 + 3) * 8;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 56);

  lx = Lexer("t=(((4)));");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 4);

  lx = Lexer("t=(80-10)/7 + 4;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 14);

  // test negative case
  lx = Lexer("t=-1-3;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == -4);

  lx = Lexer("t=-1--3;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 2);

  lx = Lexer("t=-(1+2-10);");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 7);

  lx = Lexer("t=-(-3);");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 3);

  lx = Lexer("t=----1;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 1);

  lx = Lexer("t=5 - - - - (3 + 4) - 2;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("t") == 10);

  // Test invalid expressions
  bool exceptionThrown = false;
  try {
    lx = Lexer("t=1+;");
    i = Interpreter(Parser(lx));
    i.interpret();
  } catch (int e) {
    if(e == Interpreter::UNEXPECTED_TOKEN)
      exceptionThrown = true;
  }
  assert(exceptionThrown);

  exceptionThrown = false;
  try {
    lx = Lexer("t=1++3;");
    i = Interpreter(Parser(lx));
    i.interpret();
  } catch (int e) {
    if(e == Interpreter::UNEXPECTED_TOKEN)
      exceptionThrown = true;
  }
  assert(exceptionThrown);

  // Test multiple statements
  lx = Lexer("f=5+4; g=f*2;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 9);
  assert(i.getRuntimeValue("g") == 18);

  lx = Lexer("f=5+4; g=f*f;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 9);
  assert(i.getRuntimeValue("g") == 81);

  lx = Lexer("f=5+4; g=-f*f+10;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 9);
  assert(i.getRuntimeValue("g") == -71);

  lx = Lexer("f=5+4; g=-f*f+10; f=10; f=f; g=f+1;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 10);
  assert(i.getRuntimeValue("g") == 11);

  // comparison operator tests
  lx = Lexer("f=1<2; ff=1>2; g=1>0; gg=1<0; h=1>1; hh=1<1; i=1==1;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 1);
  assert(i.getRuntimeValue("ff") == 0);
  assert(i.getRuntimeValue("g") == 1);
  assert(i.getRuntimeValue("gg") == 0);
  assert(i.getRuntimeValue("h") == 0);
  assert(i.getRuntimeValue("hh") == 0);
  assert(i.getRuntimeValue("i") == 1);

  lx = Lexer("f= 10==(5+5*1);");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("f") == 1);

  lx = Lexer("f=10; g=f==10;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("g") == 1);

  lx = Lexer("f=10; g=-f==-10;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("g") == 1);

  lx = Lexer("f=10; g=f<f;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("g") == 0);

  lx = Lexer("f=10; g=21>-f+30;");
  i = Interpreter(Parser(lx));
  i.interpret();
  assert(i.getRuntimeValue("g") == 1);
}

int main() {
  lexerTests();
  interpreterTests();
  cout << "All tests passed" << endl;
  return 0;
}
