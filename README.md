# mini-interpreter

I'm making this simple interpreter to understand how interpreters and compilers really work under the hood. The end goal is to make a mini-language that supports variables, conditionals, and loops. This project is currently in active development.

The end goal is to create a programming language that supports arithmetic, variables (only numbers and strings), conditionals, loops, and print statements. An example program would look something like this:
```
  # compute sum from 1 to 10
  counter = 1;
  total = 0;
  while(counter <= 10) {
    total = total + counter;
    loop_counter = loop_counter + 1;
  }
  print total
```

## Test
Run `make test` to run the various lexer and interpreter tests that I have written
