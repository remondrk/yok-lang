import sys

patterns = {
    'VAR_DECL': 'create a unique , uninitialized variable and name it {id}',
    'VAR_ASSIGN': 'take the value of {value} and assign it to the variable {id}',
    'LABEL': 'this place will be referred to as {id} so that we can find it when we need to',
    'GO': 'go upwards and downwards until you find a place that is referred to as {id}',
    'CALL': 'find a place that is referred to as {id} , however ,  return back here when you are told to do so',
    'RET': 'return back to the previous place that you promised to return back',
    'GO_UP': 'go upwards until you find the place that is referred to as {id}',
    'GO_DOWN': 'go downwards until you find the place that is referred to as {id}',
    'IF_SKIP': 'if it happens to be that {value} {id} {value} ignore the following {quantity} line',
    'UNLESS_SKIP': 'unless it happens to be that {value} {id} {value} ignore the following {quantity} line',
    'INPUT': 'wait here until there is a useful ingress from the user',
    'WRITE_LN': 'write {value} to the console and skip to the next line afterwards',
    'WRITE': 'write {value} to the console , but do not skip to the next line',
    'DIVIDE': 'take the value of {value} and divide it using the value of {value}',
    'MULTIPLY': 'repeat the string or the number {value} an amount of times equal to the number {value}',
    'ADD': 'take two numbers or strings , {value} and {value} , then merge their values together',
    'SUBTRACT': 'perform a subtraction between {value} and {value} , the first one being the minuend'
}

path = sys.argv[1]

with open(path, 'w') as read_file:
    read_file.write("// this header file is auto-generated using generate-parser-patterns.py script\n\n#pragma once\n")
    for key, val in patterns.items():
        pattern_list = '{ '
        for tok in val.split():
            if tok == '{value}':
                pattern_list += 'Match(MatchType::VALUE), '
            elif tok == '{quantity}':
                pattern_list += 'Match(MatchType::QUANTITY), '
            elif tok == '{id}':
                pattern_list += 'Match(MatchType::ID), '
            else:
                pattern_list += f'Match(\"{tok}\"), '
        read_file.write(f'#define PATTERN_{key} {pattern_list}' + ' }\n')