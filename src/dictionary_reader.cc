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

#include "dictionary_reader.h"

#include "config.h"
#include "xml.h"

const char gcide_cli::DictionaryReader::DICTIONARY_PATH[] =
    CMAKE_INSTALL_PREFIX "/share/gcide-cli/gcide_xml-0.51/xml_files/gcide.xml";

gcide_cli::DictionaryReader::DictionaryReader()
{
    parser.set_substitute_entities(true);
    parser.parse_file(DICTIONARY_PATH);
}

gcide_cli::DictionaryEntry
gcide_cli::DictionaryReader::find_entry(
    const Options& options, const Glib::ustring& name) const
{
    const xmlpp::Node* ent_node = find_node_if(
        parser.get_document()->get_root_node(), make_ent_node_finder(name));
    if (!ent_node)
        throw EntryNotFoundError{name};
    const xmlpp::Node* p_node = ent_node->get_parent();
    return entry_for_p_node(options, name, p_node);
}

gcide_cli::DictionaryEntry
gcide_cli::entry_for_p_node(const Options& options, const Glib::ustring& name,
    const xmlpp::Node* p_node)
{
    DictionaryEntry entry{name};
    if (options.display_definition) {
        const xmlpp::Node* def_node = find_element_with_name(p_node, "def");
        if (!def_node)
            throw ParsingError{"Failed to find definition node for entry."};
        entry.definition = gather_child_text(def_node);
    }
    if (options.display_pronunciation) {
        const xmlpp::Node* pronunciation_node =
            find_element_with_name(p_node, "pr");
        if (!pronunciation_node)
            throw ParsingError{"Failed to find pronunciation node for entry."};
        entry.pronunciation = gather_child_text(pronunciation_node);
    }
    if (options.display_pos) {
        const xmlpp::Node* pos_node = find_element_with_name(p_node, "pos");
        if (!pos_node)
            throw ParsingError{
                "Failed to find part-of-speech node for entry."};
        entry.pos = gather_child_text(pos_node);
    }
    if (options.display_source) {
        const xmlpp::Node* source_node =
            find_element_with_name(p_node, "source");
        if (!source_node)
            throw ParsingError{"Failed to find source for entry."};
        entry.source = gather_child_text(source_node);
    }
    return entry;
}

gcide_cli::ParsingError::ParsingError(const Glib::ustring& what_arg)
    : std::runtime_error{what_arg}
{
}

gcide_cli::EntryNotFoundError::EntryNotFoundError(const Glib::ustring& name)
    : std::runtime_error{"Failed to find entry for " + name}, name{name}
{
}
