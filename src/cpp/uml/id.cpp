#include "uml/id.h"
#include <stdlib.h>
#include <time.h>

using namespace UML;

ID::ID() {
    for (size_t i = 0; i < 21; i++) {
        m_data[i] = 0;
    }
}

char ID::base64(bool in[6]) const {
    // 1xxxxx
    if (in[0]) {
        // 11xxxx
        if (in[1]) {
            // 111xxx
            if (in[2]) {
                // 1111xx
                if (in[3]) {
                    // 11111x
                    if (in[4]) {
                        // 111111
                        if (in[5]) {
                            return '&';
                        } 
                        // 111110
                        else {
                            return '_';
                        }
                    }
                    // 11110x
                    else {
                        // 111101
                        if (in[5]) {
                            return '9';
                        }
                        // 111100
                        else {
                            return '8';
                        }
                    }
                }
                // 1110xx
                else {
                    // 11101x 
                    if (in[4]) {
                        // 111011
                        if (in[5]) {
                            return '7';
                        }
                        // 111010
                        else {
                            return '6';
                        }
                    }
                    // 11100x
                    else {
                        // 111001
                        if (in[5]) {
                            return '5';
                        }
                        // 111000
                        else {
                            return '4';
                        }
                    }
                }
            } 
            // 110xxx
            else {
                // 1101xx
                if (in[3]) {
                    // 11011x
                    if (in[4]) {
                        // 110111
                        if (in[5]) {
                            return '3';
                        }
                        // 110110
                        else {
                            return '2';
                        }
                    }
                    // 11010x
                    else {
                        // 110101
                        if (in[5]) {
                            return '1';
                        }
                        // 110100
                        else {
                            return '0';
                        }
                    }
                }
                // 1100xx
                else {
                    // 11001x
                    if (in[4]) {
                        // 110011
                        if (in[5]) {
                            return 'z';
                        }
                        // 110010
                        else {
                            return 'y';
                        }
                    }
                    // 11000x
                    else {
                        // 110001
                        if (in[5]) {
                            return 'x';
                        }
                        // 110000
                        else {
                            return 'w';
                        }
                    }
                }
            }
        } 
        // 10xxxx
        else {
            // 101xxx
            if (in[2]) {
                // 1011xx
                if (in[3]) {
                    // 10111x
                    if (in[4]) {
                        // 101111
                        if (in[5]) {
                            return 'v';
                        }
                        // 101110
                        else {
                            return 'u';
                        }
                    }
                    // 10110x
                    else {
                        // 101101
                        if (in[5]) {
                            return 't';
                        }
                        // 101100
                        else {
                            return 's';
                        }
                    }
                }
                // 1010xx
                else {
                    //10101x
                    if (in[4]) {
                        // 101011
                        if (in[5]) {
                            return 'r';
                        }
                        // 101010
                        else {
                            return 'q';
                        }
                    } 
                    // 10100x
                    else {
                        // 101001
                        if (in[5]) {
                            return 'p';
                        }
                        // 101000
                        else {
                            return 'o';
                        }
                    }
                }
            } 
            // 100xxx
            else {
                // 1001xx
                if (in[3]) {
                    // 10011x
                    if (in[4]) {
                        // 100111
                        if (in[5]) {
                            return 'n';
                        }
                        // 100110
                        else {
                            return 'm';
                        }
                    }
                    // 10010x
                    else {
                        // 100101
                        if (in[5]) {
                            return 'l';
                        }
                        // 100100
                        else {
                            return 'k';
                        }
                    }
                }
                // 1000xx
                else {
                    // 10001x
                    if (in[4]) {
                        // 100011
                        if (in[5]) {
                            return 'j';
                        }
                        // 100010
                        else {
                            return 'i';
                        }
                    }
                    // 10000x
                    else {
                        // 100001
                        if (in[5]) {
                            return 'h';
                        }
                        // 100000
                        else {
                            return 'g';
                        }
                    }
                }
            }
        }
    } 
    // 0xxxxx
    else {
        // 01xxxx
        if (in[1]) {
            // 011xxx
            if (in[2]) {
                // 0111xx
                if (in[3]) {
                    // 01111x
                    if (in[4]) {
                        // 011111
                        if (in[5]) {
                            return 'f';
                        } 
                        // 011110
                        else {
                            return 'e';
                        }
                    }
                    // 01110x
                    else {
                        // 011101
                        if (in[5]) {
                            return 'd';
                        }
                        // 011100
                        else {
                            return 'c';
                        }
                    }
                }
                // 0110xx
                else {
                    // 01101x 
                    if (in[4]) {
                        // 011011
                        if (in[5]) {
                            return 'b';
                        }
                        // 011010
                        else {
                            return 'a';
                        }
                    }
                    // 01100x
                    else {
                        // 011001
                        if (in[5]) {
                            return 'Z';
                        }
                        // 011000
                        else {
                            return 'Y';
                        }
                    }
                }
            } 
            // 010xxx
            else {
                // 0101xx
                if (in[3]) {
                    // 01011x
                    if (in[4]) {
                        // 010111
                        if (in[5]) {
                            return 'X';
                        }
                        // 010110
                        else {
                            return 'W';
                        }
                    }
                    // 01010x
                    else {
                        // 010101
                        if (in[5]) {
                            return 'V';
                        }
                        // 010100
                        else {
                            return 'U';
                        }
                    }
                }
                // 0100xx
                else {
                    // 01001x
                    if (in[4]) {
                        // 010011
                        if (in[5]) {
                            return 'T';
                        }
                        // 010010
                        else {
                            return 'S';
                        }
                    }
                    // 01000x
                    else {
                        // 010001
                        if (in[5]) {
                            return 'R';
                        }
                        // 010000
                        else {
                            return 'Q';
                        }
                    }
                }
            }
        } 
        // 00xxxx
        else {
            // 001xxx
            if (in[2]) {
                // 0011xx
                if (in[3]) {
                    // 00111x
                    if (in[4]) {
                        // 001111
                        if (in[5]) {
                            return 'P';
                        }
                        // 001110
                        else {
                            return 'O';
                        }
                    }
                    // 00110x
                    else {
                        // 001101
                        if (in[5]) {
                            return 'N';
                        }
                        // 001100
                        else {
                            return 'M';
                        }
                    }
                }
                // 0010xx
                else {
                    // 00101x
                    if (in[4]) {
                        // 001011
                        if (in[5]) {
                            return 'L';
                        }
                        // 001010
                        else {
                            return 'K';
                        }
                    } 
                    // 00100x
                    else {
                        // 001001
                        if (in[5]) {
                            return 'J';
                        }
                        // 001000
                        else {
                            return 'I';
                        }
                    }
                }
            } 
            // 000xxx
            else {
                // 0001xx
                if (in[3]) {
                    // 00011x
                    if (in[4]) {
                        // 000111
                        if (in[5]) {
                            return 'H';
                        }
                        // 000110
                        else {
                            return 'G';
                        }
                    }
                    // 00010x
                    else {
                        // 000101
                        if (in[5]) {
                            return 'F';
                        }
                        // 000100
                        else {
                            return 'E';
                        }
                    }
                }
                // 0000xx
                else {
                    // 00001x
                    if (in[4]) {
                        // 000011
                        if (in[5]) {
                            return 'D';
                        }
                        // 000010
                        else {
                            return 'C';
                        }
                    }
                    // 00000x
                    else {
                        // 000001
                        if (in[5]) {
                            return 'B';
                        }
                        // 000000
                        else {
                            return 'A';
                        }
                    }
                }
            }
        }
    }

    return '?';
}

