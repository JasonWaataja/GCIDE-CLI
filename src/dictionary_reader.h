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

#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include <glibmm/ustring.h>
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
     * Long constructor, requires parsing dictionary. May throw a ParsingError
     * but only if the dictionary is installed incorrectly.
     */
    DictionaryReader();

    void print_word_entries(
        const Options& options, const std::vector<Glib::ustring>& words);
    /*
     * A null pointer for the entry indictates it wasn't found. Removes entries
     * from words when they're found.
     */
    std::map<Glib::ustring, std::shared_ptr<DictionaryEntry>> find_entries(
        const Options& options, std::vector<Glib::ustring>& words);
    static void check_node(const xmlpp::Node* node, const Options& options,
        std::vector<Glib::ustring>& words,
        std::map<Glib::ustring, std::shared_ptr<DictionaryEntry>>& results);

private:
    xmlpp::DomParser parser;
};

/*
 * May throw a ParsingError. The name p_node refers both to the fact that
 * it is a <p> xml tag and the parent of the original <ent> tag found.
 */
DictionaryEntry entry_for_p_node(const Options& options,
    const Glib::ustring& name, const xmlpp::Node* p_node);
bool is_matching_p_node(const xmlpp::Node* node, const Glib::ustring& name);

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
