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

#ifndef GCIDE_CLI_WORD_GATHERING_H
#define GCIDE_CLI_WORD_GATHERING_H

#include <string>
#include <vector>

#include "options.h"

namespace gcide_cli {

std::vector<std::string> gather_words(const Options& options);

/* T must be able to read lines. */
template <typename T>
std::vector<std::string> gather_words_from_stream(T& stream);

template <typename T>
std::vector<std::string>
gather_words_from_stream(T& stream)
{
    std::vector<std::string> words;
    std::string line;
    while (std::getline(stream, line))
        words.push_back(line);
    return words;
}
} /* namespace gcide_cli */

#endif /* GCIDE_CLI_WORD_GATHERING_H */
