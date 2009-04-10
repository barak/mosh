/*
 * OSCompat.h -
 *
 *   Copyright (c) 2009  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
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
 *  $Id: OScompat.h 261 2008-07-25 06:16:44Z higepon $
 */

#ifndef SCHEME_OSCOMPAT_
#define SCHEME_OSCOMPAT_

#include "scheme.h"

namespace scheme {

    ucs4char** getCommandLine(int argc, char* argv[]);
    ucs4char* getEnv(const ucs4string& key);
    Object getEnvAlist();
    Object readDirectory(const ucs4string& dir);
    ucs4string stringError(int num);
    ucs4string getMoshExecutablePath(bool& isErrorOccured);

    class File EXTEND_GC
    {
    public:
        File();
        File(int desc);
        virtual ~File();

        bool open(const ucs4string& file, int flags, int mode);
        bool isOpen() const;
        void close();
        int write(uint8_t* buf, size_t size);
        int read(uint8_t* buf, size_t size);
        int64_t seek(int64_t offset, int whence);
        int dup(int target);
        int64_t size() const;

        static bool isExist(const ucs4string& path);
        static bool isWritable(const ucs4string& path);
        static bool isReadable(const ucs4string& path);

    private:
        int desc_;
        bool isOpen_;
    };
}; // namespace scheme

#endif // SCHEME_OSCOMPAT_