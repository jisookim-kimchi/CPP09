#include "BitcoinExchange.hpp"
#include <fstream> 
#include <sstream> 
#include <iostream> 
#include <cstdlib>  
#include <limits>    
#include <iomanip> 
#include <stdexcept>

void BitcoinExchange::readDataFile(const std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Error:Could not open file : " + path);
    }
    std::string line;
    if (!std::getline(file, line))
    {
        throw std::runtime_error("Error: file is empty : " + path);
    }
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::string::size_type commaPos = line.find(',');

        if (commaPos == std::string::npos)
            continue;
        std::string date;
        if (!parseDate(line.substr(0, commaPos), date))
            continue;
        double rate;
        if (!parseRate(line.substr(commaPos + 1), rate))
            continue;
        _exchangeRates.insert(std::make_pair(date, rate));
    }
}

/** 
    @brief : check if the given string is a valid number.
    for year : 1900 ~ 2100, 
    for month : 1 ~ 12, 
    for day : max 'day' is 31, except 30 for `april, june, september, november`.
    regarding of day : if month is `february`, max `day` is 29 or 28.
*/
bool BitcoinExchange::isValidDate(const std::string &date) const
{
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());

    bool leapYear = (year % 400 == 0) ||
                (year % 4 == 0 && year % 100 != 0);

    int maxDay = 31;
    if (month == 2)
        maxDay = leapYear ? 29 : 28;
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        maxDay = 30;

    if (year < 1 || month < 1 || month > 12 || day < 1 || day > maxDay)
        return false;
    return true;
}

/** 
    @brief : trim value fronted white space and behind white space.
    @brief : check if the given string is a valid number for date.
*/
bool BitcoinExchange::parseDate(const std::string &rawDate, std::string &date) const
{
    size_t start = 0;
    while (start < rawDate.size() && (rawDate[start] == ' ' || rawDate[start] == '\t'))
        ++start;
    size_t end = rawDate.size();
    while (end > start && (rawDate[end - 1] == ' ' || rawDate[end - 1] == '\t'))
        --end;
    date = rawDate.substr(start, end - start);

    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (std::string::size_type i = 0; i < date.size(); ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    return isValidDate(date);
}

/**
    @brief : trim value fronted white space and behind white space.
    @brief : check if the given string is a '+' number and not exceed the allowed range or is not numericaly valid.
*/
bool BitcoinExchange::parseValue(const std::string &valueStr, double &value) const
{
    size_t start = 0;
    while (start < valueStr.size() && (valueStr[start] == ' ' || valueStr[start] == '\t'))
        ++start;
    size_t end = valueStr.size();
    while (end > start && (valueStr[end - 1] == ' ' || valueStr[end - 1] == '\t'))
        --end;
    std::string trimmed = valueStr.substr(start, end - start);

    if (trimmed.empty())
    {
        std::cerr << "Error: bad input => " << valueStr << std::endl;
        return false;
    }
    double num;
    try
    {
        std::size_t endPos = 0;
        num = std::stod(trimmed, &endPos);
        if (endPos != trimmed.size())
        {
            std::cerr << "Error: bad input => " << trimmed << std::endl;
            return false;
        }
    }
    catch (const std::exception &)
    {
        std::cerr << "Error: bad input => " << trimmed << std::endl;
        return false;
    }
    if (num < 0)
    {
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (num > 1000)
    {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    value = num;
    return true;
}

/**
    @brief : trim value fronted white space and behind white space.
    @brief : check if the given string is a '+' number and not exceed the allowed range or is not numericaly valid.
*/
bool BitcoinExchange::parseRate(const std::string &rateStr, double &rate) const
{
    size_t start = 0;
    while (start < rateStr.size() && (rateStr[start] == ' ' || rateStr[start] == '\t'))
        ++start;
    size_t end = rateStr.size();
    while (end > start && (rateStr[end - 1] == ' ' || rateStr[end - 1] == '\t'))
        --end;
    std::string trimmed = rateStr.substr(start, end - start);

    if (trimmed.empty())
        return false;

    double num;
    try
    {
        std::size_t endPos = 0;
        num = std::stod(trimmed, &endPos);
        if (endPos != trimmed.size())
        {
            return false;
        }
    }
    catch (const std::exception &)
    {
        return false;
    }
    if (num < 0)
        return false;

    rate = num;
    return true;
}

void BitcoinExchange::processLine(const std::string &line)
{
    std::string::size_type separateBar = line.find('|');
    if (separateBar == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return;
    }

    std::string rawDate = line.substr(0, separateBar);
    std::string date;
    if (!parseDate(rawDate, date))
    {
        std::cerr << "Error: bad input => " << rawDate << std::endl;
        return;
    }

    double value;
    if (!parseValue(line.substr(separateBar + 1), value))
        return;

    try
    {
        double exchangeRate = findClosestRate(date);
        std::cout << date << " => " << value << " = "
                  << value * exchangeRate << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void BitcoinExchange::readInputFile(const std::string &path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        throw std::runtime_error("Error : Could not open file : " + path);
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        processLine(line);
    }
}


/** 
    @brief : *compare the date in the `input.txt` with dates in `data.csv`.
              find the closest date that is not later than the input date,
              then return its exchange rate.
*/
double BitcoinExchange::findClosestRate(const std::string &input_date) const
{
    std::map<std::string, double>::const_iterator it;
    std::map<std::string, double>::const_iterator closest;
    
    closest = _exchangeRates.end();
    
    for (it = _exchangeRates.begin(); it != _exchangeRates.end(); ++it)
    {
        if (it->first > input_date)
            break;
        closest = it;
        if (it->first == input_date)
            return it->second;
    }
    if (closest == _exchangeRates.end())
        throw std::runtime_error("Error: date is too old.");

    return closest->second;
}


