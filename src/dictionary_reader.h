/*
 * Copyright (c) 2017 Jason Waataja
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef GCIDE_CLI_DICTIONARY_READER_H
#define GCIDE_CLI_DICTIONARY_READER_H

#include <functional>
#include <stdexcept>

#include <libxml++/libxml++.h>

#include "dictionary_entry.h"

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
    DictionaryEntry find_entry(const std::string& name);

private:
    xmlpp::DomParser parser;
};

/* Runs func on node and all its children recursively. */
void iterate_node(
    const xmlpp::Node* node, std::function<void(const xmlpp::Node*)> func);

/* Returns nullptr if the node could not be found. */
const xmlpp::Node* find_node_if(const xmlpp::Node* root,
    std::function<bool(const xmlpp::Node*)> predicate);

std::function<bool(const xmlpp::Node*)> make_ent_node_finder(
    const std::string& name);

/* May return nullptr at some point. */
const xmlpp::Node* find_def_node(const xmlpp::Node* ent_node);

/*
 * Goes through each child of node and combines the text of any text nodes it
 * finds.
 */
std::string gather_child_text(const xmlpp::Node* node);

class ParsingError : public std::runtime_error {
public:
    ParsingError(const std::string& what_arg);
};

class EntryNotFoundError : public std::runtime_error {
public:
    EntryNotFoundError(const std::string& name);
    std::string name;
};
} /* namespace gcide_cli */

#endif /* GCIDE_CLI_DICTIONARY_READER_H */
