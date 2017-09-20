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

#ifndef GCIDE_CLI_XML_H
#define GCIDE_CLI_XML_H

#include <functional>

#include <glibmm/ustring.h>
#include <libxml++/libxml++.h>

namespace gcide_cli {

/*
 * Runs func on node and all its children recursively. Handler should return
 * true to continue, false to stop iteration.
 */
void iterate_node(
    const xmlpp::Node* node, std::function<bool(const xmlpp::Node*)> func);

bool iterate_node_helper(
    const xmlpp::Node* node, std::function<bool(const xmlpp::Node*)> func);

/* Returns nullptr if the node could not be found. */
const xmlpp::Node* find_node_if(const xmlpp::Node* root,
    std::function<bool(const xmlpp::Node*)> predicate);

/* Not case sensitive by default. */
const xmlpp::Node* find_node_with_name(const xmlpp::Node* root,
    const Glib::ustring& name, bool case_sensitive = false);

const xmlpp::Element* find_element_with_name(const xmlpp::Node* root,
    const Glib::ustring& name, bool case_sensitive = false);

std::function<bool(const xmlpp::Node*)> make_ent_node_finder(
    const Glib::ustring& name);

/*
 * Goes through each child of node and combines the text of any text nodes it
 * finds.
 */
Glib::ustring gather_child_text(const xmlpp::Node* node);

} /* namespace gcide_cli */

#endif /* GCIDE_CLI_XML_H */
