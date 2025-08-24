#include "validators.h"
#include <QRegularExpression>
#include <QDate>
#include <QTime>

bool Validators::isValidEmail(const QString& email)
{
    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return emailRegex.match(email).hasMatch();
}

bool Validators::isValidUsername(const QString& username)
{
    // Username should be 3-20 characters, alphanumeric and underscore only
    QRegularExpression usernameRegex(R"(^[a-zA-Z0-9_]{3,20}$)");
    return usernameRegex.match(username).hasMatch();
}

bool Validators::isValidPassword(const QString& password)
{
    // Password should be at least 6 characters
    return password.length() >= 6;
}

bool Validators::isValidFlightNumber(const QString& flightNumber)
{
    // Flight number should be 2-6 characters, alphanumeric
    QRegularExpression flightRegex(R"(^[A-Z0-9]{2,6}$)");
    return flightRegex.match(flightNumber.toUpper()).hasMatch();
}

bool Validators::isValidPrice(double price)
{
    return price > 0.0 && price <= 10000.0;
}

bool Validators::isValidSeatCount(int seatCount)
{
    return seatCount > 0 && seatCount <= 500;
}

bool Validators::isValidDate(const QString& date)
{
    QRegularExpression dateRegex(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!dateRegex.match(date).hasMatch()) {
        return false;
    }
    
    QDate qdate = QDate::fromString(date, "yyyy-MM-dd");
    return qdate.isValid() && qdate >= QDate::currentDate();
}

bool Validators::isValidTime(const QString& time)
{
    QRegularExpression timeRegex(R"(^\d{2}:\d{2}$)");
    if (!timeRegex.match(time).hasMatch()) {
        return false;
    }
    
    QTime qtime = QTime::fromString(time, "HH:mm");
    return qtime.isValid();
}

// EmailValidator implementation
QValidator::State Validators::EmailValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)
    
    if (input.isEmpty()) {
        return QValidator::Intermediate;
    }
    
    if (Validators::isValidEmail(input)) {
        return QValidator::Acceptable;
    }
    
    // Check if it's a partial valid email
    QRegularExpression partialEmailRegex(R"([a-zA-Z0-9._%+-]*@?[a-zA-Z0-9.-]*\.?[a-zA-Z]*)");
    if (partialEmailRegex.match(input).hasMatch()) {
        return QValidator::Intermediate;
    }
    
    return QValidator::Invalid;
}

// UsernameValidator implementation
QValidator::State Validators::UsernameValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)
    
    if (input.isEmpty()) {
        return QValidator::Intermediate;
    }
    
    if (Validators::isValidUsername(input)) {
        return QValidator::Acceptable;
    }
    
    // Check if it contains only valid characters
    QRegularExpression validCharsRegex(R"(^[a-zA-Z0-9_]*$)");
    if (validCharsRegex.match(input).hasMatch()) {
        if (input.length() < 3) {
            return QValidator::Intermediate;
        } else if (input.length() <= 20) {
            return QValidator::Intermediate;
        }
    }
    
    return QValidator::Invalid;
}

// FlightNumberValidator implementation
QValidator::State Validators::FlightNumberValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)
    
    if (input.isEmpty()) {
        return QValidator::Intermediate;
    }
    
    QString upperInput = input.toUpper();
    
    if (Validators::isValidFlightNumber(upperInput)) {
        return QValidator::Acceptable;
    }
    
    // Check if it contains only valid characters
    QRegularExpression validCharsRegex(R"(^[A-Z0-9]*$)");
    if (validCharsRegex.match(upperInput).hasMatch()) {
        if (upperInput.length() <= 6) {
            return QValidator::Intermediate;
        }
    }
    
    return QValidator::Invalid;
}

// PriceValidator implementation
QValidator::State Validators::PriceValidator::validate(QString& input, int& pos) const
{
    Q_UNUSED(pos)
    
    if (input.isEmpty()) {
        return QValidator::Intermediate;
    }
    
    // Check if it's a valid number format
    QRegularExpression numberRegex(R"(^\d*\.?\d{0,2}$)");
    if (!numberRegex.match(input).hasMatch()) {
        return QValidator::Invalid;
    }
    
    bool ok;
    double price = input.toDouble(&ok);
    
    if (!ok) {
        return QValidator::Intermediate;
    }
    
    if (Validators::isValidPrice(price)) {
        return QValidator::Acceptable;
    }
    
    return QValidator::Invalid;
} 