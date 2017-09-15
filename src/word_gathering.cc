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

#include "word_gathering.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

std::vector<std::string>
gcide_cli::gather_words(const Options& options)
{
    /* Start with the words included as argument. */
    std::vector<std::string> words{options.words};
    if (options.has_file) {
        std::ifstream reader{options.file};
        if (!reader.is_open())
            throw std::runtime_error{"Failed to open file " + options.file};
        std::vector<std::string> file_words{gather_words_from_stream(reader)};
        words.insert(words.end(), file_words.begin(), file_words.end());
        return words;
    }
    if (options.use_stdin) {
        std::vector<std::string> input_words{
            gather_words_from_stream(std::cin)};
        words.insert(words.end(), input_words.begin(), input_words.end());
    }
    return words;
}
