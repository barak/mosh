/*
 * reader.cpp -
 *
 *   Copyright (c) 2008  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  $Id: reader.cpp 183 2008-07-04 06:19:28Z higepon $
 */

#include "Object.h"
#include "Object-inl.h"
#include "Pair.h"
#include "Pair-inl.h"
#include "SString.h"
#include "TextualInputPort.h"
#include "TextualOutputPort.h"
#include "Reader.h"
#include "EqHashTable.h"
#include "ProcedureMacro.h"
#include "Scanner.h"

using namespace scheme;

Object ReaderContext::read(TextualInputPort* port, bool& errorOccured)
{
    extern int yyparse ();
    MOSH_ASSERT(port);
    port_ = port;
    for (;;) {
        const bool isParseError = yyparse() == 1;
        port->scanner()->emptyBuffer();
        if (isParseError) {
            errorOccured = true;
            return Object::Undef;
        }
        // undef means #; ignored datum
        if (!parsed_.isUndef()) {
            return parsed_;
        }
    }
}

ucs4string ReaderHelper::readSymbol(const ucs4string& s)
{
    ucs4string ret;
    for (ucs4string::const_iterator it = s.begin(); it != s.end(); ++it) {
        const ucs4char ch = *it;
        if (ch == '\\') {
            const ucs4char ch2 = *(++it);
            if (it == s.end()) break;
            switch (ch2)
            {
            case 'x':
            {
                ucs4char currentChar = 0;
                for (int i = 0; ; i++) {
                    const ucs4char hexChar = *(++it);
                    if (it == s.end()) {
                        fprintf(stderr, "invalid \\x in symbol end");
                        break;
                    } else if (hexChar == ';') {
                        ret += currentChar;
                        break;
                    } else if (isdigit(hexChar)) {
                        currentChar = (currentChar << 4) | (hexChar - '0');
                    } else if ('a' <= hexChar && hexChar <= 'f') {
                        currentChar = (currentChar << 4) | (hexChar - 'a' + 10);
                    } else if ('A' <= hexChar && hexChar <= 'F') {
                        currentChar = (currentChar << 4) | (hexChar - 'A' + 10);
                    } else {
                        fprintf(stderr, "invalid \\x in symbol <%c>", (char)hexChar);
                        break;
                    }
                }
                break;
            }
            default:
                ret += ch;
                ret += ch2;
                break;
            }
        } else {
            ret += ch;
        }
    }
    return ret;
}

ucs4string ReaderHelper::readString(const ucs4string& s)
{
    ucs4string ret;
    for (ucs4string::const_iterator it = s.begin(); it != s.end(); ++it) {
        const ucs4char ch = *it;
        if (ch == '\\') {
            const ucs4char ch2 = *(++it);
            if (it == s.end()) break;
            switch (ch2)
            {
            case '"':
                ret += '"';
                break;
            case '\\':
                ret += '\\';
                break;
            case 'a':
                ret += 0x07;
                break;
            case 'b':
                ret += 0x08;
                break;
            case 't':
                ret += 0x09;
                break;
            case 'n':
                ret += 0x0a;
                break;
            case 'v':
                ret += 0x0b;
                break;
            case 'f':
                ret += 0x0c;
                break;
            case 'r':
                ret += 0x0d;
                break;
            case 'x':
            {
                ucs4char currentChar = 0;
                for (int i = 0; ; i++) {
                    const ucs4char hexChar = *(++it);
                    if (it == s.end()) {
                        fprintf(stderr, "invalid \\x in string end");
                        break;
                    } else if (hexChar == ';') {
                        ret += currentChar;
                        break;
                    } else if (isdigit(hexChar)) {
                        currentChar = (currentChar << 4) | (hexChar - '0');
                    } else if ('a' <= hexChar && hexChar <= 'f') {
                        currentChar = (currentChar << 4) | (hexChar - 'a' + 10);
                    } else if ('A' <= hexChar && hexChar <= 'F') {
                        currentChar = (currentChar << 4) | (hexChar - 'A' + 10);
                    } else {
                        fprintf(stderr, "invalid \\x in string <%c>", (char)hexChar);
                        break;
                    }
                }
                break;
            }
            default:
                /* \<intraline whitespace>*<line ending>
                   <intraline whitespace>*
                   NB: Lexical syntax has already checked by the scanner. */
                for (;;) {
                    switch (*it) {
                    /* <line ending> */
                    case '\r':
                    case '\n':
                    case 0x0085: // next line
                    case 0x2028: // line separator
                    /* <intraline whitespace> */
                    case '\t':
                    /* <Unicode Zs> */
                    case 0x0020:
                    case 0x00a0:
                    case 0x1680:
                    case 0x180e:
                    case 0x2000: case 0x2001: case 0x2002: case 0x2003:
                    case 0x2004: case 0x2005: case 0x2006: case 0x2007:
                    case 0x2008: case 0x2009: case 0x200a:
                    case 0x202f:
                    case 0x205f:
                    case 0x3000:
                        ++it;
                        continue;
                        break;  /* NOTREACHED */
                    default:
                        ;       // do nothing
                    }
                    --it;
                    break;
                }
                break;
            }
        } else {
            ret += ch;
        }
    }
    return ret;
}
