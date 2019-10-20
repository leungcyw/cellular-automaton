#include "Automaton.h"

Automaton::Automaton(int rule) {
  setRule(rule);
  resetToFirstGen();
  displayWidth = DEFAULT_DISPLAY_WIDTH;
}

bool Automaton::setRule(int rule) {
  if (rule < MIN_RULE || rule > MAX_RULE) {
    return false;
  }
  rules[0] = (rule & 128) >> 7;
  rules[1] = (rule & 64) >> 6;
  rules[2] = (rule & 32) >> 5;
  rules[3] = (rule & 16) >> 4;
  rules[4] = (rule & 8) >> 3;
  rules[5] = (rule & 4) >> 2;
  rules[6] = (rule & 2) >> 1;
  rules[7] = rule & 1;
  return true;
}

bool Automaton::setDisplayWidth(int width) {
  if (width % 2 == 0 || width > MAX_DISPLAY_WIDTH || width < 1) {
      return false;
  }
  displayWidth = width;
  return true;
}

void Automaton::resetToFirstGen() {
  thisGen.clear();
  thisGen.push_back(1);
  extremeBit = 0;
}

void Automaton::propagateNewGen() {
  // Pad with spaces
  for (int i = 0; i < 2; i++) {
    thisGen.insert(thisGen.begin(), extremeBit);
    thisGen.insert(thisGen.end(), extremeBit);
  }

  // Calculate nextGen
  vector<int> nextGen;

  for (int i = thisGen.size() - 2; i >= 1; i--) {
    int num = thisGen[i - 1];
    num = num << 1;
    num = num | thisGen[i];
    num = num << 1;
    num = num | thisGen[i + 1];
    int ind = 7 - num;
    nextGen.insert(nextGen.begin(), rules[ind]);
  }
  thisGen = nextGen;

  // Set new extremeBit
  bool tmp;
  if (extremeBit == 0) {
    tmp = rules[7];
  } else {
    tmp = rules[0];
  }
  extremeBit = tmp;
  return;
}

string Automaton::toStringCurrentGen(char charFor0, char charFor1) const {
  std::stringstream ss;
  if (thisGen.size() < (unsigned int)displayWidth) {
    int pad = (displayWidth - thisGen.size()) / 2;
    for (int i = 0; i < pad; i++) {
      if (extremeBit == 1) {
        ss << charFor1;
      } else {
        ss << charFor0;
      }
    }
    for (unsigned int j = 0; j < thisGen.size(); j++) {
      if (thisGen[j] == 1) {
        ss << charFor1;
      } else {
        ss << charFor0;
      }
    }
    for (int i = 0; i < pad; i++) {
      if (extremeBit == 1) {
        ss << charFor1;
      } else {
        ss << charFor0;
      }
    }
    return ss.str();
  }


  if (thisGen.size() > (unsigned int)displayWidth) {
    unsigned int pad = (thisGen.size() - displayWidth) / 2;
    unsigned int upper = pad + displayWidth;
    for (unsigned int j = 0; j < thisGen.size(); j++) {
      if (j >= pad && j < upper) {
        if (thisGen[j] == 1) {
          ss << charFor1;
        } else {
          ss << charFor0;
        }
      }
    }
    return ss.str();
  }


  for (unsigned int j = 0; j < thisGen.size(); j++) {
      if (thisGen[j] == 1) {
        ss << charFor1;
      } else {
        ss << charFor0;
      }
  }
  return ss.str(); 
}
