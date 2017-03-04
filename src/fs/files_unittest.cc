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

#include "fs/files.h"

#include "catch.hpp"
#include "logger.h"

#include "fs/virtfs.h"
#include "fs/virtfstools.h"

#include "utils/delete2.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

TEST_CASE("Files renameFile")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);

    const int sz = 1234567;
    char *buf = new char[sz];
    for (int f = 0; f < sz; f ++)
        buf[f] = f;

    const std::string name1 = "file1.test";
    const std::string name2 = "file2.test";
    FILE *file = fopen(name1.c_str(), "w+b");
    fwrite(buf, 1, sz, file);
    fclose(file);

    REQUIRE(0 == Files::renameFile(name1, name2));
    char *buf2 = new char[sz];
    FILE *file2 = fopen(name2.c_str(), "rb");
    REQUIRE_FALSE(nullptr == file2);
    fread(buf2, 1, sz, file2);
    fclose(file2);
    ::remove(name1.c_str());
    ::remove(name2.c_str());

    for (int f = 0; f < sz; f ++)
        REQUIRE(buf[f] == buf2[f]);

    delete [] buf;
    delete [] buf2;
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("Files existsLocal")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);
    REQUIRE(Files::existsLocal(VirtFs::getPath("help/about.txt")) == true);
    REQUIRE_FALSE(Files::existsLocal(VirtFs::getPath("help/about1.txt")));
    REQUIRE_FALSE(Files::existsLocal(VirtFs::getPath("help1/about.txt")));
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("Files loadTextFileString")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);
    REQUIRE(VirtFs::loadTextFileString("test/simplefile.txt") ==
        "this is test \nfile.");
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("Files loadTextFile")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);

    StringVect lines;
    VirtFs::loadTextFile("test/simplefile.txt", lines);
    REQUIRE(lines.size() == 2);
    REQUIRE(lines[0] == "this is test ");
    REQUIRE(lines[1] == "file.");
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("Files saveTextFile")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);

    const std::string dir = VirtFs::getPath("test");
    REQUIRE(dir.size() > 0);
    Files::saveTextFile(dir, "tempfile.txt", "test line\ntext line2");
    std::string data = VirtFs::loadTextFileString("test/tempfile.txt");
    ::remove((dir + "/tempfile.txt").c_str());
    REQUIRE(data == "test line\ntext line2\n");
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}

TEST_CASE("Files getFilesInDir")
{
    dirSeparator = "/";
    logger = new Logger();
    ResourceManager::init();
    VirtFs::addDirToSearchPathSilent("data", Append_false);
    VirtFs::addDirToSearchPathSilent("../data", Append_false);

    StringVect list;
    VirtFs::getFilesInDir("test",
        list,
        ".gpl");
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == "test/palette.gpl");

    list.clear();
    VirtFs::getFilesInDir("perserver/default",
        list,
        ".xml");
    REQUIRE(list.size() == 5);
    REQUIRE(list[0] == "perserver/default/charcreation.xml");
    REQUIRE(list[1] == "perserver/default/deadmessages.xml");
    REQUIRE(list[2] == "perserver/default/defaultcommands.xml");
    REQUIRE(list[3] == "perserver/default/features.xml");
    REQUIRE(list[4] == "perserver/default/weapons.xml");
    ResourceManager::deleteInstance();
    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    delete2(logger);
//    VirtFs::deinit();
}