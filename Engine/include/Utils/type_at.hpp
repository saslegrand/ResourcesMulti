#pragma once

template <unsigned N, typename FirstType, typename ...RemainingTypes>
struct type_at
{
    typedef typename type_at<N - 1, FirstType, RemainingTypes...>::type type;
};

template <typename FirstType, typename ...RemainingTypes>
struct type_at<0, FirstType, RemainingTypes...>
{
    typedef FirstType type;
};