std::string ID::string() const {
    std::string ret = "";
    size_t bitIndex = 0;
    size_t dataIndex = 0;
    for (size_t i = 0; i < 28; i++) {
        bool uint6[6];
        for (size_t j = 0; j < 6; j++) {
            uint6[j] = (m_data[dataIndex] >> (bitIndex % 8)) & 1;
            bitIndex++;
            if (bitIndex > 7) {
                bitIndex = 0;
                dataIndex++;
            }
        }
        ret += base64(uint6);
    }
    return ret;
}

uint8_t* ID::getBytes() {
    return m_data;
}

void ID::setBytes(uint8_t* bytes) {
    for (size_t i = 0; i < 21; i++) {
        m_data[i] = *bytes;
        bytes++;
    }
}

bool ID::isNull() const {
    for (size_t i = 0; i < 21; i++) {
        if (m_data[i] != 0) {
            return false;
        }
    }
    return true;
}

ID ID::randomID() {
    ID id;
    for (size_t i = 0; i < 21; i++) {
        id.m_data[i] = rand() % 256;
    }

    return id;
}

ID ID::nullID() {
    ID ret;
    return ret;
}

ID ID::fromString(std::string id) {
    if (id.size() != 28) {
        throw InvalidUmlID_Exception("id is not proper length, must be 28 characters");
    }
    ID ret;
    bool boolArr[168];
    size_t i = 0;
    for (auto const& c : id) {
        if (c == 'A') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'B') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'C') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'D') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'E') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'F') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'G') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'H') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'I') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'J') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'K') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'L') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'M') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'N') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'O') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'P') {
            boolArr[i] = 0;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'Q') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'R') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'S') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'T') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'U') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'V') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'W') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'X') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'Y') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'Z') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'a') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'b') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'c') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'd') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'e') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'f') {
            boolArr[i] = 0;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'g') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'h') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'i') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'j') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'k') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'l') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'm') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'n') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'o') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'p') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'q') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'r') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 's') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 't') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'u') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'v') {
            boolArr[i] = 1;
            boolArr[i+1] = 0;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == 'w') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == 'x') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == 'y') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == 'z') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == '0') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == '1') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == '2') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == '3') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 0;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == '4') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == '5') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == '6') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == '7') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 0;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else if (c == '8') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 0;
        } else if (c == '9') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 0;
            boolArr[i+5] = 1;
        } else if (c == '_') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 0;
        } else if (c == '&') {
            boolArr[i] = 1;
            boolArr[i+1] = 1;
            boolArr[i+2] = 1;
            boolArr[i+3] = 1;
            boolArr[i+4] = 1;
            boolArr[i+5] = 1;
        } else {
            throw InvalidUmlID_Exception(c + " is not a base64 character");
        }

        i+=6;
    }

    for (size_t i = 0; i < 21; i++) {
        uint8_t data = 0;
        for (size_t j = 0; j < 8; j++) {
            if (boolArr[8*i +j]) {
                data |= 1 << j;
            }
        }
        ret.m_data[i] = data;
    }

    return ret;
}