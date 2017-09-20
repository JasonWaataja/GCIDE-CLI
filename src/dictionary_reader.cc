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

#include <algorithm>
#include <iostream>

#include "config.h"
#include "util.h"
#include "xml.h"

const char gcide_cli::DictionaryReader::DICTIONARY_PATH[] =
    GCIDE_CLI_CMAKE_INSTALL_PREFIX
    "/share/gcide-cli/gcide_xml-0.51/xml_files/gcide.xml";

gcide_cli::DictionaryReader::DictionaryReader()
{
    parser.set_substitute_entities(true);
    parser.parse_file(DICTIONARY_PATH);
}

void
gcide_cli::DictionaryReader::print_word_entries(
    const Options& options, const std::vector<Glib::ustring>& words)
{
    auto remaining_words = words;
    auto results = find_entries(options, remaining_words);
    bool on_first = true;
    for (const Glib::ustring& word : words) {
        auto entry = results[word];
        if (!on_first)
            std::cout << std::endl;
        else
            on_first = false;
        if (entry)
            std::cout << entry->to_string(options) << std::endl;
        else
            std::cerr << "Failed to find entry for " << word << std::endl;
    }
}

std::map<Glib::ustring, std::shared_ptr<gcide_cli::DictionaryEntry>>
gcide_cli::DictionaryReader::find_entries(
    const Options& options, std::vector<Glib::ustring>& words)
{
    std::map<Glib::ustring, std::shared_ptr<DictionaryEntry>> results;
    for (const Glib::ustring& word : words)
        results[word] = std::shared_ptr<DictionaryEntry>{};
    iterate_node(
        parser.get_document()->get_root_node(), [&](const xmlpp::Node* node) {
            check_node(node, options, words, results);
            /* Stop if all the words are exhausted. */
            return !words.empty();
        });
    return results;
}

void
gcide_cli::DictionaryReader::check_node(const xmlpp::Node* node,
    const Options& options, std::vector<Glib::ustring>& words,
    std::map<Glib::ustring, std::shared_ptr<DictionaryEntry>>& results)
{
    const xmlpp::Element* element_node{find_element_with_name(node, "ent")};
    if (!element_node)
        return;
    for (const Glib::ustring& word : words) {
        if (is_matching_p_node(node, word)) {
            try {
                auto entry = std::make_shared<DictionaryEntry>(
                    entry_for_p_node(options, word, node));
                results[word] = entry;
                words.erase(std::remove(words.begin(), words.end(), word),
                    words.end());
            } catch (const ParsingError& e) {
                std::cerr << "Error while searching word " << word
                          << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

bool
gcide_cli::is_matching_p_node(
    const xmlpp::Node* node, const Glib::ustring& name)
{
    if (node->get_name() != "p")
        return false;
    if (!node)
        return false;
    const xmlpp::Element* ent_node = find_element_with_name(node, "ent");
    if (!ent_node)
        return false;
    const xmlpp::TextNode* text_node = ent_node->get_first_child_text();
    return text_node && string_iequal(text_node->get_content(), name);
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
