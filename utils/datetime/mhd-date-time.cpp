#include "mhd-date-time.hpp"
#include <algorithm>
#include <cctype>

// Utility function for string splitting
std::vector<std::string> split_string(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Default constructor - Current datetime
MhdDateTime::MhdDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    this->day = ltm->tm_mday;
    this->month = ltm->tm_mon + 1;
    this->year = ltm->tm_year + 1900;
    this->hour = ltm->tm_hour;
    this->minute = ltm->tm_min;
    this->second = ltm->tm_sec;
    
    this->is_correct();
}

// Copy constructor
MhdDateTime::MhdDateTime(const MhdDateTime& dt) {
    this->day = dt.day;
    this->month = dt.month;
    this->year = dt.year;
    this->hour = dt.hour;
    this->minute = dt.minute;
    this->second = dt.second;
}

// Constructor with date only (time set to 00:00:00)
MhdDateTime::MhdDateTime(int day, int month, int year) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->hour = 0;
    this->minute = 0;
    this->second = 0;
    this->is_correct();
}

// Constructor with full datetime
MhdDateTime::MhdDateTime(int day, int month, int year, int hour, int minute, int second) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->hour = hour;
    this->minute = minute;
    this->second = second;
    this->is_correct();
}

// Constructor from string (supports multiple formats)
MhdDateTime::MhdDateTime(const std::string& str) {
    std::string input = str;
    
    // Remove extra whitespace
    // input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
    try {
        // Check for SQLite format: YYYY-MM-DD HH:MM:SS
        if (input.find('-') != std::string::npos && input.find(':') != std::string::npos) {
            size_t space_pos = input.find(" ");
            if (space_pos != std::string::npos) {
                std::string date_part = input.substr(0, space_pos);
                std::string time_part = input.substr(space_pos + 1);
                
                // Parse date part (YYYY-MM-DD)
                std::vector<std::string> date_tokens = split_string(date_part, '-');
                if (date_tokens.size() != 3) {
                    throw DateTimeException("Invalid date format");
                }
                
                this->year = std::stoi(date_tokens[0]);
                this->month = std::stoi(date_tokens[1]);
                this->day = std::stoi(date_tokens[2]);
                
                // Parse time part (HH:MM:SS)
                std::vector<std::string> time_tokens = split_string(time_part, ':');
                if (time_tokens.size() < 2 || time_tokens.size() > 3) {
                    throw DateTimeException("Invalid time format");
                }
                
                this->hour = std::stoi(time_tokens[0]);
                this->minute = std::stoi(time_tokens[1]);
                this->second = time_tokens.size() == 3 ? std::stoi(time_tokens[2]) : 0;
            } else {
                throw DateTimeException("Invalid datetime format - missing space separator");
            }
        }
        // Check for date only format: DD-MM-YYYY
        else if (input.find('-') != std::string::npos && input.find(':') == std::string::npos) {
            std::vector<std::string> tokens = split_string(input, '-');
            if (tokens.size() != 3) {
                throw DateTimeException("Invalid date format");
            }
            
            this->day = std::stoi(tokens[0]);
            this->month = std::stoi(tokens[1]);
            this->year = std::stoi(tokens[2]);
            this->hour = 0;
            this->minute = 0;
            this->second = 0;
        }
        else {
            throw DateTimeException("Unsupported datetime format");
        }
    } catch (const std::invalid_argument& e) {
        throw DateTimeException("Invalid number format in datetime string");
    } catch (const std::out_of_range& e) {
        throw DateTimeException("Number out of range in datetime string");
    }
    
    this->is_correct();
}

// Validation function
void MhdDateTime::is_correct() const {
    if (this->year <= 0) {
        throw DateTimeException("Year must be at least 1");
    }
    if (this->month < 1 || this->month > 12) {
        throw DateTimeException("Month must be between 1 and 12");
    }
    if (this->day < 1 || this->day > 31) {
        throw DateTimeException("Day must be between 1 and 31");
    }
    if (this->hour < 0 || this->hour > 23) {
        throw DateTimeException("Hour must be between 0 and 23");
    }
    if (this->minute < 0 || this->minute > 59) {
        throw DateTimeException("Minute must be between 0 and 59");
    }
    if (this->second < 0 || this->second > 59) {
        throw DateTimeException("Second must be between 0 and 59");
    }
    
    // Check days in month
    int days_in_month = get_days_in_month(this->month, this->year);
    if (this->day > days_in_month) {
        throw DateTimeException("Invalid day for the given month and year");
    }
}

// Helper function to get days in a month
int MhdDateTime::get_days_in_month(int month, int year) const {
    switch (month) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return is_leap_year(year) ? 29 : 28;
        default:
            return 31;
    }
}

