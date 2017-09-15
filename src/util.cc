/*
 * This file is part of GCIDE-CLI.
 *
 * GCIDE-CLI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GCIDE-CLI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GCIDE-CLI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.h"

#include <algorithm>
#include <locale>

bool
gcide_cli::string_iequal(const Glib::ustring& s1, const Glib::ustring& s2)
{
    return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(),
        [](char c1, char c2) { return std::toupper(c1) == std::toupper(c2); });
}
