/* mktime_utc.c -- Convert between calendar time (or epoch seconds) and
   broken-down time representation for given tomezone.

  version 0.1, July 20th, 2019

  Copyright (C) 2019 Matt HK Lo

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Matt HK Lo
  hklo.tw@gmail.com
*/

#include <time.h>

static void gmtime_x(time_t* _t, struct tm* _tm)
{
#ifdef _WIN32
    gmtime_s(_tm, _t);
#else
    gmtime_r(_t, _tm);
#endif
}

static void localtime_x(time_t* _t, struct tm* _tm)
{
#ifdef _WIN32
    localtime_s(_tm, _t);
#else
    localtime_r(_t, _tm);
#endif
}

static time_t timediff_local_utc0()
{
    /*
        Use now as a fixed time point (in epoch seconds),
        find out the broken-down time representations for
        both UTC+0 and local timezone.
    */
    time_t now = time(NULL);
    struct tm tm_utc, tm_local;
    gmtime_x(&now, &tm_utc);
    localtime_x(&now, &tm_local);

    /*
        View the above 2 broken-down time representations as in local,
        and convert back to be epoch seconds. This gives the seconds
        difference between UTC+0 and local timezone. Daylight saving
        time shall be considered if it's in effect.
     */
    tm_local.tm_isdst = -1;
    time_t t1 = mktime(&tm_utc);
    time_t t2 = mktime(&tm_local);

    return t1 - t2;
}

#define UTC_TZ(x) (x*3600)

/*
 * Similar to mktime(), but work in given timezone.
 *
 * Example:
 *     Find the epoch seconds for 2019/7/1 13:00:00 UTC-8:
 *    
 *     struct tm t;
 *     t.tm_year = 119; t.tm_mon = 6; t.tm_mday = 1; t.tm_hour = 13; 
 *     t.tm_min = t.tm_sec = t.tm_isdst = 0;
 *
 *     time_t epoch_seconds = mktime_utc(&t, UTC_TZ(-8));
 *
 * Validation:
 *     Verify the result with online tool:
 *     https://www.epochconverter.com/timezones
 */
time_t mktime_utc(struct tm *_tm, time_t _utc_tz)
{
    /*
       To get epoch seconds in target timezone (_utc_tz), first view
       _tm in local timezone and get the epoch seconds by mktime(). Then
       apply the time_diff to get the epoch seconds of _tm in UTC+0.
       Finally apply the _utc_tz to get the epoch seconds of _tm in target
       timezone.
     */
    return mktime(_tm) - timediff_local_utc0() - _utc_tz;
}

/*
 * Similar to localtime(), but work in given timezone.
 * Please note that Daylight Saving Time is always viewed as not in effect.
 *
 * Example:
 *     Covert the epoch seconds to broken-down time representation in UTC+2:
 *
 *     time_t sec = 1562014800; // 2019/7/1 13:00:00 UTC-8
 *     struct tm t;
 *     localtime_utc(&sec, &t, UTC_TZ(+2));
 *
 * Validation:
 *     Verify the result with online tool:
 *     https://www.epochconverter.com/timezones
 */
struct tm* localtime_utc(const time_t* _t, struct tm* _tm, time_t _utc_tz)
{
    time_t t = *_t + _utc_tz;
    gmtime_x(&t, _tm);
    return _tm;
}