// Leap year check
bool MhdDateTime::is_leap_year(int year) const {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// Update tm structure for formatting
void MhdDateTime::update_tm_structure() const {
    tm_.tm_year = year - 1900;
    tm_.tm_mon = month - 1;
    tm_.tm_mday = day;
    tm_.tm_hour = hour;
    tm_.tm_min = minute;
    tm_.tm_sec = second;
    tm_.tm_wday = get_day_week();
    tm_.tm_yday = 0;  // Day of year (not calculated here)
    tm_.tm_isdst = -1; // Let system determine DST
}

// Getters - Date
int MhdDateTime::get_day() const { return this->day; }
int MhdDateTime::get_month() const { return this->month; }
int MhdDateTime::get_year() const { return this->year; }

// Getters - Time
int MhdDateTime::get_hour() const { return this->hour; }
int MhdDateTime::get_minute() const { return this->minute; }
int MhdDateTime::get_second() const { return this->second; }

// Get day name
std::string MhdDateTime::get_day_name() const {
    std::string days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    return days[get_day_week()];
}

// Get month name
std::string MhdDateTime::get_month_name() const {
    std::string months[12] = {"January", "February", "March", "April", "May", "June", 
                             "July", "August", "September", "October", "November", "December"};
    return months[this->month - 1];
}

// Setters
void MhdDateTime::set_date(int day, int month, int year) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->is_correct();
}

void MhdDateTime::set_time(int hour, int minute, int second) {
    this->hour = hour;
    this->minute = minute;
    this->second = second;
    this->is_correct();
}

void MhdDateTime::set_datetime(int day, int month, int year, int hour, int minute, int second) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->hour = hour;
    this->minute = minute;
    this->second = second;
    this->is_correct();
}

// Properties
bool MhdDateTime::is_bissextil() const {
    return is_leap_year(this->year);
}

// Day of week calculation using Zeller's congruence
int MhdDateTime::get_day_week() const {
    int d = this->day;
    int m = this->month;
    int y = this->year;
    
    if (m < 3) {
        m += 12;
        y--;
    }
    
    int k = y % 100;
    int j = y / 100;
    
    int h = (d + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) - 2 * j) % 7;
    
    return (h + 5) % 7;
}

bool MhdDateTime::is_today() const {
    MhdDateTime now;
    return (this->day == now.day && this->month == now.month && this->year == now.year);
}

bool MhdDateTime::is_now() const {
    MhdDateTime now;
    int diff = abs(this->diff_seconds(now));
    return diff <= 60; // Within 1 minute
}

// DateTime manipulation
MhdDateTime MhdDateTime::next_day() const {
    MhdDateTime dt(*this);
    
    int days_in_current_month = get_days_in_month(dt.month, dt.year);
    
    if (dt.day < days_in_current_month) {
        dt.day++;
    } else {
        dt.day = 1;
        if (dt.month < 12) {
            dt.month++;
        } else {
            dt.month = 1;
            dt.year++;
        }
    }
    
    return dt;
}

MhdDateTime MhdDateTime::previous_day() const {
    MhdDateTime dt(*this);
    
    if (dt.day > 1) {
        dt.day--;
    } else {
        if (dt.month > 1) {
            dt.month--;
            dt.day = get_days_in_month(dt.month, dt.year);
        } else {
            dt.month = 12;
            dt.year--;
            dt.day = 31;
        }
    }
    
    return dt;
}

MhdDateTime MhdDateTime::add_hours(int hours) const {
    return *this + (hours * 3600);
}

MhdDateTime MhdDateTime::add_minutes(int minutes) const {
    return *this + (minutes * 60);
}

MhdDateTime MhdDateTime::add_seconds(int seconds) const {
    return *this + seconds;
}

// Difference calculations
int MhdDateTime::diff_days(const MhdDateTime& other) const {
    MhdDateTime dt1(this->day, this->month, this->year);
    MhdDateTime dt2(other.day, other.month, other.year);
    
    return (dt1.to_timestamp() - dt2.to_timestamp()) / (24 * 3600);
}

int MhdDateTime::diff_hours(const MhdDateTime& other) const {
    return (this->to_timestamp() - other.to_timestamp()) / 3600;
}

int MhdDateTime::diff_minutes(const MhdDateTime& other) const {
    return (this->to_timestamp() - other.to_timestamp()) / 60;
}

int MhdDateTime::diff_seconds(const MhdDateTime& other) const {
    return this->to_timestamp() - other.to_timestamp();
}

