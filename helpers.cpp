#include "helpers.h"
#include <QRegularExpression>
#include <QTextStream>
#include <QDebug>
#include <QRandomGenerator>

QString Helpers::formatDateTime(const QDateTime& dateTime, const QString& format)
{
    return dateTime.toString(format);
}

QString Helpers::formatDate(const QDate& date, const QString& format)
{
    return date.toString(format);
}

QString Helpers::formatTime(const QTime& time, const QString& format)
{
    return time.toString(format);
}

QString Helpers::formatDuration(const QDateTime& start, const QDateTime& end)
{
    qint64 seconds = start.secsTo(end);
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    
    if (hours > 0) {
        return QString("%1h %2m").arg(hours).arg(minutes);
    } else {
        return QString("%1m").arg(minutes);
    }
}

QString Helpers::formatCurrency(double amount, const QString& currency)
{
    return QString("%1%2").arg(currency).arg(QString::number(amount, 'f', 2));
}

QString Helpers::formatPrice(double price)
{
    return formatCurrency(price, "$");
}

QString Helpers::capitalize(const QString& text)
{
    if (text.isEmpty()) {
        return text;
    }
    
    QString result = text.toLower();
    result[0] = result[0].toUpper();
    return result;
}

QString Helpers::truncate(const QString& text, int maxLength)
{
    if (text.length() <= maxLength) {
        return text;
    }
    
    return text.left(maxLength - 3) + "...";
}

QString Helpers::generateRandomString(int length)
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString result;
    
    for (int i = 0; i < length; ++i) {
        result += chars[QRandomGenerator::global()->bounded(chars.length())];
    }
    
    return result;
}

QString Helpers::sanitizeInput(const QString& input)
{
    // Remove potentially dangerous characters
    QString sanitized = input;
    sanitized.remove(QRegularExpression("[<>\"'&]"));
    return sanitized.trimmed();
}

bool Helpers::isNumeric(const QString& text)
{
    QRegularExpression numericRegex(R"(^\d+(\.\d+)?$)");
    return numericRegex.match(text).hasMatch();
}

bool Helpers::isAlphaNumeric(const QString& text)
{
    QRegularExpression alphaNumericRegex(R"(^[a-zA-Z0-9]+$)");
    return alphaNumericRegex.match(text).hasMatch();
}

bool Helpers::isValidDateRange(const QDate& start, const QDate& end)
{
    return start.isValid() && end.isValid() && start <= end;
}

bool Helpers::isValidTimeRange(const QTime& start, const QTime& end)
{
    return start.isValid() && end.isValid() && start <= end;
}

QDateTime Helpers::stringToDateTime(const QString& dateTimeStr)
{
    // Try common formats
    QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm:ss");
    if (dateTime.isValid()) {
        return dateTime;
    }
    
    dateTime = QDateTime::fromString(dateTimeStr, "dd/MM/yyyy HH:mm");
    if (dateTime.isValid()) {
        return dateTime;
    }
    
    dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-ddTHH:mm:ss");
    if (dateTime.isValid()) {
        return dateTime;
    }
    
    return QDateTime();
}

QString Helpers::dateTimeToString(const QDateTime& dateTime)
{
    return dateTime.toString("yyyy-MM-dd HH:mm:ss");
}

double Helpers::stringToDouble(const QString& numberStr, bool* ok)
{
    return numberStr.toDouble(ok);
}

int Helpers::stringToInt(const QString& numberStr, bool* ok)
{
    return numberStr.toInt(ok);
}

double Helpers::calculateTotalPrice(double basePrice, int seatCount)
{
    return basePrice * seatCount;
}

int Helpers::calculateAvailableSeats(int totalSeats, int bookedSeats)
{
    return qMax(0, totalSeats - bookedSeats);
}

double Helpers::calculateOccupancyRate(int totalSeats, int bookedSeats)
{
    if (totalSeats == 0) {
        return 0.0;
    }
    
    return (static_cast<double>(bookedSeats) / totalSeats) * 100.0;
}

QString Helpers::getFlightStatus(const QDateTime& departureTime)
{
    QDateTime now = QDateTime::currentDateTime();
    
    if (departureTime < now) {
        return "DEPARTED";
    } else if (departureTime.addSecs(-3600) <= now && now <= departureTime) {
        return "BOARDING";
    } else if (departureTime.addSecs(-7200) <= now && now < departureTime.addSecs(-3600)) {
        return "CHECK-IN";
    } else {
        return "SCHEDULED";
    }
}

QString Helpers::getBookingStatus(const QString& status)
{
    if (status.toUpper() == "CONFIRMED") {
        return "Confirmed";
    } else if (status.toUpper() == "CANCELLED") {
        return "Cancelled";
    } else if (status.toUpper() == "PENDING") {
        return "Pending";
    } else {
        return capitalize(status);
    }
}

QString Helpers::getStatusColor(const QString& status)
{
    QString upperStatus = status.toUpper();
    
    if (upperStatus == "CONFIRMED" || upperStatus == "SCHEDULED") {
        return "#27ae60"; // Green
    } else if (upperStatus == "CANCELLED") {
        return "#e74c3c"; // Red
    } else if (upperStatus == "PENDING" || upperStatus == "BOARDING") {
        return "#f39c12"; // Orange
    } else if (upperStatus == "CHECK-IN") {
        return "#3498db"; // Blue
    } else if (upperStatus == "DEPARTED") {
        return "#95a5a6"; // Gray
    } else {
        return "#2c3e50"; // Dark gray
    }
}

QString Helpers::getPriorityColor(int priority)
{
    switch (priority) {
        case 1: return "#e74c3c"; // Red - High
        case 2: return "#f39c12"; // Orange - Medium
        case 3: return "#27ae60"; // Green - Low
        default: return "#95a5a6"; // Gray - Default
    }
}

QString Helpers::formatFileSize(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    
    if (bytes >= GB) {
        return QString("%1 GB").arg(QString::number(static_cast<double>(bytes) / GB, 'f', 2));
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(QString::number(static_cast<double>(bytes) / MB, 'f', 2));
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(QString::number(static_cast<double>(bytes) / KB, 'f', 2));
    } else {
        return QString("%1 bytes").arg(bytes);
    }
}

QString Helpers::formatPercentage(double value)
{
    return QString("%1%").arg(QString::number(value, 'f', 1));
} 