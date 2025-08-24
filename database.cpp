#include "database.h"
#include <QCryptographicHash>
#include <QSqlRecord>
#include <QRandomGenerator>

Database::Database(QObject *parent) : QObject(parent), m_initialized(false)
{
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

Database& Database::getInstance()
{
    static Database instance;
    return instance;
}

bool Database::initialize(const QString& dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        qDebug() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    
    if (!createTables()) {
        qDebug() << "Failed to create tables";
        return false;
    }
    
    m_initialized = true;
    return true;
}

bool Database::createTables()
{
    QSqlQuery query;
    
    // Users table
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "username TEXT PRIMARY KEY, "
                   "password TEXT NOT NULL, "
                   "email TEXT UNIQUE NOT NULL, "
                   "full_name TEXT NOT NULL, "
                   "role TEXT NOT NULL, "
                   "created_date DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Failed to create users table:" << query.lastError().text();
        return false;
    }
    
    // Flights table
    if (!query.exec("CREATE TABLE IF NOT EXISTS flights ("
                   "flight_number TEXT PRIMARY KEY, "
                   "origin TEXT NOT NULL, "
                   "destination TEXT NOT NULL, "
                   "departure_time DATETIME NOT NULL, "
                   "arrival_time DATETIME NOT NULL, "
                   "total_seats INTEGER NOT NULL, "
                   "booked_seats INTEGER DEFAULT 0, "
                   "price REAL NOT NULL, "
                   "aircraft_model TEXT NOT NULL, "
                   "status TEXT DEFAULT 'SCHEDULED', "
                   "created_date DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
        qDebug() << "Failed to create flights table:" << query.lastError().text();
        return false;
    }
    
    // Bookings table
    if (!query.exec("CREATE TABLE IF NOT EXISTS bookings ("
                   "booking_id TEXT PRIMARY KEY, "
                   "username TEXT NOT NULL, "
                   "flight_number TEXT NOT NULL, "
                   "seat_count INTEGER NOT NULL, "
                   "total_price REAL NOT NULL, "
                   "booking_date DATETIME NOT NULL, "
                   "status TEXT DEFAULT 'CONFIRMED', "
                   "FOREIGN KEY (username) REFERENCES users(username), "
                   "FOREIGN KEY (flight_number) REFERENCES flights(flight_number))")) {
        qDebug() << "Failed to create bookings table:" << query.lastError().text();
        return false;
    }
    
    // Insert default admin user if not exists
    if (!userExists("admin")) {
        addUser("admin", "admin123", "admin@cloudwings.com", "System Administrator", "admin");
    }
    
    return true;
}

bool Database::isConnected() const
{
    return m_initialized && m_db.isOpen();
}

QString Database::hashPassword(const QString& password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool Database::verifyPassword(const QString& password, const QString& hash)
{
    return hashPassword(password) == hash;
}

bool Database::addUser(const QString& username, const QString& password, const QString& email, 
                       const QString& fullName, const QString& role)
{
    if (userExists(username)) {
        return false;
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, email, full_name, role) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashPassword(password));
    query.addBindValue(email);
    query.addBindValue(fullName);
    query.addBindValue(role);
    
    return query.exec();
}

bool Database::authenticateUser(const QString& username, const QString& password, QString& role)
{
    QSqlQuery query;
    query.prepare("SELECT password, role FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (!query.exec() || !query.next()) {
        qDebug() << "[AUTH] User not found or query failed for username:" << username;
        return false;
    }
    
    QString storedHash = query.value(0).toString();
    role = query.value(1).toString();
    QString inputHash = hashPassword(password);
    qDebug() << "[AUTH] Username:" << username << "StoredHash:" << storedHash << "InputHash:" << inputHash;
    
    return verifyPassword(password, storedHash);
}

bool Database::userExists(const QString& username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

bool Database::updateUserProfile(const QString& username, const QString& email, const QString& fullName)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET email = ?, full_name = ? WHERE username = ?");
    query.addBindValue(email);
    query.addBindValue(fullName);
    query.addBindValue(username);
    
    return query.exec();
}

bool Database::changePassword(const QString& username, const QString& newPassword)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET password = ? WHERE username = ?");
    query.addBindValue(hashPassword(newPassword));
    query.addBindValue(username);
    
    return query.exec();
}

