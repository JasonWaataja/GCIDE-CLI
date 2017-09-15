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

#include <iostream>

#include <glibmm/convert.h>

#include "config.h"
#include "util.h"

const char gcide_cli::DictionaryReader::DICTIONARY_PATH[] =
    CMAKE_INSTALL_PREFIX
    "/share/gcide-cli/gcide_xml-0.51/xml_files/gcide.xml";

gcide_cli::DictionaryReader::DictionaryReader()
{
    parser.set_substitute_entities(true);
    parser.parse_file(DICTIONARY_PATH);
}

gcide_cli::DictionaryEntry
gcide_cli::DictionaryReader::find_entry(const std::string& name) const
{
    const xmlpp::Node* ent_node = find_node_if(
        parser.get_document()->get_root_node(), make_ent_node_finder(name));
    if (!ent_node)
        throw EntryNotFoundError{name};
    const xmlpp::Node* def_node = find_def_node(ent_node);
    if (!def_node)
        throw ParsingError{"Failed to find definition node for entry."};
    const xmlpp::Element* as_entry =
        dynamic_cast<const xmlpp::Element*>(def_node);
    if (!as_entry)
        throw ParsingError{"Failed to cast to element node."};
    DictionaryEntry entry{name};
    std::string def_text = gather_child_text(def_node);
    entry.definition = def_text;
    return entry;
}

void
gcide_cli::iterate_node(
    const xmlpp::Node* node, std::function<void(const xmlpp::Node*)> func)
{
    if (!node)
        return;
    func(node);
    for (const xmlpp::Node* child_node : node->get_children())
        iterate_node(child_node, func);
}

const xmlpp::Node*
gcide_cli::find_node_if(
    const xmlpp::Node* root, std::function<bool(const xmlpp::Node*)> predicate)
{
    if (!root)
        return nullptr;
    if (predicate(root))
        return root;
    for (const xmlpp::Node* child_node : root->get_children()) {
        const xmlpp::Node* node = find_node_if(child_node, predicate);
        if (node)
            return node;
    }
    return nullptr;
}

std::function<bool(const xmlpp::Node*)>
gcide_cli::make_ent_node_finder(const std::string& name)
{
    return [name](const xmlpp::Node* node) -> bool {
        /* Parsing fails if there's a special symbol in the middle. */
        try {
            const xmlpp::Element* as_element =
                dynamic_cast<const xmlpp::Element*>(node);
            if (!as_element)
                return false;
            if (as_element->get_name() != "ent")
                return false;
            const xmlpp::TextNode* text_node =
                as_element->get_first_child_text();
            return text_node && string_iequal(text_node->get_content(), name);
        } catch (const Glib::ConvertError&) {
            return false;
        }
    };
}

const xmlpp::Node*
gcide_cli::find_def_node(const xmlpp::Node* ent_node)
{
    /* Get the <p> node that is right above the <ent> node. */
    const xmlpp::Node* p_node = ent_node->get_parent();
    return find_node_if(p_node, [](const xmlpp::Node* node) {
        const xmlpp::Element* as_element =
            dynamic_cast<const xmlpp::Element*>(node);
        return as_element && as_element->get_name() == "def";
    });
}

std::string
gcide_cli::gather_child_text(const xmlpp::Node* node)
{
    /*
     * TODO: Consider using a vector of strings and then join later with a
     * separator.
     */
    std::string text;
    if (!node)
        return text;
    iterate_node(node, [&text](const xmlpp::Node* node) {
        const xmlpp::TextNode* as_text =
            dynamic_cast<const xmlpp::TextNode*>(node);
        if (!as_text)
            return;
        text += as_text->get_content();
    });
    return text;
}

gcide_cli::ParsingError::ParsingError(const std::string& what_arg)
    : std::runtime_error{what_arg}
{
}

gcide_cli::EntryNotFoundError::EntryNotFoundError(const std::string& name)
    : std::runtime_error{"Failed to find entry for " + name}, name{name}
{
}
