// Compute digits of pi using the Rabinowitz and Wagon "spigot" algorithm.
// (Rabinowitz, S. and Wagon, S., A spigot algorithm for the digits of pi,
// American Mathematical Monthly, v. 102, n. 3, March 1995, pp. 195-203.)
// This algorithm is slow(n^2), but it is easy to code and does not use an
// enormous amount of memory.
//
// Copyright © 2002 Tempest Software, Inc.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Pi
{
public:
    // This is the convenient way to generate PI to "numdigits" places.
    static std::string pi(int numdigits);

    // Construct a PI generator that is good for up to "numdigits" digits.
    Pi(int numdigits) : numdigits(numdigits), a(10*numdigits / 3),
                        nines(0), predigit(0), count(0)
    {
        std::fill(a.begin(), a.end(), 2);
    }

    // This is the other way: generate the digits one at a time.
    // The caller is responsible for making sure the function is not
    // called too many times.
    char operator()();

private:
    // The InnerLoop is used to transform the a[] array when computing
    // each successive digit of pi. The "q" value needs to be exported
    // so the caller supplies the reference, which is used by the loop.
    // The "i" and "p" members are private to the loop.
    class InnerLoop {
    public:
        InnerLoop(int& q, int size) : i(size), q(q), p(2*size - 1)
        {
          q = 0;
        }
        int operator()(int a_elem)
        {
            // This is the actual inner loop. This is the time-critical
            // portion of the computation.
            std::ldiv_t div = std::ldiv(10*a_elem + q*i, p);
            q = div.quot;
            p -= 2;
            --i;
            return div.rem;
        }
    private:
        int i, p, &q;
    };

    std::vector<int> a;           // spigot array
    int numdigits;                // max number of digits to be computed
    int nines;                    // # of accumulated '9' or '0' characters
    int count;                    // number of digits generated so far
    int predigit;                 // working storage
    int q;
};

std::string Pi::pi(int numdigits)
{
    Pi digits(numdigits);
    // First digit is zero, so don't count it: if the user wants 10
    // digits, compute 11, of which 10 are significant.
    std::string result(numdigits + 1, '\0');
    std::generate(result.begin(), result.end(), digits);
    return result;
}

char Pi::operator()()
{
    if (count > numdigits)
      //        throw std::out_of_range(std::string("Too many calls to nextdigit()"));
        throw std::string("Too many calls to nextdigit()");

    if (count == numdigits)
        return static_cast<char>(predigit + '0');

    if (nines > 0) {
        --nines;
        return q == 10 ? '0' : '9';
    }

    while (true)
    {
		// Cast the size to a plain int. Thus, this program will not work
		// if you try to generate more than INT_MAX digits. If you need that
		// many digits, find a better algorithm.
        InnerLoop inner_loop(q, static_cast<int>(a.size()));
        std::transform(a.begin(), a.end(), a.begin(), inner_loop);
        a.back() = q % 10;
        q /= 10;
        if (q == 9)
            ++nines;
        else if (q == 10)
        {
            char result = static_cast<char>(predigit + 1 + '0');
            predigit = 0;
            return result;
        }
        else
        {
            char result = static_cast<char>(predigit + '0');
            predigit = q;
            return result;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        std::cerr << "usage: pi #DIGITS [FILE]" << std::endl;
        return 1;
    }
    int numdigits = std::atoi(argv[1]);
    std::string pi(Pi::pi(numdigits));

    if (argc > 2)
    {
        std::ofstream out(argv[2]);
        out << pi << std::endl;
    }
    else
        std::cout << pi << std::endl;
}