QList<QMap<QString, QVariant>> Database::getAllUsers()
{
    QList<QMap<QString, QVariant>> users;
    QSqlQuery query("SELECT username, email, full_name, role, created_date FROM users ORDER BY username");
    
    while (query.next()) {
        QMap<QString, QVariant> user;
        user["username"] = query.value("username");
        user["email"] = query.value("email");
        user["full_name"] = query.value("full_name");
        user["role"] = query.value("role");
        user["created_date"] = query.value("created_date");
        users.append(user);
    }
    
    return users;
}

QMap<QString, QVariant> Database::getUserByUsername(const QString& username)
{
    QMap<QString, QVariant> user;
    QSqlQuery query;
    query.prepare("SELECT username, email, full_name, role, created_date FROM users WHERE username = ?");
    query.addBindValue(username);
    
    if (query.exec() && query.next()) {
        user["username"] = query.value("username");
        user["email"] = query.value("email");
        user["full_name"] = query.value("full_name");
        user["role"] = query.value("role");
        user["created_date"] = query.value("created_date");
    }
    
    return user;
}

bool Database::deleteUser(const QString& username)
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE username = ?");
    query.addBindValue(username);
    
    return query.exec();
}

bool Database::addFlight(const QString& flightNumber, const QString& origin, const QString& destination,
                         const QDateTime& departureTime, const QDateTime& arrivalTime,
                         int totalSeats, double price, const QString& aircraftModel)
{
    if (flightExists(flightNumber)) {
        return false;
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO flights (flight_number, origin, destination, departure_time, "
                  "arrival_time, total_seats, price, aircraft_model) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(flightNumber);
    query.addBindValue(origin);
    query.addBindValue(destination);
    query.addBindValue(departureTime);
    query.addBindValue(arrivalTime);
    query.addBindValue(totalSeats);
    query.addBindValue(price);
    query.addBindValue(aircraftModel);
    
    return query.exec();
}

bool Database::updateFlight(const QString& flightNumber, const QString& origin, const QString& destination,
                            const QDateTime& departureTime, const QDateTime& arrivalTime,
                            int totalSeats, double price, const QString& aircraftModel)
{
    QSqlQuery query;
    query.prepare("UPDATE flights SET origin = ?, destination = ?, departure_time = ?, "
                  "arrival_time = ?, total_seats = ?, price = ?, aircraft_model = ? "
                  "WHERE flight_number = ?");
    query.addBindValue(origin);
    query.addBindValue(destination);
    query.addBindValue(departureTime);
    query.addBindValue(arrivalTime);
    query.addBindValue(totalSeats);
    query.addBindValue(price);
    query.addBindValue(aircraftModel);
    query.addBindValue(flightNumber);
    
    return query.exec();
}

bool Database::deleteFlight(const QString& flightNumber)
{
    QSqlQuery query;
    query.prepare("DELETE FROM flights WHERE flight_number = ?");
    query.addBindValue(flightNumber);
    
    return query.exec();
}

QList<QMap<QString, QVariant>> Database::getFlights(const QString& origin, const QString& destination, const QDate& date)
{
    QList<QMap<QString, QVariant>> flights;
    QString sql = "SELECT * FROM flights WHERE 1=1";
    QList<QVariant> params;
    
    if (!origin.isEmpty()) {
        sql += " AND origin LIKE ?";
        params.append("%" + origin + "%");
    }
    
    if (!destination.isEmpty()) {
        sql += " AND destination LIKE ?";
        params.append("%" + destination + "%");
    }
    
    if (date.isValid()) {
        sql += " AND DATE(departure_time) = ?";
        params.append(date.toString("yyyy-MM-dd"));
    }
    
    sql += " AND departure_time > datetime('now') ORDER BY departure_time";
    
    QSqlQuery query;
    query.prepare(sql);
    
    for (const QVariant& param : params) {
        query.addBindValue(param);
    }
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> flight;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                flight[record.fieldName(i)] = record.value(i);
            }
            flights.append(flight);
        }
    }
    
    return flights;
}

