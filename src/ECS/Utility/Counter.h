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

        static Int next()
        {
            return s_counter++;
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
            // hotfix, makes Counter a bit redundant
            static const Int s_ID = Counter<Int, Group>::next();

            return s_ID;
        }
    };
}