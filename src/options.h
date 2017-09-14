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

#include <string>
#include <stdexcept>
#include <vector>

namespace gcide_cli {

class Options {
public:
    /* Could be passed to getopt or getopt_long. */
    static const char optstring[];
    /* Should the definition of each word be displayed? Usually true. */
    bool display_definition = true;
    /* Words to search. */
    std::vector<std::string> words;
    /* If true, don't read from stdin. */
    bool has_input_file = false;
    /* Path of file to read words from, one per line. */
    std::string input_file;

    /* Does nothing at this point. */
    bool help = false;
    /* Does nothing at this point. */
    bool version = true;
    /* Does nothing at this point. */
    bool display_word = false;
    /* Does nothing at this point. */
    bool display_pronunciation = false;
    /* Does nothing at this point. */
    bool display_pos = false;
    /* Does nothing at this point. */
    bool display_source = false;

    Options(int argc, char* argv[]);
};

class OptionError : public std::runtime_error {
public:
    OptionError(const std::string& what_arg);
};

class UnknownOptionError : public OptionError {
public:
    /* Deduces error message from getopt global variables. */
    UnknownOptionError();
    static std::string unknown_option_message();
};

class MissingRequiredArgumentError : public OptionError {
public:
    /* Deduces error message from getopt global variables. */
    MissingRequiredArgumentError();
    static std::string missing_required_argument_message();
};
} /* namespace gcide_cli */

#endif /* GCIDE_CLI_OPTION_H */
