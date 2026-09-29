#pragma once
#include <iostream>
#include <string>
#include <map>

/*
    1. read data from `data.csv` and store it in a map. key is `date` and `value` is the exchange rate.
    2. first try find exact date in `input.txt`.
    3. if not found; find the closest date that is less that the given date.
*/
class BitcoinExchange
{
public:
    BitcoinExchange(){}
    ~BitcoinExchange(){}
    BitcoinExchange(const BitcoinExchange &other) : _exchangeRates(other._exchangeRates){}
    BitcoinExchange &operator=(const BitcoinExchange &other)
    {
        if (this != &other)
        {
            _exchangeRates = other._exchangeRates;
        }
        return *this;
    }
    void readDataFile(const std::string &path);
    void readInputFile(const std::string &path);
    double findClosestRate(const std::string &date) const;
    
private:
    bool isValidDate(const std::string &date) const;
    bool parseDate(const std::string &rawDate, std::string &date) const;
    bool parseValue(const std::string &valueStr, double &value) const;
    bool parseRate(const std::string &rateStr, double &rate) const;
    void processLine(const std::string &line);
    // std::string _dataFilePath;
    std::map <std::string, double> _exchangeRates;
};

