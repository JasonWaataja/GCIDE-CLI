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

#include "dictionary_entry.h"

#include "util.h"

gcide_cli::DictionaryEntry::DictionaryEntry()
{
}

gcide_cli::DictionaryEntry::DictionaryEntry(const std::string& name)
    : name{name}
{
}

std::string
gcide_cli::DictionaryEntry::to_string(const Options& options) const
{
    std::vector<std::string> elements;
    if (options.display_word)
        elements.push_back(name);
    if (options.display_definition)
        elements.push_back(definition);

    return string_join(elements.begin(), elements.end());
}
