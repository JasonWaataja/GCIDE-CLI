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

#ifndef GCIDE_CLI_DICTIONARY_READER_H
#define GCIDE_CLI_DICTIONARY_READER_H

#include <stdexcept>

#include <libxml++/libxml++.h>

#include "dictionary_entry.h"
#include "options.h"

namespace gcide_cli {

/* A class that reads the dictionary and can then peform searches. */
class DictionaryReader {
public:
    /* Where to find the dictionary files. */
    static const char DICTIONARY_PATH[];

    /*
     * Long constructor, requires parsing dictionary. May throw a ParsingError.
     */
    DictionaryReader();

    /* May throw an EntryNotFoundError. */
    DictionaryEntry find_entry(
        const Options& options, const Glib::ustring& name) const;

private:
    xmlpp::DomParser parser;
};

/*
 * May throw a ParsingError. The name p_node refers both to the fact that
 * it is a <p> xml tag and the parent of the original <ent> tag found.
 */
DictionaryEntry entry_for_p_node(const Options& options,
    const Glib::ustring& name, const xmlpp::Node* p_node);

class ParsingError : public std::runtime_error {
public:
    ParsingError(const Glib::ustring& what_arg);
};

class EntryNotFoundError : public std::runtime_error {
public:
    EntryNotFoundError(const Glib::ustring& name);
    Glib::ustring name;
};
} /* namespace gcide_cli */

#endif /* GCIDE_CLI_DICTIONARY_READER_H */
