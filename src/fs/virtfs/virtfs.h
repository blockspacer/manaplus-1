/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FS_VIRTFS_H
#define FS_VIRTFS_H

#include "enums/fs/fsentrytype.h"

#include "enums/simpletypes/append.h"

#include "localconsts.h"

#include <string>
#include <vector>

struct VirtFile;
struct VirtFsEntry;
struct VirtList;

namespace VirtFs
{
    void init(const std::string &restrict name);
    void updateDirSeparator();
    const char *getDirSeparator();
    const char *getBaseDir();
    const char *getUserDir();
    bool exists(std::string name);
    VirtList *enumerateFiles(std::string dir) RETURNS_NONNULL;
    bool isDirectory(std::string name);
    bool isSymbolicLink(const std::string &restrict name);
    void freeList(VirtList *restrict const handle);
    VirtFile *openRead(std::string filename);
    VirtFile *openWrite(std::string filename);
    VirtFile *openAppend(std::string filename);
    bool setWriteDir(const std::string &restrict newDir);
    bool mountDir(std::string newDir,
                  const Append append);
    bool mountDirSilent(std::string newDir,
                        const Append append);
    bool unmountDir(std::string oldDir);
    bool unmountDirSilent(std::string oldDir);
    bool mountZip(std::string newDir,
                  const Append append);
    bool unmountZip(std::string oldDir);
    std::string getRealDir(std::string filename);
    bool mkdir(const std::string &restrict dirName);
    bool remove(const std::string &restrict filename);
    bool deinit();
    void permitLinks(const bool val);
    int64_t read(VirtFile *restrict const handle,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(VirtFile *restrict const file);
    int64_t fileLength(VirtFile *restrict const file);
    int64_t tell(VirtFile *restrict const file);
    int seek(VirtFile *restrict const file,
             const uint64_t pos);
    int eof(VirtFile *restrict const file);

    bool mountDirInternal(const std::string &restrict newDir,
                          const Append append);
    bool unmountDirInternal(std::string oldDir);
    std::vector<VirtFsEntry*> &getEntries();
    VirtFsEntry *searchEntryByRootInternal(const std::string &restrict
                                           root);
    VirtFsEntry *searchEntryInternal(const std::string &restrict root,
                                     const FsEntryTypeT type);
    void addEntry(VirtFsEntry *const entry,
                  const Append append);
#ifdef UNITTESTS
    bool mountDirSilent2(std::string newDir,
                         const Append append);
#endif  // UNITTESTS
    char *loadFile(const std::string &restrict fileName,
                   int &restrict fileSize);
}  // namespace VirtFs

extern const char *dirSeparator;

#endif  // FS_VIRTFS_H
