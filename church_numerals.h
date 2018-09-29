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

namespace Church{

    template<typename R>
    using church_number_func_t = std::function<R(R)>;

    template<typename R>
    using church_number_t = std::function<church_number_func_t<R>(church_number_func_t<R>)>;

    using church_number_func_t_void = std::function<void()>;
    using church_number_t_void = std::function<church_number_func_t_void(church_number_func_t_void)>;

    template<typename R>
    auto church_one = church_number_t<R>([](church_number_func_t<R> f)->church_number_func_t<R>{
        return [f](R x)->R{
            return f(x);
        };
    });

    template<>
    auto church_one<void> = church_number_t_void([](church_number_func_t_void f)->church_number_func_t_void{
        return [f]()->void{
            return f();
        };
    });

    template<typename R>
    auto church_zero = church_number_t<R>([](church_number_func_t<R> f)->church_number_func_t<R>{
        return [f](R x)->R{
            return x;
        };
    });

    template<>
    auto church_zero<void> = church_number_t_void([](church_number_func_t_void f)->church_number_func_t_void{
        return [f]()->void{
            ;
        };
    });

    template<typename R>
    constexpr church_number_t<R> church_add(church_number_t<R> first,church_number_t<R> second){
        return [first,second](church_number_func_t<R> f)->church_number_func_t<R>{
            return [f,first,second](R x)->R{
                return first(f)(second(f)(x)); 
            };
        };
    }

    template<typename R>
    constexpr church_number_t<R> church_mult(church_number_t<R> first,church_number_t<R> second){
        return [first,second](church_number_func_t<R> f)->church_number_func_t<R>{
            return [f,first,second](R x)->R{
                return first(second(f))(x); 
            };
        };
    }

    template<typename R>
    constexpr church_number_t<R> church_pred(church_number_t<R> num){
        return [num](church_number_func_t<R> f)->church_number_func_t<R>{
            return [num,f](R x)->R{
                bool indicator = true;

                auto pred_func = [&indicator,f](R x)->R{
                    if (indicator){
                        indicator = false;
                        return x;
                    } 
                    return f(x);
                };

                auto retval = num(pred_func)(x);
                return retval;
            };
        };
    }

}//namespace Church

#endif
