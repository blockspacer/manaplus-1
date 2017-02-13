/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2017  The ManaPlus Developers
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

#ifndef RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H
#define RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H

#include "resources/memorycounter.h"

#include <map>
#include <set>

#include "localconsts.h"

class Resource;

struct SDL_Surface;
struct SDL_RWops;

/**
 * A class for loading and managing resources.
 */
class ResourceManager final : public MemoryCounter
{
    friend class Resource;

    public:
        typedef Resource *(*loader)(SDL_RWops *rw,
                                    const std::string &name);
        typedef Resource *(&generator)(const void *const data);

        ResourceManager();

        A_DELETE_COPY(ResourceManager)

        /**
         * Destructor. Cleans up remaining resources, warning about resources
         * that were still referenced.
         */
        ~ResourceManager();

        /**
         * Creates a resource and adds it to the resource map.
         *
         * @param idPath The resource identifier path.
         * @param fun    A function for generating the resource.
         * @param data   Extra parameters for the generator.
         * @return A valid resource or <code>NULL</code> if the resource could
         *         not be generated.
         */
        Resource *get(const std::string &idPath,
                      generator fun,
                      const void *const data) A_WARN_UNUSED;

        Resource *getFromCache(const std::string &idPath) A_WARN_UNUSED;

        Resource *getFromCache(const std::string &filename,
                               const int variant) A_WARN_UNUSED;

        bool addResource(const std::string &idPath,
                         Resource *const resource);

        /**
         * Releases a resource, placing it in the set of orphaned resources.
         */
        void release(Resource *const res);

        void clearDeleted(const bool full = true);

        void decRefDelete(Resource *const res);

        /**
         * Move resource to deleted resources list.
         */
        void moveToDeleted(Resource *const res);

        void scheduleDelete(SDL_Surface *const surface);

        void clearScheduled();

        /**
         * Deletes the class instance if it exists.
         */
        static void deleteInstance();

        int size() const noexcept2 A_WARN_UNUSED
        { return CAST_S32(mResources.size()); }

        typedef std::map<std::string, Resource*> Resources;
        typedef Resources::iterator ResourceIterator;
        typedef Resources::const_iterator ResourceCIterator;

#ifdef DEBUG_DUMP_LEAKS
        Resources* getResources() A_WARN_UNUSED
        { return &mResources; }

        Resources* getOrphanedResources() A_WARN_UNUSED
        { return &mOrphanedResources; }
#endif  // DEBUG_DUMP_LEAKS

        bool cleanOrphans(const bool always = false);

        void cleanProtected();

        bool isInCache(const std::string &idPath) const A_WARN_UNUSED;

        Resource *getTempResource(const std::string &idPath) A_WARN_UNUSED;

        void clearCache();

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        std::string getCounterName() const override final
        { return "ResourceManager"; }

        static void init();

#ifndef UNITTESTS
    private:
#endif  // UNITTESTS
        /**
         * Deletes the resource after logging a cleanup message.
         */
        static void cleanUp(Resource *const resource);

        static void logResource(const Resource *const res);

        std::set<SDL_Surface*> deletedSurfaces;
        Resources mResources;
        Resources mOrphanedResources;
        std::set<Resource*> mDeletedResources;
        time_t mOldestOrphan;
        bool mDestruction;
};

extern ResourceManager *resourceManager;

#endif  // RESOURCES_RESOURCEMANAGER_RESOURCEMANAGER_H
