// this header file is auto-generated using gen-parser-pattern.py script

#pragma once
#define PATTERN_VAR_DECL { Match("create"), Match("a"), Match("unique"), Match(","), Match("uninitialized"), Match("variable"), Match("and"), Match("name"), Match("it"), Match(MatchType::ID),  }
#define PATTERN_VAR_ASSIGN { Match("take"), Match("the"), Match("value"), Match("of"), Match(MatchType::VALUE), Match("and"), Match("assign"), Match("it"), Match("to"), Match("the"), Match("variable"), Match(MatchType::ID),  }
#define PATTERN_LABEL { Match("this"), Match("place"), Match("must"), Match("be"), Match("referred"), Match("to"), Match("as"), Match(MatchType::ID), Match("so"), Match("that"), Match("we"), Match("can"), Match("find"), Match("it"), Match("when"), Match("we"), Match("need"), Match("to"),  }
#define PATTERN_GO { Match("go"), Match("upwards"), Match("and"), Match("downwards"), Match("until"), Match("you"), Match("find"), Match("a"), Match("place"), Match("that"), Match("is"), Match("referred"), Match("to"), Match("as"), Match(MatchType::ID),  }
#define PATTERN_CALL { Match("find"), Match("a"), Match("place"), Match("that"), Match("is"), Match("referred"), Match("to"), Match("as"), Match(MatchType::ID), Match(","), Match("however"), Match("return"), Match("back"), Match("here"), Match("when"), Match("you"), Match("are"), Match("told"), Match("to"), Match("do"), Match("so"),  }
#define PATTERN_RET { Match("return"), Match("back"), Match("to"), Match("the"), Match("place"), Match("that"), Match("you"), Match("promised"), Match("to"), Match("return"), Match("back"),  }
#define PATTERN_GO_UP { Match("go"), Match("upwards"), Match("until"), Match("you"), Match("find"), Match("the"), Match("place"), Match("that"), Match("is"), Match("referred"), Match("to"), Match("as"), Match(MatchType::ID),  }
#define PATTERN_GO_DOWN { Match("go"), Match("downwards"), Match("until"), Match("you"), Match("find"), Match("the"), Match("place"), Match("that"), Match("is"), Match("referred"), Match("to"), Match("as"), Match(MatchType::ID),  }
#define PATTERN_IF_SKIP { Match("if"), Match("it"), Match("happens"), Match("to"), Match("be"), Match("that"), Match(MatchType::VALUE), Match(MatchType::ID), Match(MatchType::VALUE), Match("ignore"), Match("the"), Match("following"), Match(MatchType::QUANTITY), Match("line/lines"),  }
#define PATTERN_UNLESS_SKIP { Match("unless"), Match("it"), Match("happens"), Match("to"), Match("be"), Match("that"), Match(MatchType::VALUE), Match(MatchType::ID), Match(MatchType::VALUE), Match("ignore"), Match("the"), Match("following"), Match(MatchType::QUANTITY), Match("line/lines"),  }
#define PATTERN_INPUT { Match("wait"), Match("right"), Match("here"), Match("until"), Match("there"), Match("is"), Match("a"), Match("useful"), Match("ingress"), Match("from"), Match("the"), Match("user"),  }
#define PATTERN_WRITE_LN { Match("write"), Match("the"), Match("line"), Match(MatchType::VALUE), Match("to"), Match("the"), Match("console"), Match(","), Match("skipping"), Match("to"), Match("the"), Match("new"), Match("line"), Match("afterwards"),  }
#define PATTERN_WRITE { Match("write"), Match("the"), Match("line"), Match(MatchType::VALUE), Match("to"), Match("the"), Match("console"), Match(","), Match("do"), Match("not"), Match("skip"), Match("to"), Match("the"), Match("new"), Match("line"),  }
#define PATTERN_DIVIDE { Match("take"), Match("the"), Match("value"), Match("of"), Match(MatchType::VALUE), Match("and"), Match("divide"), Match("it"), Match("using"), Match("the"), Match("value"), Match("of"), Match(MatchType::VALUE),  }
#define PATTERN_MULTIPLY { Match("repeat"), Match("the"), Match("string"), Match("or"), Match("the"), Match("number"), Match(MatchType::VALUE), Match("an"), Match("amount"), Match("of"), Match("times"), Match("equal"), Match("to"), Match("the"), Match("number"), Match(MatchType::VALUE),  }
#define PATTERN_ADD { Match("take"), Match("two"), Match("numbers"), Match("or"), Match("strings"), Match(","), Match(MatchType::VALUE), Match("and"), Match(MatchType::VALUE), Match(","), Match("then"), Match("merge"), Match("their"), Match("values"), Match("together"),  }
#define PATTERN_SUBTRACT { Match("perform"), Match("a"), Match("subtraction"), Match("between"), Match(MatchType::VALUE), Match("and"), Match(MatchType::VALUE), Match(","), Match("the"), Match("first"), Match("one"), Match("being"), Match("the"), Match("minuend"),  }
