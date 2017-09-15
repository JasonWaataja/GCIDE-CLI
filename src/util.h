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

#ifndef GCIDE_CLI_UTIL_H
#define GCIDE_CLI_UTIL_H

#include <glibmm/ustring.h>

namespace gcide_cli {

/*
 * TODO: Figure out if I should use boost::iequal here, taking in a new
 * dependency or use glib's g_ascii_strncasecmp or g_utf8_casefold/strcmp.
 */
/* Case insensitive string comparison. */
bool string_iequal(const Glib::ustring& s1, const Glib::ustring& s2);

/*
 * Combine each string element in the range [first, last), separated by
 * separator. T is an iterator that dereferences to a string.
 */
template <typename T>
Glib::ustring string_join(T first, T last, Glib::ustring separator = "\n");

} /* namespace gcide_cli */

template <typename T>
Glib::ustring
gcide_cli::string_join(T first, T last, Glib::ustring separator)
{
    Glib::ustring str;
    bool on_first = true;
    for (auto iter = first; iter != last; ++iter) {
        if (!on_first)
            str += separator;
        else
            on_first = false;
        str += *iter;
    }
    return str;
}

#endif /* GCIDE_CLI_UTIL_H */
