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

#include "xml.h"

#include "util.h"

using xmlpp::Node;

void
gcide_cli::iterate_node(
    const Node* node, std::function<bool(const Node*)> func)
{
    iterate_node_helper(node, func);
}

bool
gcide_cli::iterate_node_helper(
    const Node* node, std::function<bool(const Node*)> func)
{
    if (!node)
        return true;
    if (!func(node))
        return false;
    for (const Node* child_node : node->get_children()) {
        if (!iterate_node_helper(child_node, func))
            return false;
    }
    return true;
}

const Node*
gcide_cli::find_node_if(
    const Node* root, std::function<bool(const Node*)> predicate)
{
    if (!root)
        return nullptr;
    if (predicate(root))
        return root;
    for (const Node* child_node : root->get_children()) {
        const Node* node = find_node_if(child_node, predicate);
        if (node)
            return node;
    }
    return nullptr;
}

const Node*
gcide_cli::find_node_with_name(
    const Node* root, const Glib::ustring& name, bool case_sensitive)
{
    return find_node_if(root, [&name, case_sensitive](const Node* node) {
        if (case_sensitive)
            return name == node->get_name();
        return string_iequal(name, node->get_name());
    });
}

const xmlpp::Element*
gcide_cli::find_element_with_name(
    const Node* root, const Glib::ustring& name, bool case_sensitive)
{
    const Node* node{
        find_node_if(root, [&name, case_sensitive](const Node* node) {
            const xmlpp::Element* as_element{
                dynamic_cast<const xmlpp::Element*>(node)};
            if (!node)
                return false;
            if (case_sensitive)
                return name == as_element->get_name();
            return string_iequal(name, node->get_name());
        })};
    /* This might be able to be a safe static_cast. */
    return dynamic_cast<const xmlpp::Element*>(node);
}

std::function<bool(const Node*)>
gcide_cli::make_ent_node_finder(const Glib::ustring& name)
{
    return [name](const Node* node) -> bool {
        const xmlpp::Element* as_element =
            dynamic_cast<const xmlpp::Element*>(node);
        if (!as_element)
            return false;
        if (as_element->get_name() != "ent")
            return false;
        const xmlpp::TextNode* text_node = as_element->get_first_child_text();
        return text_node && string_iequal(text_node->get_content(), name);
    };
}

Glib::ustring
gcide_cli::gather_child_text(const Node* node)
{
    /*
     * TODO: Consider using a vector of strings and then join later with a
     * separator.
     */
    Glib::ustring text;
    if (!node)
        return text;
    iterate_node(node, [&text](const Node* node) {
        const xmlpp::TextNode* as_text =
            dynamic_cast<const xmlpp::TextNode*>(node);
        if (!as_text)
            return true;
        text += as_text->get_content();
        return true;
    });
    return text;
}
