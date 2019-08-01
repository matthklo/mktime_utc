# mktime_utc
A tweaked version of mktime() and localtime() that to be able to work in any timezone you want.

# Why?

If you ever want to convert epoch seconds to a break-down time representation, you should have found out that mktime() is all you can count on. But unfortunately it could only work in local timezone. If you want do the convertion in any desired timezone, try use mktime_utc() and localtime_utc() provided here as a drop-in-replacement.

# Quick examples

Find the epoch seconds for 2019/7/1 13:00:00 UTC-8:

    struct tm t;
    t.tm_year = 119; t.tm_mon = 6; t.tm_mday = 1; t.tm_hour = 13; 
    t.tm_min = t.tm_sec = t.tm_isdst = 0;
    
    time_t epoch_seconds = mktime_utc(&t, UTC_TZ(-8));
    
Covert the epoch seconds to broken-down time representation in UTC+2:

    time_t sec = 1562014800; // 2019/7/1 13:00:00 UTC-8
    struct tm t;
    
    localtime_utc(&sec, &t, UTC_TZ(+2));

# Limitation
* Daylight Saving Time is not considered.
* On Windows platform, it requires gmtime_s() and localtime_s() to work.
* On POSIX-compliant platform, it requires gmtime_r() and localtime_r() to work.

# How to verify?

You can verify the result with [EpochConverter online tool](https://www.epochconverter.com/timezones).

# License
zlib
