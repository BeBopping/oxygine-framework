#pragma once
#include "oxygine-include.h"
#include "core/ref_counter.h"
#include "core/intrusive_ptr.h"
#include "core/Mutex.h"
#include <vector>
#include <unordered_map>

namespace oxygine
{
    inline void hash_combine(std::size_t& seed) { }

    template <typename T, typename... Rest>
    inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        hash_combine(seed, rest...);
    }

    class MaterialX;
    typedef intrusive_ptr<MaterialX> spMaterialX;

    class MaterialCache
    {
    public:
        template<class T>
        intrusive_ptr<T> cache(const T& other)
        {
            return (T*)clone_(other);
        }

        void clear();
        void removeUnused();
        size_t getTotalMaterials() const { return _materials.size(); }

    protected:
        typedef std::unordered_multimap<size_t, spMaterialX> materials;
        materials _materials;

        Mutex _lock;

        MaterialX* clone_(const MaterialX& other);
    };

    MaterialCache& mc();
}