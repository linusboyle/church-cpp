/**
 * church_numerals.h
 * Copyright (c) 2018 Linus Boyle <linusboyle@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHURCH_NUMERALS_H
#define CHURCH_NUMERALS_H

#include <functional>

namespace Church {

    constexpr auto church_true = [](auto&& first) constexpr{
        return [&](auto&&) constexpr {
            return first;
        };
    };//logic true

    constexpr auto church_false = [](auto&&) constexpr{
        return [](auto&& second) constexpr {
            return second;
        };
    };//logic false

    constexpr auto church_and = [](auto&& lboolean) constexpr {
        return [&](auto&& rboolean) constexpr{
            return lboolean(rboolean)(lboolean);
        };
    };//logic and

    constexpr auto church_or = [](auto&& lboolean) constexpr {
        return [&](auto&& rboolean) constexpr{
            return lboolean(lboolean)(rboolean);
        };
    };//logic or 

    constexpr auto church_not = [](auto&& boolean) constexpr{
        return boolean(church_false)(church_true);
    };//logic not

    constexpr auto church_xor = [](auto&& lboolean) constexpr {
        return [&](auto&& rboolean) constexpr{
            return lboolean(church_not(rboolean))(rboolean);
        };
    };//logic xor

    constexpr auto church_if = [](auto&& boolean) constexpr {
        return [&](auto&& thenclause) constexpr {
            return [&](auto&& elseclause) constexpr {
                return boolean(thenclause,elseclause);
            };
        };
    };//if-then-else clause

    constexpr auto church_one = [](auto&& f) constexpr {
        return [=](auto&&... params) constexpr {
            static_assert(sizeof...(params)<=1,"parameters exceed limit");
            return f(params...);
        }; 
    };//church numeral one

    constexpr auto church_zero = [](auto&&) constexpr {
        return [](auto&&... params) constexpr {
            static_assert(sizeof...(params)<=1,"parameters exceed limit");
            if constexpr(sizeof...(params)==0){
                return;
            }
            else {
                constexpr auto retval = std::get<0>(std::make_tuple(params...));
                return retval;
            }
        };
    };//church numeral zero

    constexpr auto church_succ = [](auto&& num) constexpr {
        return [=](auto&& f) constexpr{
            return [=](auto&&... params) constexpr {
                if constexpr(sizeof...(params)!=0)
                    return f(num(f)(params...));
                else{
                    f();
                    num(f)();
                }
            };
        };
    };//church successor

    constexpr auto church_add = [](auto&& first,auto&& second) constexpr {
        return [=](auto&& f) constexpr {
            return [=](auto&&... params) constexpr {
                if constexpr(sizeof...(params)!=0)
                    return first(f)(second(f)(params...));
                else{
                    first(f)();
                    second(f)();
                }
            };
        };
    };//numeral add

    constexpr auto church_mult = [](auto&& first,auto&& second) constexpr {
        return [=](auto&& f) constexpr {
            return [=](auto&&... params) constexpr {
                return first(second(f))(params...); 
            };
        };
    };//numeral mult

    constexpr auto church_pred = [](auto&& num) constexpr {
        return [=](auto&& f) constexpr {
            return [=](auto&&... params) constexpr {
                bool indicator = true;

                auto pred_func = [&](auto&&... p) constexpr{
                    if (indicator){
                        indicator = false;
                        if constexpr(sizeof...(p)==0){
                            return;
                        } else{
                            return std::get<0>(std::make_tuple(p...));
                        }
                    } 
                    return f(p...);
                };

                auto retval = num(pred_func)(params...);
                return retval;
            };
        };
    };//numeral predecessor

    constexpr auto church_minus = [](auto&& first,auto&& second) constexpr {
        return second(church_pred)(first);
    };//numeral substraction

    //constexpr auto church_expo = [](auto&& first,auto&& second) constexpr {
        //return second(first);
    //};//numeral exponentiation,i.e first^second

    constexpr auto church_iszero = [](auto&& num) constexpr {
        constexpr auto test_func = [](auto&&) constexpr->decltype(church_false){
            return church_false;
        };

        return num(test_func)(church_true);
    };//is zero

    constexpr auto church_leq = [](auto&& first,auto&& second) constexpr {
        return church_iszero(church_minus(first,second));
    };//less than or equal

    constexpr auto church_eq = [](auto&& first,auto&& second) constexpr {
        return church_and(church_leq(first,second))(church_leq(second,first));
    };//equal

    //YOU CAN WRITE YOUR OWN IMPL OF LESSTHAN .etc LIKE THIS,SO HERE IGNORED
    
}//namespace Church

#endif
