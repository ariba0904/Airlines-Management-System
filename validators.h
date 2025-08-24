#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <QString>
#include <QRegularExpression>
#include <QValidator>

class Validators
{
public:
    static bool isValidEmail(const QString& email);
    static bool isValidUsername(const QString& username);
    static bool isValidPassword(const QString& password);
    static bool isValidFlightNumber(const QString& flightNumber);
    static bool isValidPrice(double price);
    static bool isValidSeatCount(int seatCount);
    static bool isValidDate(const QString& date);
    static bool isValidTime(const QString& time);
    
    // Validator classes
    class EmailValidator : public QValidator
    {
    public:
        QValidator::State validate(QString& input, int& pos) const override;
    };
    
    class UsernameValidator : public QValidator
    {
    public:
        QValidator::State validate(QString& input, int& pos) const override;
    };
    
    class FlightNumberValidator : public QValidator
    {
    public:
        QValidator::State validate(QString& input, int& pos) const override;
    };
    
    class PriceValidator : public QValidator
    {
    public:
        QValidator::State validate(QString& input, int& pos) const override;
    };
};

#endif // VALIDATORS_H 