// Formatting functions
std::string MhdDateTime::to_sqlite_string() const {
    update_tm_structure();
    std::ostringstream ss;
    ss << std::put_time(&tm_, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string MhdDateTime::to_iso_string() const {
    update_tm_structure();
    std::ostringstream ss;
    ss << std::put_time(&tm_, "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

std::string MhdDateTime::to_string(const std::string& format) const {
    if (format == "sqlite" || format == "SQL") {
        return to_sqlite_string();
    } else if (format == "iso" || format == "ISO") {
        return to_iso_string();
    } else {
        // Default format: DD-MM-YYYY HH:MM:SS
        std::ostringstream ss;
        ss << (day < 10 ? "0" : "") << day << "-"
           << (month < 10 ? "0" : "") << month << "-"
           << year << " "
           << (hour < 10 ? "0" : "") << hour << ":"
           << (minute < 10 ? "0" : "") << minute << ":"
           << (second < 10 ? "0" : "") << second;
        return ss.str();
    }
}

std::string MhdDateTime::to_date_string(const std::string& format) const {
    if (format == "iso" || format == "ISO") {
        std::ostringstream ss;
        ss << year << "-"
           << (month < 10 ? "0" : "") << month << "-"
           << (day < 10 ? "0" : "") << day;
        return ss.str();
    } else {
        std::ostringstream ss;
        ss << (day < 10 ? "0" : "") << day << "-"
           << (month < 10 ? "0" : "") << month << "-"
           << year;
        return ss.str();
    }
}

std::string MhdDateTime::to_time_string(const std::string& format) const {
    std::ostringstream ss;
    ss << (hour < 10 ? "0" : "") << hour << ":"
       << (minute < 10 ? "0" : "") << minute << ":"
       << (second < 10 ? "0" : "") << second;
    return ss.str();
}

// Utility functions
int MhdDateTime::age_in_years(const MhdDateTime& to_datetime) const {
    int years = to_datetime.year - this->year;
    
    if (to_datetime.month < this->month || 
        (to_datetime.month == this->month && to_datetime.day < this->day)) {
        years--;
    }
    
    return years;
}

int MhdDateTime::age_in_years() const {
    MhdDateTime now;
    return age_in_years(now);
}

time_t MhdDateTime::to_timestamp() const {
    tm time_struct = {};
    time_struct.tm_year = year - 1900;
    time_struct.tm_mon = month - 1;
    time_struct.tm_mday = day;
    time_struct.tm_hour = hour;
    time_struct.tm_min = minute;
    time_struct.tm_sec = second;
    time_struct.tm_isdst = -1;
    
    return mktime(&time_struct);
}

MhdDateTime MhdDateTime::from_timestamp(time_t timestamp) {
    tm* time_struct = localtime(&timestamp);
    return MhdDateTime(
        time_struct->tm_mday,
        time_struct->tm_mon + 1,
        time_struct->tm_year + 1900,
        time_struct->tm_hour,
        time_struct->tm_min,
        time_struct->tm_sec
    );
}

// Operators
MhdDateTime& MhdDateTime::operator=(const MhdDateTime& dt) {
    if (this == &dt) {
        return *this;
    }
    this->day = dt.day;
    this->month = dt.month;
    this->year = dt.year;
    this->hour = dt.hour;
    this->minute = dt.minute;
    this->second = dt.second;
    return *this;
}

MhdDateTime MhdDateTime::operator+(int seconds) const {
    time_t timestamp = this->to_timestamp() + seconds;
    return MhdDateTime::from_timestamp(timestamp);
}

MhdDateTime MhdDateTime::operator-(int seconds) const {
    time_t timestamp = this->to_timestamp() - seconds;
    return MhdDateTime::from_timestamp(timestamp);
}

int MhdDateTime::operator-(const MhdDateTime& dt) const {
    return this->diff_seconds(dt);
}

// Comparison operators
bool MhdDateTime::operator==(const MhdDateTime& dt) const {
    return (this->day == dt.day && this->month == dt.month && this->year == dt.year &&
            this->hour == dt.hour && this->minute == dt.minute && this->second == dt.second);
}

bool MhdDateTime::operator<(const MhdDateTime& dt) const {
    return this->to_timestamp() < dt.to_timestamp();
}

bool MhdDateTime::operator<=(const MhdDateTime& dt) const {
    return this->to_timestamp() <= dt.to_timestamp();
}

bool MhdDateTime::operator>(const MhdDateTime& dt) const {
    return this->to_timestamp() > dt.to_timestamp();
}

bool MhdDateTime::operator>=(const MhdDateTime& dt) const {
    return this->to_timestamp() >= dt.to_timestamp();
}

// Stream operator
std::ostream& operator<<(std::ostream& os, const MhdDateTime& dt) {
    os << dt.to_string();
    return os;
}