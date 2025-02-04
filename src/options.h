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

#ifndef GCIDE_CLI_OPTION_H
#define GCIDE_CLI_OPTION_H

#include <stdexcept>
#include <vector>

#include <glibmm/ustring.h>

namespace gcide_cli {

class Options {
public:
    /* Could be passed to getopt or getopt_long. */
    static const char optstring[];
    /* Should the definition of each word be displayed? Usually true. */
    bool display_definition = true;
    /* Words to search. */
    std::vector<Glib::ustring> words;
    /* If true, don't read from stdin. */
    bool has_file = false;
    /* Path of file to read words from, one per line. */
    Glib::ustring file;
    bool use_stdin = false;

    /* Does nothing at this point. */
    bool help = false;
    /* Does nothing at this point. */
    bool version = false;
    bool display_word = false;
    /* Does nothing at this point. */
    bool display_pronunciation = false;
    /* Does nothing at this point. */
    bool display_pos = false;
    /* Does nothing at this point. */
    bool display_source = false;

    /* May throw an OptionError, caller should print usage in that case. */
    Options(int argc, char* argv[]);
};

class OptionError : public std::runtime_error {
public:
    OptionError(const Glib::ustring& what_arg);
};

class UnknownOptionError : public OptionError {
public:
    /* Deduces error message from getopt global variables. */
    UnknownOptionError();
    static Glib::ustring unknown_option_message();
};

class MissingRequiredArgumentError : public OptionError {
public:
    /* Deduces error message from getopt global variables. */
    MissingRequiredArgumentError();
    static Glib::ustring missing_required_argument_message();
};
} /* namespace gcide_cli */

#endif /* GCIDE_CLI_OPTION_H */
