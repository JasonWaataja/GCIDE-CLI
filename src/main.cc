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

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "dictionary_entry.h"
#include "dictionary_reader.h"
#include "options.h"
#include "word_gathering.h"

namespace gcide_cli {

void print_word_entry(const Options& options, const DictionaryReader& reader,
    const std::string& word);

} /* namespace gcide_cli */

void
gcide_cli::print_word_entry(const Options& options,
    const gcide_cli::DictionaryReader& reader, const std::string& word)
{
    try {
        DictionaryEntry entry{reader.find_entry(word)};
        std::cout << entry.to_string(options) << std::endl;
    } catch (const EntryNotFoundError& e) {
        std::cerr << e.what() << std::endl;
    }
}

int
main(int argc, char* argv[])
{
    try {
        gcide_cli::Options options(argc, argv);
        gcide_cli::DictionaryReader dict_reader;
        std::vector<std::string> words{gcide_cli::gather_words(options)};
        bool on_first = true;
        for (const std::string& word : words) {
            if (!on_first)
                std::cout << std::endl;
            else
                on_first = false;
            print_word_entry(options, dict_reader, word);
        }
    } catch (const gcide_cli::OptionError& e) {
        std::cerr << "Error parsing options:" << std::endl;
        std::cerr << e.what() << std::endl;
        gcide_cli::print_usage();
        return EXIT_FAILURE;
    } catch (const gcide_cli::ParsingError& e) {
        std::cerr << "Error parsing dictionary:" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
