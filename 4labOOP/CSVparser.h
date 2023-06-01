#pragma once
#include <fstream> 
#include <string>
#include <tuple>
#include <iostream>
#include <sstream>

//Reader
//recursion
template<class Ch, class Tr, class Head, class... Tail>
class TupleReader {
    public:
        static std::tuple<Head, Tail...> read(std::basic_istream<Ch,Tr> &input, size_t columnNumber = 1) {
            if (input.eof()) {
                throw std::invalid_argument("Nothing to read\n");
            }
            Head data;
            input >> data;
            if (input.fail()) {
                throw std::runtime_error("Failed to read data\n");
            }
            return std::tuple_cat( std::make_tuple(data), TupleReader<Ch, Tr, Tail...>::read(input, ++columnNumber) );
        }
};

template<class Ch, class Tr, class Head>
class TupleReader<Ch, Tr, Head> {
    public:
        static std::tuple<Head> read(std::basic_istream<Ch,Tr> &input, size_t columnNumber = 1) {
            if (input.eof()) {
                throw std::invalid_argument("Nothing to read\n");
            }
            Head data;
            input >> data;
            if (input.fail()) {
                throw std::runtime_error("Failed to read data\n");
            }
            return std::make_tuple(data);
        }
};

//Printer
//recursion
template<class Tuple, std::size_t Size>
class TuplePrinter {
    public:
        static void print(std::ostream &output, const Tuple &tuple) {
            TuplePrinter<Tuple, Size - 1>::print(output, tuple);
            output << ", " << std::get<Size-1>(tuple);
        }
};

template<class Tuple>
class TuplePrinter<Tuple, 1> {
    public:
        static void print(std::ostream &output, const Tuple &tuple) {
            output << std::get<0>(tuple);
        }
};

template<typename... Args>
std::ostream& operator<<(std::ostream &output, const std::tuple<Args...> &tuple) {
    TuplePrinter<decltype(tuple), sizeof...(Args)>::print(output, tuple);
    return output;
}

template<typename... Args> 
class CSVparser {
    private:
        std::ifstream &input_;
        std::streampos fileStart_;

    public:
        CSVparser(std::ifstream &input) : input_{input} {
            fileStart_ = input_.tellg();
        }

        class Tuple {
            private:
                CSVparser &CSVparser_;
                std::tuple<Args...> tuple_;
                bool lastTuple_;
                size_t lineNumber_;
                std::streampos currentPos_;

            public:
                Tuple(CSVparser &CSVparser, bool isLastTuple)
                : CSVparser_{CSVparser}, lastTuple_{isLastTuple}, lineNumber_{1}, currentPos_{CSVparser.fileStart_} {
                    if (isLastTuple) return;
                    readTuple();
                }

                void readTuple() {
                    if (CSVparser_.input_.eof()) {
                        lastTuple_ = true;
                        return;
                    }
                    CSVparser_.input_.clear();
                    CSVparser_.input_.seekg(currentPos_);

                    std::string currentLine;
                    std::getline(CSVparser_.input_, currentLine, '\n');

                    currentPos_ = CSVparser_.input_.tellg();
                    std::istringstream lineParser {currentLine};
                      
                    tuple_ = TupleReader<char, std::char_traits<char>, Args...>::read(lineParser, lineNumber_);
                    lineNumber_++;
                }

                bool operator!=(const Tuple &other) const {
                    return lastTuple_  != other.lastTuple_;
                }

                Tuple& operator++() {
                    readTuple();
                    return *this;
                }

                std::tuple<Args...> operator*() const {
                    return tuple_;
                }

                ~Tuple() = default;
        };

        Tuple begin() {
            input_.seekg(fileStart_);
            return Tuple{*this, false};
        }

        Tuple end() {
            return Tuple{*this, true};
        }
};
