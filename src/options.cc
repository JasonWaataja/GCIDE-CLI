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

#include "options.h"

#include <getopt.h>

const char gcide_cli::Options::optstring[] = ":hvi:dwpPs";

gcide_cli::Options::Options(int argc, char* argv[])
{
    option long_options[]{{"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"input-file", required_argument, NULL, 'i'},
        {"disable-definition", no_argument, NULL, 'd'},
        {"word", no_argument, NULL, 'w'},
        {"pronunciation", no_argument, NULL, 'p'},
        {"part-of-speech", no_argument, NULL, 'P'},
        {"source", no_argument, NULL, 's'}, {0, 0, 0, 0}};
    int getopt_val = getopt_long(argc, argv, optstring, long_options, NULL);
    while (getopt_val != -1) {
        switch (getopt_val) {
        case 0:
            break;
        case '?':
            throw UnknownOptionError{};
            break;
        case ':':
            throw MissingRequiredArgumentError{};
            break;
        case 'h':
            help = true;
            break;
        case 'v':
            version = true;
            break;
        case 'i':
            has_input_file = true;
            input_file = optarg;
            break;
        case 'w':
            display_word = true;
            break;
        case 'p':
            display_pronunciation = true;
            break;
        case 'P':
            display_pos = true;
            break;
        case 's':
            display_source = true;
            break;
        case 'd':
            display_definition = false;
            break;
        }
        getopt_val = getopt_long(argc, argv, optstring, long_options, NULL);
    }
    /* Take the remaining options and treat them as words to search. */
    for (int i = optind; i < argc; ++i)
        words.push_back(argv[i]);
}

gcide_cli::OptionError::OptionError(const std::string& what_arg)
    : std::runtime_error{what_arg}
{
}

gcide_cli::UnknownOptionError::UnknownOptionError()
    : OptionError{unknown_option_message()}
{
}

std::string
gcide_cli::UnknownOptionError::unknown_option_message()
{
    if (optopt != 0)
        return "Unknown option " + optopt;
    return "Unknown option";
}

gcide_cli::MissingRequiredArgumentError::MissingRequiredArgumentError()
    : OptionError{missing_required_argument_message()}
{
}

std::string
gcide_cli::MissingRequiredArgumentError::missing_required_argument_message()
{
    if (optopt != 0)
        return "Missing required argument for " + optopt;
    return "Missing required argument";
}
