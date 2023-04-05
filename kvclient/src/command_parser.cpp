#include "kvclient/CommandParser.hpp"

using namespace kvclient;

TParseCommand CommandParser::parseLine(const std::string &line) {
  std::vector<std::string> tokens = parseTokens(line);

  if (tokens.size() == 0) {
    throw InvalidCommandException("Please specify a command");
  }

  std::string command = tokens[0];

  if (command == SET) {
    return parseSet(tokens);
  } else if (command == DEL) {
    return parseDel(tokens);
  } else if (command == GET) {
    return parseGet(tokens);
  } else if (command == HSET) {
    return parseHSet(tokens);
  } else if (command == HGET) {
    return parseHGet(tokens);
  } else if (command == HDEL) {
    return parseHDel(tokens);
  } else if (command == QUIT) {
    return QuitCommand{};
  } else {
    throw InvalidCommandException("Please specify a proper command");
  }
}

std::vector<std::string> CommandParser::parseTokens(const std::string &line) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(line);
  bool in_quotes = false;
  char quote_char = '\0';
  char c;

  while (iss.get(c)) {
    if ((c == '"' || c == '\'') && (!in_quotes || quote_char == c)) {
      in_quotes = !in_quotes;
      if (in_quotes) {
        quote_char = c;
      } else {
        quote_char = '\0';
      }
      continue;
    }

    if (c == '\\') {
      if (in_quotes &&
          (iss.peek() == '"' || iss.peek() == '\'' || iss.peek() == '\\')) {
        iss.get(c);
      } else if (in_quotes) {
        throw InvalidCommandException("ERR syntax error");
      }
    } else if (std::isspace(c) && !in_quotes) {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      continue;
    }

    token.push_back(c);
  }

  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

SetCommand CommandParser::parseSet(const std::vector<std::string> &tokens) {
  if (tokens.size() != 3) {
    throw InvalidCommandException(
        "Invalid SET command format. Format: SET key value");
  }
  return SetCommand{tokens[1], tokens[2]};
}

GetCommand CommandParser::parseGet(const std::vector<std::string> &tokens) {
  if (tokens.size() != 2) {
    throw InvalidCommandException(
        "Invalid GET command format. Format: GET key");
  }
  return GetCommand{tokens[1]};
}

DelCommand CommandParser::parseDel(const std::vector<std::string> &tokens) {
  if (tokens.size() == 1) {
    throw InvalidCommandException(
        "Invalid DEL command format. Format: DEL key [key ...]");
  }
  return DelCommand{std::vector<std::string>(tokens.begin() + 1, tokens.end())};
}

HSetCommand CommandParser::parseHSet(const std::vector<std::string> &tokens) {
  if (tokens.size() % 2 == 1 || tokens.size() < 4) {
    throw InvalidCommandException(
        "Invalid HSET command format. Format: HSET key field value [field "
        "value ...]");
  }
  std::vector<std::pair<std::string, std::string>> arg2;
  int n = tokens.size();
  for (int i = 2; i < n - 1; i += 2) {
    arg2.push_back({tokens[i], tokens[i + 1]});
  }
  return HSetCommand{tokens[1], arg2};
}

HGetCommand CommandParser::parseHGet(const std::vector<std::string> &tokens) {
  if (tokens.size() != 3) {
    throw InvalidCommandException(
        "Invalid HGET command format. Format: HGET key field");
  }
  return HGetCommand{tokens[1], tokens[2]};
}

HDelCommand CommandParser::parseHDel(const std::vector<std::string> &tokens) {
  if (tokens.size() < 3) {
    throw InvalidCommandException(
        "Invalid HDEL command format. Format: HDEL key field [field ...]");
  }
  return HDelCommand{
      tokens[1], std::vector<std::string>(tokens.begin() + 2, tokens.end())};
}