QList<QMap<QString, QVariant>> Database::searchFlights(const QString& origin, const QString& destination, 
                                                       const QDate& date, int passengers)
{
    QList<QMap<QString, QVariant>> flights;
    QSqlQuery query;
    
    query.prepare("SELECT * FROM flights WHERE origin = ? AND destination = ? "
                  "AND DATE(departure_time) = ? AND (total_seats - booked_seats) >= ? "
                  "AND departure_time > datetime('now') ORDER BY departure_time");
    query.addBindValue(origin);
    query.addBindValue(destination);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.addBindValue(passengers);
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> flight;
            flight["flight_number"] = query.value("flight_number");
            flight["origin"] = query.value("origin");
            flight["destination"] = query.value("destination");
            flight["departure_time"] = query.value("departure_time").toDateTime();
            flight["arrival_time"] = query.value("arrival_time").toDateTime();
            flight["total_seats"] = query.value("total_seats");
            flight["booked_seats"] = query.value("booked_seats");
            flight["price"] = query.value("price");
            flight["aircraft_model"] = query.value("aircraft_model");
            flight["status"] = query.value("status");
            flights.append(flight);
        }
    }
    
    return flights;
}

QList<QMap<QString, QVariant>> Database::getFlightsByDate(const QDate& date)
{
    QList<QMap<QString, QVariant>> flights;
    QSqlQuery query;
    
    query.prepare("SELECT * FROM flights WHERE DATE(departure_time) = ?");
    query.addBindValue(date.toString("yyyy-MM-dd"));
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> flight;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                flight[record.fieldName(i)] = record.value(i);
            }
            flights.append(flight);
        }
    }
    
    return flights;
}

QMap<QString, QVariant> Database::getFlight(const QString& flightNumber)
{
    QMap<QString, QVariant> flight;
    QSqlQuery query;
    query.prepare("SELECT * FROM flights WHERE flight_number = ?");
    query.addBindValue(flightNumber);
    
    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            flight[record.fieldName(i)] = record.value(i);
        }
    }
    
    return flight;
}

bool Database::flightExists(const QString& flightNumber)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM flights WHERE flight_number = ?");
    query.addBindValue(flightNumber);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

QString Database::generateBookingId()
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString bookingId = "CW";
    
    for (int i = 0; i < 8; ++i) {
        bookingId += chars[QRandomGenerator::global()->bounded(chars.length())];
    }
    
    return bookingId;
}

