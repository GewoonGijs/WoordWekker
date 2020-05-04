// ntp.h

#ifndef NTP_H
#define NTP_H

#define TIME_BETWEEN_REQS 10000

class NTPClient {

public:
  void NTPStart();
  void NTPRequest();
  void NTPHandleResponse();
  unsigned long getLastTime() { return _lasttime; }

protected:
  unsigned long _lasttime = 0;
  unsigned long _lastsent = 0;

};

#endif //NTP_H
