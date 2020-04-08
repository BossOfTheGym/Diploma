#pragma once

namespace ecs::util
{
    template <typename Int, typename Group>
    class Counter
    {
    public:
        static Int get()
        {
            return s_counter;
        }

    protected:
        static Int s_counter;
    };

    template <typename Int, typename Group>
    Int Counter<Int, Group>::s_counter = 0;


    template <typename Int, typename T, typename Group = void>
    class TypeCounter : public Counter<Int, Group>
    {
    public:
        static Int get() 
        {
            return s_ID;
        }

    private:
        static const Int s_ID;
    };

    template <typename Int, typename T, typename Group>
    const Int TypeCounter<Int, T, Group>::s_ID = Counter<Int, Group>::s_counter++;
}