bool Database::createBooking(const QString& username, const QString& flightNumber, 
                             int seatCount, double totalPrice, const QString& bookingDate)
{
    QString bookingId = generateBookingId();
    
    QSqlQuery query;
    query.prepare("INSERT INTO bookings (booking_id, username, flight_number, seat_count, "
                  "total_price, booking_date, status) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(bookingId);
    query.addBindValue(username);
    query.addBindValue(flightNumber);
    query.addBindValue(seatCount);
    query.addBindValue(totalPrice);
    query.addBindValue(bookingDate);
    query.addBindValue("CONFIRMED");
    
    if (query.exec()) {
        return reserveSeats(flightNumber, seatCount);
    }
    
    return false;
}

bool Database::cancelBooking(const QString& bookingId)
{
    QSqlQuery query;
    query.prepare("SELECT flight_number, seat_count FROM bookings WHERE booking_id = ?");
    query.addBindValue(bookingId);
    if (query.exec() && query.next()) {
        QString flightNumber = query.value("flight_number").toString();
        int seatCount = query.value("seat_count").toInt();
        // Update booking status
        query.prepare("UPDATE bookings SET status = 'CANCELLED' WHERE booking_id = ?");
        query.addBindValue(bookingId);
        if (query.exec()) {
            return releaseSeats(flightNumber, seatCount);
        }
    }
    return false;
}

bool Database::cancelBooking(const QString& bookingId, int seatsToCancel)
{
    QSqlQuery query;
    query.prepare("SELECT flight_number, seat_count, total_price FROM bookings WHERE booking_id = ?");
    query.addBindValue(bookingId);

    if (query.exec() && query.next()) {
        QString flightNumber = query.value("flight_number").toString();
        int seatCount = query.value("seat_count").toInt();
        double totalPrice = query.value("total_price").toDouble();

        if (seatsToCancel <= 0 || seatsToCancel > seatCount) {
            return false; // Invalid seat count
        }

        if (seatsToCancel == seatCount) {
            // Cancel all seats (full cancellation)
            query.prepare("UPDATE bookings SET status = 'CANCELLED' WHERE booking_id = ?");
            query.addBindValue(bookingId);
            if (query.exec()) {
                return releaseSeats(flightNumber, seatCount);
            }
        } else {
            // Partial cancellation: reduce seat_count and total_price
            double pricePerSeat = totalPrice / seatCount;
            int newSeatCount = seatCount - seatsToCancel;
            double newTotalPrice = pricePerSeat * newSeatCount;
            query.prepare("UPDATE bookings SET seat_count = ?, total_price = ? WHERE booking_id = ?");
            query.addBindValue(newSeatCount);
            query.addBindValue(newTotalPrice);
            query.addBindValue(bookingId);
            if (query.exec()) {
                return releaseSeats(flightNumber, seatsToCancel);
            }
        }
    }
    return false;
}

QList<QMap<QString, QVariant>> Database::getUserBookings(const QString& username)
{
    QList<QMap<QString, QVariant>> bookings;
    QSqlQuery query;
    query.prepare("SELECT b.*, f.origin, f.destination, f.departure_time, f.arrival_time, f.price "
                  "FROM bookings b JOIN flights f ON b.flight_number = f.flight_number "
                  "WHERE b.username = ? ORDER BY b.booking_date DESC");
    query.addBindValue(username);
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> booking;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                booking[record.fieldName(i)] = record.value(i);
            }
            bookings.append(booking);
        }
    }
    
    return bookings;
}

QList<QMap<QString, QVariant>> Database::getAllBookings()
{
    QList<QMap<QString, QVariant>> bookings;
    QSqlQuery query("SELECT b.*, u.full_name, f.origin, f.destination, f.departure_time "
                    "FROM bookings b "
                    "LEFT JOIN users u ON b.username = u.username "
                    "LEFT JOIN flights f ON b.flight_number = f.flight_number "
                    "ORDER BY b.booking_date DESC");
    
    while (query.next()) {
        QMap<QString, QVariant> booking;
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            booking[record.fieldName(i)] = record.value(i);
        }
        bookings.append(booking);
    }
    
    return bookings;
}

QMap<QString, QVariant> Database::getBooking(const QString& bookingId)
{
    QMap<QString, QVariant> booking;
    QSqlQuery query;
    query.prepare("SELECT b.*, u.full_name, f.origin, f.destination, f.departure_time, f.arrival_time, f.price "
                  "FROM bookings b "
                  "LEFT JOIN users u ON b.username = u.username "
                  "LEFT JOIN flights f ON b.flight_number = f.flight_number "
                  "WHERE b.booking_id = ?");
    query.addBindValue(bookingId);
    
    if (query.exec() && query.next()) {
        QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            booking[record.fieldName(i)] = record.value(i);
        }
    }
    
    return booking;
}

bool Database::updateSeatAvailability(const QString& flightNumber, int bookedSeats)
{
    QSqlQuery query;
    query.prepare("UPDATE flights SET booked_seats = ? WHERE flight_number = ?");
    query.addBindValue(bookedSeats);
    query.addBindValue(flightNumber);
    
    return query.exec();
}

int Database::getAvailableSeats(const QString& flightNumber)
{
    QSqlQuery query;
    query.prepare("SELECT total_seats - booked_seats FROM flights WHERE flight_number = ?");
    query.addBindValue(flightNumber);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    
    return 0;
}

bool Database::reserveSeats(const QString& flightNumber, int seatCount)
{
    QSqlQuery query;
    query.prepare("UPDATE flights SET booked_seats = booked_seats + ? WHERE flight_number = ?");
    query.addBindValue(seatCount);
    query.addBindValue(flightNumber);
    
    return query.exec();
}

