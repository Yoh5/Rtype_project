#pragma once

#include "dependencies.hpp"

namespace BJENGINE
{

    template <typename Component>
    class sparse_array
    {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std ::vector<value_type>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    public:
        sparse_array()
        {
            std::fill(_data.begin(), _data.end(), std::nullopt);
            _size = 0;
        }
        sparse_array(sparse_array const &) {}
        sparse_array(sparse_array &&) noexcept {}
        ~sparse_array() {}
        sparse_array &operator=(sparse_array const &) {}
        sparse_array &operator=(sparse_array &&) noexcept {}
        reference_type operator[](size_t idx)
        {
            return _data[idx];
        }

        const_reference_type operator[](size_t idx) const
        {
            return _data[idx];
        }

        bool contains(int idx)
        {
            return (idx < size());
        }

        iterator begin()
        {
            return _data.begin();
        }
        const_iterator begin() const
        {
            return _data.begin();
        }
        const_iterator cbegin() const
        {
            return _data.cbegin();
        }
        iterator end()
        {
            return _data.end();
        }
        const_iterator end() const
        {
            return _data.end();
        }
        const_iterator cend() const
        {
            return _data.cend();
        }
        size_type size() const
        {
            return _data.size();
        }
        void insert_at(size_type pos, Component &component)
        {
            while (size() <= pos)
                _data.push_back(std::nullopt);
            _data[pos] = std::move(component);
            _size++;
        }
        void erase(size_type pos)
        {
            if ((pos < 0) || (pos >= size()))
            {
                throw std::exception();
            }
            _data[pos] = std::nullopt;
        }
        size_type get_index(value_type const &component) const
        {
            for (int i = 0; i < size(); i++)
            {
                if (std::addressof(component) == std::addressof(_data[i]))
                    return i;
            }
            throw std::exception();
        }

    private:
        container_t _data;
        size_type _size;
    };
}