bool Database::releaseSeats(const QString& flightNumber, int seatCount)
{
    QSqlQuery query;
    query.prepare("UPDATE flights SET booked_seats = MAX(0, booked_seats - ?) WHERE flight_number = ?");
    query.addBindValue(seatCount);
    query.addBindValue(flightNumber);
    
    return query.exec();
}

QMap<QString, QVariant> Database::getDashboardStats()
{
    QMap<QString, QVariant> stats;
    
    // Total flights today (now: all upcoming flights)
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM flights WHERE departure_time >= datetime('now')");
    if (query.exec() && query.next()) {
        stats["flights_today"] = query.value(0).toInt();
    }
    
    // Bookings made today (by booking_date)
    query.prepare("SELECT COUNT(*) FROM bookings WHERE DATE(booking_date, 'localtime') = DATE('now', 'localtime') AND status = 'CONFIRMED'");
    if (query.exec() && query.next()) {
        stats["bookings_made_today"] = query.value(0).toInt();
    }
    query.prepare("SELECT COALESCE(SUM(total_price), 0) FROM bookings WHERE DATE(booking_date, 'localtime') = DATE('now', 'localtime') AND status = 'CONFIRMED'");
    if (query.exec() && query.next()) {
        stats["revenue_made_today"] = query.value(0).toDouble();
    }

    // Bookings for flights departing today (by departure_time)
    query.prepare("SELECT COUNT(*) FROM bookings b JOIN flights f ON b.flight_number = f.flight_number WHERE DATE(f.departure_time) = DATE('now') AND b.status = 'CONFIRMED'");
    if (query.exec() && query.next()) {
        stats["bookings_departing_today"] = query.value(0).toInt();
    }
    query.prepare("SELECT COALESCE(SUM(b.total_price), 0) FROM bookings b JOIN flights f ON b.flight_number = f.flight_number WHERE DATE(f.departure_time) = DATE('now') AND b.status = 'CONFIRMED'");
    if (query.exec() && query.next()) {
        stats["revenue_departing_today"] = query.value(0).toDouble();
    }
    query.prepare("SELECT COUNT(DISTINCT b.username) FROM bookings b JOIN flights f ON b.flight_number = f.flight_number WHERE DATE(f.departure_time) = DATE('now') AND b.status = 'CONFIRMED'");
    if (query.exec() && query.next()) {
        stats["passengers_departing_today"] = query.value(0).toInt();
    }

    // Total registered passengers
    query.prepare("SELECT COUNT(*) FROM users WHERE role = 'passenger'");
    if (query.exec() && query.next()) {
        stats["total_passengers"] = query.value(0).toInt();
    }
    
    return stats;
}

QList<QMap<QString, QVariant>> Database::getRevenueReport(const QDate& startDate, const QDate& endDate)
{
    QList<QMap<QString, QVariant>> report;
    QSqlQuery query;
    query.prepare("SELECT DATE(booking_date) as date, COUNT(*) as bookings, SUM(total_price) as revenue "
                  "FROM bookings WHERE DATE(booking_date) BETWEEN ? AND ? "
                  "GROUP BY DATE(booking_date) ORDER BY date");
    query.addBindValue(startDate.toString("yyyy-MM-dd"));
    query.addBindValue(endDate.toString("yyyy-MM-dd"));
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> row;
            row["date"] = query.value("date");
            row["bookings"] = query.value("bookings");
            row["revenue"] = query.value("revenue");
            report.append(row);
        }
    }
    
    return report;
}

QList<QMap<QString, QVariant>> Database::getFlightReport(const QDate& date)
{
    QList<QMap<QString, QVariant>> report;
    QSqlQuery query;
    query.prepare("SELECT flight_number, origin, destination, departure_time, "
                  "total_seats, booked_seats, (total_seats - booked_seats) as available_seats, "
                  "ROUND((booked_seats * 100.0 / total_seats), 2) as occupancy_rate "
                  "FROM flights WHERE DATE(departure_time) = ? ORDER BY departure_time");
    query.addBindValue(date.toString("yyyy-MM-dd"));
    
    if (query.exec()) {
        while (query.next()) {
            QMap<QString, QVariant> row;
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); ++i) {
                row[record.fieldName(i)] = record.value(i);
            }
            report.append(row);
        }
    }
    
    return report;
}
